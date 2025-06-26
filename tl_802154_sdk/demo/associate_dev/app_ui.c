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
#include "sampledevice.h"
#include "app_ui.h"

/**********************************************************************
 * LOCAL CONSTANTS
 */
app_ctx_t gDevCtx;

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

void app_dataTxTest(void)
{
    zb_mscp_data_req_t req;
    memset(&req, 0, sizeof(req));
    u8 len;
    tl_zbMacAttrGet(MAC_ATTR_PAN_ID, (u8 *)&req.dstPanId, &len);
    req.srcAddr.addrMode = ZB_ADDR_16BIT_DEV_OR_BROADCAST;    // short addr mode
    req.dstAddr.addrMode = ZB_ADDR_16BIT_DEV_OR_BROADCAST;    // short addr mode
    req.dstAddr.addr.shortAddr = 0x0000;  //send to coordinator

    req.msduHandle = 0;
    /* tx options:
     * MAC_TX_OPTION_ACKNOWLEDGED_BIT: mac ack needed(normally used for unicasting)
     * MAC_TX_OPTION_INDIRECT_TRANSMISSION_BIT: indirect transmit if the destination device is sleepy-device
     * 0:   for broadcasting
     * */
    req.txOptions = MAC_TX_OPTION_ACKNOWLEDGED_BIT;  // MAC_TX_OPTION_INDIRECT_TRANSMISSION_BIT

#if APP_MAC_SEC_ENABLE
    // don't use security
    req.sec.securityLevel = 0;
#endif

    u8 msduLen = 50;
    u8 *msdu = ev_buf_allocate(msduLen);
    if (msdu) {
        for (int i = 0; i < 50; i++) {
            msdu[i] = i;
        }
        tl_MacMcpsDataRequestSend(req, msdu, msduLen);
        ev_buf_free(msdu);
    }
}

void buttonKeepPressed(u8 btNum)
{
    if (btNum == VK_SW1) {
        gDevCtx.state = APP_FACTORY_NEW_DOING;
        nv_resetToFactoryNew();   //reset to factory new device: clean all the info in NV
        SYSTEM_RESET();
    } else if (btNum == VK_SW2) {

    }
}

void buttonShortPressed(u8 btNum)
{
    if (btNum == VK_SW1) {
        if (!gDevCtx.newDev) {
            app_dataTxTest();
        }
    } else if (btNum == VK_SW2) {

    }
}

void keyScan_keyPressedCB(kb_data_t *kbEvt)
{
    //u8 toNormal = 0;
    u8 keyCode = kbEvt->keycode[0];
    //static u8 lastKeyCode = 0xff;

    buttonShortPressed(keyCode);

    if (keyCode == VK_SW1) {
        gDevCtx.keyPressedTime = clock_time();
        gDevCtx.state = APP_FACTORY_NEW_SET_CHECK;
    }
}

void keyScan_keyReleasedCB(u8 keyCode)
{
    gDevCtx.state = APP_STATE_NORMAL;
}

void app_key_handler(void)
{
    static u8 valid_keyCode = 0xff;

    if (gDevCtx.state == APP_FACTORY_NEW_SET_CHECK) {
        if (clock_time_exceed(gDevCtx.keyPressedTime, 5 * 1000 * 1000)) {
            buttonKeepPressed(valid_keyCode);
        }
    }

    if (kb_scan_key(0 , 1)) {
        if (kb_event.cnt) {
            gDevCtx.keyPressed = 1;
            keyScan_keyPressedCB(&kb_event);
            if (kb_event.cnt == 1) {
                valid_keyCode = kb_event.keycode[0];
            }
        } else {
            keyScan_keyReleasedCB(valid_keyCode);
            valid_keyCode = 0xff;
            gDevCtx.keyPressed = 0;
        }
    }
}
