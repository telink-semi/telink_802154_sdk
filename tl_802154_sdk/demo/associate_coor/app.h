/********************************************************************************************************
 * @file    app.h
 *
 * @brief   This is the header file for app
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
#ifndef _APP_H_
#define _APP_H_

//associate device
typedef struct {
    addrExt_t   extAddr;           //!< Extended address
    u16         shortAddr;         //!< Short address
    u16 		pan_id;
}my_device_t;

typedef struct {
    ev_timer_event_t *timerLedEvt;
    ev_timer_event_t  *timerSteering;
    u32 keyPressedTime;

    u16 ledOnTime;
    u16 ledOffTime;
    u8 oriSta;          //original state before blink
    u8 sta;             //current state in blink
    u8 times;           //blink times
    u8 state;

    bool lightAttrsChanged;
} app_ctx_t;

extern app_ctx_t gCoorCtx;

extern my_device_t end_device[];
extern addr_t	dev_src_addr;


extern void coor_config(void);
extern void pan_start(void);
extern u8 add_key_material(void);
extern void mac_send_data_indirect(void *arg);
extern void data_send(void);


extern void MyAssociateIndCb(void *pData);
extern void MyDataCnfCb(void *pData);
extern void MyDisassociateIndCb(void *pData);
extern void MyStartCnfCb(void *pData);
extern void MyPullIndCb(void *pData);
extern void MyDataIndCb(void *pData);
extern void MyStateIndCb(void *pData);
extern void app_data_transmit_toggle(void);
void sampleCoorSysException(void);
void app_task(void);
#endif /* _MAC_TEST_H_ */
