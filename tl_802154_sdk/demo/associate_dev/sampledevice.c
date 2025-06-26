/********************************************************************************************************
 * @file    sampleDevice.c
 *
 * @brief   This is the source file for sampleDevice
 *
 * @author  Zigbee Group
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
 *******************************************************************************************************/
#include <ieee802154_common.h>
#include "sampledevice.h"
#include "app.h"
#include "app_ui.h"
#include "../app_common/ota/tl_specific_data_ota.h"

/*********************************************************************
 * @fn      user_init
 *
 * @brief   User level initialization code.
 *
 * @param   isRetention - if it is waking up with ram retention.
 *
 * @return  None
 */
void user_init(bool isRetention)
{
	led_init();

	if(!isRetention){
	    //802.15.4 stack Init
		gDevCtx.newDev = ieee802154_init();

		ota_init();
#if ZBHCI_EN
		/*
		 * define ZBHCI_USB_PRINT, ZBHCI_USB_CDC or ZBHCI_UART as 1 in app_cfg.h
		 * if needing to enable ZBHCI_EN
		 *
		 * */
		zbhciInit();
		ev_on_poll(EV_POLL_HCI, zbhciTask);
#endif

		ev_on_poll(EV_POLL_IDLE, app_task);

		sys_exceptHandlerRegister(sampleDeviceSysException);

	    UpperLayerCallbackSet(CALLBACK_SCAN_CONFIRM, MyScanCnfCb);
	    UpperLayerCallbackSet(CALLBACK_ASSOCIATE_CONFIRM, MyAssocCnfCb);
	    UpperLayerCallbackSet(CALLBACK_DATA_INDICATION, MyDataIndCb);
	    UpperLayerCallbackSet(CALLBACK_POLL_CONFIRM, MyPollCnfCb);
	    UpperLayerCallbackSet(CALLBACK_MCPS_DATA_CONFIRM, MyDataCnfCb);
	    UpperLayerCallbackSet(CALLBACK_BEACON_NOTIFY_INDICATION, MyBeaconIndCb);

        if (gDevCtx.newDev) {
    		dev_config();
    	    dev_start_scan();
        } else {
            ota_queryStart(QUERY_RATE);
            if (!ZB_PIB_RX_ON_WHEN_IDLE()) {
            	mac_set_pollRate(NORMAL_POLL_RATE);//data request
            }
        }



	}
	else
	{
		/* Re-config phy when system recovery from deep sleep with retention */
		mac_phyReconfig();
	}
}






void app_task(void)
{
    app_key_handler();

#if PM_ENABLE
    if (!gDevCtx.keyPressed) {
        if (!tl_stackBusy() && zb_isTaskDone()) {
            drv_pm_lowPowerEnter();
        }
    }
#endif
}



void sampleDeviceSysException(void)
{
	while(1){
		gpio_toggle(LED_1);
		WaitMs(100);
	}
}
