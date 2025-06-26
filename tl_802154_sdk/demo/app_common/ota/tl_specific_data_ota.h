/*
 * mouse_type.h
 *
 *  Created on: Feb 10, 2014
 *      Author: xuzhen
 */

#ifndef _TL_SPECIFIC_DATA_OTA_H_
#define _TL_SPECIFIC_DATA_OTA_H_
#include <ieee802154_common.h>
#include "../../../proj/tl_common.h"
#include "../tl_specific_data.h"
#if defined ZB_COORDINATOR_ROLE || defined ZB_ROUTER_ROLE
#include "../../associate_coor/app_cfg.h"
#include "../../associate_coor/version_cfg.h"
#include "../../associate_coor/app.h"
#elif defined ZB_ED_ROLE
#include "../../associate_dev/app_cfg.h"
#include <version_cfg.h>
#include "../../associate_dev/app.h"
#endif
#define		OTA_SECURITY		0



/**********************************************************************
 * MACRO DEFINES
 */
#define   MANUFACTURER_CODE                  		MANUFACTURER_CODE_TELINK		//used in OTA

#define   STARTREQ_RETRYCNT							5
#define   DATAREQ_RETRYCNT							5
#define   STARTREQ_TIME_S							1
#define   DATAREQ_TIME_MS							20
#define   DATATRY_TIME_MS							1000
#define   STOPREQ_TIME_MS							100
#define   OTA_FRAMESIZE								0x30
#define   OTA_UPGRADE_FILE_ID						0x0BEEF11E


//flash address
#define	FLASH_OTA_NEWIMAGE_ADDR						0x40000//256K
#if defined(MCU_CORE_B91)
#define	OTA_TLNK_KEYWORD_ADDROFFSET					0x20
#else
#define	OTA_TLNK_KEYWORD_ADDROFFSET					8
#endif
#define	OTA_MAX_IMAGE_SIZE							0x30000

//callback
typedef void (*ota_periodiccallback_t)(void);
/*
 * ota command
 * */
typedef enum{
	TL_CMD_OTA_START_REQ 		= 0x01,
	TL_CMD_OTA_START_RSP 		= 0x02,
	TL_CMD_OTA_STOP_REQ 		= 0x03,
	TL_CMD_OTA_STOP_RSP 		= 0x04,
	TL_CMD_OTA_DATA_REQ 		= 0x05,
	TL_CMD_OTA_DATA_RSP 		= 0x06,
}tl_otaCmdId_e;


enum{
	OTA_STA_IDLE      = 0x00,		//!<  No Error Occurred
	OTA_STA_STARTING  = 0x01,		//!<  Operation not permitted
	OTA_STA_DATAING   = 0x02,		//!<  Operation not permitted
	OTA_STA_END  	  = 0x03,		//!<  Operation not permitted
}tl_otaState_e;


enum{
	OTA_BIN_VALID      = 0x00,		//!<  No Error Occurred
	OTA_BIN_INVALID    = 0x01,		//!<  Operation not permitted
}tl_otabin_e;


enum{
	OTA_TYPE_BIN       = 0x00,		//image
}image_type_e;


enum{
	OTA_HEAD_LENGTH   = 0x06,		//!<  No Error Occurred
	OTA_HEAD_MAUNCODE    = 0x0a,		//!<  Operation not permitted
	OTA_HEAD_IMAGETYPE  = 0x0c,		//!<  Operation not permitted
	OTA_HEAD_FILEVER   = 0x0e,		//!<  Operation not permitted
	OTA_HEAD_IMAGESIZE   = 0x3a,		//!<  Operation not permitted
}tl_otaHeaderLocation_e;


typedef struct _attribute_packed_{
	u32			fileVer;
	u16			imageType;
	u16			manufaurerCode;
	u32			imageSize;	//The value represents the total image size in bytes.
}ota_preamble_t;

typedef struct _attribute_packed_{
	u32			fileVer;
	u16			imageType;
	u16			manufaurerCode;
	u32			imageSize;	//The value represents the total image size in bytes.
	u8			payloadSize;
	u8			payload[OTA_FRAMESIZE];
}ota_dataFrame_t;

typedef struct _attribute_packed_{
	/*The value is a unique 4-byte value that is included at the beginning of all ZigBee OTA upgrade image
	files in order to quickly identify and distinguish the file as being a ZigBee OTA cluster upgrade file,
	without having to examine the whole file content. This helps distinguishing the file from other file
	types on disk. The value is defined to be ¡°0x0BEEF11E¡±.*/
	u32			otaUpgradeFileID;

	/*The current OTA header version shall be 0x0100 with major version of ¡°01¡± and minor version of ¡°00¡±.*/
	u16			otaHdrVer;

	/*This value indicates full length of the OTA header in bytes, including the OTA upgrade file identifier,
	OTA header length itself to any optional fields.*/
	u16			otaHdrLen;
//	The bit mask indicates whether additional information such as Image Signature or Signing Certificate
//	are included as part of the OTA Upgrade Image.
	u16			otaHdrFC;

	u16			manufaurerCode;

	/*The manufacturer should assign an appropriate and unique image type value to each of its devices in
	651 order to distinguish the products.*/
	u16			imageType;

	//
	u32			fileVer;
	u16			zbStackVer;
	//TLNK
	u8			otaHdrString[32];
	//The value represents the total image size in bytes.
	u32			totalImageSize;

//	u8			securityCredVer;//option ����
//	addrExt_t	fileDest;
//	u16			minHdrVer;
//	u16			maxHdrVer;
}ota_hdrFields_t;



typedef struct{
	u16			BinType;
	u32			BinSize;
}ota_signInfo_t;

typedef struct{
	tl_appFrameHdr_t	hdr;
	ota_dataFrame_t  	pay;
}ota_data_t;

typedef struct{
	tl_appFrameHdr_t	hdr;
	ota_preamble_t  	pay;
}ota_cmd_t;



//functions
u8  tl_appOtaCmdHandler(u8 *pd);




void ota_init(void);
int ota_periodicQueryServerCb(void *data);
int ota_periodicDataServerCb(void *data);
void ota_startReq(void);
void ota_dataReq(void);
void ota_stopReq(void);
void ota_dataReqRetry(u16 seconds);
void ota_queryStart(u16 seconds);


#endif /* TL_AUDIO_H_ */
