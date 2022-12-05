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
		_myname = getNamePart();
		_myPid = getIdIPart();
	} else if (isRespTypeValid(_msgBody[REQRESP_MSG_FIELD_RESP])) {
		_msgType = MSG_TYPE_RESP;
		_myname = getNamePart();
		_myPid = getIdIPart();
		//TODO Looks odd
	} else if (isTopicContentValid(_msgBody[TOPIC_MSG_FIELD_EXTINFO])) {
		_msgType = MSG_TYPE_TOPIC;
		_myname = getNamePart();
		_myPid = getIdIPart();
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
		if (!validTopicMsg(_msgBody)) {
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
std::string pktmessage::getPart(int idx) {
	if ((_msgBody.size() == 0) ||
            (static_cast<unsigned long>(idx) > _msgBody.size()) ||
            (_msgType == MSG_TYPE_INVALID)) {
        return "";
    }
	return _msgBody[idx];
}
std::string pktmessage::getReqPart() {
	return getPart(REQRESP_MSG_FIELD_REQ);
}
std::string pktmessage::getRespPart() {
	return getPart(REQRESP_MSG_FIELD_RESP);
}
std::string pktmessage::getNamePart() {
	return getPart(REQRESP_MSG_FIELD_NAME);
}
std::string pktmessage::getIdIPart() {
	return getPart(REQRESP_MSG_FIELD_PID);
}
int pktmessage::getContentSize() {
	std::string count = getPart(REQRESP_MSG_FIELD_PARTCOUNT);
	try {
		return stoi(count);
	} catch (...) {
		std::cout << "WRONG MSG" << std::endl;
		return 0;
	}
	return 0;
}
std::string pktmessage::getContent(int index) {
	return getPart(REQRESP_MSG_FIELD_TEXT + index);
}
void pktmessage::clear() {
    _msgType = MSG_TYPE_INVALID;
    _msgBody.clear();
    _filled = false;
    _constructed = false;
}
void pktmessage::clean() {
	_myPid = {};
	_myname.clear();
	_msgType = MSG_TYPE_INVALID;
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
int8_t pktmessage::getMsgType() {
	return _msgType;
}
std::string pktmessage::getMsgTypeStr() {
	std::string msgType;
	if (_msgType == MSG_TYPE_REQ) {
		msgType = "REQURST";
	} else if (_msgType == MSG_TYPE_RESP) {
		msgType = "RESPONSE";
	} else if (_msgType == MSG_TYPE_TOPIC)  {
		msgType = "TOPIC";
	} else if (_msgType == MSG_TYPE_REQ) {
		msgType = "INVALID";
	} else {
		msgType = "UNKNOWN";
	}
	return msgType;
}
void pktmessage::print() {
	int count = 0;
	std::cout << "-------------------" << std::endl;
	std::cout << "Msg owner    : " << _myname << std::endl;
	std::cout << "Msg id       : " << _myPid << std::endl;
	std::cout << "Msg type     : " << getMsgType() << std::endl;
	std::cout << "Msg body Size: " << _msgBody.size() << std::endl;
	std::cout << "Msg copied   : " << _filled << std::endl;
	std::cout << "Msg const    : " << _constructed << std::endl;
	std::cout << "Valid        : " << (valid() ? "Yes" : "No") << std::endl;
	std::cout << "-------------------" << std::endl;
	for (auto &a : _msgBody) {
		std::cout << count << " | " << a << std::endl;
		count++;
	}
	std::cout << "-------------------" << std::endl;
	printPretty();
}
void pktmessage::printPretty() {
	std::string status;
	if (getMsgType() == MSG_TYPE_REQ) {
		status = getReqPart();
	} else if (getMsgType() == MSG_TYPE_RESP) {
		status = getRespPart();
	} else {
		status = getReqPart();
	}

	std::cout << std::setfill(' ') <<
		std::setw(11) << "Owner" <<
		std::setw(11) << "id" <<
		std::setw(11) << "Type" <<
		std::setw(11) << "Size" <<
		std::setw(11) << "Valid" <<
		std::setw(31) << "Status" <<
		std::setw(31) << "Content" <<
		std::endl;
	std::cout << std::setfill('-') <<
		std::setw(11) << " " <<
		std::setw(12) << " " <<
		std::setw(11) << " " <<
		std::setw(11) << " " <<
		std::setw(11) << " " <<
		std::setw(31) << " " <<
		std::setw(31) << " " <<
		setfill('\0') << std::endl;
	std::cout << std::setfill(' ') <<
		std::setw(11) << _myname <<
		std::setw(11) << _myPid <<
		std::setw(11) << getMsgTypeStr() <<
		std::setw(11) <<  _msgBody.size() <<
		std::setw(11) <<  (valid() ? "Yes" : "No") <<
		std::setw(31) << status <<
		std::setw(31) <<  getContent(0) <<
		setfill('\0') << std::endl;
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
	setMsgType();
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
		_msgBody.push_back(_myPid);     //REQRESP_MSG_FIELD_PID
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
        _msgBody.push_back(_myPid);					//REQRESP_MSG_FIELD_PID
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
		_msgBody.push_back(_myPid);
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
        _msgBody.push_back(_myPid);		//REQRESP_MSG_FIELD_PID
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
        _msgBody.push_back(_myPid);
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
		_msgBody.push_back(_myPid);
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
		_msgBody.push_back(topic);						//TOPIC_MSG_FIELD_TOPIC
		_msgBody.push_back(_myPid);		//TOPIC_MSG_FIELD_PID
		_msgBody.push_back(_myname);					//TOPIC_MSG_FIELD_PNAME
		if(adon.empty()) {
			_msgBody.push_back(TOPIC_CONTENT_PLANE_TEXT);
		} else {
			_msgBody.push_back(adon);					//TOPIC_MSG_FIELD_EXTINFO
		}
		_msgBody.push_back(std::to_string(1));			//TOPIC_MSG_FIELD_PARTCOUNT
		_msgBody.push_back(containt);					//TOPIC_MSG_FIELD_CONTENT
    }
    return true;
}

bool pktmessage::fillTopic(std::string topic, std::string adon, std::vector<std::string> &containts) {
    if (!isclean()) {
        return false;
    } else {
        _msgType = MSG_TYPE_TOPIC;
		_msgBody.push_back(topic);								//TOPIC_MSG_FIELD_TOPIC
		_msgBody.push_back(_myPid);				//TOPIC_MSG_FIELD_PID
		_msgBody.push_back(_myname);							//TOPIC_MSG_FIELD_PNAME
		if(adon.empty()) {
			_msgBody.push_back(TOPIC_CONTENT_PLANE_TEXT);
		} else {
			_msgBody.push_back(adon);							//TOPIC_MSG_FIELD_EXTINFO
		}
		_msgBody.push_back(std::to_string(containts.size()));	//TOPIC_MSG_FIELD_PARTCOUNT
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
