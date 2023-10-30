/********************************************************************************************************
 * @file     tl_specific_data_ota.c
 *
 * @brief	 includes command handler to process the Telink specific audio data
 *
 * @author	 zigbee group
 * @date     Aug. 1, 2019
 *
 * @par      Copyright (c) 2019, Telink Semiconductor (Shanghai) Co., Ltd.
 *           All rights reserved.
 *
 *			 The information contained herein is confidential and proprietary property of Telink
 * 		     Semiconductor (Shanghai) Co., Ltd. and is available under the terms
 *			 of Commercial License Agreement between Telink Semiconductor (Shanghai)
 *			 Co., Ltd. and the licensee in separate contract or the terms described here-in.
 *           This heading MUST NOT be removed from this file.
 *
 * 			 Licensees are granted free, non-transferable use of the information in this
 *			 file under Mutual Non-Disclosure Agreement. NO Warranty of ANY KIND is provided.
 *
 *******************************************************************************************************/

/*
 * tl_specific_data_ota.c
 */
#include "tl_specific_data_ota.h"

extern unsigned int xcrc32 (const unsigned char *buf, int len, unsigned int init);


/**********************************************************************
 * LOCAL VARIABLES
 */
ota_preamble_t dev_otaInfo;
u8 mcuBootAddr = 0;
/**********************************************************************
 * @brief		get mcu reboot address
 *
 * return 		0 - reboot form address 0x0
 * 				1 - reboot from address 0x40000
 */
u8 mcuBootAddrGet(void)
{
	u8 flashInfo = 0;
	flash_read(OTA_TLNK_KEYWORD_ADDROFFSET, 1, &flashInfo);
	return ((flashInfo == 0x4b) ? 0 : 1);
}






#if defined ZB_COORDINATOR_ROLE || defined ZB_ROUTER_ROLE
u8 ota_bin_state;
u32 ota_headersize;

void ota_init(void)
{
	u32 file_id=0;
	ota_hdrFields_t headinfo;
	ota_signInfo_t imageinfo;
	mcuBootAddr = mcuBootAddrGet();
	u32 flashAddr = (mcuBootAddr) ? 0 : FLASH_OTA_NEWIMAGE_ADDR;
	flash_read(flashAddr, 4, (u8 *)&file_id);
	ota_bin_state = OTA_BIN_INVALID;
	memset(&dev_otaInfo,0,sizeof(dev_otaInfo));
	ota_headersize = 0;
	if(file_id==OTA_UPGRADE_FILE_ID)
	{
		flash_read(flashAddr,sizeof(ota_hdrFields_t), (u8 *)&headinfo);
		if(headinfo.totalImageSize>=OTA_MAX_IMAGE_SIZE)
		{
			ota_bin_state = OTA_BIN_INVALID;
			return;
		}

		if(headinfo.otaHdrLen<headinfo.totalImageSize)
			flash_read(flashAddr+headinfo.otaHdrLen,sizeof(ota_signInfo_t), (u8 *)&imageinfo);
		else
		{
			ota_bin_state = OTA_BIN_INVALID;
			return;
		}

		if(headinfo.totalImageSize>=OTA_MAX_IMAGE_SIZE)
		{
			ota_bin_state = OTA_BIN_INVALID;
			return;
		}
		dev_otaInfo.fileVer = headinfo.fileVer;
		dev_otaInfo.imageType = headinfo.imageType;
		dev_otaInfo.manufaurerCode = headinfo.manufaurerCode;
		dev_otaInfo.imageSize = headinfo.totalImageSize;
		ota_headersize = dev_otaInfo.imageSize;
		ota_bin_state = OTA_BIN_VALID;
	}
}



void ota_startReqHandler(u8 *pd)
{
	ota_preamble_t *pHdr = (ota_preamble_t *)pd;
	if(pHdr->manufaurerCode!=dev_otaInfo.manufaurerCode) //filter manufaurerCode
		return;

	u8 payload_len = sizeof(ota_cmd_t);
	u8 req_cmd_len = sizeof(zb_mscp_data_req_t);
	zb_mscp_data_req_t  coor_req;
	zb_mscp_data_req_t *msg = (zb_mscp_data_req_t *)&coor_req;
	if(msg){
		memset(msg, 0, req_cmd_len);
		ota_cmd_t rsp;
		rsp.hdr.appId = TL_SPECIFIC_ID_OTA;
		rsp.hdr.cmdId = TL_CMD_OTA_START_RSP;
		rsp.hdr.seqNo = TL_SPECIFIC_SEQNO_ADD;
		rsp.hdr.len = sizeof(ota_preamble_t);


		rsp.pay.fileVer = dev_otaInfo.fileVer;//fileVer;
		rsp.pay.imageType = dev_otaInfo.imageType;//imageType;
		rsp.pay.manufaurerCode = dev_otaInfo.manufaurerCode;//manufaurerCode;
		rsp.pay.imageSize = dev_otaInfo.imageSize;//max data block

		msg->srcAddr.addrMode = ZB_ADDR_16BIT_DEV_OR_BROADCAST; //16-bit short address mode

//		msg->dstAddr.addrMode = ZB_ADDR_16BIT_DEV_OR_BROADCAST; //16-bit short address mode
//		msg->dstAddr.addr.shortAddr = end_device[0].shortAddr;
		memcpy((u8 *)&msg->dstAddr,(u8 *)&dev_src_addr,sizeof(addr_t));//copy dst address info

		u8 len=0;
		tl_zbMacAttrGet(MAC_ATTR_PAN_ID,(u8 *)&msg->dstPanId,&len);

		msg->msduHandle = 0;
		msg->txOptions = MAC_TX_OPTION_ACKNOWLEDGED_BIT|MAC_TX_OPTION_INDIRECT_TRANSMISSION_BIT;
#if OTA_SECURITY
		//security setting
		msg->sec.key_id_mode = KEY_ID_MODE_KEY_EXPLICIT_8;
		msg->sec.securityLevel = SECURITY_LEVEL_ENC_MIC_64;
		msg->sec.key_source[7] = 0x07;
		msg->sec.key_source[6] = 0x06;
		msg->sec.key_source[5] = 0x05;
		msg->sec.key_source[4] = 0x04;
		msg->sec.key_source[3] = 0x03;
		msg->sec.key_source[2] = 0x02;
		msg->sec.key_source[1] = 0x01;
		msg->sec.key_source[0] = 0x00;
		msg->sec.key_index = 0x05;
#endif

		u8 *ptr = (u8 *)&rsp;
		tl_MacMcpsDataRequestSend(coor_req,ptr,payload_len);
	}
}



void ota_stopReqHandler(u8 *pd)
{
	ota_preamble_t *pHdr = (ota_preamble_t *)pd;

	if((pHdr->fileVer==dev_otaInfo.fileVer)&&
	   (pHdr->imageType==dev_otaInfo.imageType)&&
	   (pHdr->manufaurerCode==dev_otaInfo.manufaurerCode)&&
	   (pHdr->imageSize==dev_otaInfo.imageSize))
	{
		u8 payload_len = sizeof(ota_cmd_t);
		u8 req_cmd_len = sizeof(zb_mscp_data_req_t);
		zb_mscp_data_req_t  coor_req;
		zb_mscp_data_req_t *msg = (zb_mscp_data_req_t *)&coor_req;
		if(msg){
			memset(msg, 0, req_cmd_len);
			ota_cmd_t rsp;
			rsp.hdr.appId = TL_SPECIFIC_ID_OTA;
			rsp.hdr.cmdId = TL_CMD_OTA_STOP_RSP;
			rsp.hdr.seqNo = TL_SPECIFIC_SEQNO_ADD;
			rsp.hdr.len = sizeof(ota_preamble_t);

			rsp.pay.fileVer = dev_otaInfo.fileVer;//dev_otaInfo.fileVer;
			rsp.pay.imageType = dev_otaInfo.imageType;//dev_otaInfo.imageType;
			rsp.pay.manufaurerCode = dev_otaInfo.manufaurerCode;//dev_otaInfo.manufaurerCode;
			rsp.pay.imageSize = dev_otaInfo.imageSize;//max data block

			msg->srcAddr.addrMode = ZB_ADDR_16BIT_DEV_OR_BROADCAST; //16-bit short address mode

//			msg->dstAddr.addrMode = ZB_ADDR_16BIT_DEV_OR_BROADCAST; //16-bit short address mode
//			msg->dstAddr.addr.shortAddr = end_device[0].shortAddr;
			memcpy((u8 *)&msg->dstAddr,(u8 *)&dev_src_addr,sizeof(addr_t));

			u8 len=0;
			tl_zbMacAttrGet(MAC_ATTR_PAN_ID,(u8 *)&msg->dstPanId,&len);

			msg->msduHandle = 0;
			msg->txOptions = MAC_TX_OPTION_ACKNOWLEDGED_BIT|MAC_TX_OPTION_INDIRECT_TRANSMISSION_BIT;
	#if OTA_SECURITY
			//security setting
			msg->sec.key_id_mode = KEY_ID_MODE_KEY_EXPLICIT_8;
			msg->sec.securityLevel = SECURITY_LEVEL_ENC_MIC_64;
			msg->sec.key_source[7] = 0x07;
			msg->sec.key_source[6] = 0x06;
			msg->sec.key_source[5] = 0x05;
			msg->sec.key_source[4] = 0x04;
			msg->sec.key_source[3] = 0x03;
			msg->sec.key_source[2] = 0x02;
			msg->sec.key_source[1] = 0x01;
			msg->sec.key_source[0] = 0x00;
			msg->sec.key_index = 0x05;
	#endif
			u8 *ptr = (u8 *)&rsp;
			tl_MacMcpsDataRequestSend(coor_req,ptr,payload_len);
		}
	}

}



void ota_dataReqHandler(u8 *pd)
{
	ota_preamble_t *pdHdr = (ota_preamble_t *)pd;

	if((pdHdr->fileVer==dev_otaInfo.fileVer)&&
	   (pdHdr->imageType==dev_otaInfo.imageType)&&
	   (pdHdr->manufaurerCode==dev_otaInfo.manufaurerCode)&&
	   (ota_bin_state == OTA_BIN_VALID))
	{
		u8 payload_len = sizeof(ota_data_t);
		u8 req_cmd_len = sizeof(zb_mscp_data_req_t);
		zb_mscp_data_req_t  coor_req;
		zb_mscp_data_req_t *msg = (zb_mscp_data_req_t *)&coor_req;
		if(msg){
			memset(msg, 0, req_cmd_len);
			ota_data_t rsp;
			rsp.hdr.appId = TL_SPECIFIC_ID_OTA;
			rsp.hdr.cmdId = TL_CMD_OTA_DATA_RSP;
			rsp.hdr.seqNo = TL_SPECIFIC_SEQNO_ADD;


			u8 read_len = 0;

			u32 flashAddr = (mcuBootAddr) ? 0 : FLASH_OTA_NEWIMAGE_ADDR;

			u32 ota_headercnt = pdHdr->imageSize*OTA_FRAMESIZE;
			if((ota_headercnt+OTA_FRAMESIZE)<ota_headersize)
				read_len = OTA_FRAMESIZE;
			else
				read_len = ota_headersize%OTA_FRAMESIZE;
			flash_read(flashAddr+ota_headercnt, read_len, (u8 *)rsp.pay.payload);

			rsp.pay.fileVer = dev_otaInfo.fileVer;//dev_otaInfo.fileVer;
			rsp.pay.imageType = dev_otaInfo.imageType;//dev_otaInfo.imageType;
			rsp.pay.manufaurerCode = dev_otaInfo.manufaurerCode;//dev_otaInfo.manufaurerCode;
			rsp.pay.imageSize = pdHdr->imageSize;
			rsp.pay.payloadSize = read_len;//max data block

			rsp.hdr.len = sizeof(ota_dataFrame_t) - OTA_FRAMESIZE;
			rsp.hdr.len += read_len;

			payload_len -=  OTA_FRAMESIZE;
			payload_len +=  read_len;

			msg->srcAddr.addrMode = ZB_ADDR_16BIT_DEV_OR_BROADCAST; //16-bit short address mode

//			msg->dstAddr.addrMode = ZB_ADDR_16BIT_DEV_OR_BROADCAST; //16-bit short address mode
//			msg->dstAddr.addr.shortAddr = end_device[0].shortAddr;
			memcpy((u8 *)&msg->dstAddr,(u8 *)&dev_src_addr,sizeof(addr_t));

			u8 len=0;
			tl_zbMacAttrGet(MAC_ATTR_PAN_ID,(u8 *)&msg->dstPanId,&len);

			msg->msduHandle = 0;
			msg->txOptions = MAC_TX_OPTION_ACKNOWLEDGED_BIT|MAC_TX_OPTION_INDIRECT_TRANSMISSION_BIT;
#if OTA_SECURITY
			//security setting
			msg->sec.key_id_mode = KEY_ID_MODE_KEY_EXPLICIT_8;
			msg->sec.securityLevel = SECURITY_LEVEL_ENC_MIC_64;
			msg->sec.key_source[7] = 0x07;
			msg->sec.key_source[6] = 0x06;
			msg->sec.key_source[5] = 0x05;
			msg->sec.key_source[4] = 0x04;
			msg->sec.key_source[3] = 0x03;
			msg->sec.key_source[2] = 0x02;
			msg->sec.key_source[1] = 0x01;
			msg->sec.key_source[0] = 0x00;
			msg->sec.key_index = 0x05;
#endif
			u8 *ptr = (u8 *)&rsp;
			tl_MacMcpsDataRequestSend(coor_req,ptr,payload_len);
		}
	}
}


#elif defined ZB_ED_ROLE
#define OTA_MAX_IMAGE_BLOCK_RETRIES					10
#define OTA_MAX_IMAGE_BLOCK_RSP_WAIT_TIME			5//s

ota_preamble_t binInfo;

//otaTimer
ev_timer_event_t otaQueryTimer;
ev_timer_event_t *otaDataReqTimer = NULL;
u8 	ota_state=0,ota_reqRetryCnt=0;
u32 blockDataCount=0;

u32 ota_headercnt=0,ota_headersize=0,crcValue=0;
u32 flashOffsetAddr=0;
ota_periodiccallback_t ota_dataCb = NULL;


void ota_init(void)
{
	memset((u8 *)&dev_otaInfo,0,sizeof(ota_preamble_t));
	memset((u8 *)&binInfo,0,sizeof(ota_preamble_t));

	crcValue = 0xffffffff;
	flashOffsetAddr = 0;
	blockDataCount=0;
	mcuBootAddr = mcuBootAddrGet();
}



void ota_mcuReboot(void)
{
	u32 baseAddr = 0;
	u32 newAddr = FLASH_OTA_NEWIMAGE_ADDR;
	u8 flashInfo = 0x4b;
	if(mcuBootAddr){
		baseAddr = FLASH_OTA_NEWIMAGE_ADDR;
		newAddr = 0;
	}
	flash_write((newAddr + OTA_TLNK_KEYWORD_ADDROFFSET),1,&flashInfo);//enable boot-up flag
	flashInfo = 0;
	flash_write((baseAddr + OTA_TLNK_KEYWORD_ADDROFFSET),1,&flashInfo);//disable boot-up flag
	SYSTEM_RESET();//reset
}




//TL_CMD_OTA_START_RSP
void ota_startRspHandler(u8 *pd)
{
	ota_preamble_t *pHdr = (ota_preamble_t *)pd;

	if((pHdr->manufaurerCode==MANUFACTURER_CODE)&&
	   (pHdr->imageType==IMAGE_TYPE)&&
	   (pHdr->fileVer>FILE_VERSION)&&
	   (ota_state==OTA_STA_IDLE))
	{
		if(pHdr->imageSize>0&&pHdr->imageSize<OTA_MAX_IMAGE_SIZE)
		{
			dev_otaInfo.fileVer = pHdr->fileVer;
			dev_otaInfo.imageType = pHdr->imageType;
			dev_otaInfo.manufaurerCode = pHdr->manufaurerCode;
			dev_otaInfo.imageSize = pHdr->imageSize;


			u32 baseAddr = (mcuBootAddr) ? 0 : FLASH_OTA_NEWIMAGE_ADDR;
			u16 sectorNumUsed = (pHdr->imageSize / FLASH_SECTOR_SIZE) + 1;
			for(u16 i = 0; i < sectorNumUsed; i++)
			{
				flash_erase(baseAddr + i * FLASH_SECTOR_SIZE);
			}
			ota_state = OTA_STA_STARTING;
			ev_unon_timer(&otaQueryTimer);//stop poll rate
			ota_dataReq();
			mac_set_pollRate(OTA_POLL_RATE);//data request
			ota_dataCb = ota_dataReq;
			ota_dataReqRetry(OTA_MAX_IMAGE_BLOCK_RSP_WAIT_TIME);
		}
	}
}


//TL_CMD_OTA_STOP_RSP
void ota_stopRspHandler(u8 *pd)
{
	if(ota_state != OTA_STA_END) return;
	ota_preamble_t *pHdr = (ota_preamble_t *)pd;
	if((pHdr->manufaurerCode==binInfo.manufaurerCode)&&
	   (pHdr->imageType==binInfo.imageType)&&
	   (pHdr->fileVer==binInfo.fileVer)&&
	   (pHdr->imageSize==(binInfo.imageSize + ota_headersize)))
	{
		ota_state = OTA_STA_IDLE;
		ota_mcuReboot();
	}
	else
	{
		ota_state = OTA_STA_IDLE;
		if(otaDataReqTimer)
			TL_ZB_TIMER_CANCEL(&otaDataReqTimer);
		ota_queryStart(QUERY_RATE);//ota query 30S
		mac_set_pollRate(NORMAL_POLL_RATE);//data request
	}
}



//TL_CMD_OTA_DATA_RSP
void ota_DataProcess(u8 len, u8 *pData)
{
	u8 i=0;
	if(ota_state == OTA_STA_STARTING)
	{
		for(i = 0; i < len; i++)
		{
			if(ota_headercnt==OTA_HEAD_LENGTH)//head size
			{
				ota_headersize = (*(u8 *)(pData+i))|((*(u8 *)(pData+i+1))<<8);
				ota_headersize += 6;
			}
			else if(ota_headercnt==OTA_HEAD_MAUNCODE)
			{
				binInfo.manufaurerCode = (*(u8 *)(pData+i))|(*(u8 *)(pData+i+1)<<8);
			}
			else if(ota_headercnt==OTA_HEAD_IMAGETYPE)
			{
				binInfo.imageType = (*(u8 *)(pData+i))|(*(u8 *)(pData+i+1)<<8);
			}
			else if(ota_headercnt==OTA_HEAD_FILEVER)
			{
				binInfo.fileVer = (*(u8 *)(pData+i))|(*(u8 *)(pData+i+1)<<8)|(*(u8 *)(pData+i+2)<<16)|(*(u8 *)(pData+i+3)<<24);
			}
			else if(ota_headercnt==OTA_HEAD_IMAGESIZE)
			{
				binInfo.imageSize = (*(u8 *)(pData+i))|(*(u8 *)(pData+i+1)<<8)|(*(u8 *)(pData+i+2)<<16)|(*(u8 *)(pData+i+3)<<24);
			}
			else if((ota_headercnt==ota_headersize)&&(ota_headersize!=0))
			{
				ota_state = OTA_STA_DATAING;
				break;
			}
			ota_headercnt++;
		}
	}
	if(ota_state == OTA_STA_DATAING)
	{
		u8 datalen=0;

		if(len>0)
		{
			datalen = len - i;
			u32 ota_headerbuf = ota_headercnt;
			ota_headercnt += datalen;
			u32 baseAddr = (mcuBootAddr) ? 0 : FLASH_OTA_NEWIMAGE_ADDR;
			u8 flag=0;
			if((flashOffsetAddr<=OTA_TLNK_KEYWORD_ADDROFFSET)&&((datalen+flashOffsetAddr)>=OTA_TLNK_KEYWORD_ADDROFFSET)&&(datalen>0))
			{
					flag = 1;
					pData[i+OTA_TLNK_KEYWORD_ADDROFFSET-flashOffsetAddr] = 0xff;
			}
			cfs_flash_write(baseAddr + flashOffsetAddr, datalen, &pData[i]);

			if(flag)
				pData[i+OTA_TLNK_KEYWORD_ADDROFFSET-flashOffsetAddr] = 0x4b;

			flashOffsetAddr += datalen;

			if(ota_headercnt>=(binInfo.imageSize+ota_headersize-4))//CRC
			{
				if(ota_headerbuf>=(binInfo.imageSize+ota_headersize-4))
				{
					datalen=0;
				}
				else
				{
					u16 offset = ota_headercnt-(binInfo.imageSize+ota_headersize-4);
					if(datalen>offset)
						datalen -= offset;
					else
						datalen=0;
				}
			}
			if(datalen)
			{
				crcValue = xcrc32(&pData[i], datalen, crcValue);
			}

		}
	}
}



void ota_dataRspHandler(u8 *pd)
{
	if(ota_state == OTA_STA_IDLE) return;
	ota_dataFrame_t *pHdr = (ota_dataFrame_t *)pd;

	if((pHdr->manufaurerCode==dev_otaInfo.manufaurerCode)&&
	   (pHdr->imageType==dev_otaInfo.imageType)&&
	   (pHdr->fileVer==dev_otaInfo.fileVer)&&
	   (pHdr->imageSize==blockDataCount))
	{

		if(pHdr->payloadSize==0)
		{
			if(otaDataReqTimer)
				TL_ZB_TIMER_CANCEL(&otaDataReqTimer);
			ota_state = OTA_STA_IDLE;
			ota_queryStart(QUERY_RATE);//ota query 30S
			mac_set_pollRate(NORMAL_POLL_RATE);//data request
			return;
		}
		ota_DataProcess(pHdr->payloadSize,pHdr->payload);

		blockDataCount++;
		if(ota_headercnt>=dev_otaInfo.imageSize)
		{
			u32 baseAddr = (mcuBootAddr) ? 0 : FLASH_OTA_NEWIMAGE_ADDR;
		    u32 crcBin=0;
			flash_read(baseAddr + dev_otaInfo.imageSize-ota_headersize-4, 4, (u8 *)&crcBin);

			if(crcBin==crcValue)
			{
				ota_state = OTA_STA_END;
				ota_stopReq();
				ota_dataCb = ota_stopReq;
				ota_dataReqRetry(OTA_MAX_IMAGE_BLOCK_RSP_WAIT_TIME);
			}
			else
			{
				dev_otaInfo.imageSize = 0;
				if(otaDataReqTimer)
					TL_ZB_TIMER_CANCEL(&otaDataReqTimer);
				ota_state = OTA_STA_IDLE;
				ota_queryStart(QUERY_RATE);//ota query 30S
				mac_set_pollRate(NORMAL_POLL_RATE);//data request
			}
		}
		else
		{
			ota_dataReq();
			ota_dataCb = ota_dataReq;
			ota_dataReqRetry(OTA_MAX_IMAGE_BLOCK_RSP_WAIT_TIME);
		}

	}
}




void ota_startReq(void)
{
	ota_cmd_t req;
	u8 payload_len = sizeof(ota_cmd_t);
	u8 req_cmd_len = sizeof(zb_mscp_data_req_t);
	zb_mscp_data_req_t	dev_req;
	zb_mscp_data_req_t *msg = (zb_mscp_data_req_t*)&dev_req;
	if(msg)
	{
		memset(msg, 0, req_cmd_len);
		req.hdr.appId = TL_SPECIFIC_ID_OTA;
		req.hdr.cmdId = TL_CMD_OTA_START_REQ;
		req.hdr.seqNo = TL_SPECIFIC_SEQNO_ADD;
		req.hdr.len = sizeof(ota_preamble_t);

		req.pay.fileVer = FILE_VERSION;
		req.pay.imageType = IMAGE_TYPE;
		req.pay.manufaurerCode = MANUFACTURER_CODE;
		req.pay.imageSize = 0;

		msg->srcAddr.addrMode = ZB_ADDR_16BIT_DEV_OR_BROADCAST; //16-bit short address mode
		msg->dstAddr.addrMode = ZB_ADDR_64BIT_DEV; //16-bit short address mode

		u8 len=0;
		tl_zbMacAttrGet(MAC_ATTR_PAN_ID,(u8 *)&msg->dstPanId,&len);
		tl_zbMacAttrGet(MAC_ATTR_COORDINATOR_EXTENDED_ADDRESS,(u8 *)&msg->dstAddr.addr.extAddr,&len);

		msg->msduHandle = 0;
		msg->txOptions = MAC_TX_OPTION_ACKNOWLEDGED_BIT;
#if OTA_SECURITY
		//security setting
		msg->sec.key_id_mode = KEY_ID_MODE_KEY_EXPLICIT_8;
		msg->sec.securityLevel = SECURITY_LEVEL_ENC_MIC_64;
		msg->sec.key_source[7] = 0x07;
		msg->sec.key_source[6] = 0x06;
		msg->sec.key_source[5] = 0x05;
		msg->sec.key_source[4] = 0x04;
		msg->sec.key_source[3] = 0x03;
		msg->sec.key_source[2] = 0x02;
		msg->sec.key_source[1] = 0x01;
		msg->sec.key_source[0] = 0x00;
		msg->sec.key_index = 0x05;
#endif
		u8 *ptr = (u8 *)&req;
		tl_MacMcpsDataRequestSend(dev_req,ptr,payload_len);
	}
}



void ota_dataReq(void)
{
	ota_cmd_t req;
	u8 payload_len = sizeof(ota_cmd_t);
	u8 req_cmd_len = sizeof(zb_mscp_data_req_t);
	zb_mscp_data_req_t dev_req;
	zb_mscp_data_req_t *msg = (zb_mscp_data_req_t *)&dev_req;
	if(msg){
		memset(msg, 0, req_cmd_len);
		req.hdr.appId = TL_SPECIFIC_ID_OTA;
		req.hdr.cmdId = TL_CMD_OTA_DATA_REQ;
		req.hdr.seqNo = TL_SPECIFIC_SEQNO_ADD;
		req.hdr.len = sizeof(ota_preamble_t);

		req.pay.fileVer = dev_otaInfo.fileVer;//rc_otaInfo.fileVer;
		req.pay.imageType = dev_otaInfo.imageType;//rc_otaInfo.imageType;
		req.pay.manufaurerCode = dev_otaInfo.manufaurerCode;//rc_otaInfo.manufaurerCode;
		req.pay.imageSize = blockDataCount;//max data block

		msg->srcAddr.addrMode = ZB_ADDR_16BIT_DEV_OR_BROADCAST; //16-bit short address mode
		msg->dstAddr.addrMode = ZB_ADDR_64BIT_DEV; //16-bit short address mode

		u8 len=0;
		tl_zbMacAttrGet(MAC_ATTR_PAN_ID,(u8 *)&msg->dstPanId,&len);
		tl_zbMacAttrGet(MAC_ATTR_COORDINATOR_EXTENDED_ADDRESS,(u8 *)&msg->dstAddr.addr.extAddr,&len);

		msg->msduHandle = 0;
		msg->txOptions = MAC_TX_OPTION_ACKNOWLEDGED_BIT;
#if OTA_SECURITY
		//security setting
		msg->sec.key_id_mode = KEY_ID_MODE_KEY_EXPLICIT_8;
		msg->sec.securityLevel = SECURITY_LEVEL_ENC_MIC_64;
		msg->sec.key_source[7] = 0x07;
		msg->sec.key_source[6] = 0x06;
		msg->sec.key_source[5] = 0x05;
		msg->sec.key_source[4] = 0x04;
		msg->sec.key_source[3] = 0x03;
		msg->sec.key_source[2] = 0x02;
		msg->sec.key_source[1] = 0x01;
		msg->sec.key_source[0] = 0x00;
		msg->sec.key_index = 0x05;
#endif
		u8 *ptr = (u8 *)&req;
		tl_MacMcpsDataRequestSend(dev_req,ptr,payload_len);
	}
}


void ota_stopReq(void)
{
	ota_cmd_t req;
	u8 payload_len = sizeof(ota_cmd_t);
	u8 req_cmd_len = sizeof(zb_mscp_data_req_t);
	zb_mscp_data_req_t dev_req;
	zb_mscp_data_req_t *msg = (zb_mscp_data_req_t*)&dev_req;
	if(msg)
	{
		memset(msg, 0, req_cmd_len);
		req.hdr.appId = TL_SPECIFIC_ID_OTA;
		req.hdr.cmdId = TL_CMD_OTA_STOP_REQ;
		req.hdr.seqNo = TL_SPECIFIC_SEQNO_ADD;
		req.hdr.len = sizeof(ota_preamble_t);

		req.pay.fileVer = binInfo.fileVer;//FILE_VERSION;
		req.pay.imageType = binInfo.imageType;//IMAGE_TYPE;
		req.pay.manufaurerCode = binInfo.manufaurerCode;//MANUFACTURER_CODE;
		req.pay.imageSize = binInfo.imageSize + ota_headersize;

		msg->srcAddr.addrMode = ZB_ADDR_16BIT_DEV_OR_BROADCAST; //16-bit short address mode
		msg->dstAddr.addrMode = ZB_ADDR_64BIT_DEV; //16-bit short address mode

		u8 len=0;
		tl_zbMacAttrGet(MAC_ATTR_PAN_ID,(u8 *)&msg->dstPanId,&len);
		tl_zbMacAttrGet(MAC_ATTR_COORDINATOR_EXTENDED_ADDRESS,(u8 *)&msg->dstAddr.addr.extAddr,&len);

		msg->msduHandle = 0;
		msg->txOptions = MAC_TX_OPTION_ACKNOWLEDGED_BIT;
#if OTA_SECURITY
		//security setting
		msg->sec.key_id_mode = KEY_ID_MODE_KEY_EXPLICIT_8;
		msg->sec.securityLevel = SECURITY_LEVEL_ENC_MIC_64;
		msg->sec.key_source[7] = 0x07;
		msg->sec.key_source[6] = 0x06;
		msg->sec.key_source[5] = 0x05;
		msg->sec.key_source[4] = 0x04;
		msg->sec.key_source[3] = 0x03;
		msg->sec.key_source[2] = 0x02;
		msg->sec.key_source[1] = 0x01;
		msg->sec.key_source[0] = 0x00;
		msg->sec.key_index = 0x05;
#endif
		u8 *ptr = (u8 *)&req;
		tl_MacMcpsDataRequestSend(dev_req,ptr,payload_len);
	}
}




void ota_dataReqRetry(u16 seconds)
{
	ota_reqRetryCnt=0;
	if(seconds)
	{
		if(otaDataReqTimer)
			TL_ZB_TIMER_CANCEL(&otaDataReqTimer);
		otaDataReqTimer = TL_ZB_TIMER_SCHEDULE(ota_periodicDataServerCb,NULL, seconds * 1000);
	}
}





//30S
void ota_queryStart(u16 seconds)
{
	ota_init();
	ota_state = OTA_STA_IDLE;
	if(!ev_timer_exist(&otaQueryTimer)){
		otaQueryTimer.cb = ota_periodicQueryServerCb;
		otaQueryTimer.data = NULL;
		ev_on_timer(&otaQueryTimer, seconds * 1000);
	}
}



int ota_periodicQueryServerCb(void *data)
{
	ota_startReq();//send start req
	return 0;
}



int ota_periodicDataServerCb(void *data)
{
	int ret = 0;
	ota_reqRetryCnt++;
	if(ota_reqRetryCnt>=OTA_MAX_IMAGE_BLOCK_RETRIES)
	{
		ota_reqRetryCnt = 0;
		ota_queryStart(QUERY_RATE);//ota query 30S
		mac_set_pollRate(NORMAL_POLL_RATE);//data request
		otaDataReqTimer = NULL;
		ret = -1;
	}
	else
	{
		if(ota_dataCb)
			ota_dataCb();
	}
	return ret;
}

#endif


volatile u16 seq_number;

u8  tl_appOtaCmdHandler(u8 *pd)
{
	u8 ret = INVALID_EVENT_ID;
	tl_appFrameHdr_t *pHdr = (tl_appFrameHdr_t *)pd;
	u8 *pld = ((tl_appFrameFmt_t *)pd)->payload;
#if defined ZB_COORDINATOR_ROLE || defined ZB_ROUTER_ROLE
	if(pHdr->cmdId == TL_CMD_OTA_START_REQ){//from device
		ota_startReqHandler(pld);
		seq_number = 0x100;
		ret = SUCCESS;
	}else if(pHdr->cmdId == TL_CMD_OTA_STOP_REQ){
		ota_stopReqHandler(pld);
		ret = SUCCESS;
	}else if(pHdr->cmdId == TL_CMD_OTA_DATA_REQ){
		if(seq_number!=pHdr->seqNo)
		{
		seq_number = pHdr->seqNo;
		ota_dataReqHandler(pld);
		}
		ret = SUCCESS;
	}
#elif defined ZB_ED_ROLE
	if(pHdr->cmdId == TL_CMD_OTA_START_RSP){//from coordinator
		ota_startRspHandler(pld);
		ret = SUCCESS;
	}else if(pHdr->cmdId == TL_CMD_OTA_STOP_RSP){
		ota_stopRspHandler(pld);
		ret = SUCCESS;
	}else if(pHdr->cmdId == TL_CMD_OTA_DATA_RSP){
		ota_dataRspHandler(pld);
		ret = SUCCESS;
	}
#endif
	return ret;
}





