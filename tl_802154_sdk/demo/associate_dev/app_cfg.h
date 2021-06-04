/********************************************************************************************************
 * @file	app_cfg.h
 *
 * @brief	This is the header file for app_cfg
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
/* 802154 security module */
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
#define  OTA_POLL_RATE						 	 250

/* query rate ms */
#define  QUERY_RATE						 	     30

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
#define	ZBHCI_UART					1
#define ZBHCI_USB_CDC				0
#define ZBHCI_USB_HID				0
#define ZBHCI_USB_PRINT				0



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
