#ifndef _UPPER_LAYER_H_
#define _UPPER_LAYER_H_
#include <ieee802154_common.h>
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

typedef void (* UpperLayerCb_Type)(void *pData);

extern void UpperLayerCallbackSet(unsigned char Index, UpperLayerCb_Type Callback);
extern UpperLayerCb_Type UpperLayerCallbackGet(unsigned char Index);
extern void UpperLayerProcess(void);


//the APIs are called in APP layer
//MCPS-data.request
u8 tl_MacMcpsDataRequestSend(zb_mscp_data_req_t req,u8 *payload,u8 pay_len);

//MLME-poll.request
u8  tl_MacMlmePollRequestSend(mac_mlme_poll_req_t req);

//MLME-associate.request
u8  tl_MacMlmeAssociateRequestSend(zb_mlme_associate_req_t req);

//MLME-associate.response
u8  tl_MacMlmeAssociateResponseSend(zb_mlme_associate_resp_t req);

//MLME-orphan.response
u8  tl_MacMlmeOrphanResponseSend(mac_mlme_orphan_resp_t req);

//MLME-scan.request
u8  tl_MacMlmeScanRequest(zb_mac_mlme_scan_req_t req);

//MLME-start.request
u8  tl_MacMlmeStartRequest(zb_mac_mlme_start_req_t req);

//MLME-disassociate.request
u8  tl_MacMlmeDisassociateRequestSend(zb_mlme_disassociate_req_t req);



#endif /* _UPPER_LAYER_H_ */
