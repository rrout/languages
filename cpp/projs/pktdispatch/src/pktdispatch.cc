#include "hdr.h"
#include "pktdispatch.h"
#include "pktdispatchconfig.h"
#include "pktpublisher.h"
#include "constants.h"

pktdispatch::pktdispatch() {
	std::cout << __PRETTY_FUNCTION__ << ":" << "Constructing Object" << std::endl;
	//this->startProcessing();
}

pktdispatch::~pktdispatch() {
	std::cout << __PRETTY_FUNCTION__ << ":" << "Destructing Object" << std::endl;
}

std::vector<std::string> pktdispatch::convertZmqMsgToVector(zmqpp::message &msg) {
	int msgParts = msg.parts();
	std::vector<std::string> buf;
    for (int i = 0; i < msgParts; i++) {
        buf.push_back(msg.get(i));
    }
	return buf;
}

zmqpp::message pktdispatch::convertVectorToZmqMsg(std::vector<std::string> &msg) {
	zmqpp::message message;
	for (auto &entry : msg) {
		message.add(entry);
	}
	return message;
}

void pktdispatch::printRequest(std::vector<std::string> &msg) {
	std::string req;
	for (auto &entry : msg) {
		req += entry;
		req += ":-:";
	}
	std::cout << "Req : " << req << std::endl;
}

void pktdispatch::printReply(std::vector<std::string> &msg) {

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
		}
	}
	std::cout << __PRETTY_FUNCTION__ << ":" << "Exit" << std::endl;
}

void pktdispatch::publisherRegisteryPoller() {
	zmqpp::socket socket (context, pubRegtype);
    cout << "Binding to " << pubRegEndpoint << "..." << endl;
    socket.bind(pubRegEndpoint);
    while(1) {
        // receive the message
        cout << "Receiving publishing client request..." << endl;
        zmqpp::message message;
        // decompose the message
        socket.receive(message);
		std::vector<std::string> request = convertZmqMsgToVector(message);
		printRequest(request);
		if (validation::validateReqRespMsg(request)) {
		} else {
            cout << "Request is not a valid req" << std::endl;
        }
		socket.send(message);
    }
	std::cout << __PRETTY_FUNCTION__ << ":" << "Exit" << std::endl;
}
void pktdispatch::subscriberRegisteryPoller() {
	zmqpp::socket socket (context, subRegtype);
	cout << "Binding to " << subRegEndpoint << "..." << endl;
	socket.bind(subRegEndpoint);
	while(1) {
		// receive the message
		cout << "Receiving message..." << endl;
		zmqpp::message message;
		// decompose the message
		socket.receive(message);
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

void pktdispatch::startProcessing() {
	std::cout << __PRETTY_FUNCTION__ << ":" << "--- START PROCESSING ---" << std::endl;
	std::cout << __PRETTY_FUNCTION__ << ":" << "pubDataPollerThread" << std::endl;
	//auto pubDataPollerThread = std::async(std::launch::async, &pktdispatch::publishDataPoller, this);
	std::make_unique<std::future<void>*>(new auto(std::async(std::launch::async, &pktdispatch::publishDataPoller, this))).reset();
	std::cout << __PRETTY_FUNCTION__ << ":" << "pubRegPollerThread" << std::endl;
    //auto pubRegPollerThread = std::async(std::launch::async, &pktdispatch::publisherRegisteryPoller, this);
	std::make_unique<std::future<void>*>(new auto(std::async(std::launch::async, &pktdispatch::publisherRegisteryPoller, this))).reset();
	std::cout << __PRETTY_FUNCTION__ << ":" << "subRegThread" << std::endl;
	//auto subRegThread = std::async(std::launch::async, &pktdispatch::subscriberRegisteryPoller, this);
	std::make_unique<std::future<void>*>(new auto(std::async(std::launch::async, &pktdispatch::subscriberRegisteryPoller, this))).reset();
	std::cout << __PRETTY_FUNCTION__ << ":" << "dispatchThread" << std::endl;
	//auto dispatchThread = std::async(std::launch::async, &pktdispatch::dispatchEngiene, this);
	std::make_unique<std::future<void>*>(new auto(std::async(std::launch::async, &pktdispatch::dispatchEngiene, this))).reset();
	std::cout << __PRETTY_FUNCTION__ << ":" << "--- END PROCESSING ---" << std::endl;
}


