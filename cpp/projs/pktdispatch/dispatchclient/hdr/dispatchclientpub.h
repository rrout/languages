#include "hdr.h"

class dispatchclientpub {
	private:
		std::string _topic;
		int _publishCount = 0;
		int _buffCount;
		int _maxBuffCount;
		int _maxBatchSendCount = 0;
		bool _registered = false;
		std::mutex _buffLock;
		std::vector<std::vector<std::string>> _buffer;
	protected:
	public:
		dispatchclientpub() = delete;
		dispatchclientpub(std::string topic);
		~dispatchclientpub();
		void print();
		int getBuffCount();
		int publishCount() const;
		bool registered() const;
		void registration(bool status);
		bool publish(std::string topic, std::string msg);
		bool publish(std::string topic, std::vector<std::string> msg);
		bool send(std::vector<std::string> &msg);
		bool processSend();
};
