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

#define TRUE   1
#define FALSE  0
#define PORT 8888

class ConnectionManager {
public:
    ConnectionManager();




};


#endif //MASTERENGINE_CONNECTIONMANAGER_H
