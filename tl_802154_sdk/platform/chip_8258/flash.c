/********************************************************************************************************
 * @file	flash.c
 *
 * @brief	This is the source file for B85
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
#include "flash.h"
#include "spi_i.h"
#include "irq.h"
#include "timer.h"
#include "./flash/flash_common.h"
_attribute_ram_code_ static inline int flash_is_busy(void){
	return mspi_read() & 0x01;				//  the busy bit, pls check flash spec
}

/**
 * @brief     This function serves to set flash write command.
 * @param[in] cmd - set command.
 * @return    none
 */
_attribute_ram_code_ static void flash_send_cmd(unsigned char cmd){
	mspi_high();
	sleep_us(1);
	mspi_low();
	mspi_write(cmd);
	mspi_wait();
}

/**
 * @brief     This function serves to send flash address.
 * @param[in] addr - the flash address.
 * @return    none
 */
_attribute_ram_code_ static void flash_send_addr(unsigned int addr){
	mspi_write((unsigned char)(addr>>16));
	mspi_wait();
	mspi_write((unsigned char)(addr>>8));
	mspi_wait();
	mspi_write((unsigned char)(addr));
	mspi_wait();
}

/**
 * @brief     This function serves to wait flash done.
 *            (make this a asynchronous version).
 * @param[in] none.
 * @return    none.
 */
_attribute_ram_code_ static void flash_wait_done(void)
{
	sleep_us(100);
	flash_send_cmd(FLASH_READ_STATUS_CMD);

	int i;
	for(i = 0; i < 10000000; ++i){
		if(!flash_is_busy()){
			break;
		}
	}
	mspi_high();
}


/**
 * @brief This function serves to erase a sector.
 * @param[in]   addr the start address of the sector needs to erase.
 * @return none
 */
_attribute_ram_code_ void flash_erase_sector(unsigned long addr){
	unsigned char r = irq_disable();

	flash_send_cmd(FLASH_WRITE_ENABLE_CMD);
	flash_send_cmd(FLASH_SECT_ERASE_CMD);
	flash_send_addr(addr);
	mspi_high();
	flash_wait_done();

	irq_restore(r);
}


/**
 * @brief This function writes the buffer's content to a page.
 * @param[in]   addr the start address of the page
 * @param[in]   len the length(in byte) of content needs to write into the page
 * @param[in]   buf the start address of the content needs to write into
 * @return none
 */
_attribute_ram_code_ void flash_write_page(unsigned long addr, unsigned long len, unsigned char *buf){
	unsigned char r = irq_disable();

	// important:  buf must not reside at flash, such as constant string.  If that case, pls copy to memory first before write
	flash_send_cmd(FLASH_WRITE_ENABLE_CMD);
	flash_send_cmd(FLASH_WRITE_CMD);
	flash_send_addr(addr);

	unsigned int i;
	for(i = 0; i < len; ++i){
		mspi_write(buf[i]);		/* write data */
		mspi_wait();
	}
	mspi_high();
	flash_wait_done();

	irq_restore(r);
}

/**
 * @brief This function reads the content from a page to the buf.
 * @param[in]   addr the start address of the page
 * @param[in]   len the length(in byte) of content needs to read out from the page
 * @param[out]  buf the start address of the buffer
 * @return none
 */
_attribute_ram_code_ void flash_read_page(unsigned long addr, unsigned long len, unsigned char *buf){
	unsigned char r = irq_disable();


	flash_send_cmd(FLASH_READ_CMD);
	flash_send_addr(addr);

	mspi_write(0x00);		/* dummy,  to issue clock */
	mspi_wait();
	mspi_ctrl_write(0x0a);	/* auto mode */
	mspi_wait();
	/* get data */
	for(int i = 0; i < len; ++i){
		*buf++ = mspi_get();
		mspi_wait();
	}
	mspi_high();

	irq_restore(r);
}


/* according to your application */
#if 1

/**
 * @brief This function serves to erase a page(256 bytes).
 * @param[in]   addr the start address of the page needs to erase.
 * @return none
 */
_attribute_ram_code_ void flash_erase_page(unsigned int addr)
{
	unsigned char r = irq_disable();

	flash_send_cmd(FLASH_WRITE_ENABLE_CMD);
	flash_send_cmd(FLASH_PAGE_ERASE_CMD);
	flash_send_addr(addr);
	mspi_high();
	flash_wait_done();

    irq_restore(r);
}

/**
 * @brief This function serves to erase a block(32k).
 * @param[in]   addr the start address of the block needs to erase.
 * @return none
 */
_attribute_ram_code_ void flash_erase_32kblock(unsigned int addr)
{
	unsigned char r = irq_disable();

	flash_send_cmd(FLASH_WRITE_ENABLE_CMD);
	flash_send_cmd(FLASH_32KBLK_ERASE_CMD);
	flash_send_addr(addr);
	mspi_high();
	flash_wait_done();

    irq_restore(r);
}

/**
 * @brief This function serves to erase a block(64k).
 * @param[in]   addr the start address of the block needs to erase.
 * @return none
 */
_attribute_ram_code_ void flash_erase_64kblock(unsigned int addr)
{
	unsigned char r = irq_disable();

	flash_send_cmd(FLASH_WRITE_ENABLE_CMD);
	flash_send_cmd(FLASH_64KBLK_ERASE_CMD);
	flash_send_addr(addr);
	mspi_high();
	flash_wait_done();

    irq_restore(r);
}

/**
 * @brief This function serves to erase a page(256 bytes).
 * @param[in]   addr the start address of the page needs to erase.
 * @return none
 */
_attribute_ram_code_ void flash_erase_chip(void)
{
	unsigned char r = irq_disable();

	flash_send_cmd(FLASH_WRITE_ENABLE_CMD);
	flash_send_cmd(FLASH_CHIP_ERASE_CMD);
	mspi_high();
	flash_wait_done();

    irq_restore(r);
}

/**
 * @brief This function write the 8bit status of flash.
 * @param[in]  the value of status
 * @return     status
 */
_attribute_ram_code_ unsigned char flash_write_status(unsigned char data)
{
	unsigned char r = irq_disable();
	unsigned char result;
	//int i;
	flash_send_cmd(FLASH_WRITE_ENABLE_CMD);
	flash_send_cmd(FLASH_WRITE_STATUS_CMD);
	mspi_write(data);
	mspi_wait();
	mspi_high();
	flash_wait_done();

	sleep_us(100);
	flash_send_cmd(FLASH_READ_STATUS_CMD);

	result = mspi_read();
	mspi_high();

	irq_restore(r);
	return  result;
}

/**
 * @brief This function reads the 8bit status of flash.
 * @param[in]  none
 * @return none
 */
_attribute_ram_code_ unsigned char flash_read_status(void){
	unsigned char r = irq_disable();
	unsigned char status =0;
	flash_send_cmd(FLASH_READ_STATUS_CMD);
	/* get low 8 bit status */
	status = mspi_read();
	mspi_high();
	irq_restore(r);
	return status;
}

/**
 * @brief This function write the 16bit status register of flash.
 * @param[in]  the value of status
 * @return  none
 */
_attribute_ram_code_ void flash_write_16bit_status(unsigned short data)
{
	unsigned char r = irq_disable();

	flash_send_cmd(FLASH_WRITE_ENABLE_CMD);
	flash_send_cmd(FLASH_WRITE_STATUS_CMD);
	mspi_write((unsigned char)data);
	mspi_wait();
	mspi_write((unsigned char)(data>>8));
	mspi_wait();
	mspi_high();
	flash_wait_done();
	mspi_high();

	irq_restore(r);
}

/**
 * @brief This function reads the 16bit status register of flash.
 * @param[in]  none
 * @return     the 16bit status of flash
 */
_attribute_ram_code_ unsigned short flash_read_16bit_status(void){
	unsigned char r = irq_disable();
	unsigned short status =0;

	unsigned char buf[2]={0};
	flash_send_cmd(FLASH_READ_STATUS_HIGH_CMD);
	/* get high 8 bit status */
	buf[0]=mspi_read();
	mspi_high();
    sleep_ms(1);
    /* get low 8 bit status */
	flash_send_cmd(FLASH_READ_STATUS_CMD);
	buf[1]=mspi_read();
	mspi_high();

	irq_restore(r);

	status=buf[0];
	status=status<<8;
	status=(status|buf[1]);

	return status;
}




/**
 * @brief  	Deep Power Down mode to put the device in the lowest consumption mode
 * 			it can be used as an extra software protection mechanism,while the device
 * 			is not in active use,since in the mode,  all write,Program and Erase commands
 * 			are ignored,except the Release from Deep Power-Down and Read Device ID(RDI)
 * 			command.This release the device from this mode
 * @param[in] none
 * @return none.
 */
_attribute_ram_code_ void flash_deep_powerdown(void)
{
	unsigned char r = irq_disable();

	flash_send_cmd(FLASH_POWER_DOWN);
	mspi_high();
	sleep_us(1);

    irq_restore(r);
}

/**
 * @brief		The Release from Power-Down or High Performance Mode/Device ID command is a
 * 				Multi-purpose command.it can be used to release the device from the power-Down
 * 				State or High Performance Mode or obtain the devices electronic identification
 * 				(ID)number.Release from Power-Down will take the time duration of tRES1 before
 * 				the device will resume normal operation and other command are accepted.The CS#
 * 				pin must remain high during the tRES1(8us) time duration.
 * @param[in] none
 * @return none.
 */
_attribute_ram_code_ void flash_release_deep_powerdown(void)
{
	unsigned char r = irq_disable();

	flash_send_cmd(FLASH_POWER_DOWN_RELEASE);
	mspi_high();
	flash_wait_done();
	mspi_high();

    irq_restore(r);
}
#endif
/**
 * @brief	  MAC id. Before reading UID of flash, you must read MID of flash. and then you can
 *            look up the related table to select the idcmd and read UID of flash
 * @param[in] buf - store MID of flash
 * @return    none.
 */
_attribute_ram_code_ void flash_read_mid(unsigned char *buf){
	unsigned char j = 0;
	unsigned char r = irq_disable();
	flash_send_cmd(FLASH_GET_JEDEC_ID);
	mspi_write(0x00);		/* dummy,  to issue clock */
	mspi_wait();
	mspi_ctrl_write(0x0a);	/* auto mode */
	mspi_wait();

	for(j = 0; j < 3; ++j){
		*buf++ = mspi_get();
		mspi_wait();
	}
	mspi_high();

	irq_restore(r);
}
/**
 * @brief	  UID. Before reading UID of flash, you must read MID of flash. and then you can
 *            look up the related table to select the idcmd and read UID of flash
 * @param[in] idcmd - get this value to look up the table based on MID of flash
 * @param[in] buf   - store UID of flash
 * @return    none.
 */
_attribute_ram_code_ void flash_read_uid(unsigned char idcmd,unsigned char *buf)
{
	unsigned char j = 0;
	unsigned char r = irq_disable();
	flash_send_cmd(idcmd);
	if(idcmd==FLASH_GD_PUYA_READ_UID_CMD)				//< GD/puya
	{
		flash_send_addr(0x00);
		mspi_write(0x00);		/* dummy,  to issue clock */
		mspi_wait();
	}
	else if (idcmd==FLASH_XTX_READ_UID_CMD)		//< XTX
	{
		flash_send_addr(0x80);
		mspi_write(0x00);		/* dummy,  to issue clock */
		mspi_wait();

	}
	mspi_write(0x00);			/* dummy,  to issue clock */
	mspi_wait();
	mspi_ctrl_write(0x0a);		/* auto mode */
	mspi_wait();

	for(j = 0; j < 16; ++j){
		*buf++ = mspi_get();
		mspi_wait();
	}
	mspi_high();
	irq_restore(r);
}
///**
// * @brief 		 This function serves to read flash mid and uid,and check the correctness of mid and uid.
// * @param[out]   flash_mid - Flash Manufacturer ID
// * @param[out]   flash_uid - Flash Unique ID
// * @return       0:error 1:ok
//
// */
//_attribute_ram_code_ int flash_read_mid_uid_with_check( unsigned int *flash_mid ,unsigned char *flash_uid)
//{
//	  unsigned char no_uid[16]={0x51,0x01,0x51,0x01,0x51,0x01,0x51,0x01,0x51,0x01,0x51,0x01,0x51,0x01,0x51,0x01};
//	  int i,f_cnt=0;
//	  unsigned int mid;
//	  flash_read_mid((unsigned char*)&mid);
//	  mid = mid&0xffff;
//	  *flash_mid  = mid;
//	 /*
//	   Flash Type    CMD        MID      Company
//	   GD25LD40C 	 0x4b     0x60C8     GD
//	   GD25LD05C  	 0x4b 	  0x60C8     GD
//	   P25Q40L   	 0x4b     0x6085     PUYA
//	   MD25D40DGIG	 0x4b     0x4051     GD
//	   GD25D10C      0x4b     0x40C8     GD
//	   PN25F04C      0x5a     0x311C     XTX
//	 */
//	  if( (mid == 0x60C8) || (mid == 0x6085) ||(mid == 0x4051)||(mid==0x40C8)){
//		  flash_read_uid(FLASH_GD_PUYA_READ_UID_CMD,(unsigned char *)flash_uid);
//	  }
//	  else if(mid==0x311C){
//		  flash_read_uid(FLASH_XTX_READ_UID_CMD	,(unsigned char *)flash_uid);
//	  }
//	  else{
//		  return 0;
//	  }
//	  for(i=0;i<16;i++){
//		if(flash_uid[i]==no_uid[i]){
//			f_cnt++;
//		}
//	  }
//	  if(f_cnt==16){//no uid flash
//			  return 0;
//
//	  }else{
//		  return  1;
//	  }
//}




/*******************************************************************************************************************
 *												Secondary interface
 ******************************************************************************************************************/

/**
 * @brief		This function serves to get flash vendor.
 * @param[in]	none.
 * @return		0 - err, other - flash vendor.
 */
unsigned int flash_get_vendor(unsigned int flash_mid)
{
	switch(flash_mid&0x0000ffff)
	{
	case 0x0000325E:
		return FLASH_ETOX_ZB;
	case 0x000060C8:
		return FLASH_ETOX_GD;
	case 0x00004051:
		return FLASH_ETOX_GD;
	case 0x00006085:
		return FLASH_SONOS_PUYA;
	case 0x000060EB:
		return FLASH_SONOS_TH;
	case 0x000060CD:
		return FLASH_SST_TH;
	default:
		return 0;
	}
}
