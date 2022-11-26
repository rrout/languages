#include "hdr.h"
#include "pktpublisher.h"
#include "pktdispatchconfig.h"

pktpublisher::pktpublisher(std::string topic) {
	std::cout << __PRETTY_FUNCTION__ << "Constructing" << std::endl;
	_topic = topic;
	_maxBuffer = PD_MAX_BUFFER_COUNT;
	_publishCount = 0;
	_publishError = 0;
	_publishSendCount = 0;
	_publishSendError = 0;
	piblishorCount  = 1;
}

pktpublisher::~pktpublisher() {
	std::cout << __PRETTY_FUNCTION__ << "Desstructing" << std::endl;
}


void pktpublisher::addEntry(std::string name) {
	if (!entryExist(name)) {
		std::pair<std::string, std::string> entry(name, "");
		pubEntryList.push_back(entry);
	}
}

void pktpublisher::delEntry(std::string name) {
	std::pair<std::string, std::string> p;
	if (entryExist(name)) {
		for (auto &e : pubEntryList) {
			if (e.first == name) {
				p = e;
			}
		}
		//auto p = getPublisherEntry(name);
		pubEntryList.remove(p);
	}
}

bool pktpublisher::entryExist(std::string name) {
	for (auto &e : pubEntryList) {
		if (e.first == name) {
			return true;
		}
	}
	return false;
}

std::pair<std::string, std::string> &pktpublisher::getPublisherEntry(std::string name) {
	for (auto &e : pubEntryList) {
		if (e.first == name) {
			return e;
		}
	}
	//return nullptr;
}

void pktpublisher::printEntries() {
	std::cout << "Entries" << std::endl <<
		"-----------------------------" << std::endl;
	for (auto &e : pubEntryList) {
		std::cout << e.first << "  |  " <<
			e.second << std::endl;
	}
}

std::string pktpublisher::getTopic() {
	return _topic;
}

void pktpublisher::publishCount() {
	_publishCount++;
}

int pktpublisher::getPublishCount() {
	return _publishCount;
}

void pktpublisher::publishSendCount() {
	_publishSendCount++;
}

int pktpublisher::getPublishSendCount() {
	return _publishSendCount;
}

void pktpublisher::publishSendError() {
	_publishSendError++;
}

int pktpublisher::getPublishSendError() {
	return _publishSendError;
}

int pktpublisher::errcount() {
	return _publishError;
}

int pktpublisher::currentBuffCount() {
	std::lock_guard mutex(lock);
	return _infos.size();
}

bool pktpublisher::publish(std::vector<std::string> &buffer) {
	std::cout << __PRETTY_FUNCTION__ << "Publishing" << std::endl;
	if(buffer.size() == 0) {
		return false;
	}
	//Copy to a new vector
	std::lock_guard mutex(lock);
	if (_infos.size() >= PD_MAX_BUFFER_COUNT) {
		std::cout << __PRETTY_FUNCTION__ <<
			"Buffer size : " << _infos.size() <<
			" greater than max " << PD_MAX_BUFFER_COUNT <<
			" Dropping a buffer to make space for new one" <<
			std::endl;
		_infos.pop_back();
	}
	_infos.push_front(buffer);
	publishCount();

	pktdispatchconfig *inst = pktdispatchconfig::getInstance();
	inst->notifyPublishMgmt();

	return true;
}

bool pktpublisher::print() {
	std::cout << "Topic : " << _topic <<
		" | Publisher Count : " << piblishorCount <<
		" | Buffer Count : " << currentBuffCount() <<
		" | Publish Count : " << getPublishCount() <<
		" | Send Count : " << getPublishSendCount() <<
		std::endl;
	printEntries();
	return true;
}

bool pktpublisher::isBuffPresent() {
	return (_infos.empty()) ? false : true;
}

bool pktpublisher::printBuf() {
	std::lock_guard mutex(lock);
	if (isBuffPresent()) {
		for (auto const &l : _infos) {
			if(!l.empty()) {
				std::cout << "Topic : " << l[0] << std::endl;
			}
		}
	}
	return true;
}

bool pktpublisher::publishBuf(std::vector<std::string> buffer) {
	pktdispatchconfig *inst = pktdispatchconfig::getInstance();
	std::cout << __PRETTY_FUNCTION__ << "Publishing Topic : " << buffer[0] << std::endl;
	publishSendCount();
	inst->updataSubscriberStatistics(getTopic());
	//TODO
	return true;
}

bool pktpublisher::processBufferBatch() {

	return true;
}

bool pktpublisher::processBuffer() {
	pktdispatchconfig *inst = pktdispatchconfig::getInstance();
	std::vector<std::string> buf;
	if (!inst->isSubscriberPresent(_topic)) {
		return false;
	}
	while (isBuffPresent()) {
		{
			std::lock_guard mutex(lock);
			buf = _infos.back();
			_infos.pop_back();
		}
		//https://stackoverflow.contextom/questions/21531096/can-i-use-stdasync-without-waiting-for-the-future-limitation
		std::make_unique<std::future<bool>*>(new auto(std::async(std::launch::async, &pktpublisher::publishBuf, this, buf))).reset();
		//auto sendThread = std::async(std::launch::async, &pktpublisher::publishBuf, this, buf);

	}
	return true;
}

