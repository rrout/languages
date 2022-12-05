#include "hdr.h"
#include "constants.h"
#include "endpoint.h"
#include "pktdispatchendpoint.h"

pktdispatchendpoint::pktdispatchendpoint() {
	std::cout << __PRETTY_FUNCTION__ << ":" << "Constructing Object" << std::endl;
	myName = "SERVER";
	myAddress = getHostName();
}
pktdispatchendpoint::pktdispatchendpoint(std::string name, std::string role) {
    std::cout << __PRETTY_FUNCTION__ << ":" << "Constructing Object" << std::endl;
    myName = name;
    myAddress = getHostName();
	myRole = role;
}
pktdispatchendpoint::~pktdispatchendpoint() {
std::cout << __PRETTY_FUNCTION__ << "Destructing object" << std::endl;
	cleanup();
}
std::string pktdispatchendpoint::getName() {
	return myName;
}
std::string pktdispatchendpoint::getAddress() {
	return myAddress;
}
std::string pktdispatchendpoint::getRole() {
	return myRole;
}
bool pktdispatchendpoint::registerTopic(std::string topic) {
	if (!pubEps.contains(topic)) {
		std::tuple<std::string, endPoint *, int> *ep = getHealthyEndpoint(pubEndpoints);
		if (ep) {
			std::get<2>(*ep) +=1;
			pubEps[topic] = *ep;
		} else {
			std::cout << __PRETTY_FUNCTION__ << "NULL PTR Returned" << std::endl;
			return false;
		}
	} else {
		std::cout << __PRETTY_FUNCTION__ << "Topic : " << topic << " already exist" << std::endl;
	}
	if (!subEps.contains(topic)) {
		std::tuple<std::string, endPoint *, int> *ep = getHealthyEndpoint(subEndpoints);
		if (ep) {
			std::get<2>(*ep) +=1;
			subEps[topic] = *ep;
		} else {
			std::cout << __PRETTY_FUNCTION__ << "NULL PTR Returned" << std::endl;
			return false;
		}
	} else {
		std::cout << __PRETTY_FUNCTION__ << "Topic : " << topic << " already exist" << std::endl;
	}
	return true;
}
bool pktdispatchendpoint::unregisterTopic(std::string topic) {
	if (!pubEps.contains(topic)) {
		std::string endpoint = std::get<0>(pubEps[topic]);
		auto ep = getEndpoint(pubEndpoints, endpoint);
		if (ep) {
			std::get<2>(*ep) -=1;
			pubEps.erase(topic);
		} else {
			std::cout << __PRETTY_FUNCTION__ << "NULL PTR Returned" << std::endl;
			return false;
		}
	} else {
		std::cout << __PRETTY_FUNCTION__ << "Topic : " << topic << " noot available" << std::endl;
	}
	if (!subEps.contains(topic)) {
		std::string endpoint = std::get<0>(subEps[topic]);
		auto ep = getEndpoint(subEndpoints, endpoint);
		if (ep) {
			std::get<2>(*ep) -=1;
			pubEps.erase(topic);
		} else {
			std::cout << __PRETTY_FUNCTION__ << "NULL PTR Returned" << std::endl;
			return false;
		}
	} else {
		std::cout << __PRETTY_FUNCTION__ << "Topic : " << topic << " not available" << std::endl;
	}
	return true;
}
std::string pktdispatchendpoint::getPublisherEndpoint(std::string topic) {
	std::string endpoint;
	if (pubEps.contains(topic))
	{
		endpoint = std::get<0>(pubEps[topic]);
	}
	if (!endpoint.empty())
	{
		return endpoint;
	}
	return {};
}
std::string pktdispatchendpoint::getPublisherFQEndpoint(std::string topic) {
	std::string endpoint = getPublisherEndpoint(topic);
	std::string fqep;
	std::vector<std::string> ss = spilt(endpoint, ':');
	ss[1] = myAddress;
	for (auto &s : ss) {
		fqep += s;
	}
	return fqep;
}

endPoint * pktdispatchendpoint::getPublisherConnection(std::string topic) {
	endPoint *con = nullptr;
	if (pubEps.contains(topic))
	{
		con = std::get<1>(pubEps[topic]);
		if (!con) {
			std::cout << __PRETTY_FUNCTION__ << "NULL PTR Returned" << std::endl;
		}
	}
	return con;
}
std::string pktdispatchendpoint::getSubscriberEndpoint(std::string topic) {
	std::string endpoint;
	if (subEps.contains(topic))
	{
		endpoint = std::get<0>(subEps[topic]);
	}
	if (!endpoint.empty())
	{
		return endpoint;
	}
	return {};
}
std::string pktdispatchendpoint::getSubscriberFQEndpoint(std::string topic) {
	std::string endpoint = getSubscriberEndpoint(topic);
	std::string fqep;
	std::vector<std::string> ss = spilt(endpoint, ':');
	ss[1] = myAddress;
	for (auto &s : ss) {
		fqep += s;
	}
	return fqep;
}
endPoint * pktdispatchendpoint::getSubscriberConnection(std::string topic) {
	endPoint *con = nullptr;
	if (subEps.contains(topic))
	{
		con = std::get<1>(subEps[topic]);
		if (!con) {
			std::cout << __PRETTY_FUNCTION__ << "NULL PTR Returned" << std::endl;
		}
	}
	return con;
}
void pktdispatchendpoint::printPubEps() {
	std::cout << "Topic to Publisher Endpoints" << std::endl;
	std::cout << "---------------------------------" << std::endl;
	for (auto &pubep : pubEps) {
		std::cout << "TOPIC : " << pubep.first <<
			" PEP: " << std::get<0>(pubep.second) <<
			" CON: " << std::get<1>(pubep.second) <<
			std::endl;
	}
}
void pktdispatchendpoint::printSubEps() {
	std::cout << "Topic to Subscriber Endpoints" << std::endl;
	std::cout << "---------------------------------" << std::endl;
	for (auto &subep : subEps) {
		std::cout << "TOPIC : " << subep.first <<
			" SEP: " << std::get<0>(subep.second) <<
			" CON: " << std::get<1>(subep.second) <<
			std::endl;
	}
}

std::tuple<std::string, endPoint *, int> pktdispatchendpoint::createEndpoint(std::string endpoint, endPointType_t type) {
	if (endpoint.empty() == true) {
		std::cout << "Invalid param" << std::endl;
		std::abort();
		return {};
	}
	std::tuple<std::string, endPoint *, int> config;
	std::get<0>(config) = endpoint;
    std::get<1>(config) = new endPoint(endpoint, type);
    std::get<2>(config) = 0;
	return config;
}
bool pktdispatchendpoint::updateEndpoint(std::tuple<std::string, endPoint *, int> &epDb, std::string endpoint, endPointType_t type) {
}
bool pktdispatchendpoint::releaseEndpoint(std::tuple<std::string, endPoint *, int> &epDb, std::string endpoint, endPointType_t type) {
}
std::tuple<std::string, endPoint *, int> *pktdispatchendpoint::getEndpoint(std::list<std::tuple<std::string, endPoint *, int>> &list, std::string endpoint) {
	if (endpoint.empty() == true) {
		std::cout << "Invalid param" << std::endl;
		std::abort();
		return nullptr;
	}
	for (auto &l : list) {
		if (std::get<0>(l) == endpoint) {
			return &l;
		}
	}
	return nullptr;
}
bool pktdispatchendpoint::isEndpointExistInList(std::string endpoint, std::list<std::tuple<std::string, endPoint *, int>> &list) {
	if (endpoint.empty() == true) {
		std::cout << "Invalid param" << std::endl;
		std::abort();
		return false;
	}
	for (auto &l : list) {
		if (endpoint == std::get<0>(l)) {
			return true;
		}
	}
	return false;
}
bool pktdispatchendpoint::removeEndpoint(std::list<std::tuple<std::string, endPoint *, int>> &list, std::string endpoint) {
}

std::tuple<std::string, endPoint *, int> *pktdispatchendpoint::getHealthyEndpoint(std::list<std::tuple<std::string, endPoint *, int>> &list) {
	int item = 10000, count = 0;
	int score = 10000;
	for (auto &l : list) {
		if (std::get<2>(l) < score) {
			item = count;
			score = std::get<2>(l);
		}
		count++;
	}
	count = 0;
	for (auto &l : list) {
		if (item ==  count) {
			return &l;
		}
		count++;
	}
	return nullptr;
}
bool pktdispatchendpoint::addPubEndpoint(std::string endpoint) {
	if (endpoint.empty()) {
		std::cout << "Invalid param" << std::endl;
		std::abort();
		return false;
	}
	if (!isEndpointExistInList(endpoint, pubEndpoints)) {
		pubEndpoints.push_back(createEndpoint(endpoint, SERVER_PULL));
	} else {
		return false;
	}
	return true;
}
endPoint * pktdispatchendpoint::getPubEndpointConnection(std::string endpoint) {
	if (endpoint.empty()) {
		std::cout << "Invalid param" << std::endl;
		std::abort();
		return nullptr;
	}
	if (isEndpointExistInList(endpoint, pubEndpoints)) {
		auto ep = getEndpoint(pubEndpoints, endpoint);
		if (ep) {
			return std::get<1>(*ep);
		} else {
			std::cout << __PRETTY_FUNCTION__ << "NULL PTR Returned" << std::endl;
		}
	}
	print();
	return nullptr;
}
bool pktdispatchendpoint::addSubEndpoint(std::string endpoint) {
	if (endpoint.empty()) {
		std::cout << "Invalid param" << std::endl;
		std::abort();
        return false;
    }
	if (!isEndpointExistInList(endpoint, subEndpoints)) {
        subEndpoints.push_back(createEndpoint(endpoint, SERVER_PUBLISH));
    } else {
        return false;
    }
    return true;
}
bool pktdispatchendpoint::addMgmtEndpoint(std::string endpoint) {
    if (endpoint.empty()) {
		std::cout << "Invalid param" << std::endl;
		std::abort();
        return false;
    }
	mgmtEndpoint = createEndpoint(endpoint, SERVER_RESP);
	return true;
}
endPoint *pktdispatchendpoint::getMgmtConnection() {
	if (std::get<1>(mgmtEndpoint) != nullptr) {
		return std::get<1>(mgmtEndpoint);
	}
	return nullptr;
}
bool pktdispatchendpoint::addAdvEndpoint(std::string endpoint) {
    if (endpoint.empty()) {
		std::cout << "Invalid param" << std::endl;
		std::abort();
        return false;
    }
	advEndpoint = createEndpoint(endpoint, SERVER_PUBLISH);
	return true;
}
endPoint *pktdispatchendpoint::getAdvConnection() {
	if (std::get<1>(advEndpoint) != nullptr) {
		return std::get<1>(advEndpoint);
	}
	return nullptr;
}
void pktdispatchendpoint::cleanup() {
	std::cout << "Cleaning up MEP : " << std::get<0>(mgmtEndpoint) << "CON: " << std::get<1>(mgmtEndpoint) << std::endl;
	delete std::get<1>(mgmtEndpoint);
	std::cout << "Cleaning up AEP : " << std::get<0>(advEndpoint) << "CON: " << std::get<1>(advEndpoint) << std::endl;
	delete  std::get<1>(advEndpoint);
	for (auto &pubep : pubEndpoints) {
		std::cout << "Clean up Publisher EP : " << std::get<0>(pubep) << " CON: " << std::get<1>(pubep) << std::endl;
		delete std::get<1>(pubep);
	}
	for (auto &subep : subEndpoints) {
		std::cout << "Clean up Subscriber EP : " << std::get<0>(subep) << " CON: " << std::get<1>(subep) << std::endl;
		delete std::get<1>(subep);
	}

}
void pktdispatchendpoint::printPubEndpoint() {
	std::cout << "Publisher Endpoints" << std::endl;
	std::cout << "---------------------------------" << std::endl;
	for (auto &pubep : pubEndpoints) {
		std::cout << "EP : " << std::get<0>(pubep) <<
			" CON: " << std::get<1>(pubep) <<
			" UC : " << std::get<2>(pubep) <<
			std::endl;
	}
}
void pktdispatchendpoint::printSubEndpoint() {
	std::cout << "Subscriber Endpoints" << std::endl;
	std::cout << "---------------------------------" << std::endl;
	for (auto &subep : subEndpoints) {
		std::cout << "EP : " << std::get<0>(subep) <<
			" CON: " << std::get<1>(subep) <<
			" UC : " << std::get<2>(subep) <<
			std::endl;
	}
}
void pktdispatchendpoint::print() {
	std::cout << "MEP : " << std::get<0>(mgmtEndpoint) << "CON: " << std::get<1>(mgmtEndpoint) << std::endl;
	std::cout << "AEP : " << std::get<0>(advEndpoint) << "CON: " << std::get<1>(advEndpoint) << std::endl;
	printPubEndpoint();
	printSubEndpoint();
	printPubEps();
	printSubEps();
}
