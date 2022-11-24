#include "hdr.h"
#include "constants.h"

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
		zmqpp::endpoint_t pubRegEndpoint = "tcp://*:4243";
		zmqpp::socket_type pubRegtype = zmqpp::socket_type::reply;
		zmqpp::endpoint_t subRegEndpoint = "tcp://*:4244";
		zmqpp::socket_type subRegtype = zmqpp::socket_type::reply;
		zmqpp::endpoint_t dispatchEndpoint = "tcp://*:4245";
		zmqpp::socket_type dispatchtype = zmqpp::socket_type::publish;
		zmqpp::context context;
	protected:
	public:
		pktdispatch();
		~pktdispatch();
		std::vector<std::string> convertZmqMsgToVector(zmqpp::message &msg);
		zmqpp::message convertVectorToZmqMsg(std::vector<std::string> &msg);
		void printRequest(std::vector<std::string> &msg);
		void printReply(std::vector<std::string> &msg);
		void publishDataPoller();
		void publisherRegisteryPoller();
		void subscriberRegisteryPoller();
		void dispatchEngiene();
		void startProcessing();
};

#define PD_MAX_BUFFER_COUNT				10

#endif //__PKTDISPATCH_H__
