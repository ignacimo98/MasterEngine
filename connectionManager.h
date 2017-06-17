//
// Created by Ignacio Mora on 6/14/17.
//

#ifndef MASTERENGINE_CONNECTIONMANAGER_H
#define MASTERENGINE_CONNECTIONMANAGER_H


#include <stdio.h>
#include <string.h>   //strlen
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>   //close
#include <arpa/inet.h>    //close
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros
#include <thread>
#include "JSON library/json.hpp"
#include "tableManager.h"
#include "jsonUtils.h"
#include "operationManager.h"

#define TRUE   1
#define FALSE  0
#define PORT 8888
#define MAX_SIZE 100000000000000000

using json = nlohmann::json;


class ConnectionManager {
public:
    ConnectionManager();
    void waitForConnections();
    void waitForMessage();
    void readJSON();
    bool isDisk(int fd);
    bool isClient(int fd);
    void insert(std::string name, std::vector<std::string> column)
    TableManager* tables;
    OperationManager* operations;


private:

    int opt = TRUE;
    int master_socket , addrlen , new_socket , allSockets[45] ,
            maxSockets = 45 , activity, i , valread , sd, counter = 0,
            diskSockets[15], maxDisks = 15, queryExecutorSocket = 0,
            clientSockets[30], maxClients = 30;
    int max_sd;
    struct sockaddr_in address;


    char buffer[1024*10];  //data buffer of 10K

    //set of socket descriptors
    fd_set readfds;

};


#endif //MASTERENGINE_CONNECTIONMANAGER_H
