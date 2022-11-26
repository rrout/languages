#include "hdr.h"
#include "pktmessage.h"

pktmessage::pktmessage() {
	std::cout << __PRETTY_FUNCTION__ << ":" << "Const Def Object" << std::endl;
	_msgType = MSG_TYPE_INVALID;
}
pktmessage::pktmessage(std::string myname) {
	std::cout << __PRETTY_FUNCTION__ << ":" << "Const Object" << std::endl;
	_msgType = MSG_TYPE_INVALID;
	_myname = myname;
}
pktmessage::pktmessage(std::vector<std::string> &msg) {
	for (auto &a : msg) {
        _msgBody.push_back(a);
    }
	setMsgType();
    _constructed = true;
}
pktmessage::pktmessage(zmqpp::message &msg) {
	for (unsigned long i = 0; i < msg.parts(); i++) {
		_msgBody.push_back(msg.get(i));
	}
	setMsgType();
}
pktmessage::~pktmessage() {
	std::cout << __PRETTY_FUNCTION__ << ":" << "Destructing Object" << std::endl;
}

void pktmessage::setMsgType() {
	if (_msgBody.size() == 0 || _msgBody.size() < 4) {
		_msgType =  MSG_TYPE_INVALID;
	} else if (isReqTypeValid(_msgBody[REQRESP_MSG_FIELD_REQ]) &&
			(_msgBody[REQRESP_MSG_FIELD_REQ] != REQ_TYPE_RESPONCE)) {
		_msgType = MSG_TYPE_REQ;
	} else if (isRespTypeValid(_msgBody[REQRESP_MSG_FIELD_RESP])) {
		_msgType = MSG_TYPE_RESP;
	//} else if (isTopicTypeValid(_msgBody[TOPIC_MSG_FIELD_TOPIC])) {
	//	_msgType = MSG_TYPE_TOPIC;
	} else {
		_msgType = MSG_TYPE_INVALID;
	}
}
bool pktmessage::valid() {
	if (_msgBody.size() == 0 || _msgBody.size() < 4) {
		return false;
	}
	setMsgType();
	if (_msgType == MSG_TYPE_INVALID) {
		return false;
	} else if (_msgType == MSG_TYPE_REQ || _msgType == MSG_TYPE_RESP) {
		if (!validReqRespMsg(_msgBody)) {
			return false;
		}
	} else if (_msgType == MSG_TYPE_TOPIC) {
		if (!validReqRespMsg(_msgBody)) {
			return false;
		}
	} else {
		return false;
	}
	return true;
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
std::string pktmessage::getmsgfield(int field) {
	if ((_msgBody.size() == 0) ||
			(static_cast<unsigned long>(field) > _msgBody.size()) ||
			(_msgType == MSG_TYPE_INVALID)) {
		return "NONE";
	}
	return _msgBody[field];
}
void pktmessage::clear() {
    _msgType = MSG_TYPE_INVALID;
    _myname.clear();
    _msgBody.clear();
    _filled = false;
    _constructed = false;
}
bool pktmessage::isclean() {
	if (_msgType == MSG_TYPE_INVALID && _msgBody.size() == 0) {
		return true;
	}
	return false;
}
void pktmessage::print() {
	int count = 0;
	std::cout << "-------------------" << std::endl;
	std::cout << "Msg type     : " << _msgType << std::endl;
	std::cout << "Msg body Size: " << _msgBody.size() << std::endl;
	std::cout << "Msg Prop     : " << std::endl;
	std::cout << "Valid        : " << (valid() ? "Yes" : "No") << std::endl;
	std::cout << "-------------------" << std::endl;
	for (auto &a : _msgBody) {
		std::cout << count << " | " << a << std::endl;
		count++;
	}
	std::cout << "-------------------" << std::endl;
}
void pktmessage::construct(std::vector<std::string> &msg) {
	for (auto &a : msg) {
        _msgBody.push_back(a);
    }
    setMsgType();
    _constructed = true;
}
void pktmessage::construct(zmqpp::message &msg) {
	for (unsigned long i = 0; i < msg.parts(); i++) {
        _msgBody.push_back(msg.get(i));
    }
    setMsgType();
    _constructed = true;
}
void pktmessage::copy(std::vector<std::string> &msg) {
	for (auto &a : msg) {
        _msgBody.push_back(a);
    }
    setMsgType();
    _constructed = true;
}
void pktmessage::copy(zmqpp::message &msg) {
	for (unsigned long i = 0; i < msg.parts(); i++) {
        _msgBody.push_back(msg.get(i));
    }
    _constructed = true;
}
bool pktmessage::validReqRespMsg(std::vector<std::string> &reqRespMsg) {
	if ((reqRespMsg.size() == 0)) {
		return false;
	} else if (reqRespMsg.size() < 6) {
		return false;
	} else if (!isReqTypeValid(reqRespMsg[REQRESP_MSG_FIELD_REQ])) {
		return false;
	} else if (!isRespTypeValid(reqRespMsg[REQRESP_MSG_FIELD_RESP])) {
		return false;
	} else {
		return true;
	}
	return true;
}
bool pktmessage::validTopicMsg(std::vector<std::string> &topicMsg) {
	if (topicMsg.size() == 0) {
		return false;
	} else if (topicMsg.size() < 5) {
		return false;
	} else {
		return true;
	}
	return true;
}
bool pktmessage::fillReq(std::string reqType) {
    if (!isclean()) {
        return false;
    } else {
        _msgType = MSG_TYPE_REQ;
		_msgBody.push_back(reqType);                    //REQRESP_MSG_FIELD_REQ
		_msgBody.push_back(std::to_string(_myPid));     //REQRESP_MSG_FIELD_PID
		_msgBody.push_back(_myname);                    //REQRESP_MSG_FIELD_NAME
		_msgBody.push_back(RESP_TYPE_NONE);             //REQRESP_MSG_FIELD_RESP
		_msgBody.push_back(std::to_string(0));          //REQRESP_MSG_FIELD_PARTCOUNT
        _msgBody.push_back("Generic");					//REQRESP_MSG_FIELD_TEXT
    }
    return true;
}
bool pktmessage::fillReq(std::string reqType, std::string &request) {
    if (!isclean()) {
        return false;
    } else {
        _msgType = MSG_TYPE_REQ;
		_msgBody.push_back(reqType);								//REQRESP_MSG_FIELD_REQ
        _msgBody.push_back(std::to_string(_myPid));					//REQRESP_MSG_FIELD_PID
		_msgBody.push_back(_myname);								//REQRESP_MSG_FIELD_NAME
        _msgBody.push_back(RESP_TYPE_NONE);							//REQRESP_MSG_FIELD_RESP
        _msgBody.push_back(std::to_string(request.size()));			//REQRESP_MSG_FIELD_PARTCOUNT
        _msgBody.push_back(request);								//REQRESP_MSG_FIELD_TEXT
	}
	return true;
}
bool pktmessage::fillReq(std::string reqType, std::vector<std::string> &requests) {
	if (!isclean()) {
        return false;
    } else {
        _msgType = MSG_TYPE_REQ;
		_msgBody.push_back(reqType);
		_msgBody.push_back(std::to_string(_myPid));
		_msgBody.push_back(_myname);
		_msgBody.push_back(RESP_TYPE_NONE);
        _msgBody.push_back(std::to_string(requests.size()));;
        for (auto &a : requests) {
            _msgBody.push_back(a);
        }
    }
	return true;
}

bool pktmessage::fillResp(std::string respType) {
    if (!isclean()) {
		std::cout << __PRETTY_FUNCTION__ << ":" << "UnClean msg to fill" << std::endl;
        return false;
    } else {
        _msgType = MSG_TYPE_RESP;
        _msgBody.push_back(REQ_TYPE_RESPONCE);			//REQRESP_MSG_FIELD_REQ
        _msgBody.push_back(std::to_string(_myPid));		//REQRESP_MSG_FIELD_PID
        _msgBody.push_back(_myname);					//REQRESP_MSG_FIELD_NAME
        _msgBody.push_back(respType);					//REQRESP_MSG_FIELD_RESP
        _msgBody.push_back("0");						//REQRESP_MSG_FIELD_PARTCOUNT
        _msgBody.push_back("Generic");
    }
    return true;
}
bool pktmessage::fillResp(std::string respType, std::string &response) {
    if (!isclean()) {
		std::cout << __PRETTY_FUNCTION__ << ":" << "UnClean msg to fill" << std::endl;
        return false;
    } else {
        _msgType = MSG_TYPE_RESP;
        _msgBody.push_back(REQ_TYPE_RESPONCE);
        _msgBody.push_back(std::to_string(_myPid));
        _msgBody.push_back(_myname);
        _msgBody.push_back(respType);
        _msgBody.push_back(std::to_string(1));
        _msgBody.push_back(response);
    }
    return true;
}
bool pktmessage::fillResp(std::string respType, std::vector<std::string> &responses) {
	if (!isclean()) {
		std::cout << __PRETTY_FUNCTION__ << ":" << "UnClean msg to fill" << std::endl;
        return false;
    } else {
        _msgType = MSG_TYPE_REQ;
		_msgBody.push_back(REQ_TYPE_RESPONCE);
		_msgBody.push_back(std::to_string(_myPid));
		_msgBody.push_back(_myname);
		_msgBody.push_back(respType);
		_msgBody.push_back(std::to_string(responses.size()));
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
        return false;
    } else {
        _msgType = MSG_TYPE_TOPIC;
        _msgBody[TOPIC_MSG_FIELD_TOPIC] = topic;
        _msgBody[TOPIC_MSG_FIELD_PID] = _myPid;
        _msgBody[TOPIC_MSG_FIELD_PNAME] = _myname;
        _msgBody[TOPIC_MSG_FIELD_EXTINFO] = adon;
		for (auto &a : containts) {
            _msgBody.push_back(a);
        }
    }
    return true;
}

bool pktmessage::compose(zmqpp::message &msg) {
	if (msg.parts() != 0) {
		throw std::invalid_argument("Invalid msg to compose !");
		return false;
	} else if (_msgBody.size() == 0) {
		throw std::invalid_argument("Invalid buffer to compose from!");
		return false;
	} else {
		for (auto &a : _msgBody) {
			msg.add(a);
		}
	}
	return true;
}
