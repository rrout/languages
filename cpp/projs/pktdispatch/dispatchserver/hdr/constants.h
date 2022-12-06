#include "hdr.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#ifndef __CONSTANTS_H__
#define __CONSTANTS_H__
#define MGMT_ENDPOINT		"tcp://*:4243"
#define ADV_ENDPOINT		"tcp://*:4244"
#define PUB_ENDPOINT		"tcp://*:4242"
#define SUB_ENDPOINT		"tcp://*:4245"

#define PD_MAX_BUFFER_COUNT             10
#define ADV_INT_SEC						10
#define ADV_INT_MILISEC					(ADV_INT_SEC * 1000)
#define ERR_INT_SEC						60
#define ERR_INT_MILISEC					(ERR_INT_SEC * 1000)

#define TOPIC_FORMAT_START				"TOPIC-"


typedef enum {
    PUSH,
    SERVER_PUSH,
    PULL,
    SERVER_PULL,
    REQ,
    SERVER_REQ,
    RESP,
    SERVER_RESP,
    PUBLISH,
    SERVER_PUBLISH,
    SUBSCRIBE,
    SERVER_SUBSCRIBE
} endPointType_t;

typedef enum {
    STATUS_OK,
    STATUS_FAIL,
    STATUS_EEXIST,
    STATUS_INTERNAL,
    STATUS_MAX
}pktConfigStatus_t;

inline std::string getHostName() {
    std::string name;
    struct addrinfo hints, *info, *p;
    int gai_result;

    char hostname[1024];
    hostname[1023] = '\0';
    gethostname(hostname, 1023);

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; /*either IPV4 or IPV6*/
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_CANONNAME;
    if ((gai_result = getaddrinfo(hostname, "http", &hints, &info)) != 0) {
            fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(gai_result));
                exit(1);
    }
    for(p = info; p != NULL; p = p->ai_next) {
        if (name.empty()){
                name = p->ai_canonname;
                }
            printf("hostname: %s\n", p->ai_canonname);
    }
    freeaddrinfo(info);
    return name;

}
inline std::vector<std::string> spilt(std::string s, const char delim) {
    std::stringstream ss(s);
    vector<string> tokens;
    std::string temp_str;
    while(getline(ss, temp_str, delim)){
        tokens.push_back(temp_str);
    }
    return tokens;
}
inline std::string dateTimeNow()
{
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");
    return ss.str();
}
inline bool validTopicName(std::string topicName) {
	return topicName.starts_with(TOPIC_FORMAT_START);
}

#endif //__CONSTANTS_H__
