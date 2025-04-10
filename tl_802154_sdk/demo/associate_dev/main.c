#include "zb_common.h"
#include "app.h"
#include "sampledevice.h"

volatile u32 tick = 0xaa55;
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
	drv_wd_setInterval(600);
    drv_wd_start();
#endif

#if VOLTAGE_DETECT_ENABLE
    tick = clock_time();
    tick /= 25;
#endif

    while(1) {
#if (MODULE_WATCHDOG_ENABLE)
		drv_wd_clear();
#endif

#if VOLTAGE_DETECT_ENABLE
		if(clock_time_exceed(tick, 200 * 1000)){
			voltage_detect();
			tick = clock_time();
		}
#endif
    	tl_zbTaskProcedure();
        UpperLayerProcess();
        ev_main();
#if (MODULE_WATCHDOG_ENABLE)
		drv_wd_clear();
#endif
    }
}


