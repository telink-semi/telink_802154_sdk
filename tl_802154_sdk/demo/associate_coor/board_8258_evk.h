/********************************************************************************************************
 * @file    board_8258_evk.h
 *
 * @brief   This is the header file for board_8258_evk
 *
 * @author  Zigbee Group
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *			All rights reserved.
 *
 *          Licensed under the Apache License, Version 2.0 (the "License");
 *          you may not use this file except in compliance with the License.
 *          You may obtain a copy of the License at
 *
 *              http://www.apache.org/licenses/LICENSE-2.0
 *
 *          Unless required by applicable law or agreed to in writing, software
 *          distributed under the License is distributed on an "AS IS" BASIS,
 *          WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *          See the License for the specific language governing permissions and
 *          limitations under the License.
 *
 *******************************************************************************************************/
#pragma once

/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
extern "C" {
#endif

#define HW_USB_CFG()				do{ \
										usb_set_pin_en();	\
									}while(0)

// BUTTON
#define BUTTON1               		GPIO_PD1
#define PD1_FUNC			  		AS_GPIO
#define PD1_OUTPUT_ENABLE	  		0
#define PD1_INPUT_ENABLE	  		1
#define	PULL_WAKEUP_SRC_PD1	  		PM_PIN_PULLUP_10K

#define BUTTON2               		GPIO_PD2
#define PD2_FUNC			  		AS_GPIO
#define PD2_OUTPUT_ENABLE	  		0
#define PD2_INPUT_ENABLE	  		1
#define	PULL_WAKEUP_SRC_PD2	  		PM_PIN_PULLUP_10K


// LED
/***************************************************************
* LED_R			GPIO_PD0	//D5 -- red
* LED_G			GPIO_PD4	//D3 -- green
* LED_B			GPIO_PD5	//D2 -- blue
* LED_W			GPIO_PD3	//D4 -- yellow
****************************************************************/
//LED_R and LED_G as GPIO.
#define LED_R						GPIO_PD0
#define LED_G						GPIO_PD4

#define PD4_FUNC					AS_GPIO
#define PD4_OUTPUT_ENABLE			1
#define PD4_INPUT_ENABLE			0

#define PD0_FUNC					AS_GPIO
#define PD0_OUTPUT_ENABLE			1
#define PD0_INPUT_ENABLE			0

#define LED_1						LED_R
#define LED_2						LED_G


// USB
#if ZBHCI_USB_PRINT || ZBHCI_USB_CDC || ZBHCI_USB_HID
#define HW_USB_CFG()				do{ \
										usb_set_pin_en();	\
									}while(0)
#endif
#define LED_POWER               	LED_R
#define LED_PERMIT              	LED_G

//ADC
#if VOLTAGE_DETECT_ENABLE
#define VOLTAGE_DETECT_ADC_PIN  GPIO_PC5
#endif

// DEBUG
#if UART_PRINTF_MODE
	#define	DEBUG_INFO_TX_PIN	    GPIO_PC7//print
#endif

// UART
#if ZBHCI_UART
	#define UART_TX_PIN         	UART_TX_PB1
	#define UART_RX_PIN         	UART_RX_PB0

	#define UART_PIN_CFG()			uart_gpio_set(UART_TX_PIN, UART_RX_PIN);// uart tx/rx pin set
#endif


enum {
    VK_SW1 = 0x01,
    VK_SW2 = 0x02,
    VK_SW3 = 0x03,
    VK_SW4 = 0x04
};

#define	KB_MAP_NORMAL           { \
                                    {VK_SW1, VK_SW3}, \
                                    {VK_SW2, VK_SW4}, \
                                }

#define	KB_MAP_NUM              KB_MAP_NORMAL
#define	KB_MAP_FN               KB_MAP_NORMAL

#define KB_DRIVE_PINS           {GPIO_PB2, GPIO_PB3}
#define KB_SCAN_PINS            {GPIO_PB4, GPIO_PB5}

#define	KB_LINE_MODE            0
#define	KB_LINE_HIGH_VALID      0
/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
