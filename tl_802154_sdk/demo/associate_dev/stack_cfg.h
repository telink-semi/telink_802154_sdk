/********************************************************************************************************
 * @file    stack_cfg.h
 *
 * @brief   This is the header file for stack_cfg
 *
 * @author  Zigbee Group
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *                      All rights reserved.
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

/**
 *  @brief  Working channel
 *          Valid value: 11 ~ 26
 */
#define DEFAULT_CHANNEL                 24

/**
 *  @brief  NVRAM
 */
#define NV_ENABLE                       1

/* ieee802154 security module */
//#define ZB_MAC_SECURITY               1


/**********************************************************************
 * Following configuration will calculated automatically
 */

/**
   Auto definition for the role
 */
#if (COORDINATOR)
    #define ZB_COORDINATOR_ROLE         1
#elif (END_DEVICE)
    #define ZB_ED_ROLE                  1
#endif


/***********************************************************************
 * If PM_ENABLE is set, the macro ZB_MAC_RX_ON_WHEN_IDLE must be ZERO.
 */
#if ZB_ED_ROLE
#if PM_ENABLE
    #define ZB_MAC_RX_ON_WHEN_IDLE      0//must 0
#endif

#ifndef ZB_MAC_RX_ON_WHEN_IDLE
    #define ZB_MAC_RX_ON_WHEN_IDLE      0//set 0 or 1
#endif
#endif
