#include "hdr.h"
#ifndef __DISPATCHCLIENTSUB_H__
#define __DISPATCHCLIENTSUB_H__
typedef std:: function<void(std::string, std::vector<std::string>)> Callback;
class dispatchclientsub {
	private:
		std::string _topic;
		bool _registered = false;
		int _recvCount = 0;
		std::mutex _lock;
		std::vector<std::vector<std::string>> _buffer;
        std::condition_variable s_cond;
		int _buffCount = 0;
		Callback  _callback;
	protected:
	public:
		dispatchclientsub() = delete;
		dispatchclientsub(std::string topic);
		~dispatchclientsub();
		void print();
		int getBuffCount() const;
		bool registered() const;
		void registration(bool status);
		int getRecvCount() const;
		bool subscribe(Callback callback);
		void processRecieve();
		void processCallbacks();
		void processCallbacksNotify();
		bool dispatchCallback(std::vector<std::string> msg);
		void start();
};
#endif //__DISPATCHCLIENTSUB_H__
