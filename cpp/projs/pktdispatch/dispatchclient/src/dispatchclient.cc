#include "hdr.h"
#include "dispatchclient.h"

dispatchclient::dispatchclient(std::string endpoint) :
	_endpoint(endpoint) {
	std::cout << __PRETTY_FUNCTION__ << "Constructing" << std::endl;
	_name = genClientName();
	start();

}
dispatchclient::dispatchclient(std::string name, std::string endpoint) :
	_name(name), _endpoint(endpoint){
	std::cout << __PRETTY_FUNCTION__ << "Constructing" << std::endl;
	start();
}
dispatchclient::~dispatchclient() {
	std::cout << __PRETTY_FUNCTION__ << "Destructing" << std::endl;
}
bool dispatchclient::connectProcessing(std::string endpoint) {
	std::cout << __PRETTY_FUNCTION__ << "Connecting to " << endpoint << std::endl;
	//TODO
	std::cout << __PRETTY_FUNCTION__ << "Connection ESTABLISHED" << std::endl;
	_connected  = true;
	std::condition_variable hb_cond;
	std::mutex mtx;
	while (1) {
		//Send Heart beat and wait for recieve
		//TODO
		auto timeout = std::chrono::system_clock::now() + std::chrono::seconds(60);
		std::unique_lock<std::mutex> lck(mtx);
		auto res = hb_cond.wait_until(lck, timeout);
		if (res == std::cv_status::timeout) {
			std::cout << __PRETTY_FUNCTION__ << "Connection HEARTBEAT" << std::endl;
			//TODO
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

