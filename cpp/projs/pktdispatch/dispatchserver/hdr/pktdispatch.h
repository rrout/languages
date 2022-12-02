#include "hdr.h"
#include "pktmessage.h"

#ifndef __PKTDISPATCH_H__
#define __PKTDISPATCH_H__
typedef enum {
	STATUS_OK,
	STATUS_FAIL,
	STATUS_EEXIST,
	STATUS_INTERNAL,
	STATUS_MAX
}pktConfigStatus_t;

class pktdispatch {
	private:
		zmqpp::endpoint_t pubDataPollerEndpoint = "tcp://*:4242";
		zmqpp::socket_type pubDataPollertype = zmqpp::socket_type::pull;
		zmqpp::endpoint_t mgmtendpoint = "tcp://*:4243";
		zmqpp::socket_type mgmtendpointType = zmqpp::socket_type::reply;
		zmqpp::endpoint_t infoPubEndpoint = "tcp://*:4244";
		zmqpp::socket_type infoPubEndpointType = zmqpp::socket_type::publish;
		zmqpp::endpoint_t dispatchEndpoint = "tcp://*:4245";
		zmqpp::socket_type dispatchtype = zmqpp::socket_type::publish;
		zmqpp::context context;
	protected:
	public:
		pktdispatch();
		~pktdispatch();
		void publishDataPoller();
		void dispatchMgmtPoller();
		void infoPublishPoller();
		void dispatchEngiene();
		bool processRqust(pktmessage &req, pktmessage &res);
		void startProcessing();
};

#define PD_MAX_BUFFER_COUNT				10

#endif //__PKTDISPATCH_H__
