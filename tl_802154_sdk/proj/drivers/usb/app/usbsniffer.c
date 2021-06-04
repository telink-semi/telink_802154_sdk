/********************************************************************************************************
 * @file	usbsniffer.c
 *
 * @brief	This is the source file for usbsniffer
 *
 * @author	Driver & Zigbee Group
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
#include "usbsniffer.h"
#include "../usb.h"

#if (__PROJECT_TL_SNIFFER__)

#define	SNIFFER_HDR_LEN			8

u8 *g_sniffer_sendAddr = NULL;
u8 g_sniffer_sendRspData = 0;
u8 g_sniffer_remind_pktLen = 0;
u8 g_sniffer_rfChannel = 11;
bool g_sniffer_rfCaptureStarted = 0;


void usbSniffer_processControlRequest(u8 bmRequestType, u8 data_request, u8 bRequest, u16 wIndex)
{
	switch(bmRequestType){
		case (REQDIR_DEVICETOHOST | REQTYPE_VENDOR | REQREC_DEVICE):
			if(USB_IRQ_SETUP_REQ == data_request){
				if(0xc0 == bRequest){// Get board version
					usbhw_reset_ctrl_ep_ptr();
					usbhw_write_ctrl_ep_data(0x31);
					usbhw_write_ctrl_ep_data(0x25);
					usbhw_write_ctrl_ep_data(0x31);
					usbhw_write_ctrl_ep_data(0x05);
					usbhw_write_ctrl_ep_data(0x02);
					usbhw_write_ctrl_ep_data(0x00);
					usbhw_write_ctrl_ep_data(0x01);
					usbhw_write_ctrl_ep_data(0x00);
				}else if(0xc6 == bRequest){
					usbhw_reset_ctrl_ep_ptr();
					g_sniffer_sendRspData++;
					if(g_sniffer_sendRspData < 4)
						usbhw_write_ctrl_ep_data(0x01);
					else{
						usbhw_write_ctrl_ep_data(0x04);
						g_sniffer_sendRspData = 0;
					}
				}
			}
			break;
		case (REQDIR_HOSTTODEVICE | REQTYPE_VENDOR | REQREC_DEVICE):
			if(USB_IRQ_SETUP_REQ == data_request){
				if(0xd0 == bRequest){				// start capture
					g_sniffer_rfCaptureStarted = 1;
				}else if(0xd1 == bRequest){			// stop capture
					g_sniffer_rfCaptureStarted = 0;
				}
			}else{
				if(0xd2 == bRequest){
					if(0 == wIndex){// set channel
						g_sniffer_rfChannel = usbhw_read_ctrl_ep_data();
					}
				}
			}
			break;
	}
}

u8 usbSniffer_isTxBusy(void)
{
    return usbhw_is_ep_busy(SNIFFER_TX_EPNUM);
}

void usbSniffer_init(void)
{
	u16 usbID = 0;
	flash_read(CFG_TELINK_USB_ID, 2, (u8 *)&usbID);
	if(usbID == 0xffff){
		do{
			usbID = (u16)drv_u32Rand();
		}while((usbID == 0xffff) || (usbID == 0));

		flash_write(CFG_TELINK_USB_ID, 2, (u8 *)&usbID);
	}
	USB_Descriptor_Device_t *p = (USB_Descriptor_Device_t *)usbdesc_get_device();
	p->ReleaseNumber = usbID;
}

void usbSniffer_sendRemainMsg(void){
	if(usbSniffer_isTxBusy() || !g_sniffer_remind_pktLen){
		return;
	}

	u8 len = g_sniffer_remind_pktLen;
	u8 *pData = g_sniffer_sendAddr;
	if(g_sniffer_remind_pktLen > SNIFFER_TX_EPSIZE){
		len = SNIFFER_TX_EPSIZE;
		pData += SNIFFER_TX_EPSIZE;
	}
	g_sniffer_remind_pktLen -= len;

	usbhw_reset_ep_ptr(SNIFFER_TX_EPNUM);

	for(u8 i = 0; i < len; i++){
		usbhw_write_ep_data(SNIFFER_TX_EPNUM, pData[i]);	// raw pkt
	}

	usbhw_data_ep_ack(SNIFFER_TX_EPNUM);
}

void usbSniffer_sendMsg(u8 *pData, u8 dataLen, u32 t){
	if(!g_sniffer_rfCaptureStarted || g_sniffer_remind_pktLen ||
	   !dataLen || !pData){
		return;
	}

	usbhw_reset_ep_ptr(SNIFFER_TX_EPNUM);

	usbhw_write_ep_data(SNIFFER_TX_EPNUM, 0);			// pkt  status
	usbhw_write_ep_data(SNIFFER_TX_EPNUM, dataLen + 5);	// pkt len
	usbhw_write_ep_data(SNIFFER_TX_EPNUM, 0);			// pkt  len
	usbhw_write_ep_data(SNIFFER_TX_EPNUM, t);			// time stamp
	usbhw_write_ep_data(SNIFFER_TX_EPNUM, (t >> 8));	// time stamp
	usbhw_write_ep_data(SNIFFER_TX_EPNUM, (t >> 16));	// time stamp
	usbhw_write_ep_data(SNIFFER_TX_EPNUM, (t >> 24));	// time stamp
	usbhw_write_ep_data(SNIFFER_TX_EPNUM, dataLen);		// payload len

	//HDR length 8
	if(dataLen + SNIFFER_HDR_LEN > SNIFFER_TX_EPSIZE){
		g_sniffer_remind_pktLen = dataLen + SNIFFER_HDR_LEN - SNIFFER_TX_EPSIZE;
		dataLen = SNIFFER_TX_EPSIZE - SNIFFER_HDR_LEN;
		g_sniffer_sendAddr = pData + dataLen;
	}

	for(u8 i = 0; i < dataLen; i++){
		usbhw_write_ep_data(SNIFFER_TX_EPNUM, pData[i]);	// raw pkt
	}

	usbhw_data_ep_ack(SNIFFER_TX_EPNUM);
}

#endif
