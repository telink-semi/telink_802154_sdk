/********************************************************************************************************
 * @file    app_cfg.h
 *
 * @brief   This is the header file for app_cfg
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
/* ieee802154 security module */
#define  ZB_MAC_SECURITY						 1

/* nv module */
#define  NV_ENABLE								 1

/* Watch dog module */
#define MODULE_WATCHDOG_ENABLE					 0

/* Voltage detect module */
#define VOLTAGE_DETECT_ENABLE					 1

/* PM */
#define  PM_ENABLE								 1

/* poll rate ms */
#define  NORMAL_POLL_RATE						 1*1000

/* poll rate ms */
#define  OTA_POLL_RATE						 	 200

/* query rate ms */
#define  QUERY_RATE						 	     30


/* Flash protect module */
/* Only the firmware area will be locked, the NV data area will not be locked.
 * For details, please refer to drv_flash.c file.
 */
#define FLASH_PROTECT_ENABLE                      1

/**
   Auto definition for the role
 */
#if (COORDINATOR)
    #define ZB_ROUTER_ROLE                        1
    #define ZB_COORDINATOR_ROLE                   1
#elif (END_DEVICE)
    #define ZB_ED_ROLE                            1
#endif


/***********************************************************************
 * If PM_ENABLE is set, the macro ZB_MAC_RX_ON_WHEN_IDLE must be ZERO.
 */
#if ZB_ED_ROLE
	#if PM_ENABLE
		#define ZB_MAC_RX_ON_WHEN_IDLE			  0//must 0
	#endif

	#ifndef ZB_MAC_RX_ON_WHEN_IDLE
		#define ZB_MAC_RX_ON_WHEN_IDLE			  0//set 0 or 1
	#endif
#endif

/* HCI interface */
#define	ZBHCI_UART					0
#define ZBHCI_USB_CDC				0
#define ZBHCI_USB_HID				0
#define ZBHCI_USB_PRINT				0
#define ZBHCI_SWIRE_MODE			0


#if (ZBHCI_USB_PRINT || ZBHCI_USB_CDC || ZBHCI_USB_HID || ZBHCI_UART || ZBHCI_SWIRE_MODE)
	#define ZBHCI_EN								1
#endif




/**********************************************************************
 * Product Information
 */
/**
 *  @brief  Working channel
 *          Valid value: 11 ~ 26
 */
#define DEFAULT_CHANNEL             24



/* Board ID */
#define BOARD_826x_EVK				0
#define BOARD_826x_DONGLE			1
#define BOARD_8258_EVK				2
#define BOARD_8258_DONGLE			3
#define BOARD_8278_EVK				4
#define BOARD_8278_DONGLE			5
#define BOARD_B91_EVK				6
#define BOARD_B91_DONGLE			7

/* Board define */
#if defined(MCU_CORE_826x)
	#define BOARD					BOARD_826x_DONGLE
	/* system clock config */
	#define CLOCK_SYS_CLOCK_HZ  	32000000
#elif defined(MCU_CORE_8258)
	#define BOARD					BOARD_8258_DONGLE//BOARD_8258_EVK
	/* system clock config */
	#define CLOCK_SYS_CLOCK_HZ  	32000000
#elif defined(MCU_CORE_8278)
	#define BOARD					BOARD_8278_DONGLE//BOARD_8278_EVK
	/* system clock config */
	#define CLOCK_SYS_CLOCK_HZ  	32000000
#elif defined(MCU_CORE_B91)
	#define BOARD					BOARD_B91_EVK
	/* system clock config */
	#define CLOCK_SYS_CLOCK_HZ  	48000000
#else
	#error "MCU is undefined!"
#endif

/* Board include */
#if (BOARD == BOARD_826x_EVK)
	#include "board_826x_evk.h"
#elif	(BOARD == BOARD_826x_DONGLE)
	#include "board_826x_dongle.h"
#elif(BOARD == BOARD_8258_DONGLE)
	#include "board_8258_dongle.h"
#elif(BOARD == BOARD_8258_EVK)
	#include "board_8258_evk.h"
#elif(BOARD == BOARD_8278_EVK)
	#include "board_8278_evk.h"
#elif(BOARD == BOARD_8278_DONGLE)
	#include "board_8278_dongle.h"
#elif (BOARD == BOARD_B91_EVK)
	#include "board_b91_evk.h"
#elif (BOARD == BOARD_B91_DONGLE)
	#include "board_b91_dongle.h"
#endif


/* Watch dog module */
#define MODULE_WATCHDOG_ENABLE						0



/**********************************************************************
 * EV configuration
 */
typedef enum{
	EV_EVENT_MAX = 1,
}ev_event_e;

enum{
	EV_FIRED_EVENT_MAX = 1
};

typedef enum{
	EV_POLL_ED_DETECT,
	EV_POLL_HCI,
    EV_POLL_IDLE,
	EV_POLL_MAX,
}ev_poll_e;

/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
