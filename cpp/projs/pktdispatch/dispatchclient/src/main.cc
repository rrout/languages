#include "hdr.h"
#include "dispatchclient.h"
#include "dispatchclientsub.h"
#include "logger.h"

LOGGING_SYSTEM_INIT;

	void cbk(std::string topic, std::vector<std::string> msg) {
		std::cout << "Callback executed" << std::endl;
		std::cout << "Topic : " << topic << std::endl;
		std::cout << "Data :   ";
		for (auto &dat : msg) {
			std::cout << dat << std::endl;
		}
	}
Callback c = std::bind(cbk, _1, _2);
int main() {
	LOGGING_SYSTEM_FILE("dispatchclient.log");
	gLog.consolelog = false;
	std::vector<std::string> msge = {"aaaa"};
	//dispatchclient client("CLIENT:1", "tcp://localhost:4234");
	dispatchclient client("CLIENT:1", "tcp://localhost:4243");
	client.publishRegister("TOPIC-AAAAA");
	client.print();
	client.publish("TOPIC-BBBBBBBBBB", "Message");
	client.publish("TOPIC-AAAAA", "Message");
	client.print();
	client.subscribeRegister("TOPIC-AAAAA");
	client.subscribe("TOPIC-AAAAA", c);
	//std::this_thread::sleep_for(std::chrono::milliseconds(20000));
	//client.print();

	//dispatchclient cl("tcp://localhost:4243");
	//cl.print();
	while(1) {
		client.publish("TOPIC-AAAAA", "Message");
		client.publish("TOPIC-AAAAA", "Message .....................................................................................................................................................................................................................................................................................................................................................................................................................................");
		//std::this_thread::sleep_for(std::chrono::seconds(20000));
		std::this_thread::sleep_for(std::chrono::milliseconds(100000));
		std::cout << "--------------" << std::endl;
		client.print();
	}
}

