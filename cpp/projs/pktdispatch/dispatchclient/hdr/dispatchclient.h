#include "hdr.h"
#include "dispatchclientpub.h"
#include "dispatchclientsub.h"

class dispatchclient {
	private:
		std::string _name;
		std::string _endpoint;
		bool _connected = false;
		bool _reqdestroy =  false;
		std::atomic<uint8_t> threadready;
		std::mutex p_lock;
		std::condition_variable p_cond;
		std::mutex s_lock;
		std::map<std::string, dispatchclientpub *> publisher;
		std::map<std::string, dispatchclientsub *> subscriber;
	protected:
		std::string genClientName();
		bool connectProcessing(std::string endpoint);
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
