#include <iostream>
#ifndef __VALIDATION_H__
#define __VALIDATION_H__
class validation {
	public:
		static bool validateReqRespMsg(std::vector<std::string> &reqRespMsg) {
			if (reqRespMsg.size() == 0) {
				return false;
			}
			if (reqRespMsg.size() < 6) {
				return false;
			}
            try {
                req = std::stoi(reqRespMsg[REQRESP_MSG_FIELD_REQ]);
                resp = std::stoi(reqRespMsg[REQRESP_MSG_FIELD_RESP])
            } catch (...) {
                return false;
            }
			if (!isReqTypeValid(reqRespMsg[REQRESP_MSG_FIELD_REQ])) {
				return false;
			}
            if (isRspTypeValid) {
				return false;
			}
		}
    
        static bool validateTopicMsg(std::vector<std::string> &topicMsg) {
            int req, pid, resp;
			if (topicMsg.size() == 0) {
				return false;
			}
			if (topicMsg.size() < 5) {
				return false;
			}
		}
    return true;
}


#endif //__VALIDATION_H__
