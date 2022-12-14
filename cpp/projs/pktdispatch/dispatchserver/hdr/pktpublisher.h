#include "hdr.h"
#include "pktdispatchendpoint.h"

#ifndef __PKTPUBLISHER_H__
#define __PKTPUBLISHER_H__
class pktpublisher {
	private:
		std::mutex lock;
		std::string _topic;
		int _maxBuffer = 0;
		int _publishCount = 0;
		int _publishError = 0;
		int _publishSendCount = 0;
		int _publishSendError = 0;
		std::list<std::vector<std::string>> _infos;
	protected:
	public:
		pktpublisher() = delete;
		pktpublisher(std::string topic);
		~pktpublisher();
		std::list<std::pair<std::string, std::string>> pubEntryList;
		void addEntry(std::string name);
		void delEntry(std::string name);
		bool entryExist(std::string name);
		std::pair<std::string, std::string> &getPublisherEntry(std::string name);
		void printEntries();
		int piblishorCount;
		std::string getTopic();
		void publishCount();
		int getPublishCount();
		void publishSendCount();
		int getPublishSendCount();
		void publishSendError();
		int getPublishSendError();
		int errcount();
		int currentBuffCount();
		bool publish(std::vector<std::string> &buffer);
		bool print();
		bool isBuffPresent();
		bool printBuf();
		bool publishBuf(std::vector<std::string> buffer);
		bool processBufferBatch();
		bool processBuffer();
};


#endif //__PKTPUBLISHER_H__
