/********************************************************************************************************
 * @file	drv_flash.h
 *
 * @brief	This is the header file for drv_flash
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

#define  CFS_ENABLE 1
typedef void (*drv_flash_write)(unsigned long addr, unsigned long len, unsigned char *buf);
typedef void (*drv_flash_read)(unsigned long addr, unsigned long len, unsigned char *buf);
typedef void (*drv_flash_erase)(unsigned long addr);

typedef u8 (*drv_flash_lock)(u32 blockSize);
typedef u8 (*drv_flash_unlock)(void);

typedef struct {
    u32 mid;
    drv_flash_unlock unlock;
    drv_flash_lock lock;
    u32 blockSize;
} drv_flash_opt_t;

typedef struct{
	drv_flash_write write;
	drv_flash_read read;
	drv_flash_erase erase;
}drv_flash_t;

void flash_write(u32 addr, u32 len, u8 *buf);
bool flash_writeWithCheck(u32 addr, u32 len, u8 *buf);
void flash_read(u32 addr, u32 len, u8 *buf);
void flash_erase(u32 addr);
void flash_loadOpt(void);
void flash_lock(void);
void flash_unlock(void);


void cfs_flash_write(u32 addr, u32 len, u8 *buf);
void cfs_flash_read(u32 addr, u32 len, u8 *buf);
