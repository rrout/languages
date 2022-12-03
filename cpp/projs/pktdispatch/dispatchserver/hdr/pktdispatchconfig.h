#include "hdr.h"
#include "pktdispatch.h"
#include "pktpublisher.h"
#include "pktsubscriber.h"
#include "pktdispatchendpoint.h"

#ifndef __PKTDISPATCHCONFIG_H__
#define __PKTDISPATCHCONFIG_H__
class pktdispatchconfig {
	private:
			bool isStarted = false;
			static pktdispatchconfig *instance;
			pktdispatch *dispatcher = nullptr;
			//pktdispatchendpoint *endpoints = nullptr;
	protected:
		// The Singleton's constructor should always be private to prevent
		// direct construction calls with the `new` operator.
		pktdispatchconfig();
	public:
		// Singletons should not be cloneable.
		pktdispatchconfig(pktdispatchconfig &other) = delete;
		// Singletons should not be assignable.
		void operator=(const pktdispatchconfig &) = delete;
		~pktdispatchconfig();
		static pktdispatchconfig * createInstance();
		static pktdispatchconfig * getInstance();
		void parse(std::string &configfile);
		void configure(std::string &config);

		pktdispatchendpoint endpoints;

		// Topic - Publisher map
		std::mutex p_mutex;
		std::condition_variable p_cond;
		std::map<std::string, pktpublisher *> publisher;
		pktConfigStatus_t cratePublisher(std::string topic, std::string name);
		pktConfigStatus_t deletePublisher(std::string topic, std::string name);
		void setPublisher(std::string topic, std::string name);
		void removePublisher(std::string topic, std::string name);
		pktpublisher * getPublisher(std::string topic);
		bool prntPublisherList();
		bool isPublishPending();
		void startPublishMgmt();
		void notifyPublishMgmt();

		// Topic - Subscriber map
		std::mutex s_mutex;
		std::condition_variable s_cond;
		std::map<std::string, std::list<pktsubscriber *>> subscribers;
		pktConfigStatus_t addSubscriber(std::string subscriberName, std::string topic);
		pktConfigStatus_t delSubscriber(std::string subscriberName, std::string topic);
		bool createOrAddSubscriber(std::string subscriberName, std::string topic);
		bool removeSubscriber(std::string subscriberName, std::string topic);
		bool isSubscriberPresent(std::string topic);
		bool isSubscriberPresent(std::string subscriberName, std::string topic);
		pktsubscriber *getSubscriber(std::string subscriberName, std::string topic);
		void updataSubscriberStatistics(std::string topic);
		bool printSubscriberlist();

		void cmdLineProcess();
		bool start();
};

#endif //__PKTDISPATCHCONFIG_H__
