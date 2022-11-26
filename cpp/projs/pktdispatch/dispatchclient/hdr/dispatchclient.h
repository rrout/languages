#inclide "hdr.h"

class dispatchclient {
	private:
		std::string _name;
		std::string _endpoint;
		bool _connected = false;
		std::map<std::string, dispatchclientpub *> publisher;
		std::map<std::string, dispatchclientsub *> subscriber;
	protected:
		bool checkPublisher();
		bool isReady();
		bool isReady(std::string topic);
		void print();
		bool start();
	public:
		dispatchclient() = delete;
		dispatchclient(std::string endpoint);
		dispatchclient(std::string name, std::string endpoint);
		~dispatchclient();

		bool publishRegister(std::string topic);
		bool publish(std::string topic, std::string msg);
		bool publish(std::string topic, std::vector<std::string> msg);

		bool subscribeRegister(std::string topic);
		bool subscribe(std::string topic, std::string callback);

};
