#include "hdr.h"
#include "pktsubscriber.h"

pktsubscriber::pktsubscriber(std::string name, std::string topic) {
	std::cout << __PRETTY_FUNCTION__ << "Constructing" << std::endl;
	if (name.empty()== true || topic.empty()== true) {
        std::cout << __PRETTY_FUNCTION__ << "Invalid param" << std::endl;
        throw std::invalid_argument("Invalid param");
    }
	_topic = topic;
	_name = name;
	_pktSent = 0;
}

pktsubscriber::~pktsubscriber() {
	std::cout << __PRETTY_FUNCTION__ << "Desstructing" << std::endl;
}

std::string pktsubscriber::getTopic() {
	return _topic;
}

std::string pktsubscriber::getName() {
	return _name;
}

void pktsubscriber::pktSent() {
	_pktSent++;
}

int pktsubscriber::getPktSent() {
	return _pktSent;
}

bool pktsubscriber::print() {
	std::cout << "Topic : " << _topic <<
        " | Subscriber Name : " << _name <<
		" | Sent : " << getPktSent() <<
        std::endl;
	return true;
}

