/********************************************************************************************************
 * @file	mac_phy.h
 *
 * @brief	This is the header file for mac_phy
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
#ifndef MAC_PHY_H
#define MAC_PHY_H


/*********************************************************************
 * CONSTANTS
 */
#define RF_PKT_BUFF_LEN				(144)//(128+32) //(128+16)

#define CCA_THRESHOLD       		(-70)
#define RSSI_PASS_THRESHOLD			(-30)

/*********************************************************************
 * ENUMS
 */
enum{
    RF_SUCC,
    RF_CSMA_FAILURE,
};

enum{
    RF_RX_OFF,
    RF_RX_ON,
};

enum{
    CCA_ED_ONLY,
    CCA_CS_ONLY,
    CCA_ED_AND_CS,
    CCA_ED_OR_CS,
};

enum{
    RF_STATE_TX = 0,
    RF_STATE_RX,
    RF_STATE_ED,
    RF_STATE_OFF,
};

/*
 * Definition for RF Setting identifier.
 */
enum{
	SUCC,
    RF_BLACK_LIST_FULL,
    RF_BLACK_LIST_NO_MEMBER,
};

/*
 * Definition for RF Setting identifier.
 */
enum{
    /* General ID */
    RF_ID_CHANNEL               = 0x00,
    RF_ID_TX_POWER,
    RF_ID_RX_ONOFF,
    RF_ID_GENERAL_END           = 0x9F,
    
    /* Protocol Specific ID*/
    RF_ID_ZIGBEE                = 0xA0,
    RF_ID_MAC_SHORT_ADDR,
    RF_ID_MAC_EXT_ADDR,
    RF_ID_MAC_PANID,
    RF_ID_MAC_FRAME_PENDING,
    RF_ID_MAC_PAN_COORD,
    RF_ID_MAC_RECV_START,
    RF_ID_BECON_RECV,

    RF_ID_BLE                   = 0xB0,
};

/*********************************************************************
 * TYPES
 */
typedef enum{
    PHY_CCA_IDLE       = 0x04,
    PHY_CCA_TRX_OFF    = 0x03,
    PHY_CCA_BUSY       = 0x00,
}phy_ccaSts_t;

/**
 *  @brief  Definition for Telink RX packet format
 */
typedef struct{
    u32 dmaLen;
    u8  rssi;
    u8  rseverd[7];
    u8  payloadLen;
    u8  payload[1];
}rf_recvPkt_t;

/**
 *  @brief  Definition for Telink TX packet format
 */
typedef struct{
    u32 dmaLen;
    u8  rfLen;
    u8  payload[1];
}rf_sendPkt_t;

/*
 * Definition for RF Rx buffer format
 */
typedef struct{
    u8	*psdu;
    u32 timeStamp;
    s8  rssi;
    u8  psduLen;
}rx_buf_t;


/*
 * parameters for phy/mac
 */
typedef struct{
	u16 turnArnd;  //phy turn around
	s8  ccaThres;    //threshold for cca
}rf_phy_params_t;


extern u8 g_zb_txPowerSet;
extern rf_phy_params_t g_phyParams;


/*********************************************************************
 * Public Functions
 */


/*********************************************************************
 * @fn      rf_reset
 *
 * @brief   Reset RF module.
 *
 * @param   none
 *
 * @return  none
 */
void rf_reset(void);

#define mac_phyReconfig()	rf_reset();

/*********************************************************************
 * @fn      rf_init
 *
 * @brief   Initialize RF module and do the RF power up.
 *
 * @param   none
 *
 * @return  none
 */
void rf_init(void);

/*********************************************************************
 * @fn      rf_setRxBuf
 *
 * @brief   Set RX buffer.
 *
 * @param   pBuf - RX buffer
 *
 * @return  none
 */
void rf_setRxBuf(u8 *pBuf);

/*********************************************************************
 * @fn      rf_TrxStateGet
 *
 * @brief   Get current TRX state.
 *
 * @param   none
 *
 * @return  state
 */
u8 rf_TrxStateGet(void);

/*********************************************************************
 * @fn      rf_setTrxState
 *
 * @brief   Go to specified state and set related register.
 *
 * @param   state - Specified state
 *
 * @return  none
 */
void rf_setTrxState(u8 state);

/*********************************************************************
 * @fn      rf_setChannel
 *
 * @brief   Set specified channel to RF module.
 *
 * @param   ch - 11~26
 *
 * @return  none
 */
void rf_setChannel(u8 chn);

/*********************************************************************
 * @fn      rf_getChannel
 *
 * @brief   Get specified channel.
 *
 * @param   none
 *
 * @return  chn
 */
u8 rf_getChannel(void);

/*********************************************************************
 * @fn      rf_setTxPower
 *
 * @brief   Set specified transmitted power.
 *
 * @param   txPower - Specified power
 *
 * @return  none
 */
void rf_setTxPower(u8 txPower);

/*********************************************************************
 * @fn      rf_getLqi
 *
 * @brief   Get calculated Link Quality value from specified rssi
 *
 * @param   rssi
 *
 * @return  lqi result
 */
u8 rf_getLqi(s8 rssi);

/*********************************************************************
 * @fn      rf_startEDScan
 *
 * @brief   Start ED detect
 *
 * @param   none
 *
 * @return  none
 */
void rf_startEDScan(void);

/*********************************************************************
 * @fn      rf_stopEDScan
 *
 * @brief   Stop Energy Detect
 *
 * @param   none
 *
 * @return  ED result
 */
u8 rf_stopEDScan(void);

/*********************************************************************
 * @fn      rf_performCCA
 *
 * @brief   Perform CCA
 *
 * @param   none
 *
 * @return  phy_ccaSts_t
 */
u8 rf_performCCA(void);

void rf802154_tx_ready(u8 *buf, u8 len);
void rf802154_tx(void);

/*********************************************************************
 * @fn      rf_paInit
 *
 * @brief   Initialize PA.
 *
 * @param   none
 *
 * @param   none
 *
 * @return  none
 */
void rf_paInit(u32 TXEN_pin, u32 RXEN_pin);
void rf_paShutDown(void);


#endif  /* MAC_PHY_H */
