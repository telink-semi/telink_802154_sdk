/********************************************************************************************************
 * @file	drv_flash.c
 *
 * @brief	This is the source file for drv_flash
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
#include "../tl_common.h"

#if FLASH_PROTECT_ENABLE

/*
 * Only the firmware area will be locked, the NV data area will not be locked.
 *
 * The SDK will select how much space to lock based on the size of the flash, to ensure
 * that the firmware area is protected while the NV data area is not locked. If the
 * conditions cannot be met, give up using the flash lock function.
 */
//#define IS_FLASH_LOCK_NOT_ALLOWED()     ((NV_BASE_ADDRESS < 0x20000) ? 1 : 0)


drv_flash_opt_t *pFlashOpt = NULL;
drv_flash_opt_t g_flashOptTable = {0};
bool g_flashLocked = FALSE;

const drv_flash_opt_t c_flashOptList[] = {
#if defined(MCU_CORE_8258)
    //512K
    {0x13325e, flash_unlock_mid13325e, flash_lock_mid13325e, FLASH_LOCK_ALL_512K_MID13325E},
    {0x134051, flash_unlock_mid134051, flash_lock_mid134051, FLASH_LOCK_ALL_512K_MID134051},
    {0x136085, flash_unlock_mid136085, flash_lock_mid136085, FLASH_LOCK_ALL_512K_MID136085},
    {0x1360c8, flash_unlock_mid1360c8, flash_lock_mid1360c8, FLASH_LOCK_ALL_512K_MID1360C8},
    {0x1360eb, flash_unlock_mid1360eb, flash_lock_mid1360eb, FLASH_LOCK_ALL_512K_MID1360EB},
    //1M
    {0x14325e, flash_unlock_mid14325e, flash_lock_mid14325e, FLASH_LOCK_ALL_1M_MID14325E},
    {0x1460c8, flash_unlock_mid1460c8, flash_lock_mid1460c8, FLASH_LOCK_ALL_1M_MID1460C8},
    {0x11460c8, flash_unlock_mid011460c8, flash_lock_mid011460c8, FLASH_LOCK_ALL_1M_MID011460C8}
#elif defined(MCU_CORE_8278)
    //128K
    //{0x11325e, flash_unlock_mid11325e, flash_lock_mid11325e, FLASH_LOCK_NONE_MID11325E},
    //{0x1160c8, flash_unlock_mid1160c8, flash_lock_mid1160c8, FLASH_LOCK_NONE_MID1160C8},
    //512K
    {0x13325e, flash_unlock_mid13325e, flash_lock_mid13325e, FLASH_LOCK_ALL_512K_MID13325E},
    {0x1360c8, flash_unlock_mid1360c8, flash_lock_mid1360c8, FLASH_LOCK_ALL_512K_MID1360C8},
    //1M
    {0x14325e, flash_unlock_mid14325e, flash_lock_mid14325e, FLASH_LOCK_ALL_1M_MID14325E},
    {0x1460c8, flash_unlock_mid1460c8, flash_lock_mid1460c8, FLASH_LOCK_ALL_1M_MID1460C8},
    {0x146085, flash_unlock_mid146085, flash_lock_mid146085, FLASH_LOCK_ALL_1M_MID146085},
    //2M
    {0x1570cd, flash_unlock_mid1570cd, flash_lock_mid1570cd, FLASH_LOCK_LOW_512K_MID1570CD},
#elif defined(MCU_CORE_B91)
    //1M
    {0x146085, flash_unlock_mid146085, flash_lock_mid146085, FLASH_LOCK_ALL_1M_MID146085},
    //2M
    {0x156085, flash_unlock_mid156085, flash_lock_mid156085, FLASH_LOCK_ALL_2M_MID156085},
    //4M
    {0x166085, flash_unlock_mid166085, flash_lock_mid166085, FLASH_LOCK_ALL_4M_MID166085}
#else
    {0, NULL, NULL, 0}
#endif
};
#define FLASH_OPT_LIST_NUM      (sizeof(c_flashOptList) / sizeof(drv_flash_opt_t))
#endif	/* FLASH_PROTECT_ENABLE */

void flash_loadOpt(void)
{
#if FLASH_PROTECT_ENABLE
#if defined(MCU_CORE_8258)
	flash_read_mid((unsigned char *)&g_flashOptTable.mid);
#elif defined(MCU_CORE_8278) || defined(MCU_CORE_B91)
    g_flashOptTable.mid = flash_read_mid();
#else
    return;
#endif

//    if (IS_FLASH_LOCK_NOT_ALLOWED()) {
//        pFlashOpt = NULL;
//        return;
//    }

    for (u8 i = 0; i < FLASH_OPT_LIST_NUM; i++) {
        if (g_flashOptTable.mid == c_flashOptList[i].mid) {
            g_flashOptTable.unlock = c_flashOptList[i].unlock;
            g_flashOptTable.lock = c_flashOptList[i].lock;
            g_flashOptTable.blockSize = c_flashOptList[i].blockSize;

            pFlashOpt = &g_flashOptTable;
            return;
        }
    }
#endif
}

void flash_lock(void)
{
#if FLASH_PROTECT_ENABLE
    if (pFlashOpt && pFlashOpt->lock) {
        if (g_flashLocked) {
            return;
        }

        if (pFlashOpt->lock(pFlashOpt->blockSize) == 1) {
            g_flashLocked = TRUE;
        }
    }
#endif
}

void flash_unlock(void)
{
#if FLASH_PROTECT_ENABLE
    if (pFlashOpt && pFlashOpt->unlock) {
        if (!g_flashLocked) {
            return;
        }

        if (pFlashOpt->unlock() == 1) {
            g_flashLocked = FALSE;
        }
    }
#endif
}



void flash_write(u32 addr, u32 len, u8 *buf){
#if (MODULE_WATCHDOG_ENABLE)
		drv_wd_clear();
#endif

#if (VOLTAGE_DETECT_ENABLE)
    if (drv_get_adc_data() < BATTERY_SAFETY_THRESHOLD) {
        return;
    }
#endif
	flash_write_page(addr, len, buf);
}

bool flash_writeWithCheck(u32 addr, u32 len, u8 *buf)
{
    s32 toalLen = (s32)len;
    s8 copyLen = 48;
    u8 pTemp[48];
    u8 wLen = 0;
    u32 sAddr = (u32)addr;
    u8 *pSrc = buf;

#if (MODULE_WATCHDOG_ENABLE)
    drv_wd_clear();
#endif

#if (VOLTAGE_DETECT_ENABLE)
    if (drv_get_adc_data() < BATTERY_SAFETY_THRESHOLD) {
        return FALSE;
    }
#endif

    flash_write_page(addr, len, buf);

    while (toalLen > 0) {
        wLen = (toalLen > copyLen) ? copyLen : toalLen;
        flash_read(sAddr, wLen, pTemp);
        if (memcmp(pTemp, pSrc, wLen)) {
            return FALSE;
        }
        toalLen -= wLen;
        sAddr += wLen;
        pSrc += wLen;
    }

    return TRUE;
}



void flash_read(u32 addr, u32 len, u8 *buf){
	flash_read_page(addr, len, buf);
}

void flash_erase(u32 addr){
#if (MODULE_WATCHDOG_ENABLE)
	drv_wd_clear();
#endif

#if (VOLTAGE_DETECT_ENABLE)
    if (drv_get_adc_data() < BATTERY_SAFETY_THRESHOLD) {
        return;
    }
#endif
	flash_erase_sector(addr);
}

#ifdef CFS_ENABLE


void cfs_flash_op(u8 opmode, u32 addr, u32 len, u8 *buf){
	u32 re = addr%256;

	u32 pageReLen = (re)?(256 -re):256;

	u32 writeLen = 0;
	u32 remainLen = len;

	do{
		if(remainLen <= pageReLen){
			writeLen = remainLen;
			remainLen = 0;
		}else{
			remainLen -= pageReLen;
			writeLen = pageReLen;
			pageReLen = 256;
		}
		if(opmode){
			flash_write_page(addr,writeLen,buf);
		}else{
			flash_read_page(addr,writeLen,buf);
		}
		buf += writeLen;
		addr += writeLen;
	}while(remainLen);
}

void cfs_flash_write(u32 addr, u32 len, u8 *buf){
	cfs_flash_op(1, addr, len, buf);
}

void cfs_flash_read(u32 addr, u32 len, u8 *buf){
	cfs_flash_op(0, addr, len, buf);
}
#endif

