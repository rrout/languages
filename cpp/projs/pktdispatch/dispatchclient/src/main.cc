#include "hdr.h"
#include "dispatchclient.h"
#include "dispatchclientsub.h"

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
	std::vector<std::string> msge = {"aaaa"};
	//dispatchclient client("CLIENT:1", "tcp://localhost:4234");
	dispatchclient client("CLIENT:1", "tcp://localhost:4243");
	client.publishRegister("AAAAA");
	client.print();
	client.publish("BBBBBBBBBB", "Message");
	client.publish("AAAAA", "Message");
	client.print();
	client.subscribeRegister("AAAAA");
	client.subscribe("AAAAA", c);
	//std::this_thread::sleep_for(std::chrono::milliseconds(20000));
	//client.print();

	//dispatchclient cl("tcp://localhost:4243");
	//cl.print();
	while(1) {
		client.publish("AAAAA", "Message");
		client.publish("AAAAA", "Message .....................................................................................................................................................................................................................................................................................................................................................................................................................................");
		//std::this_thread::sleep_for(std::chrono::seconds(20000));
		std::this_thread::sleep_for(std::chrono::milliseconds(100000));
		std::cout << "--------------" << std::endl;
		client.print();
	}
}

