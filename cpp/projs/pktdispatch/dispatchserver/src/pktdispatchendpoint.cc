#include "hdr.h"
#include "pktdispatchendpoint.h"

endPoint::endPoint(std::string endpoint, endPointType_t type) {
	std::cout << __PRETTY_FUNCTION__ << "Constructing object" << std::endl;
	_endpointType = type;
	_socktype = getsockettype(type);
    _connection = new zmqpp::socket(_context, _socktype);
	if (isSocketCatagoryServer()) {
		_connection->bind(endpoint);
	} else {
		_connection->connect(endpoint);
	}
}

endPoint::~endPoint() {
	std::cout << __PRETTY_FUNCTION__ << "Destroying object" << std::endl;
	if (_connection != nullptr) {
		delete _connection;
	}
}
zmqpp::socket_type endPoint::getsockettype(endPointType_t type) {
	switch (type) {
		case PUSH:
		case SERVER_PUSH:
			{
				return zmqpp::socket_type::push;
			}
			break;
		case PULL:
		case SERVER_PULL:
			{
				return zmqpp::socket_type::pull;
			}
			break;
		case REQ:
		case SERVER_REQ:
			{
                return zmqpp::socket_type::request;
            }
            break;
		case RESP:
		case SERVER_RESP:
			{
                return zmqpp::socket_type::reply;
            }
            break;
		case PUBLISH:
		case SERVER_PUBLISH:
			{
                return zmqpp::socket_type::publish;
            }
            break;
		case SUBSCRIBE:
		case SERVER_SUBSCRIBE:
		{
			return zmqpp::socket_type::subscribe;
		}
		break;
	}
}
bool endPoint::isSocketCatagoryServer() {
	switch (_endpointType) {
		case PUSH:
		case PULL:
		case REQ:
		case RESP:
		case PUBLISH:
		case SUBSCRIBE:
		{
			return false;
		}
		break;
		case SERVER_PUSH:
		case SERVER_PULL:
		case SERVER_REQ:
		case SERVER_RESP:
		case SERVER_PUBLISH:
		case SERVER_SUBSCRIBE:
		{
			return true;
		}
		break;
	}
	return true;
}
bool endPoint::send(std::vector<std::string> &msg) {
	pktmessage message(msg);
	return send(message);
}
bool endPoint::send(pktmessage &msg) {
	zmqpp::message message;
	msg.compose(message);
	_connection->send(message);
	return true;
}
bool endPoint::recieve(std::vector<std::string> &msg) {
	pktmessage message;
	recieve(message);
	//TODO
	return true;
}
bool endPoint::recieve(pktmessage &msg) {
	zmqpp::message message;
	_connection->receive(message);
	msg.copy(message);
    return true;
}
bool endPoint::subscribe(std::string topic) {
	_connection->subscribe(topic);
	return true;
}

pktdispatchendpoint * pktdispatchendpoint::instance = nullptr;
pktdispatchendpoint::pktdispatchendpoint() {
	std::cout << __PRETTY_FUNCTION__ << ":" << "Constructing Object" << std::endl;
}
pktdispatchendpoint::~pktdispatchendpoint() {
std::cout << __PRETTY_FUNCTION__ << "Destructing object" << std::endl;
}
pktdispatchendpoint * pktdispatchendpoint::getInstance() {
	if (!instance) {
        instance = new pktdispatchendpoint();
        std::cout << __PRETTY_FUNCTION__ << ":" << "Creating instance" << std::endl;
        return instance;
    }
    return instance;
}

void pktdispatchendpoint::configure(std::string &config) {
	std::cout << __PRETTY_FUNCTION__ << "Configuring object" << std::endl;
}

bool pktdispatchendpoint::setEp(std::pair <std::string, endPoint *> &ep, std::string endpoint, endPointType_t type) {
	if (endpoint.empty() || !ep.first.empty()) {
		std::cout << __PRETTY_FUNCTION__ << "Invalid param" << std::endl;

		return false;
	}
	ep.first = endpoint;
	ep.second = new endPoint(endpoint, type);
	return true;
}


bool pktdispatchendpoint::updateEp(std::pair <std::string, endPoint *> &ep, std::string endpoint, endPointType_t type) {
	if (endpoint.empty() || ep.first.empty()) {
		std::cout << __PRETTY_FUNCTION__ << "Invalid param" << std::endl;
		return false;
	}
	std::string oldEp= ep.first;
	endPoint *oldObj =  ep.second;
	if (oldEp == endpoint) {
		std::cout << __PRETTY_FUNCTION__ << "Same as old" << std::endl;
		return true;
	} else {
		ep.first = endpoint;
		ep.second = new endPoint(endpoint, type);

		delete oldObj;
	}
	return true;
}

std::string pktdispatchendpoint::getEp(std::pair <std::string, endPoint *> &ep) {
	if (!ep.first.empty()) {
		return ep.first;
	}
	return {};
}

endPoint *pktdispatchendpoint::getConnection(std::pair <std::string, endPoint *> &ep) {
	if (!ep.first.empty()) {
		return ep.second;
	}
	return nullptr;
}

bool pktdispatchendpoint::setEp(std::map<std::string, std::pair <std::string, endPoint *>> &ep, std::string topic, std::string endpoint , endPointType_t type) {
	if (topic.empty() || endpoint.empty()) {
        std::cout << __PRETTY_FUNCTION__ << "Invalid param" << std::endl;
        return false;
    }
	if (!ep.contains(topic)) {
		ep[topic].first = endpoint;
		ep[topic].second = new endPoint(endpoint, type);
		return true;
	} else {
		std::cout << __PRETTY_FUNCTION__ << "EP already exist in the map . Use update insteed....." << std::endl;
		return false;
	}
	return true;
}

bool pktdispatchendpoint::updateEp(std::map<std::string, std::pair <std::string, endPoint *>> &ep, std::string topic, std::string endpoint, endPointType_t type) {
	if (topic.empty() || endpoint.empty()) {
		std::cout << __PRETTY_FUNCTION__ << "Invalid param" << std::endl;
		return false;
	}
	if (ep.contains(topic)) { 
		if (ep[topic].first == endpoint) {
			std::cout << __PRETTY_FUNCTION__ << "Same as old" << std::endl;
			return true;
		}
		std::string oldEp = ep[topic].first;
		endPoint *old = ep[topic].second;
		ep[topic].first = endpoint;
		ep[topic].second = new endPoint(endpoint, type);

		delete old;
		return true;
	} else {
		return setEp(ep, topic, endpoint, type);
	}
	return false;
}

std::string pktdispatchendpoint::getEp(std::map<std::string, std::pair <std::string, endPoint *>> &ep, std::string topic) {
	if (topic.empty()) {
		std::cout << __PRETTY_FUNCTION__ << "Invalid param" << std::endl;
		return {};
	}
	if (ep.contains(topic)) {
		return ep[topic].first;
	}
	return {};
}

endPoint *pktdispatchendpoint::getConnection(std::map<std::string, std::pair <std::string, endPoint *>> &ep, std::string topic) {
	if (topic.empty()) {
		std::cout << __PRETTY_FUNCTION__ << "Invalid param" << std::endl;
		return nullptr;
	}
	if (ep.contains(topic)) {
		return ep[topic].second;
	}
	return nullptr;
}


bool pktdispatchendpoint::setMgmtEp(std::string endpoint, endPointType_t type) {
	return setEp(mgmtEp, endpoint, type);
}
bool pktdispatchendpoint::updateMgmtEp(std::string endpoint, endPointType_t type) {
	return updateEp(mgmtEp, endpoint, type);
}
std::string pktdispatchendpoint::getMgmtEp() {
	return getEp(mgmtEp);
}
endPoint *pktdispatchendpoint::getMgmtConnection() {
	return getConnection(mgmtEp);
}

bool pktdispatchendpoint::setAdvEp(std::string endpoint, endPointType_t type) {
	return setEp(advEp, endpoint, type);
}
bool pktdispatchendpoint::updateAdvEp(std::string endpoint, endPointType_t type) {
	return updateEp(advEp, endpoint, type);
}
std::string pktdispatchendpoint::getAdvEp() {
	return getEp(advEp);
}
endPoint *pktdispatchendpoint::getAdvConnection() {
	return getConnection(advEp);
}


bool pktdispatchendpoint::setPubEp(std::string topic, std::string endpoint, endPointType_t type) {
	return setEp(pubEps, topic, endpoint, type);
}
bool pktdispatchendpoint::updatePubEp(std::string topic, std::string endpoint, endPointType_t type) {
	return updateEp(pubEps, topic, endpoint, type);
}
std::string pktdispatchendpoint::getPubEp(std::string topic) {
	return getEp(pubEps, topic);
}
endPoint *pktdispatchendpoint::getPubConnection(std::string topic) {
	return getConnection(pubEps, topic);
}


bool pktdispatchendpoint::setSubEp(std::string topic, std::string endpoint, endPointType_t type) {
	return setEp(subEps, topic, endpoint, type);
}
bool pktdispatchendpoint::updateSubEp(std::string topic, std::string endpoint, endPointType_t type) {
	return updateEp(subEps, topic, endpoint, type);
}
std::string pktdispatchendpoint::getSubEp(std::string topic) {
	return getEp(subEps, topic);
}
endPoint *pktdispatchendpoint::getSubConnection(std::string topic) {
	return getConnection(subEps, topic);
}
