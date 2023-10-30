#include "zb_common.h"
#include "sampledevice.h"
#include "app.h"
#include "../app_common/ota/tl_specific_data_ota.h"

void led_init(void)
{
	drv_gpio_write(LED_1, 0);
	drv_gpio_write(LED_2, 0);
}

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
		zb_init();

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

		dev_config();

	    UpperLayerCallbackSet(CALLBACK_SCAN_CONFIRM, MyScanCnfCb);
	    UpperLayerCallbackSet(CALLBACK_ASSOCIATE_CONFIRM, MyAssocCnfCb);
	    UpperLayerCallbackSet(CALLBACK_DATA_INDICATION, MyDataIndCb);
	    UpperLayerCallbackSet(CALLBACK_POLL_CONFIRM, MyPollCnfCb);
	    UpperLayerCallbackSet(CALLBACK_MCPS_DATA_CONFIRM, MyDataCnfCb);
	    UpperLayerCallbackSet(CALLBACK_BEACON_NOTIFY_INDICATION, MyBeaconIndCb);

	    dev_start_scan();
	}
	else
	{
		/* Re-config phy when system recovery from deep sleep with retention */
		mac_phyReconfig();
	}
}






void app_task(void)
{
#if PM_ENABLE
	drv_pm_lowPowerEnter();
#endif
}



void sampleDeviceSysException(void)
{
	while(1){
		gpio_toggle(LED_1);
		WaitMs(100);
	}
}
