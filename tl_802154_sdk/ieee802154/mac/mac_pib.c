/********************************************************************************************************
 * @file	mac_pib.c
 *
 * @brief	This is the source file for mac_pib
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
#include <ieee802154_common.h>


/* PIB access and min/max table type */
typedef struct{
    u16 offset;
    u16 len;
    u32 min;
    u32 max;
}mac_pibTbl_t;

//Telink ieee address range
const u8 startIEEEAddr[] = {0x38, 0xc1, 0xa4};

/*
 *  @brief security related values
 */
TN_MEM_DEF(macKeyIDMem, mac_keyid_lookup_desc_t, MAC_KEYID_LOOKUP_TABLE_MAX_LEN);
TN_MEM_DEF(macDeviceMem, mac_deviceDesc_t, MAC_DEV_TABLE_MAX_LEN);
TN_MEM_DEF(macKeyUsageMem, mac_keyusageDesc_t, MAC_KEY_USAGE_TABLE_MAX_LEN);
TN_MEM_DEF(macSecLvlMem, mac_seclevelDesc_t, MAC_SEC_LVL_TABLE_MAX_LEN);
TN_MEM_DEF(macKeyMem, mac_keyDesc_t, MAC_KEY_TABLE_MAX_LEN);
TN_MEM_DEF(macKeyDevMem, mac_keydevDesc_t, MAC_KEY_DEV_TABLE_MAX_LEN);

LIST_DEF(macKeyTable);
LIST_DEF(macDevTable);
LIST_DEF(macSecLvlTable);
LIST_DEF(macKeyDevTable);

void mac_secMaterial_Init(u8 coldReset)
{
    /* initialize security related PIBs */
	if(coldReset)
	{
		tn_mem_init(&macKeyIDMem);
		tn_mem_init(&macDeviceMem);
		tn_mem_init(&macKeyUsageMem);
		tn_mem_init(&macSecLvlMem);
		tn_mem_init(&macKeyMem);
		tn_mem_init(&macKeyDevMem);

		tn_list_init(macKeyTable);
		tn_list_init(macDevTable);
		tn_list_init(macSecLvlTable);
		tn_list_init(macKeyDevTable);
	}
		g_zbMacPib.keyTable = (void *)macKeyTable;
		g_zbMacPib.devTable = (void *)macDevTable;
		g_zbMacPib.secLvlTable = (void *)macSecLvlTable;
}



void *mac_secMaterial_alloc(mac_secMaterial_t type)
{
    void *ret = NULL;

    switch (type) {
        case MAC_SECMATERIAL_KEYID_LOOKUP_DESC:
            ret = tn_mem_alloc(&macKeyIDMem);
            break;
        case MAC_SECMATERIAL_DEVICE_DESC:
            ret = tn_mem_alloc(&macDeviceMem);
            break;
        case MAC_SECMATERIAL_KEYUSAGE_DESC:
            ret = tn_mem_alloc(&macKeyUsageMem);
            break;
        case MAC_SECMATERIAL_SECLEVEL_DESC:
            ret = tn_mem_alloc(&macSecLvlMem);
            break;
        case MAC_SECMATERIAL_KEY_DESC:
            ret = tn_mem_alloc(&macKeyMem);
            break;
        case MAC_SECMATERIAL_KEYDEV_DESC:
            ret = tn_mem_alloc(&macKeyDevMem);
            break;
        default:
            break;
    }

    return ret;
}

char mac_secMaterial_free(mac_secMaterial_t type, void *item)
{
    char ret = -1;

    switch (type) {
        case MAC_SECMATERIAL_KEYID_LOOKUP_DESC:
            ret = tn_mem_free(&macKeyIDMem, item);
            break;
        case MAC_SECMATERIAL_DEVICE_DESC:
            ret = tn_mem_free(&macDeviceMem, item);
            break;
        case MAC_SECMATERIAL_KEYUSAGE_DESC:
            ret = tn_mem_free(&macKeyUsageMem, item);
            break;
        case MAC_SECMATERIAL_SECLEVEL_DESC:
            ret = tn_mem_free(&macSecLvlMem, item);
            break;
        case MAC_SECMATERIAL_KEY_DESC:
            ret = tn_mem_free(&macKeyMem, item);
            break;
        case MAC_SECMATERIAL_KEYDEV_DESC:
            ret = tn_mem_free(&macKeyDevMem, item);
            break;
        default:
            break;
    }

    return ret;
}


static void mac_key_free(mac_keyDesc_t *key)
{
    //free key_id_lookup_list
    while (key->key_id_lookup_list_entries--) {
        tn_mem_free(&macKeyIDMem, tn_list_pop(key->key_id_lookup_list));
    }

    //free key_device_list
    while (key->key_device_list_entries--) {
        tn_mem_free(&macKeyDevMem, tn_list_pop(key->key_device_list));
    }

    //free key_usage_list
    while (key->key_usage_list_entries--) {
        tn_mem_free(&macKeyUsageMem, tn_list_pop(key->key_usage_list));
    }

    //free key
    tn_mem_free(&macKeyMem, (void *)key);
}

const mac_pibTbl_t g_zbMacPibTbl[] = {
	{OFFSETOF(tl_zb_mac_pib_t, ackWaitDuration), 	sizeof(u8), 54, 54},                         /* MAC_ACK_WAIT_DURATION */
	{OFFSETOF(tl_zb_mac_pib_t, associationPermit), 	sizeof(bool), FALSE, TRUE},                /* MAC_ASSOCIATION_PERMIT */
	{OFFSETOF(tl_zb_mac_pib_t, autoReq), 			sizeof(bool), FALSE, TRUE},                      /* MAC_AUTO_REQUEST */
	{OFFSETOF(tl_zb_mac_pib_t, battLifeExt), 		sizeof(bool), FALSE, TRUE},                      /* MAC_BATT_LIFE_EXT */
	{OFFSETOF(tl_zb_mac_pib_t, battLifeExtPeriods), sizeof(u8), 6, 6},                        /* MAC_BATT_LIFE_EXT_PERIODS */
	{OFFSETOF(tl_zb_mac_pib_t, beaconPayload), 		sizeof(zb_mac_beacon_payload_t), 0, 0},                          /* MAC_BEACON_PAYLOAD */
	{OFFSETOF(tl_zb_mac_pib_t, beaconPayloadLen), 	sizeof(u8), 0, 52},                      /* MAC_BEACON_PAYLOAD_LENGTH */
	{OFFSETOF(tl_zb_mac_pib_t, beaconOrder), 		sizeof(u8), 0, 15},                              /* MAC_BEACON_ORDER */
	{OFFSETOF(tl_zb_mac_pib_t, beaconTxTime), 		sizeof(u32), 1, 1},                             /* MAC_BEACON_TX_TIME */
	{OFFSETOF(tl_zb_mac_pib_t, beaconSeqNum), 		sizeof(u8), 0x00, 0xFF},                              /* MAC_BSN */
	{OFFSETOF(tl_zb_mac_pib_t, coordExtAddress), 	sizeof(addrExt_t), 0, 0},  /* MAC_COORD_EXTENDED_ADDRESS */
	{OFFSETOF(tl_zb_mac_pib_t, coordShortAddress), 	sizeof(u16), 0, 0},                        /* MAC_COORD_SHORT_ADDRESS */
	{OFFSETOF(tl_zb_mac_pib_t, seqNum), 			sizeof(u8), 0x00, 0xFF},                                 /* MAC_DSN */
	{OFFSETOF(tl_zb_mac_pib_t, gtsPermit), 			sizeof(bool), FALSE, TRUE},                        /* MAC_GTS_PERMIT */
	{OFFSETOF(tl_zb_mac_pib_t, maxCsmaBackoffs), 	sizeof(u8), 0, 254},                         /* MAC_MAX_CSMA_BACKOFFS */
	{OFFSETOF(tl_zb_mac_pib_t, minBe), 				sizeof(u8), 0, 8},                                     /* MAC_MIN_BE */
	{OFFSETOF(tl_zb_mac_pib_t, panId), 				sizeof(u16), 0, 0},                                    /* MAC_PAN_ID */
	{OFFSETOF(tl_zb_mac_pib_t, promiscuousMode), 	sizeof(bool), FALSE, TRUE},                  /* MAC_PROMISCUOUS_MODE */
	{OFFSETOF(tl_zb_mac_pib_t, rxOnWhenIdle), 		sizeof(bool), FALSE, TRUE},                     /* MAC_RX_ON_WHEN_IDLE */
	{OFFSETOF(tl_zb_mac_pib_t, shortAddress), 		sizeof(u16), 0, 0},                             /* MAC_SHORT_ADDRESS */
	{OFFSETOF(tl_zb_mac_pib_t, superframeOrder), 	sizeof(u8), 0, 15},                          /* MAC_SUPERFRAME_ORDER */
	{OFFSETOF(tl_zb_mac_pib_t, transactionPersistenceTime), sizeof(u16), 0, 0},               /* MAC_TRANSACTION_PERSISTENCE_TIME */
	{OFFSETOF(tl_zb_mac_pib_t, associatedPanCoord), sizeof(bool), FALSE, TRUE},               /* MAC_ASSOCIATED_PAN_COORD */
	{OFFSETOF(tl_zb_mac_pib_t, maxBe), 				sizeof(u8), 0, 8},                                     /* MAC_MAX_BE */
	{OFFSETOF(tl_zb_mac_pib_t, frameTotalWaitTime), sizeof(u16), 0x00, 0xFF},              /* MAC_MAX_FRAME_RESPONSE_TIME */
	{OFFSETOF(tl_zb_mac_pib_t, frameRetryNum), 		sizeof(u8), 0, 7},                           /* MAC_MAX_FRAME_RETRIES */
	{OFFSETOF(tl_zb_mac_pib_t, respWaitTime), 		sizeof(u8), 2, 64},                         /* MAC_RESPONSE_WAIT_TIME */
	{OFFSETOF(tl_zb_mac_pib_t, syncSymbolOffset), 	sizeof(u8), 0, 0},                          /* MAC_SYNC_SYMBOL_OFFSET */
	{OFFSETOF(tl_zb_mac_pib_t, timeStampSupported), sizeof(bool), FALSE, TRUE},               /* MAC_TIMESTAMP_SUPPORTED */
	{OFFSETOF(tl_zb_mac_pib_t, securityEnabled), 	sizeof(bool), FALSE, TRUE},                  /* MAC_SECURITY_ENABLED */
	{OFFSETOF(tl_zb_mac_pib_t, extAddress), 		sizeof(addrExt_t), FALSE, TRUE},                  /* MAC_SECURITY_ENABLED */
	{OFFSETOF(tl_zb_mac_pib_t, txPower), 			sizeof(u8), 0, 11},                  /* MAC_SECURITY_ENABLED */


    /* Security related PIBs*/
    {OFFSETOF(tl_zb_mac_pib_t, keyTable), sizeof(void *), 0, 0},
    {OFFSETOF(tl_zb_mac_pib_t, keyTableEntries), sizeof(u8), 0, MAC_KEY_TABLE_MAX_LEN},
    {OFFSETOF(tl_zb_mac_pib_t, devTable), sizeof(void *), 0, 0},
    {OFFSETOF(tl_zb_mac_pib_t, devTableEntries), sizeof(u8), 0, MAC_DEV_TABLE_MAX_LEN},
    {OFFSETOF(tl_zb_mac_pib_t, secLvlTable), sizeof(void *), 0, 0},
    {OFFSETOF(tl_zb_mac_pib_t, secLvlTableEntries), sizeof(u8), 0, MAC_SEC_LVL_TABLE_MAX_LEN},
    {OFFSETOF(tl_zb_mac_pib_t, frameCounter), sizeof(u32), 0, 0xffffffff},
    {OFFSETOF(tl_zb_mac_pib_t, autoReqSecLvl), sizeof(u8), 0, 7},
    {OFFSETOF(tl_zb_mac_pib_t, autoReqKeyIdMode), sizeof(u8), 0, 3},
    {OFFSETOF(tl_zb_mac_pib_t, autoReqKeySource), 8, 0, 0},
    {OFFSETOF(tl_zb_mac_pib_t, autoReqKeyIndex), sizeof(u8), 1, 0xff},
    {OFFSETOF(tl_zb_mac_pib_t, defaultKeySource), 8, 0, 0},
    {OFFSETOF(tl_zb_mac_pib_t, panCoordExtAddr), 8, 0, 0},
    {OFFSETOF(tl_zb_mac_pib_t, panCoordShortAddr), sizeof(u16), 0, 0}
};


/*
 * MLME-SET.request to set MAC pib attribute
 *
 * @param attribute MAC PIB attribute MAC PIB attribute id(Table 86  MAC PIB attributes)
 *
 * @param value the pointer value of the attribute
 *
 * @param index:if the attribute is related to security,the means of index are entry number
 * 				othsewise the index means len, the length of the attribute value
 *
 * @return MAC_SUCCESS if it's successful, or MAC_INVALID_PARAMETER
 *
 * */
_CODE_MAC_ u8 tl_zbMacAttrSet(u8 attribute, u8 *value, u8 index){
    void *tmp_item = NULL;
    void *tmp_item2 = NULL;


	if(attribute == MAC_TX_POWER){
		if(*value > 11){
			*value = 11;
		}
		ZB_RADIO_TX_POWER_SET(*value);
		return MAC_SUCCESS;
	}else if(attribute == MAC_PHY_ATTR_CURRENT_CHANNEL){
		g_zbMacPib.phyChannelCur = *value;
		return MAC_SUCCESS;
	}

	if(attribute > MAC_PIB_ATTRIBUTE_STOP || attribute < MAC_PIB_ATTRIBUTE_START){
		return MAC_STA_INVALID_PARAMETER;
	}

	 /* security related PIBs*/
	u8 pibIndex = index;
	if (attribute == MAC_KEY_TABLE) {
		if (pibIndex > g_zbMacPib.keyTableEntries) {  //pibIndex must be within current number of KeyDescriptor
			return MAC_STA_INVALID_PARAMETER;
		}
		else if (pibIndex == g_zbMacPib.keyTableEntries) { //pibIndex equals to the TableEntries indicating adding a new entry to the tail of current table
			tn_list_add((tn_list_t)g_zbMacPib.keyTable, value);
			g_zbMacPib.keyTableEntries++;
		}
		else { //set an exist entry to a new value
			tmp_item = tn_list_head((tn_list_t)g_zbMacPib.keyTable);
			while (pibIndex--) {
				tmp_item2 = tmp_item;
				tmp_item = tn_list_item_next(tmp_item);
			}
			tn_list_remove((tn_list_t)g_zbMacPib.keyTable, tmp_item);
			if (tmp_item2) {
				tn_list_insert((tn_list_t)g_zbMacPib.keyTable, tmp_item2, value);
			}
			else {
				tn_list_push((tn_list_t)g_zbMacPib.keyTable, value);
			}
			mac_key_free((mac_keyDesc_t *)tmp_item);
		}
		return MAC_SUCCESS;
	}

	if (attribute == MAC_DEVICE_TABLE) {
		if (pibIndex > g_zbMacPib.devTableEntries) {  //pibIndex must be within current number of DeviceDescriptor
			return MAC_STA_INVALID_PARAMETER;
		}
		else if (pibIndex == g_zbMacPib.devTableEntries) {
			tn_list_add((tn_list_t)g_zbMacPib.devTable, value);
			g_zbMacPib.devTableEntries++;
		}
		else {
			tmp_item = tn_list_head((tn_list_t)g_zbMacPib.devTable);
			while (pibIndex--) {
				tmp_item2 = tmp_item;
				tmp_item = tn_list_item_next(tmp_item);
			}
			tn_list_remove((tn_list_t)g_zbMacPib.devTable, tmp_item);
			if (tmp_item2) {
				tn_list_insert((tn_list_t)g_zbMacPib.devTable, tmp_item2, value);
			}
			else {
				tn_list_push((tn_list_t)g_zbMacPib.devTable, value);
			}
			tn_mem_free(&macDeviceMem, tmp_item);
		}
		return MAC_SUCCESS;
	}

	if (attribute == MAC_SECURITY_LEVEL_TABLE) {
		if (pibIndex > g_zbMacPib.secLvlTableEntries) {  //pibIndex must be within current number of SecurityLevelDescriptor
			return MAC_STA_INVALID_PARAMETER;
		}
		else if (pibIndex == g_zbMacPib.secLvlTableEntries) {
			tn_list_add((tn_list_t)g_zbMacPib.secLvlTable, value);
			g_zbMacPib.secLvlTableEntries++;
		}
		else {
			tmp_item = tn_list_head((tn_list_t)g_zbMacPib.secLvlTable);
			while (pibIndex--) {
				tmp_item2 = tmp_item;
				tmp_item = tn_list_item_next(tmp_item);
			}
			tn_list_remove((tn_list_t)g_zbMacPib.secLvlTable, tmp_item);
			if (tmp_item2) {
				tn_list_insert((tn_list_t)g_zbMacPib.secLvlTable, tmp_item2, value);
			}
			else {
				tn_list_push((tn_list_t)g_zbMacPib.secLvlTable, value);
			}
			tn_mem_free(&macSecLvlMem, tmp_item);
		}
		return MAC_SUCCESS;
	}

	u8 len = index,offset=0;
	if(attribute>=MAC_KEY_TABLE)
	{
		offset = MAC_KEY_TABLE - MAC_TX_POWER;
		offset -= 1;
	}

	if(len > g_zbMacPibTbl[attribute-MAC_PIB_ATTRIBUTE_START-offset].len){
		return MAC_STA_INVALID_PARAMETER;
	}

	memcpy((u8 *)&g_zbMacPib+g_zbMacPibTbl[attribute-MAC_PIB_ATTRIBUTE_START-offset].offset, value, len);

	return MAC_SUCCESS;
}


/*
 * MLME-GET.request to get MAC pib attribute
 *
 * @param attribute MAC PIB attribute MAC PIB attributee id(Table 86  MAC PIB attributes)
 *
 * @param value the pointer to the value of the attribute
 *
 * @param index:if the attribute is related to security,the means of index are entry number
 * 				othsewise the index means len, the length of the attribute value
 *
 * @return MAC_SUCCESS if it's successful, or MAC_INVALID_PARAMETER
 *
 * */
_CODE_MAC_ u8 tl_zbMacAttrGet(u8 attribute, u8* value, u8* index){
    void *tmp_item = NULL;

	if(attribute == MAC_PHY_ATTR_CURRENT_CHANNEL){
		*value = g_zbMacPib.phyChannelCur;
		*index = 1;
		return MAC_SUCCESS;
	}

	if(attribute > MAC_PIB_ATTRIBUTE_STOP || attribute < MAC_PIB_ATTRIBUTE_START){
		return MAC_STA_INVALID_PARAMETER;
	}

	 /* security related PIBs*/
	u8 pibIndex = *(u8 *)index;
	    if (attribute == MAC_KEY_TABLE) {
	        if (pibIndex >= g_zbMacPib.keyTableEntries) {  //pibIndex must be within current number of KeyDescriptor
	            return MAC_STA_INVALID_PARAMETER;
	        }
	        else {
	            tmp_item = tn_list_head((tn_list_t)g_zbMacPib.keyTable);
	            while (pibIndex--) {
	                tmp_item = tn_list_item_next(tmp_item);
	            }
	            *((mac_keyDesc_t **)value) = (mac_keyDesc_t *)tmp_item;
	            return MAC_SUCCESS;
	        }
	    }

	    if (attribute == MAC_DEVICE_TABLE) {
	        if (pibIndex >= g_zbMacPib.devTableEntries) {  //index must be within current number of DeviceDescriptor
	            return MAC_STA_INVALID_PARAMETER;
	        }
	        else {
	            tmp_item = tn_list_head((tn_list_t)g_zbMacPib.devTable);
	            while (pibIndex--) {
	                tmp_item = tn_list_item_next(tmp_item);
	            }
	            *((mac_deviceDesc_t **)value) = (mac_deviceDesc_t *)tmp_item;
	            return MAC_SUCCESS;
	        }
	    }

	    if (attribute == MAC_SECURITY_LEVEL_TABLE) {
	        if (pibIndex >= g_zbMacPib.secLvlTableEntries) {  //index must be within current number of SecurityLevelDescriptor
	            return MAC_STA_INVALID_PARAMETER;
	        }
	        else {
	            tmp_item = tn_list_head((tn_list_t)g_zbMacPib.secLvlTable);
	            while (pibIndex--) {
	                tmp_item = tn_list_item_next(tmp_item);
	            }
	            *((mac_seclevelDesc_t **)value) = (mac_seclevelDesc_t *)tmp_item;
	            return MAC_SUCCESS;
	        }
	    }


	u8 len = g_zbMacPibTbl[attribute-MAC_PIB_ATTRIBUTE_START].len;
	memcpy(value, (u8 *)&g_zbMacPib+g_zbMacPibTbl[attribute-MAC_PIB_ATTRIBUTE_START].offset, len);
	if(attribute == MAC_ATTR_BEACON_PAYLOAD && g_zbMacPib.beaconPayloadLen <= len){
		len = g_zbMacPib.beaconPayloadLen;
	}
	*index = len;
	return MAC_SUCCESS;
}



/*
 * MLME-GET.request to del MAC pib attribute
 *
 * @param pibAttribute MAC PIB security attribute MAC PIB attribute id(Table 86  MAC PIB attributes)
 *
 * @param pibIndex:entry number
 *
 * @return MAC_SUCCESS if it's successful, or MAC_INVALID_PARAMETER
 *
 * */
mac_sts_t tl_zbMacSecAttrDel(u8 pibAttribute, u8 pibIndex)
{
    void *tmp_item = NULL;

    /* security related PIBs*/
    if (pibAttribute == MAC_KEY_TABLE) {
        if (pibIndex >= g_zbMacPib.keyTableEntries) {  //pibIndex must be within current number of KeyDescriptor
            return MAC_STA_INVALID_PARAMETER;
        }
        else { //delete an exist entry from a new value
            tmp_item = tn_list_head((tn_list_t)g_zbMacPib.keyTable);
            while (pibIndex--) {
                tmp_item = tn_list_item_next(tmp_item);
            }
            tn_list_remove((tn_list_t)g_zbMacPib.keyTable, tmp_item);
            g_zbMacPib.keyTableEntries--;
            mac_key_free((mac_keyDesc_t *)tmp_item);
        }
        return MAC_SUCCESS;
    }

    if (pibAttribute == MAC_DEVICE_TABLE) {
        if (pibIndex >= g_zbMacPib.devTableEntries) {  //pibIndex must be within current number of DeviceDescriptor
            return MAC_STA_INVALID_PARAMETER;
        }
        else {
            tmp_item = tn_list_head((tn_list_t)g_zbMacPib.devTable);
            while (pibIndex--) {
                tmp_item = tn_list_item_next(tmp_item);
            }
            tn_list_remove((tn_list_t)g_zbMacPib.devTable, tmp_item);
            g_zbMacPib.devTableEntries--;
            tn_mem_free(&macDeviceMem, tmp_item);
        }
        return MAC_SUCCESS;
    }

    if (pibAttribute == MAC_SECURITY_LEVEL_TABLE) {
        if (pibIndex >= g_zbMacPib.secLvlTableEntries) {  //pibIndex must be within current number of SecurityLevelDescriptor
            return MAC_STA_INVALID_PARAMETER;
        }
        else {
            tmp_item = tn_list_head((tn_list_t)g_zbMacPib.secLvlTable);
            while (pibIndex--) {
                tmp_item = tn_list_item_next(tmp_item);
            }
            tn_list_remove((tn_list_t)g_zbMacPib.secLvlTable, tmp_item);
            g_zbMacPib.secLvlTableEntries--;
            tn_mem_free(&macSecLvlMem, tmp_item);
        }
        return MAC_SUCCESS;
    }

    return MAC_STA_INVALID_PARAMETER;
}




_CODE_MAC_ void generateIEEEAddr(void){
	u8 addr[8];

	flash_read(CFG_MAC_ADDRESS, 8, addr);

	if(ZB_IEEE_ADDR_IS_INVALID(addr)){
		unsigned int t0 = clock_time();
		u32 jitter = 0;
		do{
			jitter = drv_u32Rand();
			jitter &= 0xfff;
		}while(jitter == 0);
		while(!clock_time_exceed(t0, jitter));

		drv_generateRandomData(addr, 5);
		memcpy(addr+5, startIEEEAddr, 3);
		flash_write(CFG_MAC_ADDRESS, 6, addr + 2);
		flash_write(CFG_MAC_ADDRESS + 6, 2, addr);
	}else{
		/* MAC address format in TLSR serial chips:
		 * xx xx xx 38 C1 A4 xx xx
  	  	 * xx xx xx D1 19 C4 xx xx
  	  	 * xx xx xx CB 0B D8 xx xx
		 *
		 * so, it need to do shift
		 * */
		if((addr[3] == 0x38 && addr[4] == 0xC1 && addr[5] == 0xA4) ||
		   (addr[3] == 0xD1 && addr[4] == 0x19 && addr[5] == 0xC4) ||
		   (addr[3] == 0xCB && addr[4] == 0x0B && addr[5] == 0xD8)){
			flash_read(CFG_MAC_ADDRESS, 6, addr + 2);
			flash_read(CFG_MAC_ADDRESS + 6, 2, addr);
		}
	}
	ZB_IEEE_ADDR_COPY(ZB_PIB_EXTENDED_ADDRESS(), addr);
}



void updateKeyDevTable(mac_keydevDesc_t *desc)
{
	tn_list_add(macKeyDevTable, desc);
}

/*! @} */
