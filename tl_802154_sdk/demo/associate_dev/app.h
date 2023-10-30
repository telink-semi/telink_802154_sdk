#ifndef _app_H_
#define _app_H_

extern void dev_config(void);
extern void dev_active_scan(void);
extern void dev_start_associate(void);

extern int dev_activescan_callback(void *arg);
extern void dev_start_scan(void);
//callback
extern void MyScanCnfCb(unsigned char *pData);
extern void MyAssocCnfCb(unsigned char *pData);
extern void MyDataIndCb(unsigned char *pData);
extern void MyPollCnfCb(unsigned char *pData);
extern void MyDisassociateCnfCb(unsigned char *pData);
extern void MyDataCnfCb(unsigned char *pData);
extern void MyBeaconIndCb(unsigned char *pData);
extern u8 add_key_material(void);
extern void poll_data_req_send(void *arg);
extern int PollDataTimerCb(void *arg);
void mac_set_pollRate(u32 newRate);
#endif /* _MAC_TEST_H_ */
