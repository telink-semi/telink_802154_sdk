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
#include "app_cfg.h"
#include "sampleDevice.h"
#include "../app_common/tl_specific_data.h"
#include "../app_common/ota/tl_specific_data_ota.h"


typedef struct {
    u16 pan_id;
    u8 lqi;
    u8 channel;
    addr_t coor_addr;
    u8 permitjoin;
    u8 reserved[2];
} my_pan_t;

typedef struct {
	my_pan_t coor_beacon[MAX_ED_SCAN_RESULTS_SUPPORTED];
    u32		 coor_size;
} my_coor_t;

ev_timer_event_t *BeaconReqTimer = NULL;
ev_timer_event_t *PollDataTimer = NULL;


//key metrial
unsigned char default_key_source[8] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07};
unsigned char default_key_index = 0xa5;
unsigned char test_key[16] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
                              0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff};

//coor info
unsigned char  coor_ext_addr[8] = {0};
unsigned short coor_addr_short = 0x0;
unsigned short pan_id = 0;

//set key_usage_list  type 01��mac data  03:mac cmd
mac_keyusageDesc_t *pKeyUsgDesc[2] = {NULL};
mac_keyid_lookup_desc_t *pKeyIDDesc = NULL;

//rx packet
volatile u8 user_packet[16] = {0};
//poll data
void dev_config(void)
{
	u8 security=ZB_MAC_SECURITY;//enable pib security

	tl_zbMacAttrSet(MAC_ATTR_SECURITY_ENABLED, &security, 1);

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
	//set device table
	mac_deviceDesc_t *pDevDesc = mac_deviceDesc_alloc();
	if (!pDevDesc) {
		while(1);
	}
	mac_deviceDesc_set(pDevDesc, pan_id, coor_addr_short, coor_ext_addr, 0, 0);
	tl_zbMacAttrSet(MAC_DEVICE_TABLE, (u8 *)pDevDesc, 0);//add device table to macPib

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

	tl_zbMacAttrSet(MAC_KEY_TABLE, (u8 *)pKeyDesc, 0);  //add key table to macPib

	return TRUE;
}





my_coor_t received_coor = {{{0}},0};
void dev_active_scan(void)
{
	zb_mac_mlme_scan_req_t  scan_req;
	zb_mac_mlme_scan_req_t *pscan_req = (zb_mac_mlme_scan_req_t *)&scan_req;
    memset(pscan_req, 0, sizeof(zb_mac_mlme_scan_req_t));
    received_coor.coor_size = 0;
    pscan_req->scanType = ACTIVE_SCAN;
    pscan_req->scanChannels = (1<<DEFAULT_CHANNEL);//scan 24 channel
    pscan_req->scanDuration = 2;
    tl_MacMlmeScanRequest(scan_req);
}

void MyScanCnfCb(void *pData)
{
	if(pData==NULL) return;
	zb_mac_mlme_scan_conf_t *pConf = (zb_mac_mlme_scan_conf_t *)pData;
    if ((pConf->status == MAC_SUCCESS) && (pConf->scanType == ACTIVE_SCAN)) {

    }
	if(received_coor.coor_size){

		if (BeaconReqTimer) {
    		TL_ZB_TIMER_CANCEL(&BeaconReqTimer);
    	}

		dev_start_associate();
	}
}


void dev_start_associate(void)
{
    my_pan_t *pan_ptr = NULL;
    my_pan_t *pan_of_highest_lqi = NULL;
    u8 len=0;
    u32 pan_size = received_coor.coor_size;
    if(!pan_size){
    	return;
    }

    pan_of_highest_lqi = &received_coor.coor_beacon[0];
    //find out the pan with highest lqi first
    for(u32 i=0;i<pan_size;i++)
    {
    	pan_ptr = &received_coor.coor_beacon[i];
    	if(pan_ptr->lqi > pan_of_highest_lqi->lqi)
    		pan_of_highest_lqi = pan_ptr;
    }



    zb_mlme_associate_req_t assoc_req;
	zb_mlme_associate_req_t *passoc_req = (zb_mlme_associate_req_t *)&assoc_req;
	memset((u8 *)passoc_req, 0, sizeof(zb_mlme_associate_req_t));

	passoc_req->logicalChannel = pan_of_highest_lqi->channel;
	passoc_req->coordPanId = pan_of_highest_lqi->pan_id;
	passoc_req->capbilityInfo.rcvOnWhenIdle = 1; //allocate address and rx on when idle
	passoc_req->capbilityInfo.allocAddr = 1;

	if(pan_of_highest_lqi->coor_addr.addrMode == ZB_ADDR_64BIT_DEV){
		passoc_req->coordAddress.addrMode = ZB_ADDR_64BIT_DEV;
		ZB_IEEE_ADDR_COPY(passoc_req->coordAddress.addr.extAddr, (u8 *)&pan_of_highest_lqi->coor_addr.addr.extAddr);
		len = 8;
		tl_zbMacAttrSet(MAC_ATTR_COORDINATOR_EXTENDED_ADDRESS,(u8 *)&pan_of_highest_lqi->coor_addr.addr.extAddr,len);
	}else{
		passoc_req->coordAddress.addrMode = ZB_ADDR_16BIT_DEV_OR_BROADCAST;
		len = 2;
		memcpy(&passoc_req->coordAddress.addr.shortAddr, &pan_of_highest_lqi->coor_addr.addr.shortAddr, len);
		tl_zbMacAttrSet(MAC_ATTR_COORDINATOR_SHORT_ADDRESS,(u8 *)&pan_of_highest_lqi->coor_addr.addr.shortAddr,len);
	}
		len = 2;
		tl_zbMacAttrSet(MAC_ATTR_PAN_ID,(u8 *)&pan_of_highest_lqi->pan_id,len);

	tl_MacMlmeAssociateRequestSend(assoc_req);
}


int dev_activescan_callback(void *arg)
{
	dev_active_scan();
	return 0;
}


void dev_start_scan(void)
{
	if (BeaconReqTimer) {
		TL_ZB_TIMER_CANCEL(&BeaconReqTimer);
	}
	BeaconReqTimer = TL_ZB_TIMER_SCHEDULE(dev_activescan_callback, NULL, 3*1000);

}


volatile static unsigned int tdebug_cnt_poll_req = 0;
void poll_data_req_send(void *arg)
{
	mac_mlme_poll_req_t	 poll_req;
    mac_mlme_poll_req_t *ppoll_req = (mac_mlme_poll_req_t *)&poll_req;
	memset((u8 *)ppoll_req, 0, sizeof(mac_mlme_poll_req_t));
	ppoll_req->coordAddrMode = ZB_ADDR_64BIT_DEV;
	u8 len;
	tl_zbMacAttrGet(MAC_ATTR_COORDINATOR_EXTENDED_ADDRESS,(u8 *)&ppoll_req->coordAddr.extAddr,&len);
	tl_zbMacAttrGet(MAC_ATTR_PAN_ID,(u8 *)&ppoll_req->coordPanId,&len);
	tl_MacMlmePollRequestSend(poll_req);
	tdebug_cnt_poll_req++;
}







volatile static unsigned int poll_cnt = 0;



void mac_set_pollRate(u32 newRate){
	if(ZB_PIB_RX_ON_WHEN_IDLE()){
		return;
	}
	if(newRate){
	
    	if (PollDataTimer)	{
    		TL_ZB_TIMER_CANCEL(&PollDataTimer);
		}
		
		PollDataTimer = TL_ZB_TIMER_SCHEDULE(PollDataTimerCb, NULL, newRate);
	}


}

int PollDataTimerCb(void *arg)
{
    poll_data_req_send(NULL);
    poll_cnt++;
    return 0;
}


void MyAssocCnfCb(void *pData)
{
	if(pData==NULL) return;
	u8 len=0;
	zb_mlme_associate_conf_t *pCnf = (zb_mlme_associate_conf_t *)pData;
	len=8;
	tl_zbMacAttrSet(MAC_ATTR_COORDINATOR_EXTENDED_ADDRESS,(u8 *)&pCnf->parentAddress,len);


	tl_zbMacAttrGet(MAC_ATTR_PAN_ID,(u8 *)&pan_id,&len);
	tl_zbMacAttrGet(MAC_ATTR_COORDINATOR_SHORT_ADDRESS,(u8 *)&coor_addr_short,&len);

	ZB_IEEE_ADDR_COPY(coor_ext_addr, pCnf->parentAddress);
	add_key_material();

    if (pCnf->status == MAC_SUCCESS) {
    	ieee802154_info_save(); //store network info
    	gDevCtx.newDev = 0;
    	if (1 || !ZB_PIB_RX_ON_WHEN_IDLE()) {
    		mac_set_pollRate(NORMAL_POLL_RATE);//data request
    	}
        ota_queryStart(QUERY_RATE); //ota query 30S
    }
}

void MyDataIndCb(void *pData)
{
	if(pData==NULL)  return;
	zb_mscp_data_ind_t * pInd = (zb_mscp_data_ind_t *)pData;
	u8 len = pInd->msduLength;
	u8 ret = tl_appDataIndicate((u8 *)pInd->msdu,len);
	if(ret!=SUCCESS)//not OTA packet
	{
		if(len>sizeof(user_packet))
			len = sizeof(user_packet);
		memcpy((u8 *)user_packet,(u8 *)pInd->msdu,len);
	}
}

/*********************************************************************
 * @fn      MyDataCnfCb
 *
 * @brief   callback function for data-confirm
 *
 * @param   arg  the result status for sending a packet
 *
 * @return  none
 */
void MyDataCnfCb(void *arg)
{
    if (arg == NULL) {
        return;
    }

    zb_mscp_data_conf_t *pCnf = (zb_mscp_data_conf_t *)arg;

    if (pCnf->status != MAC_SUCCESS) {
        //if the status isn't MAC_SUCCESS,user can retransmit in app layer
    }
}

/*********************************************************************
 * @fn      app_pollCnfCb
 *
 * @brief   callback function for data-request confirm
 *
 * @param   arg  data-request confirm primitive
 *
 * @return  none
 */
volatile static unsigned int tdebug_cnt_poll_cnf = 0;
void MyPollCnfCb(void *pData)
{
	if (pData==NULL){
        return;
    }
    tdebug_cnt_poll_cnf++;
    mac_mlme_poll_conf_t *pCnf = (mac_mlme_poll_conf_t *)pData;
    if(pCnf->status==MAC_STA_FRAME_PENDING)//if status is MAC_STA_FRAME_PENDING,user can continue to poll
    {
//    	TL_SCHEDULE_TASK(poll_data_req_send, (void *)NULL);
    }
}



void MyBeaconIndCb(void *pData)
{
	if(pData==NULL)  return;
	zb_mlme_beacon_notify_ind_t *pInd = (zb_mlme_beacon_notify_ind_t *)pData;
    zb_superframe_spec_t frm;

    memcpy(&frm, &(pInd->panDesc.superframeSpec), sizeof(zb_superframe_spec_t));

    if (frm.associatePermit == 0) {
        return;
    }
	u8 size = received_coor.coor_size+1;
	if(size<MAX_ED_SCAN_RESULTS_SUPPORTED)
	{
		my_pan_t *pan_ptr = &received_coor.coor_beacon[received_coor.coor_size];
		pan_ptr->pan_id = pInd->panDesc.coordPanId;
		memcpy((void *)(&(pan_ptr->coor_addr)), (void *)(&(pInd->panDesc.coordAddr)), sizeof(addr_t));
		pan_ptr->lqi = pInd->panDesc.linkQuality;
		pan_ptr->channel = pInd->panDesc.logicalChannel;
		pan_ptr->permitjoin = pInd->panDesc.gtsPermit;
		received_coor.coor_size++;
	}
}
