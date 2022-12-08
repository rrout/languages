#include "hdr.h"
#include "dispatchclient.h"
#include "pktmessage.h"
#include "logger.h"

dispatchclient::dispatchclient(std::string endpoint) :
	_endpoint(endpoint) {
	_INFO << __PRETTY_FUNCTION__ << "Constructing" << std::endl;
	_name = genClientName();
	_connRetry = 5;
	start();

}
dispatchclient::dispatchclient(std::string name, std::string endpoint) :
	_name(name), _endpoint(endpoint){
	_INFO << __PRETTY_FUNCTION__ << "Constructing" << std::endl;
	_connRetry = 5;
	start();
}
dispatchclient::~dispatchclient() {
	_INFO << __PRETTY_FUNCTION__ << "Destructing" << std::endl;
}

bool dispatchclient::sendRequest(pktmessage &req, pktmessage &res) {
	bool ret = true;
	req.print();
    if (!req.valid()) {
        _INFO << __PRETTY_FUNCTION__ << "Invalid Request" << std::endl;
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
	_INFO << __PRETTY_FUNCTION__ << "Connecting to " << _endpoint << std::endl;
	int8_t count = 0;
	bool result = false;
	while (count < retry) {
		if (isConnected()) {
			result = true;
			break;
		}
		bool verChk = false;
		std::string ver, advEndpoint, err;
		std::vector<std::string> topiclist;
		pktmessage req(_name);
		pktmessage resp;

		// send VERSION REQ
		req.clear();
		resp.clear();
		req.fillReq(REQ_TYPE_GET_VERSION);
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
		req.fillReq(REQ_TYPE_CHECK_VERSION);
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

		// Send REQ_TYPE_GET_ADV_ENDPOINT
		req.clear();
		resp.clear();
		req.fillReq(REQ_TYPE_GET_ADV_ENDPOINT);
		sendRequest(req, resp);
		// Process
		if (resp.getReqPart() == REQ_TYPE_RESPONCE &&
                (resp.getRespPart() != RESP_TYPE_OK ||
                 resp.getRespPart() != RESP_TYPE_BAD_REQ)) {
            advEndpoint = resp.getContent(0);
			result = true;
        } else {
            break;
        }

		// Get Topic List
		req.clear();
		resp.clear();
		req.fillReq(REQ_TYPE_GET_TOPIC_LIST);
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
		std::cout << "advEndpoint : " << advEndpoint << std::endl;
		std::cout << "TOPIC LIST" << std::endl;
		for (auto &a : topiclist) {
			std::cout << a << std::endl;
		}
		if (result == true) {
			std::cout << __PRETTY_FUNCTION__ << "Connection ESTABLISHED" << std::endl;
			_advendpoint = advEndpoint;
			break;
		}
		count++;
	}
	return result;
}
bool dispatchclient::priodicNegotiation() {
	bool result = false;
	_INFO << __PRETTY_FUNCTION__ << "Connection HEARTBEAT" << std::endl;
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

bool dispatchclient::connectionRetry() {
	std::string advEndpoint = _advendpoint;
	bool conected  = connectNegotiation(1);
	if (conected != _connected) {
		_CRETICAL << "Connection is changing ( " << _connected << " --> " << conected << ")" << std::endl;
		_connected = conected;
	}
	if (isConnected() && (advEndpoint != _advendpoint)) {
		_CRETICAL << "Adv endpoint is changing ( " << advEndpoint << " --> " << _advendpoint << ")" << std::endl;
	}
	for (auto &pub : publisher) {
		publishRegister(pub.first);
	}
	for (auto &sub : subscriber) {
        subscribeRegister(sub.first);
    }

	return true;
}

bool dispatchclient::processAdv(pktmessage &adv) {
	adv.print();
	if (adv.valid()) {
		std::string advTopic = adv.getTopicPart();
		std::string content0 = adv.getContent(0);
		if (advTopic == TOPIC_SERVER_BROADCAST && content0 == TOPIC_CMD_SREVER_REQ_RECONNECTION) {
			connectionRetry();
		}

	}
	return true;
}

bool dispatchclient::connectProcessing(std::string endpoint) {
	_INFO << __PRETTY_FUNCTION__ << "Connection Procssing : " << endpoint << std::endl;
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
		_INFO << __PRETTY_FUNCTION__ << "** MGMT **" << std::endl;
		if (!isConnected()) {
			_connected = connectNegotiation(_connRetry);
		} else {
			_connected = priodicNegotiation();
		}
		auto timeout = std::chrono::system_clock::now() + std::chrono::seconds(6);
		std::unique_lock<std::mutex> lck(mtx);
		auto res = hb_cond.wait_until(lck, timeout);
		if (res == std::cv_status::timeout) {
			_INFO << __PRETTY_FUNCTION__ << "Connection Procssing" << std::endl;
		}
	}
	return true;
}
void dispatchclient::advProcessing() {
	while(1) {
		if (isAddEndpointConnected()) {
			if (!isAddChennelReady()) {
				std::cout << __PRETTY_FUNCTION__ << "Connecting to" <<
					"ADV endpoint : " << _advendpoint << std::endl;
				//std::this_thread::sleep_for(std::chrono::milliseconds(1000));
				zmqpp::socket_type socktype = zmqpp::socket_type::subscribe;
				_advcon = new zmqpp::socket(_context, socktype);
				_advcon->connect(_advendpoint);
				_advcon->subscribe(_name);
				_advcon->subscribe(TOPIC_SERVER_BROADCAST);
			} else {
				zmqpp::message reply;
				_advcon->receive(reply);
				pktmessage resp(reply);
				processAdv(resp);
			}
		} else {
			std::this_thread::sleep_for(std::chrono::milliseconds(10000));
		}
	}
}
std::string dispatchclient::getPubEndpoint(std::string topic) {
	pktmessage req(_name);
	pktmessage resp;
	// Send REQ_TYPE_GET_PUB_ENDPOINT
    req.clear();
    resp.clear();
    req.fillReq(REQ_TYPE_GET_PUB_ENDPOINT, topic);
    sendRequest(req, resp);
    // PROCESS Responce
    if (resp.getReqPart() == REQ_TYPE_RESPONCE &&
            (resp.getRespPart() != RESP_TYPE_OK ||
             resp.getRespPart() != RESP_TYPE_BAD_REQ)) {
		if (!resp.getContent(0).empty()) {
			return resp.getContent(0);
		}
    }
	return {};
}
std::string dispatchclient::getSubEndpoint(std::string topic) {
	pktmessage req(_name);
    pktmessage resp;
    // Send REQ_TYPE_GET_SUB_ENDPOINT
    req.clear();
    resp.clear();
    req.fillReq(REQ_TYPE_GET_SUB_ENDPOINT, topic);
    sendRequest(req, resp);
    // PROCESS Responce
    if (resp.getReqPart() == REQ_TYPE_RESPONCE &&
            (resp.getRespPart() != RESP_TYPE_OK ||
             resp.getRespPart() != RESP_TYPE_BAD_REQ)) {
        if (!resp.getContent(0).empty()) {
            return resp.getContent(0);
        }
    }
    //return std::string();
    return {};
}
bool dispatchclient::registerTopicWithServer(client_role_t role, std::string topic) {
	pktmessage req(_name);
	pktmessage resp;
	//TODO check the local topic list
	//
	//referesh list if not found locally
	//
	//Again check if found
	//
	// Send REQ_TYPE_REGISTER_PUBLISHER
	std::string request;
	if (role == DISPATCH_CLIENT_ROLE_PUBLISHER) {
		request = REQ_TYPE_REGISTER_PUBLISHER;
	} else if (role == DISPATCH_CLIENT_ROLE_SUBSCRIBER) {
		request = REQ_TYPE_REGISTER_SUBSCRIBER;
	} else {
		std::cout << __PRETTY_FUNCTION__ << "Unknown role" << std::endl;
		return false;
	}
    req.clear();
    resp.clear();
    req.fillReq(request, topic);
    sendRequest(req, resp);
    // PROCESS Responce
    if (resp.getReqPart() == REQ_TYPE_RESPONCE &&
            (resp.getRespPart() != RESP_TYPE_OK ||
             resp.getRespPart() != RESP_TYPE_BAD_REQ)) {
		return true;
    }
	return false;
}
bool dispatchclient::isAddEndpointConnected() {
	return (isConnected() && _advendpoint.empty() == false);
}
bool dispatchclient::isAddChennelReady() {
	return (isAddEndpointConnected() && _advcon != nullptr);
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
	std::cout << "Publishing Topic  : " << publisher.size() << std::endl;
	for (auto &p : publisher) {
		std::cout << p.first << " ";
	}
	std::cout << std::endl;
	std::cout << "Subscribing Topic : " << subscriber.size() << std::endl;
	for (auto &s : subscriber) {
		std::cout << s.first << " ";
	}
	std::cout << std::endl;
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
	std::thread(&dispatchclient::advProcessing, this).detach();
	threadready++;
	std::thread(&dispatchclient::publishProcessing, this).detach();

	//Wait for all helper theread to start
	while(1) {
		_INFO << __PRETTY_FUNCTION__ << "Waiting " << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		break;
	}
	_INFO << __PRETTY_FUNCTION__ << "All thread started......." << std::endl;
	return true;
}
void dispatchclient::connectionCkeckWait(int8_t timeout) {
	_INFO << __PRETTY_FUNCTION__ << "================*****=============" << std::endl;
	std::condition_variable wait_cond;
	std::mutex wait_mtx;
	std::unique_lock<std::mutex> lck(wait_mtx);
	wait_cond.wait_for(lck,std::chrono::seconds(timeout), std::bind(&dispatchclient::isConnected, this));
	_INFO << __PRETTY_FUNCTION__ << "================================" << std::endl;

}
bool dispatchclient::publishRegister(std::string topic) {
	if (publisher.contains(topic)) {
		_INFO << __PRETTY_FUNCTION__ <<
			"topic :" << topic <<
			" Already registered" <<
			std::endl;
		return true;
	}
	connectionCkeckWait(10);
	std::string pubEndpoint;
	if (isConnected()) {
		if (!publisher.contains(topic)) {
			pubEndpoint = getPubEndpoint(topic);
			_INFO << __PRETTY_FUNCTION__ << pubEndpoint << std::endl;
			if(pubEndpoint.empty()) {
				_INFO << __PRETTY_FUNCTION__ <<
				"No able to resolve publisher connection" <<
				" Topic : " <<
				topic <<
				std::endl;
				return false;
			}

			if (registerTopicWithServer(DISPATCH_CLIENT_ROLE_PUBLISHER, topic) == false) {
				_INFO << __PRETTY_FUNCTION__ <<
					"Topic registration failed with server : " <<
					topic <<
					std::endl;
				return false;
			}

			//zmqpp::context context;
			//Use the Context of our own which is single context
			//for all client wide ZMQ socketes
			//We must own the publisher
			zmqpp::socket_type socktype = zmqpp::socket_type::push;
			zmqpp::socket *con = new zmqpp::socket(_context, socktype);
			if (con) {
				con->connect(pubEndpoint);

				std::lock_guard<std::mutex> lock(p_lock);
				publisher[topic] = new dispatchclientpub(topic, _name);
				publisher[topic]->registr(_endpoint, con);
			} else {
				_CRETICAL << __PRETTY_FUNCTION__ <<
					"Alloc fail " <<
					std::endl;
			}
		} else {
			_INFO << __PRETTY_FUNCTION__ <<
				"Publisher exists  " <<
				topic <<
				std::endl;
			return true;
		}
		//TODO
	} else {
		_INFO << __PRETTY_FUNCTION__ <<
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
		_INFO << __PRETTY_FUNCTION__ <<
			"topic :" << topic <<
			" not registered" <<
			std::endl;
		return false;
	}
	if (isConnected()) {
		 publisher[topic]->publish(topic, msg);
		 publishProcessingNotify();
	} else {
		_INFO << __PRETTY_FUNCTION__ << "Client is not connected" <<
			std::endl;
		return false;
	}
	return true;
}
bool dispatchclient::publish(std::string topic, std::vector<std::string> msg) {
	if (publisher.contains(topic)) {
        _INFO << __PRETTY_FUNCTION__ <<
            "topic :" << topic <<
            " not registered" <<
            std::endl;
        return false;
    }
    if (isConnected()) {
         publisher[topic]->publish(topic, msg);
		 publishProcessingNotify();
    } else {
        _INFO << __PRETTY_FUNCTION__ << "Client is not connected" <<
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
		_INFO << __PRETTY_FUNCTION__ << "Notify Recieved" << std::endl;
		//TODO
		if (reqDestroy() == true) {
			_INFO << __PRETTY_FUNCTION__ << "Exit" << std::endl;
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
	_INFO << __PRETTY_FUNCTION__ << "Notify" << std::endl;
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
        _INFO << __PRETTY_FUNCTION__ <<
            "topic :" << topic <<
            " Already registered" <<
            std::endl;
        return true;
    }
	connectionCkeckWait(10);
	std::string subEndpoint;
    if (isConnected()) {
		if (!subscriber.contains(topic)) {
			subEndpoint = getSubEndpoint(topic);
			_INFO << __PRETTY_FUNCTION__ << subEndpoint << std::endl;
			if(subEndpoint.empty()) {
				_INFO << __PRETTY_FUNCTION__ <<
					"No able to resolve subscriber connection" <<
					" Topic : " <<
					topic <<
					std::endl;
				return false;
			}
			if (registerTopicWithServer(DISPATCH_CLIENT_ROLE_SUBSCRIBER, topic) == false) {
				_INFO << __PRETTY_FUNCTION__ <<
					"Topic registration failed with server : " <<
					topic <<
					std::endl;
				return false;
			}
			//zmqpp::context context;
			//Use the Context of our own which is single context
			//for all client wide ZMQ socketes
			//We must own the subscriber
			zmqpp::socket_type socktype = zmqpp::socket_type::subscribe;
			zmqpp::socket *con = new zmqpp::socket(_context, socktype);
			if (con) {
				con->connect(subEndpoint);

				std::lock_guard<std::mutex> lock(p_lock);
				subscriber[topic] = new dispatchclientsub(topic);
				subscriber[topic]->registr(subEndpoint, con);
			} else {
				_INFO << __PRETTY_FUNCTION__ <<
					"Alloc fail " <<
					std::endl;
				return false;
			}
		} else {
			_INFO << __PRETTY_FUNCTION__ <<
				"Subscriber exists  " <<
				topic <<
				std::endl;
			return true;
		}
    } else {
        _INFO << __PRETTY_FUNCTION__ << "Client is not connected" <<
            std::endl;
        return false;
    }
    return true;
}
bool dispatchclient::subscribe(std::string topic, std::function<void(std::string, std::vector<std::string>)> callback) {
	if (!subscriber.contains(topic)) {
		_INFO << __PRETTY_FUNCTION__ <<
			 "topic :" << topic <<
			" not registered" <<
			std::endl;
		return false;
	}
	if (isConnected()) {
		subscriber[topic]->subscribe(callback);
	} else {
		_INFO << __PRETTY_FUNCTION__ << "Client is not connected" <<
			std::endl;
		return false;
	}
	return true;
}

