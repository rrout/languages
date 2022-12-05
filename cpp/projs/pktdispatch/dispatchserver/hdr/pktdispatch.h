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
		void dispatchEngiene();
		bool processRqust(pktmessage &req, pktmessage &res);
		void sendAdv(std::string topic, pktmessage &msg);
		void startProcessing();
};


#endif //__PKTDISPATCH_H__
