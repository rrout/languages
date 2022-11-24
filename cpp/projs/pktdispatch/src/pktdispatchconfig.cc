#include "hdr.h"
#include "pktdispatchconfig.h"
pktdispatchconfig * pktdispatchconfig::instance = nullptr;

pktdispatchconfig::pktdispatchconfig() {
	if (!instance) {
		std::cout << __PRETTY_FUNCTION__ << ":" << "Constructing Object" << std::endl;
		//Infinite loop because of recurssion
		//instance = new pktdispatchconfig();
		dispatcher = new pktdispatch();
	}
}
pktdispatchconfig::~pktdispatchconfig() {
	std::cout << __PRETTY_FUNCTION__ << ":" << "Destructing Object" << std::endl;
	if (instance) {
		//delete instance;
		instance = nullptr;
		delete dispatcher;
	}
}

pktdispatchconfig * pktdispatchconfig::createInstance() {
	if (!instance) {
		instance = new pktdispatchconfig();
		std::cout << __PRETTY_FUNCTION__ << ":" << "Creating instance" << std::endl;
		return instance;
	}
	return instance;
}

pktdispatchconfig * pktdispatchconfig::getInstance() {
	if (!instance) {
		std::cout << __PRETTY_FUNCTION__ << ":" << "NULL instance" << std::endl;
	}
	return instance;
}

void pktdispatchconfig::parse(std::string &configfile) {
	if (configfile.empty()== true) {
		std::cout << __PRETTY_FUNCTION__ << "Invalid param" << std::endl;
		throw std::invalid_argument("Invalid param");
		return;
	}
	bool exist = std::ifstream(configfile).good();
	if (exist) {

	} else {
		std::cout << __PRETTY_FUNCTION__ << configfile << " file not present" << std::endl;
		throw std::invalid_argument("file not present");
	}
}

void pktdispatchconfig::configure(std::string &config) {
	if (config.empty()== true) {
        std::cout << __PRETTY_FUNCTION__ << "Invalid param" << std::endl;
        throw std::invalid_argument("Invalid param");
        return;
    }
}

pktConfigStatus_t pktdispatchconfig::cratePublisher(std::string topic) {
	if (topic.empty()== true) {
        std::cout << __PRETTY_FUNCTION__ << "Invalid param" << std::endl;
        throw std::invalid_argument("Invalid param");
        return STATUS_FAIL;
    }
	setPublisher(topic);
	return STATUS_OK;
}

pktConfigStatus_t pktdispatchconfig::deletePublisher(std::string topic) {
	if (topic.empty()== true) {
        std::cout << __PRETTY_FUNCTION__ << "Invalid param" << std::endl;
        throw std::invalid_argument("Invalid param");
        return STATUS_FAIL;
    }
	removePublisher(topic);
	return STATUS_OK;
}

pktpublisher * pktdispatchconfig::getPublisher(std::string topic) {
	std::lock_guard mutex(p_mutex);
	if(publisher.contains(topic)) {
		return publisher[topic];
	}
	return nullptr;
}

void pktdispatchconfig::setPublisher(std::string topic) {
	std::lock_guard mutex(p_mutex);
	if (publisher.find(topic) == publisher.end()) {
        std::cout << __PRETTY_FUNCTION__ << "Create publisher for topic : " << topic << std::endl;
        publisher[topic] = new pktpublisher(topic);
    } else {
        std::cout << __PRETTY_FUNCTION__ << "Pblisher Topic " << topic << " Alreay exists" <<  std::endl;
        publisher[topic]->piblishorCount++;
    }
}

void pktdispatchconfig::removePublisher(std::string topic) {
	std::lock_guard mutex(p_mutex);
	if (publisher.contains(topic)) {
        if (publisher[topic]->piblishorCount > 0) {
            publisher[topic]->piblishorCount--;
        }

		if (publisher[topic]->piblishorCount == 0) {
			delete publisher[topic];
			publisher.erase(topic);
		}
    } else {
        std::cout << __PRETTY_FUNCTION__ << "Pblisher Topic " << topic << "doesn't exist" <<  std::endl;
    }
}

bool pktdispatchconfig::prntPublisherList() {
	std::lock_guard mutex(p_mutex);
	std::cout << "Publisher List:" <<
		"-----------------------------------" << std::endl;
	for( auto &l : publisher) {
		l.second->print();
	}
	return true;
}

bool pktdispatchconfig::isPublishPending() {
	std::lock_guard mutex(p_mutex);
	bool ret = false;
	for( auto &l : publisher) {
		if(l.second->isBuffPresent()) {
			ret = true;
			break;
		}
	}
	return ret;
}


void pktdispatchconfig::startPublishMgmt() {
	std::cout << __PRETTY_FUNCTION__ << ":" << "startPublishMgmtThread" << std::endl;
	while (1) {
		std::unique_lock<std::mutex> lock(p_mutex);
#if 0
		p_cond.wait(lock, std::bind(&pktdispatchconfig::isPublishPending, this));
		//if predicate is true the above will unblock
		//and result in infanite loop, But we want it should
		//unblock only when notification is recieved
		//i.e either when someone publish new OR
		//a new subscriber joins, hence we need below one
		if(isPublishPending()) {
			p_cond.wait(lock);
		}
#else
		p_cond.wait(lock);
		//TODO
#endif
		std::cout << __PRETTY_FUNCTION__ << ":" << "Start Real Publish" << std::endl;
		for( auto &l : publisher) {
			if(l.second->isBuffPresent()) {
				l.second->processBuffer();
			}
		}

		lock.unlock();
	}
}
void pktdispatchconfig::notifyPublishMgmt() {
	p_cond.notify_one();
}

pktConfigStatus_t pktdispatchconfig::addSubscriber(std::string subscriberName, std::string topic) {
	if (subscriberName.empty()== true || topic.empty()== true) {
        std::cout << __PRETTY_FUNCTION__ << "Invalid param" << std::endl;
        throw std::invalid_argument("Invalid param");
        return STATUS_FAIL;
    }

	if (!isSubscriberPresent(subscriberName, topic)) {
		createOrAddSubscriber(subscriberName, topic);
	}

	return STATUS_OK;
}

pktConfigStatus_t pktdispatchconfig::delSubscriber(std::string subscriberName, std::string topic) {
	if (subscriberName.empty()== true || topic.empty()== true) {
        std::cout << __PRETTY_FUNCTION__ << "Invalid param" << std::endl;
        throw std::invalid_argument("Invalid param");
        return STATUS_FAIL;
    }
	if (isSubscriberPresent(subscriberName, topic)) {
		removeSubscriber(subscriberName, topic);
	} else {
		std::cout << __PRETTY_FUNCTION__ <<
			"Subscriber : " << subscriberName <<
			" not present for topic : " << topic <<
			std::endl;
	}
	return STATUS_OK;
}

bool pktdispatchconfig::createOrAddSubscriber(std::string subscriberName, std::string topic) {
	if (subscriberName.empty()== true || topic.empty()== true) {
        std::cout << __PRETTY_FUNCTION__ << "Invalid param" << std::endl;
        throw std::invalid_argument("Invalid param");
        return false;
    }
	if (isSubscriberPresent(subscriberName, topic)) {
        std::cout << __PRETTY_FUNCTION__ << "Duplicate Subscriber : " <<
            " for topic : " << topic <<
            std::endl;
        return false;
    }

	pktsubscriber * sub = new pktsubscriber(subscriberName, topic);
	std::lock_guard mutex(s_mutex);
    if (subscribers.contains(topic)) {
        std::cout << __PRETTY_FUNCTION__ <<
            "Inserting new subscriber : " << subscriberName <<
            " to the old topic : " << topic <<
            std::endl;
        subscribers[topic].push_front(sub);
    } else {
        std::cout << __PRETTY_FUNCTION__ <<
            "Creating new subscriber : " << subscriberName <<
            " and new topic : " << topic <<
            std::endl;
        subscribers[topic].push_front(sub);
    }
	return true;
}

bool pktdispatchconfig::removeSubscriber(std::string subscriberName, std::string topic) {
	if (subscriberName.empty()== true || topic.empty()== true) {
        std::cout << __PRETTY_FUNCTION__ << "Invalid param" << std::endl;
        throw std::invalid_argument("Invalid param");
        return false;
    }
	if (isSubscriberPresent(subscriberName, topic)) {
		pktsubscriber * sub = getSubscriber(subscriberName, topic);
		if (sub) {
			std::lock_guard mutex(s_mutex);
			subscribers[topic].remove(sub);
			delete sub;
			if (subscribers[topic].size() == 0) {
				subscribers.erase(topic);
			}
		}
	}
	return true;
}
bool pktdispatchconfig::isSubscriberPresent(std::string topic) {
	if (topic.empty()== true) {
        std::cout << __PRETTY_FUNCTION__ << "Invalid param" << std::endl;
        throw std::invalid_argument("Invalid param");
        return false;
    }

	std::lock_guard mutex(s_mutex);
	if(subscribers.contains(topic)) {
		if (subscribers[topic].size() > 0) {
			return true;
		}
	}
	return false;
}

bool pktdispatchconfig::isSubscriberPresent(std::string subscriberName, std::string topic) {
	if (subscriberName.empty()== true || topic.empty()== true) {
        std::cout << __PRETTY_FUNCTION__ << "Invalid param" << std::endl;
        throw std::invalid_argument("Invalid param");
        return false;
    }

	std::lock_guard mutex(s_mutex);
	if (subscribers.contains(topic)) {
		for(auto &sl : subscribers[topic]) {
			if (sl->getName() == subscriberName) {
				return true;
			}
		}
	}
	return false;
}

pktsubscriber * pktdispatchconfig::getSubscriber(std::string subscriberName, std::string topic) {
	if (subscriberName.empty()== true || topic.empty()== true) {
        std::cout << __PRETTY_FUNCTION__ << "Invalid param" << std::endl;
        throw std::invalid_argument("Invalid param");
        return nullptr;
    }

	std::lock_guard mutex(s_mutex);
	if (subscribers.contains(topic)) {
        for(auto &sl : subscribers[topic]) {
            if (sl->getName() == subscriberName) {
                return sl;
            }
        }
    }
    return nullptr;
}

void pktdispatchconfig::updataSubscriberStatistics(std::string topic) {
	if (topic.empty()== true) {
        std::cout << __PRETTY_FUNCTION__ << "Invalid param" << std::endl;
        throw std::invalid_argument("Invalid param");
        return;
    }
	if (isSubscriberPresent(topic)) {
		std::lock_guard mutex(s_mutex);
		if (subscribers.contains(topic)) {
			for(auto &sl : subscribers[topic]) {
				sl->pktSent();
			}
		}
	}
}

bool pktdispatchconfig::printSubscriberlist() {
	std::lock_guard mutex(s_mutex);
	std::cout << "Subscriber List:" <<
        "-----------------------------------" << std::endl;
    for( auto &l : subscribers) {
		std::cout << "------ Topic : " << l.first <<
			" ------" <<
			std::endl;
		std::cout << "Subscribers List : ( " <<
			l.first << " )" <<
			std::endl;
		for( auto &sl : l.second) {
			sl->print();
		}
    }
	return true;
}

void pktdispatchconfig::cmdLineProcess() {
	while(1) {
		std::this_thread::sleep_for(std::chrono::milliseconds(20000));
		std::cout << __PRETTY_FUNCTION__ <<
			"------------------------------------------" << std::endl;
		prntPublisherList();
		printSubscriberlist();
		std::cout << __PRETTY_FUNCTION__ <<
			"------------------------------------------" << std::endl;
	}
}

bool pktdispatchconfig::start() {
	std::make_unique<std::future<void>*>(new auto(std::async(std::launch::async, &pktdispatchconfig::startPublishMgmt, this))).reset();
	dispatcher->startProcessing();
	auto cliServerThread = std::async(std::launch::async, &pktdispatchconfig::cmdLineProcess, this);

	isStarted = true;
	cliServerThread.wait();
	std::cout << __PRETTY_FUNCTION__ << ":" << "Exit" << std::endl;
	return true;
}

