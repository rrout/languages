#include "hdr.h"
#include "pktdispatch.h"
#include "pktdispatchconfig.h"
#include "pktpublisher.h"
#include "pktmessage.h"

pktdispatch::pktdispatch() {
	std::cout << __PRETTY_FUNCTION__ << ":" << "Constructing Object" << std::endl;
	//this->startProcessing();
}

pktdispatch::~pktdispatch() {
	std::cout << __PRETTY_FUNCTION__ << ":" << "Destructing Object" << std::endl;
}

void pktdispatch::publishDataPoller() {
	pktdispatchconfig *inst = pktdispatchconfig::getInstance();
	zmqpp::socket socket (context, pubDataPollertype);
	// bind to the socket
	cout << "Binding to " << pubDataPollerEndpoint << "..." << endl;
	socket.bind(pubDataPollerEndpoint);
	while(1) {
		// receive the message
		cout << "Receiving message..." << endl;
		std::string topic;
		std::string address;
		std::string name;
		zmqpp::message message;
		std::string msg;
		// decompose the message
		socket.receive(message);
		int msgParts = message.parts();
		for (int i = 0; i < msgParts; i++) {
			if (i == 0) {
				topic = message.get(0);
			}
			if (i == 1) {
                address = message.get(1);
            }
			if (i == 2) {
                name = message.get(2);
            }
			std::cout << "Msg(" << i << ") : " << message.get(i) << std::endl;
		}
		std::cout << "Msg : [ " << topic << " ][ " << address << " ][ " << name << " ] : ..... " << std::endl;
		pktpublisher *p = inst->getPublisher(topic);
		if (p) {
			std::vector<std::string> buf;
			for (int i = 0; i < msgParts; i++) {
				buf.push_back(message.get(i));
			}
			p->publish(buf);
		} else {
			std::cout << __PRETTY_FUNCTION__ << "No Publisher found : " << topic << " Discarding...." <<  std::endl;
		}
	}
	std::cout << __PRETTY_FUNCTION__ << ":" << "Exit" << std::endl;
}

void pktdispatch::dispatchMgmtPoller() {
	zmqpp::socket socket (context, mgmtendpointType);
    cout << "Binding to " << mgmtendpoint << "..." << endl;
    socket.bind(mgmtendpoint);
    while(1) {
        // receive the message
        cout << "Receiving publishing client request..." << endl;
        zmqpp::message request;
		zmqpp::message reply;
        // decompose the message
        socket.receive(request);

		pktmessage req(request);
		pktmessage resp("SERVER");
		processRqust(req, resp);
		try {
			resp.compose(reply);
		} catch(...) {
			std::abort();
		}
		socket.send(reply);

		//socket.send(message);
    }
	std::cout << __PRETTY_FUNCTION__ << ":" << "Exit" << std::endl;
}

void pktdispatch::infoPublishPoller() {
	zmqpp::socket socket (context, infoPubEndpointType);
	cout << "Binding to " << infoPubEndpoint << "..." << endl;
	socket.bind(infoPubEndpoint);
	while(1) {
		// receive the message
		std::cout << __PRETTY_FUNCTION__ << __LINE__ <<
			"Receiving message..." << endl;
		zmqpp::message message;
		// decompose the message
		//socket.receive(message);
		pktmessage req("SERVER");
		std::string a = ";;;;;;;;;;;;;;;;;;;;;;";
		req.fillTopic("CLIENT:1", TOPIC_CONTENT_PLANE_TEXT, a);
		req.compose(message);
		socket.send(message);
		std::this_thread::sleep_for(std::chrono::milliseconds(60000));
	}
	std::cout << __PRETTY_FUNCTION__ << ":" << "Exit" << std::endl;
}

void pktdispatch::dispatchEngiene() {
	zmqpp::socket socket (context, dispatchtype);
	cout << "Binding to " << dispatchEndpoint << "..." << endl;
	socket.bind(dispatchEndpoint);
	while(1) {
        // receive the message
        cout << "Receiving message..." << endl;
        zmqpp::message message;
        // decompose the message
        socket.receive(message);
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
			std::string pubEndpoint = "tcp://ras-srv-1.tcn3ucaalpoungcdz0optfllyb.xx.internal.cloudapp.net:4242";
			res.fillResp(RESP_TYPE_OK, pubEndpoint);
		} else if (req.getmsgfield(REQRESP_MSG_FIELD_REQ) == REQ_TYPE_GET_SUB_ENDPOINT) {
			std::string subEndpoint = "tcp://ras-srv-1.tcn3ucaalpoungcdz0optfllyb.xx.internal.cloudapp.net:4245";
			res.fillResp(RESP_TYPE_OK, subEndpoint);
		} else if (req.getmsgfield(REQRESP_MSG_FIELD_REQ) == REQ_TYPE_GET_ADV_ENDPOINT) {
			std::string advEndpoint = "tcp://ras-srv-1.tcn3ucaalpoungcdz0optfllyb.xx.internal.cloudapp.net:4244";
			res.fillResp(RESP_TYPE_OK, advEndpoint);
		} else if (req.getmsgfield(REQRESP_MSG_FIELD_REQ) == REQ_TYPE_GET_ENCODE_AUTH_KEY) {
			res.fillResp(RESP_TYPE_BAD_REQ);
		} else if (req.getmsgfield(REQRESP_MSG_FIELD_REQ) == REQ_TYPE_GET_DECODE_AUTH_KEY) {
			res.fillResp(RESP_TYPE_BAD_REQ);
		} else if (req.getmsgfield(REQRESP_MSG_FIELD_REQ) == REQ_TYPE_GET_HEART_BEAT) {
			std::string str = "Heart Beat OK";
			res.fillResp(RESP_TYPE_OK, str);
		} else if (req.getmsgfield(REQRESP_MSG_FIELD_REQ) == REQ_TYPE_GET_TOPIC_LIST) {
			res.fillResp(RESP_TYPE_BAD_REQ);
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
	std::make_unique<std::future<void>*>(new auto(std::async(std::launch::async, &pktdispatch::dispatchEngiene, this))).reset();
	std::cout << __PRETTY_FUNCTION__ << ":" << "--- END PROCESSING ---" << std::endl;
}


