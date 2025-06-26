/********************************************************************************************************
 * @file    app_ui.c
 *
 * @brief   This is the source file for app_ui
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

/**********************************************************************
 * INCLUDES
 */
#include "ieee802154_common.h"
#include "app.h"
#include "app_ui.h"

/**********************************************************************
 * LOCAL CONSTANTS
 */


/**********************************************************************
 * TYPEDEFS
 */


/**********************************************************************
 * GLOBAL VARIABLES
 */

/**********************************************************************
 * LOCAL FUNCTIONS
 */
void led_on(u32 pin)
{
    drv_gpio_write(pin, LED_ON);
}

void led_off(u32 pin)
{
    drv_gpio_write(pin, LED_OFF);
}

void led_init(void)
{
    led_off(LED_POWER);
    led_off(LED_PERMIT);
}

void localPermitJoinState(void)
{
    u8 permit_en = 0;
    u8 len;

    tl_zbMacAttrGet(MAC_ATTR_ASSOCIATION_PERMIT, &permit_en, &len);
    permit_en ^= 1;
    tl_zbMacAttrSet(MAC_ATTR_ASSOCIATION_PERMIT, &permit_en, 1);

    if (permit_en == 1) {
        led_on(LED_PERMIT);
    } else {
        led_off(LED_PERMIT);
    }
}

void buttonKeepPressed(u8 btNum)
{
    if (btNum == VK_SW1) {
        gCoorCtx.state = APP_FACTORY_NEW_DOING;
        nv_resetToFactoryNew();   //reset to factory new device: clean all the info in NV
        SYSTEM_RESET();
    } else if (btNum == VK_SW2) {
        app_data_transmit_toggle();
    }
}

void buttonShortPressed(u8 btNum)
{
    if (btNum == VK_SW1) {
        localPermitJoinState();
    } else if (btNum == VK_SW2) {
        app_data_transmit_toggle();
    }
}

void keyScan_keyPressedCB(kb_data_t *kbEvt)
{
    //u8 toNormal = 0;
    u8 keyCode = kbEvt->keycode[0];
    //static u8 lastKeyCode = 0xff;

    buttonShortPressed(keyCode);

    if (keyCode == VK_SW1) {
        gCoorCtx.keyPressedTime = clock_time();
        gCoorCtx.state = APP_FACTORY_NEW_SET_CHECK;
    }
}

void keyScan_keyReleasedCB(u8 keyCode)
{
    gCoorCtx.state = APP_STATE_NORMAL;
}

void app_key_handler(void)
{
    static u8 valid_keyCode = 0xff;

    if (gCoorCtx.state == APP_FACTORY_NEW_SET_CHECK) {
        if (clock_time_exceed(gCoorCtx.keyPressedTime, 5 * 1000 * 1000)) {
            buttonKeepPressed(valid_keyCode);
        }
    }

    if (kb_scan_key(0 , 1)) {
        if (kb_event.cnt) {
            keyScan_keyPressedCB(&kb_event);
            if (kb_event.cnt == 1) {
                valid_keyCode = kb_event.keycode[0];
            }
        } else {
            keyScan_keyReleasedCB(valid_keyCode);
            valid_keyCode = 0xff;
        }
    }
}
