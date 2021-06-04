/********************************************************************************************************
 * @file	mac_internal.h
 *
 * @brief	This is the header file for mac_internal
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
#ifndef MAC_INTERNAL_H
#define MAC_INTERNAL_H


#define MAC_LAYER_COMM_DEBUG_EN			1
#define MAC_LAYER_DATA_DEBUG_EN			1


//MAC internal Handles
typedef enum{
	MAC_HANDLE_MIN = 0xDF,
	MAC_HANDLE_ASSOCIATION_REQ,
	MAC_HANDLE_ASSOCIATION_RSP,
	MAC_HANDLE_DISASSOCIATION_NOTIFY,
	MAC_HANDLE_BEACON,
	MAC_HANDLE_BEACON_REQ,
	MAC_HANDLE_COORDINATOR_REALIGNMENT,
	MAC_HANDLE_DEVICESTART_REQ_CMD,
	MAC_HANDLE_PANIDCONFLICT_CMD,
	MAC_HANDLE_PULL_FROM_UPLAYER,
	MAC_HANDLE_PULL_ASSOCIATION,
	MAC_HANDLE_ORPHAN_NOTIFICATION
}mac_handle_e;

/**
 realignment command format
*/
typedef struct{
	u8 cmdId;
	u16 panId;
	u16 coordShortAddr;
	u8 logicalChannel;
	u16 shortAddr;
	u8  channelPage;
}zb_coord_realignment_cmd_t;

typedef struct{
	void *pdu;
	u8	pduLen;
}mac_dataSend_t;

typedef int (*mac_indirect_timer_callback_t)(void *arg);

extern void *associationReqOrigBuffer;

void tl_zbSwitchOffRx(void);

void tl_zbMacIndirectDataTimerStart(mac_indirect_timer_callback_t cb, void *arg);
void tl_zbMacIndirectDataTimerCancel(void);

s32 tl_zbWaitForPendingDataTimeout(void *arg);

u8 tl_zbMacHdrParse(tl_zb_mac_mhr_t *mHhr, u8 *ptr);
u8 *tl_zbMacHdrBuilder(u8 *pBuf, tl_zb_mac_mhr_t *pMacHdr);

void tl_zbMacMcpsDataRequestSendConfirm(void *arg, u8 status);

mac_sts_t tl_zbMacTx(void *buf, void *pdu, u8 pduLen, bool needAck);

void tl_zbMacBeaconRequestCb(void *arg) ;
void tl_zbMacMlmeDataRequestCb(zb_mac_raw_frame_t *arg);

u8 tl_zbMacMlmeCoordRealignmentCmdSend(bool is_broadcast, addrExt_t orphaned_dev_ext_addr,
                                       u16 orphaned_dev_short_addr, void *arg);

void tl_zbMacActiveScanListAdd(void *arg);
void tl_zbMacOrphanScanStatusUpdate(void);

u8 tl_zbMacCommStatusSend(void *buf, u8 status);
u8 tl_zbMacMlmeDataRequestCmdSend(zb_mlme_data_req_cmd_t *pReq, void *arg, u8 handle);
void tl_zbMacAssociateRespReceived(u8 status);
void tl_zbMaxTxConfirmCb(void *arg, u8 status);

/*
 * callback when receive primitive from upper layer(NWK layer...)
 *
 * */
void tl_zbMacStartRequestHandler(void *arg);
void tl_zbMacAssociateRequestHandler(void *arg);
void tl_zbMacAssociateResponseHandler(void *arg);
void tl_zbMacDisassociateRequestHandler(void *arg);
void tl_zbMacOrphanResponseHandler(void *arg);
void tl_zbMacPollRequestHandler(void *arg);
void tl_zbMacResetRequestHandler(void *arg);
void tl_zbMacScanRequestHandler(void *arg);

void tl_zbMlmeCmdDisassociateNotifyRecvd(zb_mac_raw_frame_t *pData, tl_zb_mac_mhr_t *mHdr, u8 mhrLen);

/*
 * callback when receive indication primitive from PHY layer
 *
 * */
void tl_zbPhyIndication(zb_mac_raw_frame_t *buf, tl_zb_mac_mhr_t *macHdr, u8 macHdrLen);
void tl_zbPhyMldeIndication(zb_mac_raw_frame_t *pData, tl_zb_mac_mhr_t *macHdr, u8 macHdrLen);
void tl_zbPhyMlmeIndicate(zb_mac_raw_frame_t *pData, tl_zb_mac_mhr_t *macHdr, u8 macHdrLen);

mac_sts_t macDataPending(void *arg, addr_t dstAddr);
void zb_macTimerEventProc(void);

void tl_zbMacAssociateRequestStatusCheck(void *arg, u8 status);
void tl_zbMacDisassociateNotifyCmdConfirm(void *arg, u8 status);
void tl_zbMacDataRequestStatusCheck(void *arg, u8 status);
void tl_zbMacStartReqConfirm(void *arg, u8 status);
void tl_zbMacCmdPanIdConflictNotifySendCheck(void *arg, u8 status);
void tl_zbMacMlmeBeaconSendConfirm(void *arg, u8 status);
void tl_zbMacOrphanResponseStatusCheck(void *arg, u8 status);

#endif	/* MAC_INTERNAL_H */
