#include "hdr.h"
#include "constants.h"
#include "endpoint.h"

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






