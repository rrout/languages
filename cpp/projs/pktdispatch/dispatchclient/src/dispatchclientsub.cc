#include "hdr.h"
#include "dispatchclientsub.h"

dispatchclientsub::dispatchclientsub(std::string topic) {
	std::cout << __PRETTY_FUNCTION__ << "Constructing" << std::endl;
	_topic = topic;
	_registered = false;
	_recvCount = 0;
	_buffCount = 0;
	start();
}
dispatchclientsub::~dispatchclientsub() {

}
void dispatchclientsub::print() {
	std::cout << "T:" << _topic <<
		" RC:" << getRecvCount() <<
		" CBC:" << getBuffCount() <<
		std::endl;
}
int dispatchclientsub::getBuffCount() const {
	return _buffCount;
}
bool dispatchclientsub::registered() const {
	return _registered;
}
void dispatchclientsub::registration(bool status) {
	_registered = status;
}
int dispatchclientsub::getRecvCount() const {
	return _recvCount;
}
bool dispatchclientsub::subscribe(Callback callback) {
	//_callback = new std::function<void(std::string, std::vector<std::string>)>(std::move(callback));
	//_callback = std::move(callback);
	//_callback = std::bind(callback, std::placeholders::_1, std::placeholders::_2);
	_callback = callback;
	return true;
}
void dispatchclientsub::processRecieve() {
	std::cout << __PRETTY_FUNCTION__ <<
		" Started for  Topic : -----------------------" <<
		_topic << std::endl;
	while(1) {
		//TODO remove
		std::this_thread::sleep_for(std::chrono::milliseconds(2000));
		std::vector<std::string> msg = {"aaaaaaaaaaaaaaaaaaaaaaaaaaaaa"};
		//TODO
		std::lock_guard<std::mutex> lock(_lock);
		_buffer.push_back(msg);
		_buffCount = _buffer.size();
		//std::make_unique<std::future<bool>*>(new auto(std::async(std::launch::async, &dispatchclientsub::dispatchCallback, this, msg))).reset();
		_recvCount++;
		processCallbacksNotify();
		//std::lock_guard<std::mutex> mtx(s_cond_lock);
		//TODO remove
		//std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	}
}
void dispatchclientsub::processCallbacks() {
	std::cout << __PRETTY_FUNCTION__ <<
		" Started for  Topic : --------------" <<
		_topic << std::endl;
	while(1) {
		//std::unique_lock<std::mutex> lock(s_cond_lock);
		std::unique_lock<std::mutex> lock(_lock);
		s_cond.wait(lock);
		std::cout << __PRETTY_FUNCTION__ << "Notify Recieved" << std::endl;
		while (_buffer.empty() == false) {
			int sendIdx = 0;
			dispatchCallback(_buffer[sendIdx]);
			//_buffer.pop_front();
			_buffer.erase( _buffer.begin() + sendIdx );
		}
		lock.unlock();
	}
}
void dispatchclientsub::processCallbacksNotify() {
	std::cout << __PRETTY_FUNCTION__ << "Notify" << std::endl;
	s_cond.notify_one();
}
bool dispatchclientsub::dispatchCallback(std::vector<std::string> msg) {
	_callback(_topic, msg);
	return true;
}

void dispatchclientsub::start() {
	std::thread(&dispatchclientsub::processRecieve, this).detach();
	std::thread(&dispatchclientsub::processCallbacks, this).detach();
}
