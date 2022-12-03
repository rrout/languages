#include "hdr.h"






#ifndef __CONSTANTS_H__
#define __CONSTANTS_H__
#define MGMT_ENDPOINT		"tcp://*:4243"
#define ADV_ENDPOINT		"tcp://*:4244"
#define PUB_ENDPOINT		"tcp://*:4242"
#define SUB_ENDPOINT		"tcp://*:4245"

#define PD_MAX_BUFFER_COUNT             10

typedef enum {
    PUSH,
    SERVER_PUSH,
    PULL,
    SERVER_PULL,
    REQ,
    SERVER_REQ,
    RESP,
    SERVER_RESP,
    PUBLISH,
    SERVER_PUBLISH,
    SUBSCRIBE,
    SERVER_SUBSCRIBE
} endPointType_t;

typedef enum {
    STATUS_OK,
    STATUS_FAIL,
    STATUS_EEXIST,
    STATUS_INTERNAL,
    STATUS_MAX
}pktConfigStatus_t;



#endif //__CONSTANTS_H__
