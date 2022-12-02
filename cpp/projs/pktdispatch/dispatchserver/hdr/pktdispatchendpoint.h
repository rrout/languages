#include "hdr.h"
#include "pktmessage.h"





#ifndef __PKTDISPATCHNDPOING_H__
#define __PKTDISPATCHNDPOING_H__




typedef enum {
	PUSH,
	SERVER_PUSH,
	PULL,
	SERVER_PULL,
	REQ,
	SERVER_REQ,
	RESP,
	SERVER_RESP,
	PUBLISH,
	SERVER_PUBLISH,
	SUBSCRIBE,
	SERVER_SUBSCRIBE
} endPointType_t;

class endPoint {
	private:
		std::string _endpoint;
		endPointType_t _endpointType;
		zmqpp::context _context;
		zmqpp::socket *_connection;
		zmqpp::socket_type _socktype;
	protected:
	public:
		endPoint() = delete;
		endPoint(std::string endpoint, endPointType_t type);
		~endPoint();
		zmqpp::socket_type getsockettype(endPointType_t type);
		bool isSocketCatagoryServer();
		bool send(std::vector<std::string> &msg);
		bool send(pktmessage &msg);
		bool recieve(std::vector<std::string> &msg);
		bool recieve(pktmessage &msg);
		bool subscribe(std::string topic);
};
class pktdispatchendpoint {
	private:
		bool isStarted = false;
		static pktdispatchendpoint *instance;
	protected:
		std::pair <std::string, endPoint *> mgmtEp;

		std::pair <std::string, endPoint *> advEp;

		std::map<std::string, std::pair <std::string, endPoint *>> pubEps;

		std::map<std::string, std::pair <std::string, endPoint *>> subEps;
	public:
		pktdispatchendpoint();
		pktdispatchendpoint(pktdispatchendpoint &other) = delete;
		void operator=(const pktdispatchendpoint &) = delete;
		~pktdispatchendpoint();

		static pktdispatchendpoint * getInstance();
		void configure(std::string &config);

		bool setEp(std::pair <std::string, endPoint *> &ep, std::string endpoint, endPointType_t type);
		bool updateEp(std::pair <std::string, endPoint *> &ep, std::string endpoint, endPointType_t type);
		std::string getEp(std::pair <std::string, endPoint *> &ep);
		endPoint *getConnection(std::pair <std::string, endPoint *> &ep);

		bool setEp(std::map<std::string, std::pair <std::string, endPoint *>> &ep, std::string topic, std::string endpoint, endPointType_t type);
		bool updateEp(std::map<std::string, std::pair <std::string, endPoint *>> &ep, std::string topic, std::string endpoint, endPointType_t type);
		std::string getEp(std::map<std::string, std::pair <std::string, endPoint *>> &ep, std::string topic);
		endPoint *getConnection(std::map<std::string, std::pair <std::string, endPoint *>> &ep, std::string topic);


		bool setMgmtEp(std::string endpoint, endPointType_t type);
		bool updateMgmtEp(std::string endpoint, endPointType_t type);
		std::string getMgmtEp();
		endPoint *getMgmtConnection();

		bool setAdvEp(std::string endpoint, endPointType_t type);
		bool updateAdvEp(std::string endpoint, endPointType_t type);
		std::string getAdvEp();
		endPoint *getAdvConnection();


		bool setPubEp(std::string topic, std::string endpoint, endPointType_t type);
		bool updatePubEp(std::string topic, std::string endpoint, endPointType_t type);
		std::string getPubEp(std::string topic);
		endPoint *getPubConnection(std::string topic);

		bool setSubEp(std::string topic, std::string endpoint, endPointType_t type);
		bool updateSubEp(std::string topic, std::string endpoint, endPointType_t type);
		std::string getSubEp(std::string topic);
		endPoint *getSubConnection(std::string topic);

};

#endif //__PKTDISPATCHNDPOING_H__
