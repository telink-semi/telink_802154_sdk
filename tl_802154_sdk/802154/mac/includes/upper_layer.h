#ifndef _UPPER_LAYER_H_
#define _UPPER_LAYER_H_
#include "zb_common.h"
enum {
    CALLBACK_DATA_INDICATION = 0,
    CALLBACK_MCPS_DATA_CONFIRM,
    CALLBACK_POLL_CONFIRM,
    CALLBACK_START_CONFIRM,
    CALLBACK_BEACON_NOTIFY_INDICATION,
    CALLBACK_COMM_STATUS_INDICATION,
    CALLBACK_ASSOCIATE_INDICATION,
    CALLBACK_SCAN_CONFIRM,
    CALLBACK_ASSOCIATE_CONFIRM,
    CALLBACK_MAC_RESET_CONFIRM,
    CALLBACK_MLME_SYNC_LOSS_INDICATION,
    CALLBACK_POLL_INDICATION,
    CALLBACK_MLME_ORPHAN_INDICATION,
    CALLBACK_DISASSOCIATE_CONFIRM,
    CALLBACK_DISASSOCIATE_INDICATION,
    CALLBACK_MAX_NUM,
};

typedef void (* UpperLayerCb_Type)(unsigned char *pData);

extern void UpperLayerCallbackSet(unsigned char Index, UpperLayerCb_Type Callback);
extern UpperLayerCb_Type UpperLayerCallbackGet(unsigned char Index);
extern void UpperLayerProcess(void);
extern u8 tl_MacDataRequestSend(zb_mscp_data_req_t *req,u8 *payload,u8 pay_len);
#endif /* _UPPER_LAYER_H_ */
