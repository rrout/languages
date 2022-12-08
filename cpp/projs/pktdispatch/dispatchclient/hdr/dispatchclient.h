#include "hdr.h"
#include "pktmessage.h"
#include "dispatchclientpub.h"
#include "dispatchclientsub.h"

typedef enum {
	DISPATCH_CLIENT_ROLE_PUBLISHER,
	DISPATCH_CLIENT_ROLE_SUBSCRIBER
}client_role_t;

class dispatchclient {
	private:
		std::string _name;
		std::string _version;
		std::string _serverVersion;
		std::string _endpoint;
		zmqpp::context _context;
		zmqpp::socket_type _socktype;
		zmqpp::socket *_connnection =  nullptr;
		int8_t _connRetry = 0;
		bool _connected = false;
		std::string _advendpoint;
		zmqpp::socket *_advcon =  nullptr;
		bool _reqdestroy =  false;
		std::atomic<uint8_t> threadready;
		std::mutex p_lock;
		std::condition_variable p_cond;
		std::mutex s_lock;
		std::map<std::string, dispatchclientpub *> publisher;
		std::map<std::string, dispatchclientsub *> subscriber;
	protected:
		std::string genClientName();
		bool connectNegotiation(int8_t retry);
		bool connectionRetry();
		bool priodicNegotiation();
		bool connectProcessing(std::string endpoint);
		bool sendRequest(pktmessage &req, pktmessage &res);
		bool processAdv(pktmessage &adv);
		std::string getPubEndpoint(std::string topic);
		std::string getSubEndpoint(std::string topic);
		bool registerTopicWithServer(client_role_t role, std::string topic);
		bool isAddEndpointConnected();
		bool isAddChennelReady();
		void advProcessing();
		void connectionCkeckWait(int8_t timeout);
		bool isConnected();
		bool isReady();
		bool isReady(std::string topic);
		bool reqDestroy();
		void publishProcessingNotify();
		bool isPublishProcessingRequired();
		bool start();
	public:
		dispatchclient() = delete;
		dispatchclient(std::string endpoint);
		dispatchclient(std::string name, std::string endpoint);
		~dispatchclient();

		void print();
		bool publishRegister(std::string topic);
		bool publish(std::string topic, std::string msg);
		bool publish(std::string topic, std::vector<std::string> msg);
		void publishProcessing();

		bool subscribeRegister(std::string topic);
		bool subscribe(std::string topic, std::function<void(std::string, std::vector<std::string>)> callback);

};
