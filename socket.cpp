#include "socket.h"

void Socket::makeConnection(std::string ip, int port){
    if(ip == "lekeplass") server.ip = "158.36.70.56"; else server.ip = ip;
    server.port = port;
    server.sock = socket(AF_INET, SOCK_STREAM, 0);
    if(server.sock == 0) throw Server::SOCKET_ERROR;

    server.serv_addr.sin_addr.s_addr = inet_addr(server.ip.c_str());
    server.serv_addr.sin_family = AF_INET;
    server.serv_addr.sin_port = htons(server.port);
    int c = connect(server.sock, (sockaddr*)& server.serv_addr, sizeof(server.serv_addr));
    if(c < 0) throw Server::INVALID_CONNECTION;
    server.isConnected = true;
}

void Socket::abortConnection(){
    close(server.sock);
    server.sock = 0;
    server.port = 0;
    server.ip = "NULL";
    server.isConnected = false;
}

void Socket::verifyStudent(std::string studNumber){
    if(studNumber.length() != 6) throw Server::INVALID_STUDNR;
    student.number = (char*)studNumber.c_str();
}

void Socket::requestPort(){
    char buffer[10];
    char messageID = Server::MessageID::REQUEST_PORT;
    const short packageSize = sizeof(packageSize) + sizeof(messageID) + strlen(student.number);

    memset(buffer, 0, sizeof(buffer));
    memcpy(buffer, &packageSize, sizeof(packageSize));
    memcpy(buffer+2, &messageID, sizeof(messageID));
    memcpy(buffer+3, student.number, strlen(student.number));

    int s = send(server.sock, buffer, sizeof(buffer), 0);
    if(s < 0) throw Server::INVALID_PORTREQUEST;
}

short Socket::receivePort(){
    char buffer[10];
    memset(buffer, 0, sizeof(buffer));
    int r = recv(server.sock, buffer, sizeof(buffer), 0);
    if(r < 0) throw Server::INVALID_PORTREQUEST;

    short newPort;
    memcpy(&newPort, buffer+3, sizeof(newPort));
    return ntohs(newPort);
}

Server::MessageID Socket::portResponse(){
    char buffer[10];
    memset(buffer, 0, sizeof(buffer));
    int r = recv(server.sock, buffer, sizeof(buffer), 0);
    if(r < 0) throw Server::PING_ERROR;

    switch(buffer[2]){
        case Server::MessageID::PING:{
            return Server::MessageID::PING;
        }
        case Server::MessageID::PONG:{
            return Server::MessageID::PONG;
        }
        case Server::MessageID::QUIT:{
            return Server::MessageID::QUIT;
        }
        default:{
            throw Server::INVALID_PORTRESPONSE;
        }
    }
}

void Socket::pongServer(){
    char buffer[10];
    char messageID = Server::MessageID::PONG;
    const short packageSize = sizeof(packageSize) + sizeof(messageID);

    memset(buffer, 0, sizeof(buffer));
    memcpy(buffer, &packageSize, sizeof(packageSize));
    memcpy(buffer+2, &messageID, sizeof(messageID));

    int s = send(server.sock, buffer, sizeof(buffer), 0);
    if(s < 0) throw Server::PONG_ERROR;
}
