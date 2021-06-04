/********************************************************************************************************
 * @file	usbsniffer_i.h
 *
 * @brief	This is the header file for usbsniffer_i
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
#pragma once

#include "../usbdesc.h"

#if (__PROJECT_TL_SNIFFER__)


#define SNIFFER_DEVICE_DESC	{ \
	{sizeof(USB_Descriptor_Device_t), DTYPE_Device}, 		\
	0x0200, 												\
	USB_CSCP_NoDeviceClass,									\
	USB_CSCP_NoDeviceSubclass,								\
	USB_CSCP_NoDeviceProtocol,								\
	8,														\
	ID_VENDOR,												\
	ID_PRODUCT,												\
	ID_VERSION,												\
	USB_STRING_VENDOR,										\
	USB_STRING_PRODUCT,										\
	0,														\
	1														\
}

#define SNIFFER_INTERFACE {	\
	{sizeof(USB_Descriptor_Interface_t), DTYPE_Interface}, 	\
	0,														\
	0,														\
	1,														\
	CDC_CSCP_VendorSpecificProtocol,						\
	CDC_CSCP_VendorSpecificProtocol,						\
	CDC_CSCP_VendorSpecificProtocol,						\
	NO_DESCRIPTOR											\
}

#define SNIFFER_ENDPOINT { \
	{sizeof(USB_Descriptor_Endpoint_t), DTYPE_Endpoint},	\
	(ENDPOINT_DIR_IN | SNIFFER_TX_EPNUM),					\
	EP_TYPE_BULK,											\
	SNIFFER_TX_EPSIZE,										\
	5														\
}

#endif
