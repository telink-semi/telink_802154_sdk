/********************************************************************************************************
 * @file    sampleDevice.h
 *
 * @brief   This is the header file for sampleDevice
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
#ifndef _SAMPLE_DEVICE_H_
#define _SAMPLE_DEVICE_H_

typedef struct {
    ev_timer_event_t *timerLedEvt;
    ev_timer_event_t *timerSteering;
    u32 keyPressedTime;

    u16 ledOnTime;
    u16 ledOffTime;
    u8 oriSta;      //original state before blink
    u8 sta;         //current state in blink
    u8 times;       //blink times
    u8 state;

    u8 keyPressed;

    bool lightAttrsChanged;
    bool newDev;
} app_ctx_t;

extern app_ctx_t gDevCtx;



void led_init(void);
/*********************************************************************
 * @fn      user_init
 *
 * @brief   User level initialization code.
 *
 * @param   isRetention - if it is waking up with ram retention.
 *
 * @return  None
 */
void user_init(bool isRetention);
//deep
extern void app_task(void);


void sampleDeviceSysException(void);
#endif /* _MAC_TEST_H_ */
