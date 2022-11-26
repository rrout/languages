#include "hdr.h"

#ifndef __PKTSUBSCRIBER_H__
#define __PKTSUBSCRIBER_H__
class pktsubscriber {
    private:
        std::mutex lock;
        std::string _topic;
		std::string _name;
		int _pktSent;
    protected:
    public:
		pktsubscriber() = delete;
		pktsubscriber(std::string name, std::string topic);
		~pktsubscriber();
        std::string getTopic();
        std::string getName();
		void pktSent();
		int getPktSent();
		bool print();
};


#endif //__PKTSUBSCRIBER_H__
