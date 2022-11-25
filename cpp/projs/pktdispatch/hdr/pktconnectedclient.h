#include "hdr.h"
class pktconnectedclient {
	private:
	protected:
	public:
		std::string clientId;
		std::string clientName;
		std::string createdTime;
		bool alive = false;
		bool active = false;
		std::list<std:string> pubTopics;
		std::list<std:string> subTopics;

};
