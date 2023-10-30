#include "zb_common.h"
#include "samplecoor.h"
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
	(void)isRetention;

	led_init();

	//802.15.4 stack Init
	zb_init();

	ota_init();

#if ZBHCI_EN
	/*
	 * define ZBHCI_USB_PRINT, ZBHCI_USB_CDC or ZBHCI_UART as 1 in app_cfg.h
	 * if needing to enable ZBHCI_EN
	 * */
	zbhciInit();
	ev_on_poll(EV_POLL_HCI, zbhciTask);
#endif
	ev_on_poll(EV_POLL_IDLE, app_task);

	sys_exceptHandlerRegister(sampleCoorSysException);

	coor_config();

	//802.15.4 stack Init
	UpperLayerCallbackSet(CALLBACK_START_CONFIRM, MyStartCnfCb);
	UpperLayerCallbackSet(CALLBACK_ASSOCIATE_INDICATION, MyAssociateIndCb);
	UpperLayerCallbackSet(CALLBACK_MCPS_DATA_CONFIRM, MyDataCnfCb);
	UpperLayerCallbackSet(CALLBACK_POLL_INDICATION, MyPullIndCb);
	UpperLayerCallbackSet(CALLBACK_DATA_INDICATION, MyDataIndCb);
	UpperLayerCallbackSet(CALLBACK_COMM_STATUS_INDICATION, MyStateIndCb);
	pan_start();
}

//AODV:
//1.NO LABEL
//2.LABEL OUTGOING >4

//coor:  MTO router



void app_task(void)
{
//user can config ui and so on
}


void sampleCoorSysException(void)
{
	while(1){
		gpio_toggle(LED_1);
		WaitMs(100);
	}
}
