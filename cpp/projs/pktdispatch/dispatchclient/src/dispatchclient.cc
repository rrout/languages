#include "hdr.h"
#include "dispatchclient.h"
#include "pktmessage.h"

dispatchclient::dispatchclient(std::string endpoint) :
	_endpoint(endpoint) {
	std::cout << __PRETTY_FUNCTION__ << "Constructing" << std::endl;
	_name = genClientName();
	_connRetry = 5;
	start();

}
dispatchclient::dispatchclient(std::string name, std::string endpoint) :
	_name(name), _endpoint(endpoint){
	std::cout << __PRETTY_FUNCTION__ << "Constructing" << std::endl;
	_connRetry = 5;
	start();
}
dispatchclient::~dispatchclient() {
	std::cout << __PRETTY_FUNCTION__ << "Destructing" << std::endl;
}

bool dispatchclient::sendRequest(pktmessage &req, pktmessage &res) {
	bool ret = true;
	req.print();
    if (!req.valid()) {
        std::cout << __PRETTY_FUNCTION__ << "Invalid Request" << std::endl;
        ret = false;
    } else {
		zmqpp::message request;
		zmqpp::message reply;
		req.compose(request);

		_connnection->send(request);
		_connnection->receive(reply);

		res.copy(reply);
		res.print();
	}
	return ret;
}
bool dispatchclient::connectNegotiation(int8_t retry) {
	std::cout << __PRETTY_FUNCTION__ << "Connecting to " << _endpoint << std::endl;
	int8_t count = 0;
	bool result = false;
	while (count < retry) {
		if (isConnected()) {
			result = true;
			break;
		}
		bool verChk = false;
		std::string ver, pubEndpoint, err;
		std::vector<std::string> topiclist;
		pktmessage req(_name);
		pktmessage resp;

		// send VERSION REQ
		req.clear();
		resp.clear();
		req.fillReq(REQ_TYPE_GET_SUB_ENDPOINT);
		sendRequest(req, resp);
		// PROCESS Version req
		if (resp.getReqPart() == REQ_TYPE_RESPONCE &&
				(resp.getRespPart() != RESP_TYPE_OK ||
				 resp.getRespPart() != RESP_TYPE_BAD_REQ)) {
			ver = resp.getContent(0);
			result = true;
		} else {
			break;
		}

		// Send MY_VERSION CHECK
		req.clear();
		resp.clear();
		req.fillReq(REQ_TYPE_GET_SUB_ENDPOINT);
		sendRequest(req, resp);
		// Process
		if (resp.getReqPart() == REQ_TYPE_RESPONCE &&
                (resp.getRespPart() != RESP_TYPE_OK ||
                 resp.getRespPart() != RESP_TYPE_BAD_REQ)) {
            verChk = true;
			result = true;
        } else {
            break;
        }

		// Send REQ_TYPE_GET_PUB_ENDPOINT
		req.clear();
		resp.clear();
		req.fillReq(REQ_TYPE_GET_PUB_ENDPOINT);
		sendRequest(req, resp);
		// Process
		if (resp.getReqPart() == REQ_TYPE_RESPONCE &&
                (resp.getRespPart() != RESP_TYPE_OK ||
                 resp.getRespPart() != RESP_TYPE_BAD_REQ)) {
            pubEndpoint = resp.getContent(0);
			result = true;
        } else {
            break;
        }

		// Get Topic List
		req.clear();
		resp.clear();
		req.fillReq(REQ_TYPE_GET_SUB_ENDPOINT);
		sendRequest(req, resp);
		// Process
		if (resp.getReqPart() == REQ_TYPE_RESPONCE &&
                (resp.getRespPart() != RESP_TYPE_OK ||
                 resp.getRespPart() != RESP_TYPE_BAD_REQ)) {
			topiclist.clear();
			for (int i = 0; i < resp.getContentSize(); i++) {
				topiclist.push_back(resp.getContent(i));
			}
			result = true;
        } else {
            break;
        }

		std::cout << "version : " << ver << std::endl;
		std::cout << "version Chk : " << verChk << std::endl;
		std::cout << "pubEndpoint : " << pubEndpoint << std::endl;
		std::cout << "TOPIC LIST" << std::endl;
		for (auto &a : topiclist) {
			std::cout << a << std::endl;
		}
		if (result == true) {
			std::cout << __PRETTY_FUNCTION__ << "Connection ESTABLISHED" << std::endl;
			break;
		}
		count++;
	}
	return result;
}
bool dispatchclient::priodicNegotiation() {
	bool result = false;
	std::cout << __PRETTY_FUNCTION__ << "Connection HEARTBEAT" << std::endl;
	pktmessage req(_name);
    pktmessage resp;

    // Send REQ_TYPE_GET_HEART_BEAT
    req.clear();
    resp.clear();
	req.fillReq(REQ_TYPE_GET_HEART_BEAT);
	sendRequest(req, resp);
	// PROCESS Responce
	if (resp.getReqPart() == REQ_TYPE_RESPONCE &&
			(resp.getRespPart() != RESP_TYPE_OK ||
			 resp.getRespPart() != RESP_TYPE_BAD_REQ)) {
		// Send REQ_TYPE_GET_HEART_BEAT
		// Process responce
		//
		// Check if server says version incompatibe
		// invalidate the connectivity

		result = true;
	}
	result = true;
	return result;
}
bool dispatchclient::connectProcessing(std::string endpoint) {
	std::cout << __PRETTY_FUNCTION__ << "Connection Procssing : " << endpoint << std::endl;
	if (_connnection == nullptr) {
		zmqpp::context context;
		_socktype = zmqpp::socket_type::request;
		_connnection = new zmqpp::socket(context, _socktype);
		_connnection->connect(endpoint);
		_context = std::move(context);
	} else {
		std::abort();
	}
	std::condition_variable hb_cond;
	std::mutex mtx;
	while (1) {
		std::cout << __PRETTY_FUNCTION__ << "** MGMT **" << std::endl;
		if (!isConnected()) {
			_connected = connectNegotiation(_connRetry);
		} else {
			_connected = priodicNegotiation();
		}
		auto timeout = std::chrono::system_clock::now() + std::chrono::seconds(6);
		std::unique_lock<std::mutex> lck(mtx);
		auto res = hb_cond.wait_until(lck, timeout);
		if (res == std::cv_status::timeout) {
			std::cout << __PRETTY_FUNCTION__ << "Connection Procssing" << std::endl;
		}
	}
	return true;
}
bool dispatchclient::isConnected() {
	return _connected;
}
bool dispatchclient::isReady() {
	return true;
}
bool dispatchclient::isReady(std::string topic) {
	return true;
}
std::string dispatchclient::genClientName() {
	stringstream ss;
	pid_t pid = getpid();
	char name [1024] = "";
	const int res = gethostname( name, sizeof name );
	const long result = gethostid();
	// Providing a seed value
	srand((unsigned) time(NULL));
	// Get a random number
	int random = rand();
	ss << name << ":" <<
		result << ":" <<
		pid << ":" << random;
	return ss.str();
}

void dispatchclient::print() {
	std::cout << "Client Name       : " << _name << std::endl;
	std::cout << "Server Address    : " << _endpoint << std::endl;
	std::cout << "Connected         : " << isConnected() << std::endl;
	std::cout << "Ready             : " << isReady() << std::endl;
	std::cout << "Publishing Topic  : " << std::endl;
	std::cout << "Subscribing Topic : " << std::endl;
	std::cout << "Publisher Snapsort: " << std::endl;
	p_lock.lock();
	for (auto &pub : publisher) {
		pub.second->print();
	}
	p_lock.unlock();
	std::cout << "SubscriberSnapsort: " << std::endl;
	s_lock.lock();
	for (auto &sub : subscriber) {
		sub.second->print();
	}
	s_lock.unlock();
}
bool dispatchclient::start() {
	std::cout << __PRETTY_FUNCTION__ << "Statting......." <<
		_endpoint << std::endl;
	//auto t_connect = std::async(std::launch::async, &dispatchclient::connectProcessing, this, _endpoint);
	//auto t_process = std::async(std::launch::async, &dispatchclient::publishProcessing, this);
	//std::make_unique<std::future<void>*>(new auto(std::async(std::launch::async, &dispatchclient::publishProcessing, this))).reset();
	threadready++;
	std::thread(&dispatchclient::connectProcessing, this, _endpoint).detach();
	threadready++;
	std::thread(&dispatchclient::publishProcessing, this).detach();

	//Wait for all helper theread to start
	while(1) {
		std::cout << __PRETTY_FUNCTION__ << "Waiting " << threadready << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		break;
	}
	std::cout << __PRETTY_FUNCTION__ << "All thread started......." << std::endl;
	return true;
}

bool dispatchclient::publishRegister(std::string topic) {
	if (publisher.contains(topic)) {
		std::cout << __PRETTY_FUNCTION__ <<
			"topic :" << topic <<
			" Already registered" <<
			std::endl;
		return true;
	}
	std::cout << __PRETTY_FUNCTION__ << "================*****=============" << std::endl;
	std::condition_variable wait_cond;
	std::mutex wait_mtx;
	std::unique_lock<std::mutex> lck(wait_mtx);
	wait_cond.wait_for(lck,std::chrono::seconds(10), std::bind(&dispatchclient::isConnected, this));
	std::cout << __PRETTY_FUNCTION__ << "================================" << std::endl;
	if (isConnected()) {
		//TODO
		std::lock_guard<std::mutex> lock(p_lock);
		publisher[topic] = new dispatchclientpub(topic);
	} else {
		std::cout << __PRETTY_FUNCTION__ <<
			"Client is not connected" <<
			" Topic : " <<
			topic <<
			std::endl;
		return false;
	}
	return true;
}
bool dispatchclient::publish(std::string topic, std::string msg) {
	if (!publisher.contains(topic)) {
		std::cout << __PRETTY_FUNCTION__ <<
			"topic :" << topic <<
			" not registered" <<
			std::endl;
		return false;
	}
	if (isConnected()) {
		 publisher[topic]->publish(topic, msg);
		 publishProcessingNotify();
	} else {
		std::cout << __PRETTY_FUNCTION__ << "Client is not connected" <<
			std::endl;
		return false;
	}
	return true;
}
bool dispatchclient::publish(std::string topic, std::vector<std::string> msg) {
	if (publisher.contains(topic)) {
        std::cout << __PRETTY_FUNCTION__ <<
            "topic :" << topic <<
            " not registered" <<
            std::endl;
        return false;
    }
    if (isConnected()) {
         publisher[topic]->publish(topic, msg);
		 publishProcessingNotify();
    } else {
        std::cout << __PRETTY_FUNCTION__ << "Client is not connected" <<
            std::endl;
        return false;
    }
    return true;
}
void dispatchclient::publishProcessing() {
	threadready--;
	while(1) {
		std::unique_lock<std::mutex> lock(p_lock);
		//p_cond.wait(lock, std::bind(&dispatchclient::reqDestroy, this));
		//p_cond.wait(lock,[this]() { return reqDestroy(); });
		p_cond.wait(lock);
		std::cout << __PRETTY_FUNCTION__ << "Notify Recieved" << std::endl;
		//TODO
		if (reqDestroy() == true) {
			std::cout << __PRETTY_FUNCTION__ << "Exit" << std::endl;
			break;
		}
		for (auto &pub : publisher) {
			pub.second->processSend();
		}
		lock.unlock();
	}
}
bool dispatchclient::reqDestroy() {
	return _reqdestroy;
}
void dispatchclient::publishProcessingNotify() {
	std::cout << __PRETTY_FUNCTION__ << "Notify" << std::endl;
	p_cond.notify_one();
}
bool dispatchclient::isPublishProcessingRequired() {
	std::lock_guard<std::mutex> lock(p_lock);
	for (auto &pub : publisher) {
		if (pub.second->getBuffCount() > 0) {
			return true;
		}
	}
	return false;
}
bool dispatchclient::subscribeRegister(std::string topic) {
	if (subscriber.contains(topic)) {
        std::cout << __PRETTY_FUNCTION__ <<
            "topic :" << topic <<
            " Already registered" <<
            std::endl;
        return true;
    }
    if (isConnected()) {
        std::lock_guard<std::mutex> lock(p_lock);
        subscriber[topic] = new dispatchclientsub(topic);
    } else {
        std::cout << __PRETTY_FUNCTION__ << "Client is not connected" <<
            std::endl;
        return false;
    }
    return true;
}
bool dispatchclient::subscribe(std::string topic, std::function<void(std::string, std::vector<std::string>)> callback) {
	if (!subscriber.contains(topic)) {
		std::cout << __PRETTY_FUNCTION__ <<
			 "topic :" << topic <<
			" not registered" <<
			std::endl;
		return false;
	}
	if (isConnected()) {
		subscriber[topic]->subscribe(callback);
	} else {
		std::cout << __PRETTY_FUNCTION__ << "Client is not connected" <<
			std::endl;
		return false;
	}
	return true;
}

