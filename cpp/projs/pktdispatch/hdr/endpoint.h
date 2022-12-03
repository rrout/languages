#include "hdr.h"
#include "pktmessage.h"




#ifndef __ENDPOINT_H__
#define __ENDPOINT_H__
class endPoint {
    private:
        std::string _endpoint;
        endPointType_t _endpointType;
        zmqpp::context _context;
        zmqpp::socket *_connection;
        zmqpp::socket_type _socktype;
    protected:
    public:
        endPoint() = delete;
        endPoint(std::string endpoint, endPointType_t type);
        ~endPoint();
        zmqpp::socket_type getsockettype(endPointType_t type);
        bool isSocketCatagoryServer();
        bool send(std::vector<std::string> &msg);
        bool send(pktmessage &msg);
        bool recieve(std::vector<std::string> &msg);
        bool recieve(pktmessage &msg);
        bool subscribe(std::string topic);
};

#endif //__ENDPOINT_H__
