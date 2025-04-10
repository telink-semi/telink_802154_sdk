/********************************************************************************************************
 * @file	board_8258_dongle.h
 *
 * @brief	This is the header file for board_8258_dongle
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
#pragma once

/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
extern "C" {
#endif



//LED
/***************************************************************
* LED_R			GPIO_PA3	//D2 -- red			PWM1
* LED_G			GPIO_PA2	//D1 -- green		PWM0
* LED_B			GPIO_PB0	//D4 -- blue		PWM3
* LED_W			GPIO_PB1	//D5 -- white		PWM4
* LED_Y			GPIO_PA4	//D3 -- yellow		PWM2
****************************************************************/
//LED_R and LED_G as GPIO.
#define LED_R						GPIO_PA3
#define LED_G						GPIO_PA2

#define PA3_FUNC					AS_GPIO
#define PA3_OUTPUT_ENABLE			1
#define PA3_INPUT_ENABLE			0

#define PA2_FUNC					AS_GPIO
#define PA2_OUTPUT_ENABLE			1
#define PA2_INPUT_ENABLE			0

#define LED_1						LED_R
#define LED_2						LED_G

// UART
#if ZBHCI_UART
	#define UART_TX_PIN         	UART_TX_PD7
	#define UART_RX_PIN         	UART_RX_PA0

	#define UART_PIN_CFG()			uart_gpio_set(UART_TX_PIN, UART_RX_PIN);// uart tx/rx pin set
#endif




// DEBUG
#if UART_PRINTF_MODE
	#define	DEBUG_INFO_TX_PIN	    GPIO_PC7//print
#endif

// USB
#if ZBHCI_USB_PRINT || ZBHCI_USB_CDC || ZBHCI_USB_HID
#define HW_USB_CFG()				do{ \
										usb_set_pin_en();	\
									}while(0)
#endif




/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
