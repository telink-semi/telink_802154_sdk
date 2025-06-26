/********************************************************************************************************
 * @file	compiler.h
 *
 * @brief	This is the header file for compiler
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

#if defined(MCU_CORE_826x) || defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
#define _attribute_packed_
#else
#define _attribute_packed_                              __attribute__((packed))
#endif

#define _attribute_aligned_(s)					__attribute__((aligned(s)))
#define _attribute_session_(s)					__attribute__((section(s)))

#define _attribute_no_inline_   				__attribute__((noinline))

#define _attribute_data_retention_
#define _attribute_data_retention_sec_

#define _attribute_ram_code_					__attribute__((section(".ram_code")))
#define _attribute_ram_code_sec_  				__attribute__((section(".ram_code")))
#define _attribute_ram_code_sec_noinline_		__attribute__((section(".ram_code"))) __attribute__((noinline))
#define _attribute_custom_data_  				__attribute__((section(".custom_data")))
#define _attribute_custom_bss_  				__attribute__((section(".custom_bss")))
#define _attribute_text_sec_   					__attribute__((section(".text")))
#define _attribute_aes_data_sec_      			__attribute__((section(".aes_data")))

#ifndef STD_GCC //standard open source risc-V GCC
#define _attribute_ram_code_sec_optimize_o2_            __attribute__((section(".ram_code"))) __attribute__((optimize("O2"))) __attribute__((no_execit))
#define _attribute_ram_code_sec_optimize_o2_noinline_   __attribute__((noinline)) __attribute__((section(".ram_code"))) __attribute__((optimize("O2"))) __attribute__((no_execit))
#else
#define _attribute_ram_code_sec_optimize_o2_            __attribute__((section(".ram_code"))) __attribute__((optimize("O2")))
#define _attribute_ram_code_sec_optimize_o2_noinline_   __attribute__((noinline)) __attribute__((section(".ram_code"))) __attribute__((optimize("O2")))
#endif


/**
 *  _always_inline needs to be added in the following two cases:
 * 1. The subfunctions in the pm_sleep_wakeup function need to use _always_inline and _attribute_ram_code_sec_optimize_o2_, as detailed in the internal comments of pm_sleep_wakeup.
 * 2. The BLE SDK uses interrupt preemption, flash interface operations can be interrupted by high-priority interrupts, which requires that the high-priority interrupt handler function,can not have a text segment of code.
 *    So the BLE SDK provides the following requirements: Add the following function to _always_inline: rf_set_tx_rx_off, rf_set_ble_crc_adv, rf_set_ble_crc_value, rf_set_rx_maxlen, stimer_get_tick, clock_time_exceed, rf_receiving_flag, dma_config, gpio_toggle, gpio_set_low_level, gpio_set_level.
 */
#define _always_inline                                  inline __attribute__((always_inline))
