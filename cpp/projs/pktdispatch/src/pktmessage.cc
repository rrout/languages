#include "hdr.h"

pktmessage::pktmessage() { 
	
}
pktmessage::pktmessage(std::string myname) {

}
pktmessage::pktmessage(std::vector<std::string> &msg) {
	for (auto &a : msg) {
        _msgBody.push_back(a);
    }
    _constructed = true;
}
pktmessage::pktmessage(zmqpp::message_t &msg) {

}
bool pktmessage::valid() {

}
int pktmessage::type() { 
    return _msgType;
}
void pktmessage::type(int type) {
    _msgType = type;
}
std::string pktmessage::name() {
    return _myname;
}
void pktmessage::name(std::string name) {
    _myname = name;
}
void pktmessage::clear() { 
    _msgType = INVALID;
    _myname.clear();
    _msgBody.clear();
    _filled = false;
    _constructed = false;
}
bool pktmessage::isclean() {

}
void pktmessage::print() {

}
void pktmessage::construct(std::vector<std::string> &msg) {
    
    _constructed = true;
}
void pktmessage::construct(zmqpp::message_t &msg) {

    _constructed = true;
}
void pktmessage::copy(std::vector<std::string> &msg) {

    _constructed = true;
}
void pktmessage::copy(zmqpp::message_t &msg) {

    _constructed = true;
}
bool pktmessage::fillReq(std::string reqType) {
    if (!isclean()) {
        return false;
    } else {
        _msgType = MSG_TYPE_REQ;
        _msgBody[REQRESP_MSG_FIELD_REQ] = reqType;
        _msgBody[REQRESP_MSG_FIELD_PID] = _myPid;
        _msgBody[REQRESP_MSG_FIELD_NAME] = _myname;
        _msgBody[REQRESP_MSG_FIELD_RESP] = RESP_TYPE_NONE;
        _msgBody[REQRESP_MSG_FIELD_PARTCOUNT] = "0";
        _msgBody[REQRESP_MSG_FIELD_TEXT] = "Generic";
    }
    return true;
}
bool pktmessage::fillReq(std::string reqType, std::string &request) {
    if (!isclean()) {
        return false;
    } else {
        _msgType = MSG_TYPE_REQ;
        _msgBody[REQRESP_MSG_FIELD_REQ] = reqType;
        _msgBody[REQRESP_MSG_FIELD_PID] = _myPid;
		_msgBody[REQRESP_MSG_FIELD_NAME] = _myname;
        _msgBody[REQRESP_MSG_FIELD_RESP] = RESP_TYPE_NONE;
        _msgBody[REQRESP_MSG_FIELD_PARTCOUNT] = "0";
        _msgBody[REQRESP_MSG_FIELD_TEXT] = request;
	}
	return true;
}
bool pktmessage::fillReq(std::string reqType, std::vector<std::string> &requests) {
	if (!isclean()) {
        return false;
    } else {
        _msgType = MSG_TYPE_REQ;
        _msgBody[REQRESP_MSG_FIELD_REQ] = reqType;
        _msgBody[REQRESP_MSG_FIELD_PID] = _myPid;
        _msgBody[REQRESP_MSG_FIELD_NAME] = _myname;
        _msgBody[REQRESP_MSG_FIELD_RESP] = RESP_TYPE_NONE;
        _msgBody[REQRESP_MSG_FIELD_PARTCOUNT] = std::stoi(requests.size());;
        for (auto &a : requests) {
            _msgBody.push_back(a);
        }
    }
	return true;
}

bool pktmessage::fillResp(std::string respType) {
    if (!isclean()) {
        return false;
    } else {
        _msgType = MSG_TYPE_RESP;
        _msgBody[REQRESP_MSG_FIELD_REQ] = REQ_TYPE_NONE;
        _msgBody[REQRESP_MSG_FIELD_PID] = _myPid;
        _msgBody[REQRESP_MSG_FIELD_NAME] = _myname;
        _msgBody[REQRESP_MSG_FIELD_RESP] = respType;
        _msgBody[REQRESP_MSG_FIELD_PARTCOUNT] = "0";
        _msgBody[REQRESP_MSG_FIELD_TEXT] = "Generic";
    }
    return true;
}
bool pktmessage::fillResp(std::string respType, std::string &response) {
    if (!isclean()) {
        return false;
    } else {
        _msgType = MSG_TYPE_RESP;
        _msgBody[REQRESP_MSG_FIELD_REQ] = REQ_TYPE_NONE;
        _msgBody[REQRESP_MSG_FIELD_PID] = _myPid;
        _msgBody[REQRESP_MSG_FIELD_NAME] = _myname;
        _msgBody[REQRESP_MSG_FIELD_RESP] = respType;
        _msgBody[REQRESP_MSG_FIELD_PARTCOUNT] = "1";
        _msgBody[REQRESP_MSG_FIELD_TEXT] = response;
    }
    return true;
}
bool pktmessage::fillResp(std::string respType, std::vector<std::string> &responses) {
	if (!isclean()) {
        return false;
    } else {
        _msgType = MSG_TYPE_REQ;
        _msgBody[REQRESP_MSG_FIELD_REQ] = reqType;
        _msgBody[REQRESP_MSG_FIELD_PID] = _myPid;
        _msgBody[REQRESP_MSG_FIELD_NAME] = _myname;
        _msgBody[REQRESP_MSG_FIELD_RESP] = RESP_TYPE_NONE;
        _msgBody[REQRESP_MSG_FIELD_PARTCOUNT] = std::stoi(responses.size());;
        for (auto &a : responses) {
            _msgBody.push_back(a);
        }
    }
    return true;
}

bool pktmessage::fillTopic(std::string topic, std::string adon, std::string &containt) {
	if (!isclean()) {
        return false;
    } else {
		_msgType = MSG_TYPE_TOPIC;
        _msgBody[TOPIC_MSG_FIELD_TOPIC] = topic;
        _msgBody[TOPIC_MSG_FIELD_PID] = _myPid;
        _msgBody[TOPIC_MSG_FIELD_PNAME] = _myname;
        _msgBody[TOPIC_MSG_FIELD_EXTINFO] = adon;
        _msgBody[TOPIC_MSG_FIELD_CONTENT] = containt;
    }
    return true;
}

bool pktmessage::fillTopic(std::string topic, std::string adon, std::vector<std::string> &containts) {
    if (!isclean()) {
		if (!isclean()) {
        return false;
    } else {
        _msgType = MSG_TYPE_TOPIC;
        _msgBody[TOPIC_MSG_FIELD_TOPIC] = topic;
        _msgBody[TOPIC_MSG_FIELD_PID] = _myPid;
        _msgBody[TOPIC_MSG_FIELD_PNAME] = _myname;
        _msgBody[TOPIC_MSG_FIELD_EXTINFO] = adon;
		for (auto &a : requests) {
            _msgBody.push_back(a);
        }
    }
    return true;		
}

void pktmessage::compose() { }
