/********************************************************************************************************
 * @file	mac_trx_api.h
 *
 * @brief	This is the header file for mac_trx_api
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
#ifndef MAC_TRX_API_H
#define MAC_TRX_API_H


/**        
 *  @brief The number of slots contained in any superframe
 */
#define MAC_A_NUM_SUPERFRAME_SLOTS      16

/**        
 *  @brief Number of backoffs forming a superframe slot when the superframe order is equal to 0
 */
#define MAC_A_BASE_SLOT_DURATION        3

#ifdef WIN32
#define MAC_A_BASE_SUPERFRAME_DURATION  (MAC_A_BASE_SLOT_DURATION * MAC_A_NUM_SUPERFRAME_SLOTS)
#else
#define MAC_A_BASE_SUPERFRAME_DURATION  (MAC_A_BASE_SLOT_DURATION * MAC_A_NUM_SUPERFRAME_SLOTS)
#endif

#define MAC_MIN_HDR_LEN                 3
#define MAC_MAX_HDR_LEN                 23

/**        
 *  @brief Frame control field bit masks
 */
#define MAC_FCF_FRAME_TYPE_MASK         0x0007
#define MAC_FCF_SEC_ENABLED_MASK        0x0008
#define MAC_FCF_FRAME_PENDING_MASK      0x0010
#define MAC_FCF_ACK_REQUEST_MASK        0x0020
#define MAC_FCF_INTRA_PAN_MASK          0x0040
#define MAC_FCF_DST_ADDR_MODE_MASK      0x0C00
#define MAC_FCF_FRAME_VERSION_MASK      0x3000
#define MAC_FCF_SRC_ADDR_MODE_MASK      0xC000

/**        
 *  @brief MAC frame field lengths in bytes
 */
#define MAC_FCF_FIELD_LEN               2       /* frame control field */
#define MAC_SEQ_NUM_FIELD_LEN           1       /* sequence number  */
#define MAC_PAN_ID_FIELD_LEN            2       /* PAN ID  */
#define MAC_EXT_ADDR_FIELD_LEN          8       /* Extended address */
#define MAC_SHORT_ADDR_FIELD_LEN        2       /* Short address */
#define MAC_FCS_FIELD_LEN               2       /* FCS field */

/**        
 *  @brief Frame type
 */
#define MAC_FRAME_TYPE_BEACON           0
#define MAC_FRAME_TYPE_DATA             1
#define MAC_FRAME_TYPE_ACK              2
#define MAC_FRAME_TYPE_COMMAND          3
#define MAC_FRAME_TYPE_MAX_VALID        MAC_FRAME_TYPE_COMMAND

/**        
 *  @brief Frame control field bit positions
 */
#define MAC_FCF_FRAME_TYPE_POS          0
#define MAC_FCF_SEC_ENABLED_POS         3
#define MAC_FCF_FRAME_PENDING_POS       4
#define MAC_FCF_ACK_REQUEST_POS         5
#define MAC_FCF_INTRA_PAN_POS           6
#define MAC_FCF_DST_ADDR_MODE_POS       10
#define MAC_FCF_FRAME_VERSION_POS       12
#define MAC_FCF_SRC_ADDR_MODE_POS       14


//BIT(1) 1- FFD, 0- RFD
#define	MAC_CAP_GET_DEVICE_TYPE(c)		((c >> 1) & 1)
#define	MAC_CAP_GET_RXON_WHEN_IDLE(c)	((c >> 3) & 1)

#define	TX_QUEUE_BN						8


typedef enum{
	RX_BUSY 		= BIT(0),
	TX_BUSY 		= BIT(1),
	TX_ACKPACKET 	= BIT(2),
	RX_WAITINGACK 	= BIT(3),
	RX_DATAPENDING	= BIT(4),
	TX_UNDERWAY 	= BIT(5)
}rf_trxds_st_e;

typedef enum{
    MAC_TX_IDLE,
    MAC_TX_CSMA,
    MAC_TX_UNDERWAY,
    MAC_TX_WAIT_ACK,
    MAC_TX_RETRY,
    MAC_TX_TIME_OUT,
    MAC_TX_DONE,
}mac_txState_t;

typedef enum{
    MAC_TX_EV_NEW_DATA,
    MAC_TX_EV_CSMA_IDLE,
    MAC_TX_EV_CSMA_BUSY,
    MAC_TX_EV_SEND_SUCC,
    MAC_TX_EV_SEND_FAIL,
    MAC_TX_EV_ACK_RECV,
    MAC_TX_EV_ACK_RETRY,
    MAC_TX_EV_NO_ACK,
    MAC_TX_EV_HAVE_PENDING_DATA,
}mac_txEvt_t;

/**
 *  @brief Definition of MAC generic frame type, used in both TX and RX
 */
typedef struct{
	u8 fAck;                /*!< Used in TX  */
	u8 fFramePending;       /*!< Used in Poll  */
	u8 psduLen;
	u8 cnfStatus;
    u8 *txData;
}mac_genFrame_t;

typedef	mac_genFrame_t tx_data_queue;



tx_data_queue *tx_queue[TX_QUEUE_BN];

extern u8 MAC_TX_QUEUE_SIZE;
extern volatile u8 rf_busyFlag;


/***********************************************************************//**
 * @brief 		initialize the rf transceiver and state
 *
 * @param
 *
 * @return
 *
 **************************************************************************/
void mac_trxInit(void);

/***********************************************************************//**
 * @brief       trigger tx
 *
 * @param[in]   arg        - pointer to the tx buffer
 *
 * @return
 *
 **************************************************************************/
void mac_trigger_tx(void *arg);

/***********************************************************************//**
 * @brief       mac packet filter
 *
 * @param[in]   macPld        - mac payload
 *
 * @param[in]	len			  - the length of the mac payload
 *
 * @param[in]	needDrop	  - 1: this frame should be drop
 *
 * @param[in]	ackPkt		  - 1: this frame is ack frame
 *
 * @return      the address of the source address
 *
 **************************************************************************/
u8 *zb_macDataFilter(u8 *macPld, u8 len, u8 *needDrop, u8 *ackPkt);

/***********************************************************************//**
 * @brief       handle the received data
 *
 * @param[in]   rxBuf       - the address of zb buffer
 *
 * @param[in]	data		- the address of the valid data buffer
 * *
 * @param[in]	len			- data length
 * *
 * @param[in]	ackPkt		- this frame is ack packet
 * *
 * @param[in]	timestamp	- the timestamp of this packet
 * *
 * @param[in]	rssi		- RSSI of this packet
 *
 * @return      NONE
 *
 **************************************************************************/
void zb_macDataRecvHandler(u8 *rxBuf, u8 *data, u8 len, u8 ackPkt, u32 timestamp, s8 rssi);

/***********************************************************************//**
 * @brief       tx done callback
 *
 * @param[in]   None
 *
 * @return      None
 *
 **************************************************************************/
void zb_macDataSendHandler(void);

u8 tl_zbMacHdrSize(u16 frameCtrl,mac_sec_t *sec);
//u8 tl_zbMacHdrSize(u16 frameCtrl);
bool tl_zbMacStateBusy(void);
u8 mac_getTrxState(void);

#endif	/* MAC_TRX_API_H */
