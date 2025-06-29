/********************************************************************************************************
 * @file	tl_zb_mac_pib.h
 *
 * @brief	This is the header file for tl_zb_mac_pib
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
#ifndef TL_ZB_MAC_PIB_H
#define TL_ZB_MAC_PIB_H


#define	MAC_INVALID_PANID						0xFFFF

#define MAC_DEVICE_TABLE_SIZE 					4

/*
 * PHY constants
 */
#define ZB_PHY_MAX_PACKET_SIZE					127
#define ZB_PHY_TURNROUNDTIME					12
#define	ZB_US_PER_SYMBOL						16

/*
 * PHY PIB
 */
#define ZB_PHY_SYMBOLS_PER_OCTET				2
#define ZB_PHY_SHR_DURATION						40
#define ZB_PHY_MAX_FRAME_DURATION				(ZB_PHY_SHR_DURATION + (ZB_PHY_MAX_PACKET_SIZE + 1) * ZB_PHY_SYMBOLS_PER_OCTET)


/** @addtogroup zb_mac_constant Zigbee Mac Constants
 *  Definition for zigbee Mac constants
 *  @{
 */
#define ZB_MAC_BASE_SLOT_DURATION				60	 //!< The number of symbols forming a superframe slot when the superframe order is equal to 0
#define ZB_MAC_NUM_SUPERFRAME_SLOTS				16	 //!< The number of slots contained in any superframe
#define ZB_MAC_BASE_SUPERFRAME_DURATION			(ZB_MAC_BASE_SLOT_DURATION * ZB_MAC_NUM_SUPERFRAME_SLOTS)
													 //!< The number of symbols forming a superframe when the superframe order is equal to 0
#define ZB_MAC_BASE_SUPERFRAME_DURATION_US		(ZB_MAC_BASE_SLOT_DURATION * ZB_MAC_NUM_SUPERFRAME_SLOTS * ZB_US_PER_SYMBOL)
													 //!< a superframe(us) when the superframe order is equal to 0
#define ZB_MAC_UNIT_BACKOFF_PERIOD				(20) //!< The number of symbols forming the basic time period used by the CSMA-CA algorithm.
#define ZB_MAC_UNIT_BACKOFF_PERIOD_US			(ZB_MAC_UNIT_BACKOFF_PERIOD * ZB_US_PER_SYMBOL) //!< the basic time period(us) used by the CSMA-CA algorithm.
#define ZB_MAC_MIN_MPDU_OVERHEAD				(9)	  	//!< The minimum number of octets added by the MAC sublayer to the PSDU.
#define ZB_MAC_MIN_CAP_LEHGTH					(440) 	//!< The minimum number of symbols forming the CAP.
#define ZB_MAC_MAX_SIF_FRAME_SIZE				(18)	//!< The maximum size of an MPDU
#define ZB_MAC_MAX_MPDU_UNSECURITY_OVERHEAD		(25)	//!< The maximum number of octets added by the MAC sublayer to the PSDU without security
#define ZB_MAC_MAX_MAC_PAYLOAD_SIZE				(ZB_PHY_MAX_PACKET_SIZE - ZB_MAC_MIN_MPDU_OVERHEAD)
														//!< The maximum number of octets that can be transmitted in the MAC Payload field
#define ZB_MAC_MAX_MAC_SAFE_PAYLOAD_SIZE		(ZB_PHY_MAX_PACKET_SIZE - ZB_MAC_MAX_MPDU_UNSECURITY_OVERHEAD)
														//!< The maximum number of octets that can be transmitted in the MAC Payload field of an unsecured MAC frame
#define ZB_MAC_MAX_LOST_BEACONS					(4)		//!< The number of consecutive lost beacons that will cause the MAC sublayer of a receiving device to declare a loss of synchronization
#define ZB_MAC_MAX_BEACON_OVERHEAD				(75)    //!< The maximum number of octets added by the MAC sublayer to the MAC payload of a beacon frame.
#define ZB_MAC_MAX_BEACON_PAYLOAD_LENGTH		(ZB_PHY_MAX_PACKET_SIZE - ZB_MAC_MAX_BEACON_OVERHEAD)
														//!< The maximum size, in octets, of a beacon payload
#define ZB_MAC_GTS_DESC_PERSISTENCE_TIME		(4)		//!< The number of consecutive lost beacons that will cause the MAC sublayer of a receiving device to declare a loss of synchronization.

/** @} end of group zb_mac_constant */


/* re-transmit timers of the MAC layer */
#define ZB_MAC_WAIT_RESP_TIME_DEFAULT			(32)
#define	ZB_MAC_FRAME_RETRIES_MAX				(3)


//#ifdef ZB_MAC_SECURITY
/*
 *  @brief MAC security related consts
 */
#define MAC_DEV_TABLE_MAX_LEN               16  /*maximum number of devices that can securely communicate with the local device*/
#define MAC_KEY_TABLE_MAX_LEN               16
#define LOOKUP_MAX_NUM_PER_ONE_KEY          1 /*one KeyIDLookupDesc entry correspondings to one KeyDescriptor in macKeyTable*/
#define MAC_KEYID_LOOKUP_TABLE_MAX_LEN      1//(MAC_KEY_TABLE_MAX_LEN*LOOKUP_MAX_NUM_PER_ONE_KEY)
#define MAC_KEY_USAGE_TABLE_MAX_LEN         2//(6*MAC_KEY_TABLE_MAX_LEN) /* data frame + beacon frame + 4 command frame(data request,beacon request,associate request,associate response) */
#define MAC_KEY_DEV_TABLE_MAX_LEN           (MAC_DEV_TABLE_MAX_LEN>MAC_KEY_TABLE_MAX_LEN) ? (MAC_DEV_TABLE_MAX_LEN):(MAC_KEY_TABLE_MAX_LEN)//(MAC_KEY_TABLE_MAX_LEN*MAC_DEV_TABLE_MAX_LEN)  /*one entry correspondings to one DeviceDescriptor in macDeviceTable*/
#define MAC_SEC_LVL_TABLE_MAX_LEN           6 /* data frame + beacon frame + 4 command frame(data request,beacon request,associate request,associate response) */

typedef enum {
    MAC_SECMATERIAL_KEYID_LOOKUP_DESC = 0,
    MAC_SECMATERIAL_DEVICE_DESC,
    MAC_SECMATERIAL_KEYUSAGE_DESC,
    MAC_SECMATERIAL_SECLEVEL_DESC,
    MAC_SECMATERIAL_KEY_DESC,
    MAC_SECMATERIAL_KEYDEV_DESC,
}mac_secMaterial_t;

//typedef struct zb_mac_device_table_s{
//	u32    frameCounter;
//	u16    shortAddr;
//	addrExt_t extAddr;
//	u16    panId;
//}tl_zb_mac_device_table_t;
//#endif

/** @addtogroup zigbee_mac_pib_type Zigbee Mac PIB Types
 *  @{
 */
typedef struct{
	u32			beaconTxTime;			//!< The time that the device transmitted its last beacon frame
	addrExt_t	coordExtAddress;		//!< coordinator ieee address
	addrExt_t  	extAddress;				//!< ieee address
	u16			ackWaitDuration;		//!< the duration for waiting for an acknowledgment frame
	u16  		coordShortAddress;		//!< coordinator short address
	u16			panId;					//!< pan identifier
	u16 		shortAddress;  			//!< short address(joined in a network)
	u16  		superframeOrder;		//!< super-frame order
	u16 		transactionPersistenceTime;//! The maximum time (in unit periods) that a transaction is	stored by a coordinator and	indicated in its beacon.
	u16 		frameTotalWaitTime;
	u16			syncSymbolOffset;//4
	zb_mac_beacon_payload_t beaconPayload;//15
	u8  		beaconPayloadLen;
	u8  		associationPermit;
	u8			associatedPanCoord;
	u8  		autoReq;//4
	u8			battLifeExt;
	u8  		battLifeExtPeriods;
	u8  		beaconOrder;
	u8			beaconSeqNum;
	u8  		seqNum;
    u8  		frameRetryNum;
    u8			respWaitTime;
    u8			maxCsmaBackoffs;
    u8 			promiscuousMode;
    u8 			timeStampSupported;
    u8			securityEnabled;

    u8  		gtsPermit;
    u8  		minBe;
    u8  		maxBe;
    u8			phyPageCur;
    u8			phyChannelCur;
    u8 			rxOnWhenIdle;
    u8			txPower;
    u8 			resv[3];//76-bytes

//#ifdef ZB_MAC_SECURITY
    /* security */
    void           *keyTable;
    void           *devTable;
    void           *secLvlTable;
    u8             keyTableEntries;
    u8             devTableEntries;
    u8             secLvlTableEntries;
    u8             autoReqSecLvl;
    u32            frameCounter;
    u8             autoReqKeySource[8];
    u8             autoReqKeyIdMode;
    u8             autoReqKeyIndex;
    u16            panCoordShortAddr;
    u8             defaultKeySource[8];
    addrExt_t      panCoordExtAddr;
//    tl_zb_mac_device_table_t devTable[MAC_DEVICE_TABLE_SIZE];
//	u8          devTableEntry;
//	u32         frameCounter;
//	u8          secKey[16];
//#endif
}tl_zb_mac_pib_t;
/** @} end of group zigbee_mac_pib_type */


#define MAC_IB()		g_zbMacPib




void generateIEEEAddr(void);

/*
 * MLME-SET.request to set MAC pib attribute from upper layer to MAC layer
 *
 * @param attribute MAC PIB attribute  MAC PIB attribute id(Table 86 ?a MAC PIB attributes)
 *
 * @param value the pointer value of the attribute
 *
 * @param index:if the attribute is related to security,the means of index are entry number
 * 				othsewise the index means len, the length of the attribute value
 *
 * @return MAC_SUCCESS if it's successful, or MAC_INVALID_PARAMETER
 *
 * */
u8 tl_zbMacAttrSet(u8 attribute, u8 *value, u8 index);

/*
* MLME-GET.request to get MAC pib attribute from upper layer to MAC layer
*
* @param attribute MAC PIB attribute MAC PIB attribute id(Table 86 ?a MAC PIB attributes)
*
* @param value the pointer to the value of the attribute
*
* @param len the length of the attribute value
*
* @return MAC_SUCCESS if it's successful, or MAC_INVALID_PARAMETER
*
* */
u8 tl_zbMacAttrGet(u8 attribute, u8* value, u8* len);

/*
 * MLME-GET.request to del MAC pib attribute
 *
 * @param pibAttribute MAC PIB security attribute MAC PIB attribute id(Table 86  MAC PIB attributes)
 *
 * @param pibIndex:entry number
 *
 * @return MAC_SUCCESS if it's successful, or MAC_INVALID_PARAMETER
 *
 * */
mac_sts_t tl_zbMacSecAttrDel(u8 pibAttribute, u8 pibIndex);
void mac_secMaterial_Init(u8 coldReset);
void updateKeyDevTable(mac_keydevDesc_t *desc);
//void *mac_secMaterial_alloc(mac_secMaterial_t type);
//char mac_secMaterial_free(mac_secMaterial_t type, void *item);
#endif /* TL_ZB_MAC_PIB_H */
