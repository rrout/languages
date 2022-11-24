#include "hdr.h"

class pktmessage {
    private:
        std::string _myname;
        int _msgType;
        std::vector<std::string> _msgBody;
        bool _filled;
        bool _constructed;
    protected:
    public:
        pktmessage();
        pktmessage(std::string myname);
        pktmessage(std::vector<std::string> &msg);
        pktmessage(zmqpp::message_t &msg);
        int type();
        void type(int type);
        std::string name();
        void name(std::string name);
        void clear();
        bool isclean();
        bool valid();
        void print();

        void construct(std::vector<std::string> &msg);
        void construct(zmqpp::message_t &msg);

        bool fillReq(std::string reqType);
        bool fillReq(std::string reqType, std::string &request);
        bool fillReq(std::string reqType, std::vector<std::string> &requests);

        bool fillResp(std::string respType);
        bool fillResp(std::string respType, std::string &response);
        bool fillResp(std::string respType, std::vector<std::string> &responses);
        
        bool fillTopic(std::string topic, std::string adon, std::string &containt);
        bool fillTopic(std::string topic, std::string adon, std::vector<std::string> &containts);
        
        void compose();
};

