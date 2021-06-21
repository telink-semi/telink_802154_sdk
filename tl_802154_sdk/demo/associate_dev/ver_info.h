#pragma  once

#define IMAGE_TYPE_COOR								0x51
#define IMAGE_TYPE_DEVICE							0x52





#define APP_RELEASE									0x10//app release 1.0
#define APP_BUILD									0x30//app build 01
#define STACK_RELEASE								0x00//stack release 3.0
#define STACK_BUILD									0x00//stack build 01

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
