#include "hdr.h"
#include "constants.h"
#include "pktmessage.h"

#ifndef __PKTDISPATCH_H__
#define __PKTDISPATCH_H__
class pktdispatch {
	private:
	protected:
	public:
		pktdispatch();
		~pktdispatch();
		void publishDataPoller();
		void dispatchMgmtPoller();
		void infoPublishPoller();
		void maintainanceEngiene();
		bool processRqust(pktmessage &req, pktmessage &res);
		void sendAdv(std::string clientOrTopic, pktmessage &msg);
		void sendAdv(std::string clientOrTopic, std::vector<std::string> &msg);
		void sendPeriodicAdv(std::vector<std::string> &msg);
		void startProcessing();
};


#endif //__PKTDISPATCH_H__
