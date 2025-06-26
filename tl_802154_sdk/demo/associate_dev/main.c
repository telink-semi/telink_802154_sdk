#include <ieee802154_common.h>
#include "app.h"
#include "sampledevice.h"
#include "app_ui.h"
volatile u32 tick = 0,tick_loop=0;
int main(void)
{
	u8 isRetention = drv_platform_init();

#if VOLTAGE_DETECT_ENABLE
	if(!isRetention){
		voltage_detect();
	}
#endif

	os_init(isRetention);

	user_init(isRetention);

    drv_enable_irq();
#if (MODULE_WATCHDOG_ENABLE)
	drv_wd_setInterval(1000);
    drv_wd_start();
#endif

#if VOLTAGE_DETECT_ENABLE
    tick = clock_time();
#endif

    while(1) {
#if (MODULE_WATCHDOG_ENABLE)
		drv_wd_clear();
#endif
		tick_loop++;
#if VOLTAGE_DETECT_ENABLE
		if(clock_time_exceed(tick, 200 * 1000)){
			voltage_detect();
			tick = clock_time();
		}
#endif
		ieee802154_procedure();
    	ieee802154_app_task_process();

        ev_main();
#if (MODULE_WATCHDOG_ENABLE)
		drv_wd_clear();
#endif
    }
}


