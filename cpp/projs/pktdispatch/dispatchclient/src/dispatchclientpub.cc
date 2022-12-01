#include "hdr.h"
#include "dispatchclientpub.h"
#include "pktmessage.h"


dispatchclientpub::dispatchclientpub(std::string topic) {
	std::cout << __PRETTY_FUNCTION__ << "Constructing" << std::endl;
	_topic = topic;
	_publishCount = 0;
	_buffCount = 0;
	_maxBuffCount = 10;
	_maxBatchSendCount = 10;
	_registered = false;
}

dispatchclientpub::~dispatchclientpub() {
	std::cout << __PRETTY_FUNCTION__ << "Destroying" << std::endl;
}

void dispatchclientpub::print() {
	std::cout << "T:" << _topic <<
		" RG:" << registered() <<
		" PC:" << publishCount() <<
		" CBC:" << getBuffCount() <<
		" MBC:" << _maxBuffCount <<
		" BC:" << _maxBatchSendCount <<
		std::endl;
}
int dispatchclientpub::getBuffCount() {
	return _buffCount;
}
int dispatchclientpub::publishCount() const {
	return _publishCount;
}
bool dispatchclientpub::registr(std::string endpoint, zmqpp::socket *con) {
	//TODO more validations
	if (!registered()) {
		_endpoint = endpoint;
		_con = con;
	} else {
		std::cout << __PRETTY_FUNCTION__ << "Alredy Connected" << std::endl;
		return false;
	}
	_registered = true;
	return true;
}
bool dispatchclientpub::registered() const {
	return _registered;
}
void dispatchclientpub::registration(bool status) {
	_registered = status;
}
bool dispatchclientpub::publish(std::string topic, std::string msg) {
	if (getBuffCount() >= _maxBuffCount) {
		std::cout << __PRETTY_FUNCTION__ <<
			"Buffer not available" <<
			std::endl;
		return false;
	}
	std::lock_guard<std::mutex> lock(_buffLock);
	std::vector<std::string> s = {msg};
	_buffer.push_back(s);
	_buffCount++;

	return true;
}
bool dispatchclientpub::publish(std::string topic, std::vector<std::string> msg) {
	if (getBuffCount() >= _maxBuffCount) {
		std::cout << __PRETTY_FUNCTION__ <<
			"Buffer not available" <<
			std::endl;
		return false;
	}
	std::lock_guard<std::mutex> lock(_buffLock);
	_buffer.push_back(msg);
	_buffCount++;

	return true;
}
bool dispatchclientpub::send(std::vector<std::string> &msg) {
	if (msg.empty()) {
		std::cout << __PRETTY_FUNCTION__ <<
			"Invalid message to send" <<
			" DISCARDING SEND" <<
			std::endl;
		return true;
	}
	pktmessage message(_name);
	// Convert it to TOPIC message
	message.fillTopic(_topic, "", msg);
	//Validate TOPIC message.
	message.print();
	if (_con) {
		zmqpp::message req;
		message.compose(req);
		_con->send(req);
		_publishCount++;
		std::cout << __PRETTY_FUNCTION__ << " REAL SEND " << std::endl;
    } else {
		std::cout << __PRETTY_FUNCTION__ << " REAL SEND FAIL" << std::endl;
		return false;
	}
	return true;
}
bool dispatchclientpub::processSend() {
	std::lock_guard<std::mutex> lock(_buffLock);
	int count = 0;
	std::cout << __PRETTY_FUNCTION__ <<
		"Topic : " << _topic <<
		" Publish processing" <<
		std::endl;
	while (_buffer.empty() == false) {
		int sendIdx = _buffer.size()-1;
		//TODO handle send() return status
		send(_buffer[sendIdx]);
		_buffer.erase( _buffer.begin() + sendIdx );
	}
	_buffCount =  _buffer.size();
	return true;
}
