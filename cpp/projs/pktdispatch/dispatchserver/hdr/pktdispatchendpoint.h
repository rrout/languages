#include "hdr.h"
#include "pktmessage.h"
#include "constants.h"

#ifndef __PKTDISPATCHNDPOING_H__
#define __PKTDISPATCHNDPOING_H__

class pktdispatchendpoint {
	private:
		std::string myName;
		std::string myAddress;
		std::string myRole;
		std::map<std::string, std::string> myTopics; // TOPIC - Created Time

		std::tuple<std::string, endPoint *, int> mgmtEndpoint;
		std::tuple<std::string, endPoint *, int> advEndpoint;
		std::list<std::tuple<std::string, endPoint *, int>> pubEndpoints;
		std::list<std::tuple<std::string, endPoint *, int>> subEndpoints;

		std::map<std::string, std::tuple<std::string, endPoint *, int>> pubEps;
        std::map<std::string, std::tuple<std::string, endPoint *, int>> subEps;

	protected:
		void cleanup();
	public:
		pktdispatchendpoint();
		pktdispatchendpoint(std::string name, std::string role);
		~pktdispatchendpoint();
		std::string getName();
		std::string getAddress();
		std::string getRole();
		bool addTopic(std::string topic);
		bool delTopic(std::string topic);
		void printTopics();
		std::vector<std::string> getTopics();
		bool registerTopic(std::string topic);
		bool unregisterTopic(std::string topic);
		std::string getPublisherEndpoint(std::string topic);
		std::string getPublisherFQEndpoint(std::string topic);
		endPoint * getPublisherConnection(std::string topic) ;
		std::string getSubscriberEndpoint(std::string topic);
		std::string getSubscriberFQEndpoint(std::string topic);
		endPoint * getSubscriberConnection(std::string topic);
		void printPubEps();
		void printSubEps();

		std::tuple<std::string, endPoint *, int> createEndpoint(std::string endpoint, endPointType_t type);
		bool updateEndpoint(std::tuple<std::string, endPoint *, int> &epDb, std::string endpoint, endPointType_t type);
		bool releaseEndpoint(std::tuple<std::string, endPoint *, int> &epDb, std::string endpoint, endPointType_t type);

		std::tuple<std::string, endPoint *, int> *getEndpoint(std::list<std::tuple<std::string, endPoint *, int>> &list, std::string endpoint);
		bool isEndpointExistInList(std::string endpoint, std::list<std::tuple<std::string, endPoint *, int>> &list);

		bool removeEndpoint(std::list<std::tuple<std::string, endPoint *, int>> &list, std::string endpoint);
		std::tuple<std::string, endPoint *, int> *getHealthyEndpoint(std::list<std::tuple<std::string, endPoint *, int>> &list);


		bool addPubEndpoint(std::string endpoint);
		endPoint * getPubEndpointConnection(std::string endpoint);
		bool addSubEndpoint(std::string endpoint);

		bool addMgmtEndpoint(std::string endpoint);
		endPoint *getMgmtConnection();
		std::string getMgmtFQEndpoint();
		bool addAdvEndpoint(std::string endpoint);
		endPoint *getAdvConnection();
		std::string getAdvFQEndpoint();
		void printPubEndpoint();
		void printSubEndpoint();
		void print();
};

#endif //__PKTDISPATCHNDPOING_H__
