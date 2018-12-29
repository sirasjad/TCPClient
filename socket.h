#ifndef SOCKET_H
#define SOCKET_H
#include <iostream>
#include <string>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

namespace Server{
    struct serverInfo{
        int sock, port;
        std::string ip;
        bool isConnected;
        sockaddr_in serv_addr;
    };

    struct studentInfo{
        char* number = new char[6];
    };

    enum MessageID{
        REQUEST_PORT = 0x01,
        RECEIVE_PORT = 0x02,
        PING = 0x03,
        PONG = 0x04,
        QUIT = 0x05
    };

    enum Errors{
        SOCKET_ERROR,
        INVALID_CONNECTION,
        INVALID_STUDNR,
        INVALID_PORTREQUEST,
        INVALID_PORTRESPONSE,
        PING_ERROR,
        PONG_ERROR
    };
};

class Socket{
public:    
    Socket() : server{ 0, 0, "NULL", false } {}
    void makeConnection(std::string, int);
    void abortConnection();
    bool getConnectionStatus() const{ return server.isConnected; }
    char* getStudentNumber() const{ return student.number; }
    void verifyStudent(std::string);
    void requestPort();
    short receivePort();
    Server::MessageID portResponse();
    void pongServer();

private:
    Server::serverInfo server;
    Server::studentInfo student;
};

#endif
