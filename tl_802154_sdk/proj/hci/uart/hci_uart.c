/********************************************************************************************************
 * @file	hci_uart.c
 *
 * @brief	This is the source file for hci_uart
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
#include "tl_common.h"

#if (ZBHCI_EN && ZBHCI_UART)
#include "zb_task_queue.h"
#include "../zbhci.h"

void uart_txMsg(u16 u16Length, u8 *pu8Data);

#define UART_TX_BUF_SIZE    64
#define UART_RX_BUF_SIZE    64


//begin
typedef struct{
	u32 dataLen;
	u8 dataPayload[1];
}uart_rxData_t;

__attribute__((aligned(4))) u8 uartTxBuf[UART_TX_BUF_SIZE] = {0};
__attribute__((aligned(4))) u8 uartRxBuf[UART_RX_BUF_SIZE] = {0};

void uart_data_handler(void *arg){

	 /*
	 * the format of the uart rx data: length(4 Bytes) + payload
	 *
	 * */
	uart_rxData_t *rxData = (uart_rxData_t *)uartRxBuf;
	if(rxData->dataLen>0&&rxData->dataLen<64)
	{
		uart_txMsg(rxData->dataLen,rxData->dataPayload);
	}
}

void uartRcvHandler(void){

	TL_SCHEDULE_TASK(uart_data_handler, uartRxBuf);
}

void uart_txMsg(u16 u16Length, u8 *pu8Data)
{
    drv_uart_tx_start(pu8Data, u16Length);
}


void hci_uart_init(void){
	UART_PIN_CFG();
	drv_uart_init(115200, uartRxBuf, UART_RX_BUF_SIZE, uartRcvHandler);
}

#endif


