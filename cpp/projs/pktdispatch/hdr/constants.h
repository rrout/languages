#include <stdio.h>
#include <iostream>

#ifndef __CONSTANTS_H__
#define __CONSTANTS_H__

#define MSG_TYPE_REQ        0
#define MSG_TYPE_RESP       1
#define MSG_TYPE_TOPIC      2

// [ REQUEST  ]  [ MYPID ] [ MYNAME] [ RESPONCE ] [ NUM RESPONCE SRTING ] [ RESPONCE SRTING ]
#define REQRESP_MSG_FIELD_REQ            0
#define REQRESP_MSG_FIELD_PID            1
#define REQRESP_MSG_FIELD_NAME           2
#define REQRESP_MSG_FIELD_RESP           3
#define REQRESP_MSG_FIELD_PARTCOUNT      4
#define REQRESP_MSG_FIELD_TEXT           5

#define REQ_TYPE_GET_PUB_ENDPOINT		"REQ_TYPE_GET_PUB_ENDPOINT"
#define REQ_TYPE_GET_SUB_ENDPOINT		"REQ_TYPE_GET_SUB_ENDPOINT"
#define REQ_TYPE_GET_ENCODE_AUTH_KEY	"REQ_TYPE_GET_ENCODE_AUTH_KEY"
#define REQ_TYPE_GET_DECODE_AUTH_KEY	"REQ_TYPE_GET_DECODE_AUTH_KEY"
#define REQ_TYPE_GET_HEART_BEAT			"REQ_TYPE_GET_HEART_BEAT"
#define REQ_TYPE_GET_TOPIC_LIST			"REQ_TYPE_GET_TOPIC_LIST"
#define REQ_TYPE_RESPONCE				"REQ_TYPE_RESPONCE"
#define REQ_TYPE_REGISTER_PUBLISHER		"REQ_TYPE_REGISTER_PUBLISHER"
#define REQ_TYPE_REGISTER_SUBSCRIBER	"REQ_TYPE_REGISTER_SUBSCRIBER"
#define REQ_TYPE_GET_MY_REGESTRATION	"REQ_TYPE_GET_MY_REGESTRATION"
#define REQ_TYPE_NONE					"REQ_TYPE_NONE"
inline bool isReqTypeValid(std::string reqType)  {
    return ((reqType == REQ_TYPE_GET_PUB_ENDPOINT) ||
            (reqType == REQ_TYPE_GET_SUB_ENDPOINT) ||
            (reqType == REQ_TYPE_GET_ENCODE_AUTH_KEY) ||
            (reqType == REQ_TYPE_GET_DECODE_AUTH_KEY) ||
            (reqType == REQ_TYPE_GET_HEART_BEAT) ||
            (reqType == REQ_TYPE_GET_TOPIC_LIST) ||
            (reqType == REQ_TYPE_RESPONCE) ||
            (reqType == REQ_TYPE_REGISTER_PUBLISHER) ||
            (reqType == REQ_TYPE_REGISTER_SUBSCRIBER) ||
            (reqType == REQ_TYPE_GET_MY_REGESTRATION) ||
            (reqType == REQ_TYPE_NONE));
}     


#define RESP_TYPE_OK					"RESP_TYPE_OK"
#define RESP_TYPE_BAD					"RESP_TYPE_BAD"
#define RESP_TYPE_UNAUTHERIZED			"RESP_TYPE_UNAUTHERIZED"
#define RESP_TYPE_RETRY					"RESP_TYPE_RETRY"
#define RESP_TYPE_BUSY					"RESP_TYPE_BUSY"
#define RESP_TYPE_NONE					"RESP_TYPE_NONE"
inline bool isRspTypeValid(std::string respType)  {
    return ((respType == RESP_TYPE_OK) ||
            (respType == RESP_TYPE_BAD) ||
            (respType == RESP_TYPE_UNAUTHERIZED) ||
            (respType == RESP_TYPE_RETRY) ||
            (respType == RESP_TYPE_BUSY) ||
            (respType == RESP_TYPE_NONE));
}

// [ TOPIC ]  [ PUBLISHER PID ] [ PUBLISHER NAME] [ ADITIONAL INFO ] [ TOPIC CONTENT ]
#define TOPIC_MSG_FIELD_TOPIC            0
#define TOPIC_MSG_FIELD_PID              1
#define TOPIC_MSG_FIELD_PNAME            2
#define TOPIC_MSG_FIELD_EXTINFO          3
#define TOPIC_MSG_FIELD_CONTENT          4


#define PD_TOPIC_0_TOPIC_A              "TOPIC-A"
#define PD_TOPIC_1_TOPIC_B              "TOPIC-B"
#define PD_TOPIC_2_TOPIC_C              "TOPIC-C"
#define PD_TOPIC_3_TOPIC_D              "TOPIC-D"
#define PD_TOPIC_4_TOPIC_E              "TOPIC-E"
#define PD_TOPIC_5_TOPIC_F              "TOPIC-F"
#define PD_TOPIC_6_TOPIC_G              "TOPIC-G"
#define PD_TOPIC_7_TOPIC_H              "TOPIC-H"
#define PD_TOPIC_8_TOPIC_I              "TOPIC-I"


#define TOPIC_CONTENT_PLANE_TEXT				"TOPIC_CONTENT_PLANE_TEXT"	
#define TOPIC_CONTENT_PLANE_TEXT_ENCRYPTED		"TOPIC_CONTENT_PLANE_TEXT_ENCRYPTED"	
#define TOPIC_CONTENT_BINARY					"TOPIC_CONTENT_BINARY"	
#define TOPIC_CONTENT_BINARY_ENCRYPTED			"TOPIC_CONTENT_BINARY_ENCRYPTED"	
#define TOPIC_CONTENT_JSON						"TOPIC_CONTENT_JSON"	
#define TOPIC_CONTENT_JSON_ENCRYPTED			"TOPIC_CONTENT_JSON_ENCRYPTED"	
#define TOPIC_CONTENT_XML						"TOPIC_CONTENT_XML"	
#define TOPIC_CONTENT_XML_ENCRYPTED				"TOPIC_CONTENT_XML_ENCRYPTED"	
#define TOPIC_CONTENT_PLANE_FILE				"TOPIC_CONTENT_PLANE_FILE"	
#define TOPIC_CONTENT_PLANE_FILE_ENCRYPTED		"TOPIC_CONTENT_PLANE_FILE_ENCRYPTED"	
#define TOPIC_CONTENT_PROTO_BUF					"TOPIC_CONTENT_PROTO_BUF"	
#define TOPIC_CONTENT_PROTO_BUF_ENCRYPTED		"TOPIC_CONTENT_PROTO_BUF_ENCRYPTED"	
#define TOPIC_CONTENT_UNKNOWN					"TOPIC_CONTENT_UNKNOWN"
inline bool isTopicContentValid(std::string contentType)  {
    return ((contentType == TOPIC_CONTENT_PLANE_TEXT) ||
            (contentType == TOPIC_CONTENT_PLANE_TEXT_ENCRYPTED) ||
            (contentType == TOPIC_CONTENT_BINARY) ||
            (contentType == TOPIC_CONTENT_BINARY_ENCRYPTED) ||
            (contentType == TOPIC_CONTENT_JSON) ||
            (contentType == TOPIC_CONTENT_JSON_ENCRYPTED) ||
            (contentType == TOPIC_CONTENT_XML) ||
            (contentType == TOPIC_CONTENT_XML_ENCRYPTED) ||
            (contentType == TOPIC_CONTENT_PLANE_FILE) ||
            (contentType == TOPIC_CONTENT_PLANE_FILE_ENCRYPTED) ||
            (contentType == TOPIC_CONTENT_PROTO_BUF) ||
            (contentType == TOPIC_CONTENT_PROTO_BUF_ENCRYPTED) ||
            (contentType == TOPIC_CONTENT_UNKNOWN));
}

class validation {
	public:
		static bool validateReqRespMsg(std::vector<std::string> &reqRespMsg) {
			if (reqRespMsg.size() == 0) {
				return false;
			}
			if (reqRespMsg.size() < 6) {
				return false;
			}
			if (!isReqTypeValid(reqRespMsg[REQRESP_MSG_FIELD_REQ])) {
				return false;
			}
            if (isRspTypeValid(reqRespMsg[REQRESP_MSG_FIELD_RESP])) {
				return false;
			}
            return true;
		}
    
        static bool validateTopicMsg(std::vector<std::string> &topicMsg) {
			if (topicMsg.size() == 0) {
				return false;
			}
			if (topicMsg.size() < 5) {
				return false;
			}
            return true;
		}
};
#endif //__CONSTANTS_H__
