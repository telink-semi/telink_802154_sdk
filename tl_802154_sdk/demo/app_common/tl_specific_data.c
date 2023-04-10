/********************************************************************************************************
 * @file     tl_specific_data.c
 *
 * @brief	 includes command handler to process the Telink specific data
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
 *			 file under Mutual Non-Disclosure Agreement. NO WARRENTY of ANY KIND is provided.
 *
 *******************************************************************************************************/

#include "../../../proj/tl_common.h"
#include "./ota/tl_specific_data_ota.h"
#include "tl_specific_data.h"


u8 	g_tlAppDataSeqNo = 0;

u8 tl_appDataIndicate(u8 *pd, u8 len){
	u8 ret = INVALID_EVENT_ID;
	tl_appFrameHdr_t *pHdr = (tl_appFrameHdr_t *)pd;

	if(pHdr->appId == TL_SPECIFIC_ID_OTA){
		tl_appOtaCmdHandler(pd);
		ret = SUCCESS;
	}
//	else if(pHdr->appId == TL_SPECIFIC_ID_XXX)//other applications
	return ret;
}




