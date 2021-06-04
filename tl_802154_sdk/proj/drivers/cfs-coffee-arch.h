/********************************************************************************************************
 * @file	cfs-coffee-arch.h
 *
 * @brief	This is the header file for cfs-coffee-arch
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
#ifndef CFS_COFFEE_ARCH_H_
#define CFS_COFFEE_ARCH_H_


#include "cfs-coffee.h"
#include "drv_flash.h"


/** Logical sector size */
#define COFFEE_SECTOR_SIZE     4096

/** Logical page size */
#define COFFEE_PAGE_SIZE       256

/** Start offset of the file system */
#define COFFEE_START           0x30000

/** Total size in bytes of the file system */
#define COFFEE_SIZE            (1024*64)

/** Maximal filename length */
#define COFFEE_NAME_LENGTH     40

/** Number of file cache entries */
#define COFFEE_MAX_OPEN_FILES  5

/** Number of file descriptor entries */
#define COFFEE_FD_SET_SIZE     5

/** Maximal amount of log table entries read in one batch */
#define COFFEE_LOG_TABLE_LIMIT 16

/** Default reserved file size */
#define COFFEE_DYN_SIZE        (COFFEE_SECTOR_SIZE - 50)

/** Default micro-log size */
#define COFFEE_LOG_SIZE        (4 * COFFEE_PAGE_SIZE)

/** Whether Coffee will use micro logs */
#define COFFEE_MICRO_LOGS      1

/** Whether files are expected to be appended to only */
#define COFFEE_APPEND_ONLY     0


/** Erase */
#define COFFEE_ERASE(sector) \
  flash_erase(COFFEE_START + (sector) * COFFEE_SECTOR_SIZE)
/** Write */
#define COFFEE_WRITE(buf, size, offset) \
  cfs_flash_write(COFFEE_START + (offset),(size),(buf))
/** Read */
#define COFFEE_READ(buf, size, offset) \
  cfs_flash_read(COFFEE_START + (offset),(size),(buf))


#endif
