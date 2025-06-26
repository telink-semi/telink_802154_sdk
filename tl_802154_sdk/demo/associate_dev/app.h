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
#ifndef _app_H_
#define _app_H_

extern void dev_config(void);
extern void dev_active_scan(void);
extern void dev_start_associate(void);

extern int dev_activescan_callback(void *arg);
extern void dev_start_scan(void);
//callback
extern void MyScanCnfCb(void *pData);
extern void MyAssocCnfCb(void *pData);
extern void MyDataIndCb(void *pData);
extern void MyPollCnfCb(void *pData);
extern void MyDisassociateCnfCb(void *pData);
extern void MyDataCnfCb(void *pData);
extern void MyBeaconIndCb(void *pData);
extern u8 add_key_material(void);
extern void poll_data_req_send(void *arg);
extern int PollDataTimerCb(void *arg);
void mac_set_pollRate(u32 newRate);
#endif /* _MAC_TEST_H_ */
