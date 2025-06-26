/********************************************************************************************************
 * @file    version_cfg.h
 *
 * @brief   This is the header file for version_cfg
 *
 * @author  Zigbee Group
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *			All rights reserved.
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
#pragma  once

#define IMAGE_TYPE_COOR								0x51
#define IMAGE_TYPE_DEVICE							0x52





#define APP_RELEASE									0x10//app release 1.0
#define APP_BUILD									0x40//app build 4.0
#define STACK_RELEASE								0x00//stack release 0.0
#define STACK_BUILD									0x00//stack build 0.0

#define FILE_VERSION 								((APP_RELEASE << 24) | (APP_BUILD << 16) | (STACK_RELEASE << 8) | STACK_BUILD)



#define MANUFACTURER_CODE_TELINK  					0x1141

#if (MCU_CORE_8258)
#define CHIP_ID  					0x58
#elif (MCU_CORE_826x)
	#ifdef CHIP_8269
		#define CHIP_ID  			0x69
	#else
		#define CHIP_ID  			0x67
	#endif
#elif (MCU_CORE_8278)
#define CHIP_ID  					0x78
#elif (MCU_CORE_B91)
#define CHIP_ID  					0x95
#else
#define CHIP_ID  					0x0000
#endif


#define	IMAGE_TYPE								  	((CHIP_ID << 8) | IMAGE_TYPE_DEVICE)
