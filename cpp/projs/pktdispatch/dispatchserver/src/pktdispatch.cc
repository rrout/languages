#include "hdr.h"
#include "constants.h"
#include "endpoint.h"
#include "pktdispatch.h"
#include "pktdispatchconfig.h"
#include "pktpublisher.h"
#include "pktmessage.h"
#include "logger.h"

pktdispatch::pktdispatch() {
	std::cout << __PRETTY_FUNCTION__ << ":" << "Constructing Object" << std::endl;
	//this->startProcessing();
}

pktdispatch::~pktdispatch() {
	std::cout << __PRETTY_FUNCTION__ << ":" << "Destructing Object" << std::endl;
}

void pktdispatch::publishDataPoller() {
	pktdispatchconfig *inst = pktdispatchconfig::getInstance();
	while(1) {
		cout << "Receiving message..." << endl;
		pktmessage msg;
		endPoint * endpoint = inst->endpoints.getPubEndpointConnection(PUB_ENDPOINT);
		if (endpoint) {
			endpoint->recieve(msg);
			std::string topic = msg.getPart(0);
			std::cout << "TOPIC : " << topic <<
            "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << std::endl;
			msg.print();
			pktpublisher *p = inst->getPublisher(topic);
			if (p) {
				std::vector<std::string> buf;
				msg.copyback(buf);
				p->publish(buf);
			} else {
				std::cout << __PRETTY_FUNCTION__ << "No Publisher found : " << topic << " Discarding...." <<  std::endl;
			}
		} else {
			std::cout << __PRETTY_FUNCTION__ << "NULL PTR Returned" << std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(ERR_INT_MILISEC));
		}
	}
	std::cout << __PRETTY_FUNCTION__ << ":" << "Exit" << std::endl;
}

void pktdispatch::dispatchMgmtPoller() {
	pktdispatchconfig *inst = pktdispatchconfig::getInstance();
	while(1) {
		cout << "Receiving mgmt client request..." << endl;
		endPoint * endpoint = inst->endpoints.getMgmtConnection();
		if (endpoint) {
			pktmessage req;
			endpoint->recieve(req);
			pktmessage resp("SERVER");
			processRqust(req, resp);
			endpoint->send(resp);
		} else {
			std::cout << __PRETTY_FUNCTION__ << "NULL PTR Returned" << std::endl;
		}
	}
	std::cout << __PRETTY_FUNCTION__ << ":" << "Exit" << std::endl;
}

void pktdispatch::infoPublishPoller() {
	pktdispatchconfig *inst = pktdispatchconfig::getInstance();
	while(1) {
		std::cout << __PRETTY_FUNCTION__ << __LINE__ << "ADV THREAD" << std::endl;
		std::string self = inst->endpoints.getName();
		std::vector<std::string> msg;
		msg.push_back("PERIODIC HELLO FROM " + self);
		sendPeriodicAdv(msg);

		for (auto &tentry : inst->publisher) {
			std::cout << __PRETTY_FUNCTION__ << "Adv processing Topic : " << tentry.first << std::endl;
			for (auto &list : tentry.second->pubEntryList) {
				std::cout << __PRETTY_FUNCTION__ << "Adv processing Client : " << list.first << std::endl;
				sendAdv(list.first, msg);
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(ADV_INT_MILISEC));
	}
	std::cout << __PRETTY_FUNCTION__ << ":" << "Exit" << std::endl;
}

void pktdispatch::maintainanceEngiene() {
	while (1) {
		std::this_thread::sleep_for(std::chrono::milliseconds(ERR_INT_MILISEC));
	}
	std::cout << __PRETTY_FUNCTION__ << ":" << "Exit" << std::endl;
}

bool pktdispatch::processRqust(pktmessage &req, pktmessage &res) {
	bool ret = true;
	std::cout << __PRETTY_FUNCTION__ << "Processing Request" << std::endl;
	req.print();
	if (!req.valid()) {
		std::cout << __PRETTY_FUNCTION__ << "Invalid Request" << std::endl;
		std::vector<std::string> msg;
		msg.push_back("Request is not recognized : ");
		msg.push_back(req.getmsgfield(REQRESP_MSG_FIELD_REQ));
		res.fillResp(RESP_TYPE_BAD_REQ, msg);
		ret = false;
	} else {
		pktdispatchconfig *inst = pktdispatchconfig::getInstance();
		if (req.getmsgfield(REQRESP_MSG_FIELD_REQ) == REQ_TYPE_GET_PUB_ENDPOINT) {
			std::string topic = req.getmsgfield(REQRESP_MSG_FIELD_TEXT);
			if (!topic.empty() /* && topicNameValid(topic) */) {
				std::string pEp = inst->endpoints.getPublisherFQEndpoint(topic);
				if (!pEp.empty()) {
					std::cout << __PRETTY_FUNCTION__ << "Respinding Pub Endpoint : " << pEp << " TOPIC: " << topic << std::endl;
					//std::string pubEndpoint = "tcp://ras-srv-1.tcn3ucaalpoungcdz0optfllyb.xx.internal.cloudapp.net:4242";
					std::string pubEndpoint = pEp;
					res.fillResp(RESP_TYPE_OK, pubEndpoint);
				} else {
					res.fillResp(RESP_TYPE_BAD_REQ);
				}
			} else {
				res.fillResp(RESP_TYPE_BAD_REQ);
			}
		} else if (req.getmsgfield(REQRESP_MSG_FIELD_REQ) == REQ_TYPE_GET_SUB_ENDPOINT) {
			std::string topic = req.getmsgfield(REQRESP_MSG_FIELD_TEXT);
			if (!topic.empty() /* && topicNameValid(topic) */) {
				std::string sEp = inst->endpoints.getSubscriberFQEndpoint(topic);
				if (!sEp.empty()) {
					std::cout << __PRETTY_FUNCTION__ << "Respinding Sub Endpoint : " << sEp << " TOPIC: " << topic << std::endl;
					//std::string subEndpoint = "tcp://ras-srv-1.tcn3ucaalpoungcdz0optfllyb.xx.internal.cloudapp.net:4245";
					std::string subEndpoint = sEp;
					res.fillResp(RESP_TYPE_OK, subEndpoint);
				} else {
					res.fillResp(RESP_TYPE_BAD_REQ);
				}
			} else {
				res.fillResp(RESP_TYPE_BAD_REQ);
			}
		} else if (req.getmsgfield(REQRESP_MSG_FIELD_REQ) == REQ_TYPE_GET_ADV_ENDPOINT) {
			std::string aEp = inst->endpoints.getAdvFQEndpoint();
			if (!aEp.empty()) {
				std::cout << __PRETTY_FUNCTION__ << "Respinding Adv Endpoint : " << aEp  << std::endl;
				//std::string advEndpoint = "tcp://ras-srv-1.tcn3ucaalpoungcdz0optfllyb.xx.internal.cloudapp.net:4244";
				std::string advEndpoint = aEp;
				res.fillResp(RESP_TYPE_OK, advEndpoint);
			} else {
				 res.fillResp(RESP_TYPE_BAD_REQ);
			}
		} else if (req.getmsgfield(REQRESP_MSG_FIELD_REQ) == REQ_TYPE_GET_ENCODE_AUTH_KEY) {
			res.fillResp(RESP_TYPE_BAD_REQ);
		} else if (req.getmsgfield(REQRESP_MSG_FIELD_REQ) == REQ_TYPE_GET_DECODE_AUTH_KEY) {
			res.fillResp(RESP_TYPE_BAD_REQ);
		} else if (req.getmsgfield(REQRESP_MSG_FIELD_REQ) == REQ_TYPE_GET_HEART_BEAT) {
			std::string str = "Heart Beat OK";
			res.fillResp(RESP_TYPE_OK, str);
		} else if (req.getmsgfield(REQRESP_MSG_FIELD_REQ) == REQ_TYPE_GET_TOPIC_LIST) {
			std::vector<std::string> topiclist = inst->endpoints.getTopics();;
			res.fillResp(RESP_TYPE_BAD_REQ, topiclist);
		} else if (req.getmsgfield(REQRESP_MSG_FIELD_REQ) == REQ_TYPE_RESPONCE) {
			res.fillResp(RESP_TYPE_BAD_REQ);
		} else if (req.getmsgfield(REQRESP_MSG_FIELD_REQ) == REQ_TYPE_REGISTER_PUBLISHER) {
			std::string pubName = req.getmsgfield(REQRESP_MSG_FIELD_NAME);
			std::string pubId = req.getmsgfield(REQRESP_MSG_FIELD_PID);
			std::string topic = req.getmsgfield(REQRESP_MSG_FIELD_TEXT);
			std::cout << __PRETTY_FUNCTION__ << "PubRegRequest : [ " <<
				pubName << " ] [ " << pubId << " ] for [ " <<
				topic << " ]" <<
				std::endl;
			inst->cratePublisher(topic, pubName);
			res.fillResp(RESP_TYPE_OK);
		} else if (req.getmsgfield(REQRESP_MSG_FIELD_REQ) == REQ_TYPE_REGISTER_SUBSCRIBER) {
			std::string subName = req.getmsgfield(REQRESP_MSG_FIELD_NAME);
			std::string subId = req.getmsgfield(REQRESP_MSG_FIELD_PID);
			std::string topic = req.getmsgfield(REQRESP_MSG_FIELD_TEXT);
			std::cout << __PRETTY_FUNCTION__ << "SubscribeRequest : [ " <<
				subName << " ] [ " << subId << " ] for [ " <<
				topic << " ]" <<
				std::endl;
			inst->addSubscriber(subName, topic);
			res.fillResp(RESP_TYPE_OK);
		} else if (req.getmsgfield(REQRESP_MSG_FIELD_REQ) == REQ_TYPE_GET_MY_REGESTRATION) {
			res.fillResp(RESP_TYPE_BAD_REQ);
		} else if (req.getmsgfield(REQRESP_MSG_FIELD_REQ) == REQ_TYPE_NONE) {
			std::vector<std::string> msg;
			msg.push_back("Request REQ_TYPE_NONE is unprocessable");
			res.fillResp(RESP_TYPE_BAD_REQ, msg);
		} else {
			res.fillResp(RESP_TYPE_BAD_REQ);
		}
	}
	std::cout << __PRETTY_FUNCTION__ << "Processed Responce" << std::endl;
	res.type(MSG_TYPE_RESP);
	res.print();
	return true;
}
void pktdispatch::sendAdv(std::string clientOrTopic, pktmessage &msg) {
	pktdispatchconfig *inst = pktdispatchconfig::getInstance();
	endPoint *endpoint = inst->endpoints.getAdvConnection();
	if (msg.valid()) {
        if (endpoint) {
            endpoint->send(msg);
        } else {
            std::cout << __PRETTY_FUNCTION__ << "NULL PTR Returned" << std::endl;
        }
	}
}
void pktdispatch::sendAdv(std::string clientOrTopic, std::vector<std::string> &msg) {
    pktdispatchconfig *inst = pktdispatchconfig::getInstance();
    endPoint *endpoint = inst->endpoints.getAdvConnection();
	std::string self = inst->endpoints.getName();
    pktmessage message(self);
	message.fillTopic(clientOrTopic, TOPIC_CONTENT_PLANE_TEXT, msg);
    if (message.valid()) {
        if (endpoint) {
            endpoint->send(message);
        } else {
            std::cout << __PRETTY_FUNCTION__ << "NULL PTR Returned" << std::endl;
        }
    }
}

void pktdispatch::sendPeriodicAdv(std::vector<std::string> &msg) {
	pktdispatchconfig *inst = pktdispatchconfig::getInstance();
	endPoint *endpoint = inst->endpoints.getAdvConnection();
    std::string self = inst->endpoints.getName();
	if (msg.empty() ) {
		_CRETICAL << "Invalid msg" <<std::endl;
		std::abort();
	}
	pktmessage message(self);
	message.fillTopic(TOPIC_SERVER_BROADCAST, TOPIC_CONTENT_PLANE_TEXT, msg);
	if (endpoint) {
		endpoint->send(message);
	} else {
		_CRETICAL << "NULL connection" <<std::endl;
	}
}

void pktdispatch::startProcessing() {
	std::cout << __PRETTY_FUNCTION__ << ":" << "--- START PROCESSING ---" << std::endl;
	std::cout << __PRETTY_FUNCTION__ << ":" << "pubDataPollerThread" << std::endl;
	//auto pubDataPollerThread = std::async(std::launch::async, &pktdispatch::publishDataPoller, this);
	std::make_unique<std::future<void>*>(new auto(std::async(std::launch::async, &pktdispatch::publishDataPoller, this))).reset();
	std::cout << __PRETTY_FUNCTION__ << ":" << "pubRegPollerThread" << std::endl;
    //auto pubRegPollerThread = std::async(std::launch::async, &pktdispatch::dispatchMgmtPoller, this);
	std::make_unique<std::future<void>*>(new auto(std::async(std::launch::async, &pktdispatch::dispatchMgmtPoller, this))).reset();
	std::cout << __PRETTY_FUNCTION__ << ":" << "subRegThread" << std::endl;
	//auto subRegThread = std::async(std::launch::async, &pktdispatch::infoPublishPoller, this);
	std::make_unique<std::future<void>*>(new auto(std::async(std::launch::async, &pktdispatch::infoPublishPoller, this))).reset();
	std::cout << __PRETTY_FUNCTION__ << ":" << "dispatchThread" << std::endl;
	//auto dispatchThread = std::async(std::launch::async, &pktdispatch::dispatchEngiene, this);
	std::make_unique<std::future<void>*>(new auto(std::async(std::launch::async, &pktdispatch::maintainanceEngiene, this))).reset();
	std::cout << __PRETTY_FUNCTION__ << ":" << "--- END PROCESSING ---" << std::endl;
}


