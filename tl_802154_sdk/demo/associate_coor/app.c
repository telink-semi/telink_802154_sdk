/********************************************************************************************************
 * @file    app.c
 *
 * @brief   This is the source file for app
 *
 * @author  Zigbee Group
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *
 *          Licensed under the Apache License, Version 2.0 (the "License");
 *          you may not use this file except in compliance with the License.
 *          You may obtain a copy of the License at
 *
 *              http://www.apache.org/licenses/LICENSE-2.0
 *
 *          Unless required by applicable law or agreed to in writing, software
 *          distributed under the License is distributed on an "AS IS" BASIS,
 *          WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *          See the License for the specific language governing permissions and
 *          limitations under the License.
 *******************************************************************************************************/
#include <ieee802154_common.h>
#include "app.h"
#include "../app_common/tl_specific_data.h"

#define 	APP_NODE_NUM_MAX        128
#define		END_DEVICE_NUM		MAC_DEV_TABLE_MAX_LEN
my_device_t end_device[END_DEVICE_NUM] = {{{0},0,0}};
u8 device_index=0;
u8 indirect_device_index=0;
//coor info
unsigned short coor_addr_short = 0x2211;
unsigned char  device_ext_addr[8] = {0};
unsigned short device_addr_short = 0x3300;
unsigned short pan_id = 0xbeef;
addr_t	dev_src_addr;

//key material
unsigned char default_key_source[8] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07};
unsigned char default_key_index = 0xa5;
unsigned char test_key[16] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
                              0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff};

static int data_send_indir_timer_cb(void *arg);

//set key_usage_list  type 01:mac data  03:mac cmd
mac_keyusageDesc_t *pKeyUsgDesc[2] = {NULL};
mac_keyid_lookup_desc_t *pKeyIDDesc = NULL;


//rx packet
volatile u8 user_packet[16] = {0};

static u16 g_node_addr[APP_NODE_NUM_MAX];
static u16 g_node_num_cur = 0;

static u8 g_mac_msdu_handle = 0;
static u32 g_node_index = 0;
static ev_timer_event_t *g_data_txto_allnode_timer = NULL;
static u32 g_app_data_sn = 0;

s32 app_data_txto_allnode(void *arg)
{
    zb_mscp_data_req_t req;
    memset(&req, 0, sizeof(req));
    u8 len;
    tl_zbMacAttrGet(MAC_ATTR_PAN_ID,(u8 *)&req.dstPanId,&len);
    req.srcAddr.addrMode = ZB_ADDR_16BIT_DEV_OR_BROADCAST;    // short addr mode
    req.dstAddr.addrMode = ZB_ADDR_16BIT_DEV_OR_BROADCAST;    // short addr mode

#if 1
    if (g_node_num_cur > 0) {
        req.dstAddr.addr.shortAddr = g_node_addr[g_node_index];
        if (++g_node_index >= g_node_num_cur) {
            g_node_index = 0;
        }
    } else {
        return -1;
    }
#endif

    req.msduHandle = (g_mac_msdu_handle++) & 0x3f;
    /* tx options:
     * MAC_TX_OPTION_ACKNOWLEDGED_BIT: mac ack needed(normally used for unicasting)
     * MAC_TX_OPTION_INDIRECT_TRANSMISSION_BIT: indirect transmit if the destination device is sleepy-device
     * 0:   for broadcasting
     * */
    req.txOptions = MAC_TX_OPTION_ACKNOWLEDGED_BIT | MAC_TX_OPTION_INDIRECT_TRANSMISSION_BIT;
    // don't use security
#if APP_MAC_SEC_ENABLE
    req.sec.securityLevel = 0;
#endif

    u8 msduLen = 50+4;
    u8 *msdu = ev_buf_allocate(msduLen);
    u32 *pdu = (u32 *)msdu;
    *pdu = g_app_data_sn++;
    if (msdu) {
        for (int i = 4; i < msduLen; i++) {
            msdu[i] = i - 4;
        }
        tl_MacMcpsDataRequestSend(req, msdu, msduLen);
        ev_buf_free(msdu);
    }

#if 0
    if (g_node_index++ > 4) {
        return -1;
    }
#endif

    return 0;
}




void app_data_transmit_toggle(void)
{
    if (g_data_txto_allnode_timer) {
        TL_ZB_TIMER_CANCEL(&g_data_txto_allnode_timer);
        return;
    }
    g_node_index = 0;
    g_data_txto_allnode_timer = TL_ZB_TIMER_SCHEDULE(app_data_txto_allnode, NULL, 3000);
}



void coor_config(void)
{
	u8 security=ZB_MAC_SECURITY;
	tl_zbMacAttrSet(MAC_ATTR_SECURITY_ENABLED, &security, 1);

	tl_zbMacAttrSet(MAC_ATTR_SHORT_ADDRESS,(u8 *)&coor_addr_short,2);

	tl_zbMacAttrSet(MAC_DEFAULT_KEY_SOURCE,default_key_source,8);

	//COMMON Materials
	pKeyUsgDesc[0]= (mac_keyusageDesc_t *)mac_keyusageDesc_alloc();
	if (!pKeyUsgDesc[0]) {
		while(1);
	}
	mac_keyusageDesc_set(pKeyUsgDesc[0], 1, 0);
	pKeyUsgDesc[1] = (mac_keyusageDesc_t *)mac_keyusageDesc_alloc();
	if (!pKeyUsgDesc[1]) {
		while(1);
	}
	mac_keyusageDesc_set(pKeyUsgDesc[1], 3, 4);

	//set key_id_lookup_list
	pKeyIDDesc = (mac_keyid_lookup_desc_t *)mac_keyidDesc_alloc();
	if (!pKeyIDDesc) {
		while(1);
	}
	unsigned char look_data[9];
	memset(look_data, 0, 9);
	memcpy(look_data,default_key_source,sizeof(default_key_source));
	look_data[8] = default_key_index;
	mac_keyidDesc_set(pKeyIDDesc, LOOKUP_DATA_SIZE_9, look_data);//lookup data size 9 octos
}


u8 add_key_material(void)
{
	mac_deviceDesc_t *pDevDesc = NULL;

	//set device table
	for(u8 index=0;index<MAC_DEV_TABLE_MAX_LEN;index++)
	{
		tl_zbMacAttrGet(MAC_DEVICE_TABLE, (void *)&pDevDesc, &index);

		if(pDevDesc)
		{
			if(memcmp(pDevDesc->long_address,device_ext_addr,8)==0)
			{
				device_addr_short = pDevDesc->short_address;
				break;
			}
		}
		pDevDesc = NULL;
	}
	if(pDevDesc != NULL)
	{
		mac_deviceDesc_reset(pDevDesc, pan_id, device_addr_short, device_ext_addr, 0, 0);
		return TRUE;
	}

	if(device_index>=END_DEVICE_NUM)
		return FALSE;

	pDevDesc = mac_deviceDesc_alloc();
	if (!pDevDesc) {
		while(1);
	}

	device_addr_short = device_addr_short & 0xfff0;
	device_addr_short += device_index;
	mac_deviceDesc_set(pDevDesc, pan_id, device_addr_short, device_ext_addr, 0, 0);
	tl_zbMacAttrSet(MAC_DEVICE_TABLE, (u8 *)pDevDesc, device_index);//add device table to macPib

	//set key_device_list
	mac_keydevDesc_t *pKeyDevDesc = (mac_keydevDesc_t *)mac_keydevDesc_alloc();
	if (!pKeyDevDesc) {
		while(1);
	}
	mac_keydevDesc_set(pKeyDevDesc, pDevDesc, 0, 0);//add mac_deviceDesc_t  to pKeyDevDesc



	//set key table
	mac_keyDesc_t *pKeyDesc = (mac_keyDesc_t *)mac_keyDesc_alloc();
	if (!pKeyDesc) {
		while(1);
	}
	mac_keyDesc_set(pKeyDesc, &pKeyIDDesc, 1, &pKeyDevDesc, 1, pKeyUsgDesc, 2, test_key);

	tl_zbMacAttrSet(MAC_KEY_TABLE, (u8 *)pKeyDesc, device_index);  //add key table to macPib
	device_index++;
	return TRUE;
}









void pan_start(void)
{
    zb_mac_mlme_start_req_t start_req;
    zb_mac_mlme_start_req_t *pstart_req = (zb_mac_mlme_start_req_t *)&start_req;
    memset(pstart_req, 0, sizeof(zb_mac_mlme_start_req_t));
	/* TODO: fill zero params if necessary */
    pstart_req->panId = pan_id;
    pstart_req->logicalChannel = DEFAULT_CHANNEL;
    pstart_req->channelPage = 0;
    pstart_req->panCoordinator = 1;
    pstart_req->coordRealignment = 0;
    pstart_req->beaconOrder = ZB_TURN_OFF_ORDER;
    pstart_req->superframeOrder = ZB_TURN_OFF_ORDER;
    tl_MacMlmeStartRequest(start_req);
}




int data_send_indir_timer_cb(void *arg)
{
	TL_SCHEDULE_TASK(mac_send_data_indirect, (void *)NULL);
	return 0;
}




void mac_send_data_indirect(void *arg)
{
	static unsigned int test_cnt = 0xaa000001;
	zb_mscp_data_req_t data_req;
	zb_mscp_data_req_t *req = (zb_mscp_data_req_t *)&data_req;
	memset(req, 0, sizeof(zb_mscp_data_req_t));
	req->srcAddr.addrMode = ZB_ADDR_16BIT_DEV_OR_BROADCAST; //16-bit short address mode
	req->dstAddr.addrMode = ZB_ADDR_16BIT_DEV_OR_BROADCAST; //16-bit short address mode
	req->dstAddr.addr.shortAddr = end_device[indirect_device_index].shortAddr;

	u8 len=0;
	tl_zbMacAttrGet(MAC_ATTR_PAN_ID,(u8 *)&req->dstPanId,&len);

	req->msduHandle = 0;
	req->txOptions = MAC_TX_OPTION_ACKNOWLEDGED_BIT|MAC_TX_OPTION_INDIRECT_TRANSMISSION_BIT;

	//security setting
	req->sec.key_id_mode = KEY_ID_MODE_KEY_EXPLICIT_8;
	req->sec.securityLevel = SECURITY_LEVEL_ENC_MIC_64;

	memcpy(req->sec.key_source,default_key_source,sizeof(default_key_source));
	req->sec.key_index = default_key_index;


	u8 *pay = ev_buf_allocate(sizeof(test_cnt));//allocate ev buffer
	if (NULL == pay) {
		while(1);
	}
	u8 pay_len = sizeof(test_cnt);
	memcpy((u8 *)pay,(u8 *)&test_cnt,pay_len);
	test_cnt++;
	tl_MacMcpsDataRequestSend(data_req,pay,pay_len);
	ev_buf_free(pay);//free payload ev buffer
	indirect_device_index++;
	if(indirect_device_index>=device_index)
		indirect_device_index = 0;
}


ev_timer_event_t *data_send_indir_timer = NULL;
void data_send(void)
{
    if (data_send_indir_timer) {
    	TL_ZB_TIMER_CANCEL(&data_send_indir_timer);
    }
    data_send_indir_timer = TL_ZB_TIMER_SCHEDULE(data_send_indir_timer_cb, NULL, 5000);
}

void MyAssociateIndCb(void *pData)
{
	if(pData==NULL) return;
	zb_mlme_associate_ind_t *pInd = (zb_mlme_associate_ind_t *)pData;
	extAddr_t device_address;
	u16 address = 0;
	u8 idx=0,len=0;
	ZB_IEEE_ADDR_COPY(device_address, pInd->devAddress);
	ZB_IEEE_ADDR_COPY(device_ext_addr, pInd->devAddress);

	if(add_key_material()!=TRUE)//add device info to key table
	{
		return;
	}

	idx = device_addr_short&0xf;
	ZB_IEEE_ADDR_COPY(end_device[idx].extAddr, device_address);
	tl_zbMacAttrGet(MAC_ATTR_PAN_ID,(u8 *)&end_device[idx].pan_id,&len);
	address = device_addr_short;
	end_device[idx].shortAddr = address;
	zb_mlme_associate_resp_t AssociateRsp;
	zb_mlme_associate_resp_t *pResp = (zb_mlme_associate_resp_t *)&AssociateRsp;
    memset(pResp, 0, sizeof(zb_mlme_associate_resp_t));
	ZB_IEEE_ADDR_COPY(pResp->devAddress, device_address);
	pResp->shortAddress = address;
	pResp->status = MAC_SUCCESS;
	tl_MacMlmeAssociateResponseSend(AssociateRsp);
	data_send();//indirect send data
}


void MyStartCnfCb(void *pData)
{
	if(pData==NULL)  	return;
//	mac_mlme_startCnf_t *pCnf = (mac_mlme_startCnf_t *)pData;
}




void MyDataCnfCb(void *pData)
{
	if(pData==NULL)  	return;
	zb_mscp_data_conf_t *pCnf = (zb_mscp_data_conf_t *)pData;
	if(pCnf->status!=MAC_SUCCESS)
	{
			//if the status isn't MAC_SUCCESS,user can retransmit in app layer
	}
}

void MyPullIndCb(void *pData)
{
	if(pData==NULL)  	return;
//	mac_mlme_poll_ind_t *pInd = (mac_mlme_poll_ind_t *)pData;
//	pInd->addrMode
}

void MyStateIndCb(void *pData)
{
	if(pData==NULL)  	return;
	zb_mlme_comm_status_ind_t * pInd = (zb_mlme_comm_status_ind_t *)pData;
	if(pInd->status!=MAC_SUCCESS)
	{
	//check  associateresponse state
	}
}



void MyDataIndCb(void *pData)
{
	if(pData==NULL)  	return;
	zb_mscp_data_ind_t * pInd = (zb_mscp_data_ind_t *)pData;
	memcpy((u8 *)&dev_src_addr,(u8 *)&pInd->srcAddr,sizeof(addr_t));
	u8 len = pInd->msduLength;
	u8 ret = tl_appDataIndicate((u8 *)pInd->msdu,len);
	if(ret!=SUCCESS)
	{
		if(len>sizeof(user_packet))
		{
			len = sizeof(user_packet);
		}
		memcpy((u8 *)user_packet,(u8 *)pInd->msdu,len);
	}
}
