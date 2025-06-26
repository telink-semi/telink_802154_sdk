/********************************************************************************************************
 * @file	zb_config.c
 *
 * @brief	This is the source file for zb_config
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
/**********************************************************************
 * INCLUDES
 */
#include <ieee802154_common.h>

sys_diagnostics_t g_sysDiags;

/* PAN ID setting */
/*
 * Setting this to a value other than MAC_INVALID_PANID (0xFFFF) causes
 * ZDO_COORD to use this value as its PAN ID and
 * Routers and end devices to join PAN with this ID
 */
#define DEFAULT_PANID		MAC_INVALID_PANID
u16 TL_ZB_ASSOCJOIN_PERMIT_PANID = DEFAULT_PANID;

/*
 * Setting this to a value other than MAC_INVALID_PANID (0xFFFF) causes
 * Routers and end devices to filter the PAN ID when associate join a network
 */
#define FILTER_PANID		MAC_INVALID_PANID
u16 TL_ZB_ASSOCJOIN_FILTER_PANID = FILTER_PANID;





/* queue size of the software timer event */
u8 TIMER_EVENT_SIZE = TIMER_EVENT_NUM;

/* buffer pool size for zigbee pro */
u8 ZB_BUF_POOL_SIZE = ZB_BUF_POOL_NUM;
zb_buf_pool_t g_mPool;

/* for indirect pending queue */
u8 ZB_MAC_PENDING_TRANS_QUEUE_SIZE = ZB_MAC_PENDING_TRANS_QUEUE_NUM;
u8 ZB_MAC_EXT_EXPEIRY_CNT = ZB_MAC_INTERNAL_EXPIRY_CNT;














/* the offset of the rx buffer to the zb-buffer*/
u8 RX_ZBBUF_OFFSET = TL_RXPRIMITIVEHDR;

/* MAC layer TX Queue size */
u8 MAC_TX_QUEUE_SIZE = TX_QUEUE_BN;





/* Default PIB settings for MAC layer. */
const tl_zb_mac_pib_t macPibDefault = {
	.ackWaitDuration = (ZB_MAC_UNIT_BACKOFF_PERIOD + ZB_PHY_TURNROUNDTIME + ZB_PHY_SHR_DURATION + (u16)(6 * ZB_PHY_SYMBOLS_PER_OCTET)),
	.frameRetryNum = ZB_MAC_FRAME_RETRIES_MAX,
	.transactionPersistenceTime = 0x01f4,
	.panId = DEFAULT_PANID,
	.shortAddress = MAC_SHORT_ADDR_BROADCAST,
	.respWaitTime = ZB_MAC_WAIT_RESP_TIME_DEFAULT,
	.phyChannelCur = DEFAULT_CHANNEL,
	.autoReq = 0,
#if ZB_ED_ROLE
	.minBe = 0,
#else
	.minBe = 5,
#endif
	.maxBe = 8,
	.beaconOrder = 15,
	.superframeOrder = 0,
	.maxCsmaBackoffs = 4,
	.associationPermit = 0,
	.coordShortAddress = 0xffff,
#if ZB_ED_ROLE
	.rxOnWhenIdle = 0,
#else
	.rxOnWhenIdle = 1,
#endif

    /* security related */
	.keyTable = NULL,
    .keyTableEntries = 0x00,
    .devTable = NULL,
    .devTableEntries = 0x00,
    .secLvlTable = NULL,
    .secLvlTableEntries = 0x00,
    .frameCounter = 0x00,
    .autoReqSecLvl = SECURITY_LEVEL_ENC_MIC_32,
    .autoReqKeyIdMode = KEY_ID_MODE_KEY_INDEX,
    .autoReqKeySource[0] = 0xff,.autoReqKeySource[1] = 0xff,.autoReqKeySource[2] = 0xff,.autoReqKeySource[3] = 0xff,
    .autoReqKeySource[4] = 0xff,.autoReqKeySource[5] = 0xff,.autoReqKeySource[6] = 0xff,.autoReqKeySource[7] = 0xff,
    .autoReqKeyIndex = 0x01,
    //[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff},
    .defaultKeySource[0] = 0x00,.defaultKeySource[1] = 0x00,.defaultKeySource[2] = 0x00,.defaultKeySource[3] = 0x00,
    .defaultKeySource[4] = 0x00,.defaultKeySource[5] = 0x00,.defaultKeySource[6] = 0x00,.defaultKeySource[7] = 0xff,
    .panCoordExtAddr[0] = 0xff,.panCoordExtAddr[1] = 0xff,.panCoordExtAddr[2] = 0xff,.panCoordExtAddr[3] = 0xff,
    .panCoordExtAddr[4] = 0xff,.panCoordExtAddr[5] = 0xff,.panCoordExtAddr[6] = 0xff,.panCoordExtAddr[7] = 0xff,
    .panCoordShortAddr = 0,
};



/*
 * @brief:		get the size of zigbee buffer
 */
u32 zbBufferSizeGet(void){
	return (sizeof(g_mPool));
}



