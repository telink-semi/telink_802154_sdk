/********************************************************************************************************
 * @file     tl_specific_data.h
 *
 * @brief	 some command identifier/format for Telink specific data
 *
 * @author
 * @date     Aug. 1, 2019
 *
 * @par      Copyright (c) 2019, Telink Semiconductor (Shanghai) Co., Ltd.
 *           All rights reserved.
 *
 *			 The information contained herein is confidential and proprietary property of Telink
 * 		     Semiconductor (Shanghai) Co., Ltd. and is available under the terms
 *			 of Commercial License Agreement between Telink Semiconductor (Shanghai)
 *			 Co., Ltd. and the licensee in separate contract or the terms described here-in.
 *           This heading MUST NOT be removed from this file.
 *
 * 			 Licensees are granted free, non-transferable use of the information in this
 *			 file under Mutual Non-Disclosure Agreement. NO Warranty of ANY KIND is provided.
 *
 *******************************************************************************************************/

#ifndef _TL_SPECIFIC_DATA_H_
#define _TL_SPECIFIC_DATA_H_
#include "../../../proj/tl_common.h"



typedef enum{
	TL_SPECIFIC_ID_OTA 		= 0x01,
}tl_specificDataId_e;

typedef struct _attribute_packed_{
	u8					appId;
	u8					seqNo;
	u8					cmdId;
	u8					len;
}tl_appFrameHdr_t;

typedef struct _attribute_packed_{
	tl_appFrameHdr_t	hdr;
	u8					payload[0];
}tl_appFrameFmt_t;


extern u8 	g_tlAppDataSeqNo;

#define TL_SPECIFIC_SEQNO_ADD		(g_tlAppDataSeqNo++)

u8  tl_appDataIndicate(u8 *pd, u8 len);

#endif /* TL_OTA_H_ */
