#ifndef _APP_H_
#define _APP_H_

//associate device
typedef struct {
    addrExt_t   extAddr;           //!< Extended address
    u16         shortAddr;         //!< Short address
    u16 		pan_id;
}my_device_t;
extern my_device_t end_device;



extern void coor_config(void);
extern void pan_start(void);
extern void add_key_material(void);
extern void mac_send_data_indirect(void *arg);
extern void data_send(void);


extern void MyAssociateIndCb(unsigned char *pData);
extern void MyDataCnfCb(unsigned char *pData);
extern void MyDisassociateIndCb(unsigned char *pData);
extern void MyStartCnfCb(unsigned char *pData);
extern void MyPullIndCb(unsigned char *pData);
extern void MyDataIndCb(unsigned char *pData);
extern void MyStateIndCb(unsigned char *pData);

void sampleCoorSysException(void);
void app_task(void);
#endif /* _MAC_TEST_H_ */
