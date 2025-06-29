/********************************************************************************************************
 * @file	zb_common.h
 *
 * @brief	This is the header file for zb_common
 *
 * @author	Zigbee Group
 * @date	2019
 *
 * @par     Copyright (c) 2019, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
 *
 *          Redistribution and use in source and binary forms, with or without
 *          modification, are permitted provided that the following conditions are met:
 *
 *              1. Redistributions of source code must retain the above copyright
 *              notice, this list of conditions and the following disclaimer.
 *
 *              2. Unless for usage inside a TELINK integrated circuit, redistributions
 *              in binary form must reproduce the above copyright notice, this list of
 *              conditions and the following disclaimer in the documentation and/or other
 *              materials provided with the distribution.
 *
 *              3. Neither the name of TELINK, nor the names of its contributors may be
 *              used to endorse or promote products derived from this software without
 *              specific prior written permission.
 *
 *              4. This software, with or without modification, must only be used with a
 *              TELINK integrated circuit. All other usages are subject to written permission
 *              from TELINK and different commercial license may apply.
 *
 *              5. Licensee shall be solely responsible for any claim to the extent arising out of or
 *              relating to such deletion(s), modification(s) or alteration(s).
 *
 *          THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 *          ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *          WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *          DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDER BE LIABLE FOR ANY
 *          DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *          (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *          LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *          ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *          (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *          SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *******************************************************************************************************/
#ifndef ZB_COMMON_H
#define ZB_COMMON_H

/**********************************************************************
 * INCLUDES
 */
#include <ieee802154_buffer.h>
#include <ieee802154_config.h>
#include <ieee802154_task_queue.h>
#include "tl_common.h"

#include "tn_list.h"
#include "tn_mm.h"

#include "../../mac/includes/mac_phy.h"
#include "../../mac/includes/mac_security.h"
#include "../../mac/includes/mac_trx_api.h"
#include "../../mac/includes/tl_zb_mac.h"
#include "../../mac/includes/tl_zb_mac_pib.h"
#include "../../mac/includes/upper_layer.h"

/**********************************************************************
 * TYPEDEFS
 */
//for .bin map
#if 0
#define _CODE_MAC_  	__attribute__((section(".sect_mac")))
#else
#define _CODE_MAC_
#endif

/*
   functions return type.
   In general, function can return OK, BLOCKED or some error. Errors
   are negative.
   Error can be "generic" or some additional error code.
*/
enum zb_ret_e{
	/* Most common return types: ok, generic error, BLOCKED, thread exit indication. */
	RET_OK              				 = 0,
	RET_ERROR           					,
	RET_BLOCKED        						,
	RET_EXIT           						,
	RET_BUSY           						,
	RET_EOF             					,
	RET_OUT_OF_RANGE    					,
	RET_EMPTY           					,
	RET_CANCELLED       					,
	RET_PENDING                             ,
	RET_NO_MEMORY                           ,
	RET_INVALID_PARAMETER                   ,
	RET_OPERATION_FAILED                    ,
	RET_BUFFER_TOO_SMALL                    ,
	RET_END_OF_LIST                         ,
	RET_ALREADY_EXISTS                      ,
	RET_NOT_FOUND                           ,  //0x10
	RET_OVERFLOW                            ,
	RET_TIMEOUT                             ,
	RET_NOT_IMPLEMENTED                     ,
	RET_NO_RESOURCES                        ,
	RET_UNINITIALIZED                       ,
	RET_NO_SERVER                           ,
	RET_INVALID_STATE                       ,
	RET_DOES_NOT_EXIST                      ,
	RET_CONNECTION_FAILED                   ,
	RET_CONNECTION_LOST                     ,
	RET_CANCELLED_BY_USER                   ,
	RET_UNAUTHORIZED                        ,
	RET_CONFLICT                            ,
	RET_COULD_NOT_OPEN_FILE                 ,
	RET_NO_MATCH                            ,
	RET_PROTOCOL_ERROR                      ,  //0x20
	RET_VERSION                             ,
	RET_MALFORMED_ADDRESS                   ,
	RET_COULD_NOT_READ_FILE                 ,
	RET_FILE_NOT_FOUND                      ,
	RET_DIRECTORY_NOT_FOUND                 ,
	RET_CONVERSION_ERROR                    ,
	RET_INCOMPATIBLE_TYPES                  ,
	RET_INCOMPATIBLE_TYPES_IN_COMPARE       ,
	RET_INCOMPATIBLE_TYPES_IN_ASSIGNMENT    ,
	RET_INCOMPATIBLE_TYPES_IN_EXPRESSION    ,
	RET_ILLEGAL_COMPARE_OPERATION           ,
	RET_FILE_CORRUPTED                      ,
	RET_PAGE_NOT_FOUND                      ,
	RET_FILE_WRITE_ERROR                    ,
	RET_FILE_READ_ERROR                     ,
	RET_FILE_PARTIAL_WRITE                  ,   //0x30
	RET_TOO_MANY_OPEN_FILES                 ,
	RET_ILLEGAL_REQUEST                     ,
	RET_INVALID_BINDING                     ,
	RET_INVALID_GROUP                       ,
	RET_TABLE_FULL                          ,
	RET_NO_ACK                              ,
	RET_ACK_OK                              ,
	RET_NO_BOUND_DEVICE                     ,   //0x38
	RET_BUF_FULL
}zb_ret_t;

enum{
    START_VAR_ID = 0,
    MAC_PIB_ID,
    NWK_NIB_ID,
    APS_AIB_ID,
    ZDO_HANDLE_ID,
    PIB_NUM,
};

enum{
    ADDR_MAP_ID = 0,
    ADDR_MAP_CTRL_ID,
    NWK_NEIGHBOR_ID,
    NWK_NEIGHBOR_CTRL_ID,
    APS_BINDING_ID,
    APS_BINDING_CTRL_ID,
    APS_GROUP_ID,
    APS_GROUP_CTRL_ID,
    TAB_NUM,
};

enum{
	REJOIN_INSECURITY,
	REJOIN_SECURITY
};

#define	TL_ZB_PROFILE_ID						1
#define	TL_RETURN_INVALID						0xff


#define ZB_BEACON_INTERVAL_USEC 				15360 /* in microseconds */

#define	TL_SUPERFRAMETIME_TO_US(n)				(n * ZB_BEACON_INTERVAL_USEC)
#define	TL_TIMEUS_TO_SUPEFRAMETIME(t)			(t / ZB_BEACON_INTERVAL_USEC)

/**
  Convert time from beacon intervals to millisecinds

  Try to not cause overflow in 16-bit arithmetic (with some precision lost...)
*/
#define ZB_TIME_BEACON_INTERVAL_TO_MSEC(t) 		(ZB_BEACON_INTERVAL_USEC / 100 * (t) / 10)

/**
  Convert time from millisecinds to beacon intervals

  Try to not cause overflow in 16-bit arithmetic (with some precision lost...)
*/
#define ZB_MILLISECONDS_TO_BEACON_INTERVAL(ms) 	(((10l * (ms) + 3) / (ZB_BEACON_INTERVAL_USEC / 100)))

/**
 One second timeout
*/
#define ZB_TIME_ONE_SECOND 						ZB_MILLISECONDS_TO_BEACON_INTERVAL(1000)

/**
   Return 1 if the number is a power of 2, works only if x > 0
 */
#define MAGIC_IS_POWER_OF_TWO(x) 				( ((x) & ((x) - 1)) == 0 )

//APS
#define	APS_ACK_WAIT_DURATION					(3 * ZB_TIME_ONE_SECOND)

#define	APS_POLL_AFTER_REQ_TMO 					10


#define SHORT_ADDR_LEN       					2 //!< Network/short address length
#define EXT_ADDR_LEN         					8 //!< Extended/long/IEEE address length
#define SRC_ID_LEN			 					4
#define SEC_KEY_LEN			 					16

#define	TL_SETSTRUCTCONTENT(s, v)				(memset((u8 *)&s, v, sizeof(s)))

#define	COPY_U16TOBUFFER(buf, data)				do{	\
													((((u8 *)buf)[0]) = ((u8)data));			\
													((((u8 *)buf)[1]) = ((u8)(data >> 8)));		\
												}while(0)

#define	COPY_BUFFERTOU16(data, buf)				do{	\
													(data) = (buf)[0] + ((buf)[1] << 8);		\
												}while(0)

#define	COPY_U32TOBUFFER(buf, data)				do{	\
													((((u8 *)buf)[0]) =	((u8)data));			\
													((((u8 *)buf)[1]) =	((u8)(data >> 8)));		\
													((((u8 *)buf)[2]) =	((u8)(data >> 16)));	\
													((((u8 *)buf)[3]) =	((u8)(data >> 24)));	\
												}while(0)

#define	COPY_U24TOBUFFER(buf, data)				do{	\
													((((u8 *)buf)[0]) =	((u8)data));			\
													((((u8 *)buf)[1]) =	((u8)(data >> 8)));		\
													((((u8 *)buf)[2]) =	((u8)(data >> 16)));	\
												}while(0)

#define	COPY_BUFFERTOU16_BE(data, buf)			do{	\
													(data) = (buf)[1] + ((buf)[0] << 8);		\
												}while(0)

#define	COPY_U16TOBUFFER_BE(buf, data)			do{	\
													((((u8 *)buf)[0]) =	((u8)(data >> 8)));		\
													((((u8 *)buf)[1]) =	((u8)data));			\
												}while(0)

#define	COPY_BUFFERTOU32_BE(data, buf)			do{	\
													(data) = ((buf)[0] << 24) + ((buf)[1] << 16) + ((buf)[2] << 8) + ((buf)[3]); \
												}while(0)

#define	COPY_U32TOBUFFER_BE(buf, data)			do{	\
													((((u8 *)buf)[0]) =	((u8)(data >> 24)));	\
													((((u8 *)buf)[1]) =	((u8)(data >> 16)));	\
													((((u8 *)buf)[2]) =	((u8)(data >> 8)));		\
													((((u8 *)buf)[3]) =	((u8)data));			\
												}while(0)

#define ZB_IS_16BIT_SECURITY_KEY_ZERO(key) 		(!memcmp((key), g_null_securityKey, SEC_KEY_LEN))
#define ZB_IS_64BIT_ADDR_ZERO(addr) 			(!memcmp((addr), g_zero_addr, EXT_ADDR_LEN))
#define ZB_IS_64BIT_ADDR_INVALID(addr) 			(!memcmp((addr), g_invalid_addr, EXT_ADDR_LEN))
#define ZB_64BIT_ADDR_ZERO(addr)       			(memset((addr), 0, EXT_ADDR_LEN))
#define ZB_64BIT_ADDR_COPY(dst, src) 			(memcpy(dst, src, EXT_ADDR_LEN))
#define ZB_64BIT_ADDR_CMP(one, two) 			((bool)!memcmp((one), (two), EXT_ADDR_LEN))

#define ZB_EXTPANID_IS_ZERO 					ZB_IS_64BIT_ADDR_ZERO
#define ZB_EXTPANID_ZERO 						ZB_64BIT_ADDR_ZERO
#define ZB_EXTPANID_COPY 						ZB_64BIT_ADDR_COPY
#define ZB_EXTPANID_CMP 						ZB_64BIT_ADDR_CMP

#define ZB_IEEE_ADDR_IS_ZERO 					ZB_IS_64BIT_ADDR_ZERO
#define ZB_IEEE_ADDR_IS_INVALID 				ZB_IS_64BIT_ADDR_INVALID
#define ZB_IEEE_ADDR_ZERO 						ZB_64BIT_ADDR_ZERO
#define	ZB_IEEE_ADDR_INVALID(addr)				ZB_64BIT_ADDR_COPY(addr, g_invalid_addr)
#define ZB_IEEE_ADDR_COPY 						ZB_64BIT_ADDR_COPY
#define ZB_IEEE_ADDR_CMP 						ZB_64BIT_ADDR_CMP

#define ZB_SEC_KEY_IS_NULL						ZB_IS_16BIT_SECURITY_KEY_ZERO

#define ZB_MAC_FRAME_HEADER						9
#define ZB_NWK_FRAME_HEADER						(8 + NWK_MAX_SOURCE_ROUTE * 2 + 14 + 4)
#define ZB_APS_FRAME_HEADER						10

/*typedef struct{
	u8	altPanCoord: 	1;
	u8	devType: 		1;
	u8	powerSrc: 		1;
	u8	rcvOnWhenIdle: 	1;
	u8  reserved:		2;
	u8	secuCapability: 1;
	u8	allocAddr: 		1;
}capability_info_t;*/

#define	MAC_CAP_ALTPANCOORD						BIT(0)
#define	MAC_CAP_DEVTYPE							BIT(1)
#define	MAC_CAP_POWERSRC						BIT(2)
#define	MAC_CAP_RXONIDLE						BIT(3)
#define	MAC_CAP_SECURITYCAP						BIT(6)
#define	MAC_CAP_ALLOCADDR						BIT(7)

typedef struct{
	tl_zb_mac_pib_t 	macPib;  //76B
}mac_info_t;

extern mac_info_t g_zbInfo;


#define g_zbMacPib			g_zbInfo.macPib


/* Default settings. */
extern const tl_zb_mac_pib_t macPibDefault;


extern const addrExt_t g_zero_addr;
extern const addrExt_t g_invalid_addr;
extern const u8 g_null_securityKey[];
extern const u8 nwkKeyDefault[];
extern const u8 tcLinkKeyCentralDefault[];			/* default global tc link key */
extern const u8 linkKeyDistributedCertification[];	/* Certification distribute link key */
extern const u8 linkKeyDistributedMaster[];
extern const u8 touchLinkKeyCertification[];		/* Certification touch-link key */
extern const u8 touchLinkKeyMaster[];


/* diagnostics for stack */
typedef struct{
	u16 numberOfResets;
	u16 persistentMemoryWrites;

	u32 macRxCrcFail;
	u32 macTxCcaFail;
	u32 macRxBcast;
	u32 macTxBcast;
	u32 macRxUcast;
	u32 macTxUcast;
	u16 macTxUcastRetry;
	u16 macTxUcastFail;

	u16 nwkTxCnt;
	u16 nwkTxEnDecryptFail;

	u16 apsRxBcast;
	u16 apsTxBcast;
	u16 apsRxUcast;
	u16 apsTxUcastSuccess;
	u16 apsTxUcastRetry;
	u16 apsTxUcastFail;

	u16 routeDiscInitiated;
	u16 neighborAdded;
	u16 neighborRemoved;
	u16 neighborStale;
	u16 joinIndication;
	u16 childMoved;

	u16 nwkFCFailure;
	u16 apsFCFailure;
	u16 apsUnauthorizedKey;
	u16 nwkDecryptFailures;
	u16 apsDecryptFailures;
	u16 packetBufferAllocateFailures;
	u16 relayedUcast;
	u16 phytoMACqueuelimitreached;
	u16 packetValidateDropCount;

	u8  lastMessageLQI;
	s8  lastMessageRSSI;
	u8	macTxIrqTimeoutCnt;
	u8	macTxIrqCnt;
	u8	macRxIrqCnt;
	u8	phyLengthError;
	u8	panIdConflict;
	u8	panIdModified;
}sys_diagnostics_t;

extern sys_diagnostics_t g_sysDiags;


#define zb_random() 	((u16)drv_u32Rand())
#define ZB_RANDOM() 	zb_random()

u8 ieee802154_info_load(void);
void ieee802154_info_save(void);

void ieee802154_reset(void);
bool ieee802154_init(void);

void os_init(u8 isRetention);

bool tl_stackBusy(void);

void ieee802154_app_task_process(void);

void ieee802154_procedure(void);

#endif /* ZB_COMMON_H */
