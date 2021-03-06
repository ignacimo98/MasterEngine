//
// Created by Ignacio Mora on 6/14/17.
//

#include <iostream>
#include <fstream>
#include "connectionManager.h"

ConnectionManager::ConnectionManager() {

    tables = new TableManager();
    operations = new OperationManager(tables);

    //a message

    //initialise all allSockets[] to 0 so not checked
    for (i = 0; i < maxSockets; i++)
    {
        allSockets[i] = 0;
    }
    for (i = 0; i < maxDisks; i++)
    {
        diskSockets[i] = 0;
    }
    for (i = 0; i < maxClients; i++)
    {
        clientSockets[i] = 0;
    }
    //create a master socket
    if( (master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    //set master socket to allow multiple connections ,
    //this is just a good habit, it will work without this
    if( setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt,
                   sizeof(opt)) < 0 )
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    //type of socket created
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    //bind the socket to localhost port 8888
    if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    printf("Listener on port %d \n", PORT);

    //try to specify maximum of 3 pending connections for the master socket
    if (listen(master_socket, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    //accept the incoming connection
    addrlen = sizeof(address);
    puts("Waiting for connections ...");
//    std::thread t1(&ConnectionManager::waitForConnections, this);
//    std::thread t2(&ConnectionManager::waitForMessage, this);
//    std::cout << "yay" << std::endl;
//
//    t1.join();

    while(TRUE)
    {
        //clear the socket set
        FD_ZERO(&readfds);

        //add master socket to set
        FD_SET(master_socket, &readfds);
        max_sd = master_socket;

        //add child sockets to set
        for ( i = 0 ; i < maxSockets ; i++)
        {
            //socket descriptor
            sd = allSockets[i];

            //if valid socket descriptor then add to read list
            if(sd > 0)
                FD_SET( sd , &readfds);

            //highest file descriptor number, need it for the select function
            if(sd > max_sd)
                max_sd = sd;
        }

        //wait for an activity on one of the sockets , timeout is NULL ,
        //so wait indefinitely
        activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);

        if ((activity < 0) && (errno!=EINTR))
        {
            printf("select error");
        }

        //If something happened on the master socket ,
        //then its an incoming connection
        if (FD_ISSET(master_socket, &readfds))
        {
            if ((new_socket = accept(master_socket,
                                     (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
            {
                perror("accept");
                exit(EXIT_FAILURE);
            }

            //inform user of socket number - used in send and receive commands
            printf("New connection , socket fd is %d , ip is : %s , port : %d\n" , new_socket , inet_ntoa(address.sin_addr) , ntohs
                    (address.sin_port));

            json initialJSON;
            initialJSON["command"] = "identify_yourself";
            std::string initialString = initialJSON.dump();

            //send new connection greeting message
            if( send(new_socket, initialString.c_str(), initialString.size(), 0) != initialString.size() )
            {
                perror("send");
            }

            puts("Welcome message sent successfully");


            //add new socket to array of sockets
            for (i = 0; i < maxSockets; i++)
            {
                //if position is empty
                if( allSockets[i] == 0 )
                {
                    allSockets[i] = new_socket;
                    printf("Adding to list of sockets as %d\n" , i);

                    break;
                }
            }
        }

        //else its some IO operation on some other socket
        for (i = 0; i < maxSockets; i++)
        {
            sd = allSockets[i];

            if (FD_ISSET( sd , &readfds))
            {
                valread = read( sd , buffer, 1024*10);
                //Check if it was for closing , and also read the
                //incoming message
                if (valread == 0)
                {
                    //Somebody disconnected , get his details and print
                    getpeername(sd , (struct sockaddr*)&address , \
                        (socklen_t*)&addrlen);
                    printf("Host disconnected , ip %s , port %d \n" ,
                           inet_ntoa(address.sin_addr) , ntohs(address.sin_port));

                    //Close the socket and mark as 0 in list for reuse
                    close( sd );
                    allSockets[i] = 0;
                    if (queryExecutorSocket == sd) {
                        queryExecutorSocket = 0;
                    }
                    for (i = 0; i < maxClients; i++)
                    {
                        //if position is empty
                        if( clientSockets[i] == sd )
                        {
                            clientSockets[i] = 0;
                            printf("Removing client no. %d\n" , i);

                            break;
                        }
                    }
                    for (i = 0; i < maxDisks; i++)
                    {
                        //if position is empty
                        if( diskSockets[i] == sd )
                        {
                            diskSockets[i] = 0;
                            printf("Removing disk no. %d\n" , i);

                            break;
                        }
                    }

                }

                    //Echo back the message that came in
                else
                {
                    //set the string terminating NULL byte on the end
                    //of the data read
                    buffer[valread] = '\0';
                    std::ofstream jsonFile("myFile.json");
                    jsonFile << buffer;
                    jsonFile.close();
                    readJSON();
//                    send(sd , buffer , strlen(buffer) , 0 );
                }
            }
        }
    }


}


void ConnectionManager::readJSON() {
    std::ifstream jsonFile("myFile.json");
    json j;
    jsonFile >> j;
    jsonFile.close();
    std::cout << j.dump() << std::endl;
    if (j["command"] == "identifying"){
        if (j["identity"] == "diskNode"){
            for (i = 0; i < maxDisks; i++)
            {
                //if position is empty
                if( diskSockets[i] == 0 )
                {
                    diskSockets[i] = sd;
                    printf("Adding to list of disks as %d\n" , i);
//                    std::string diskString = "{\"command\":\"create_table\",\"name\":\"el nombre\",\"columnTypes\":[0,1,2],\"columnNames\":[\"nombre\",\"id\",\"estatura\"],\"rows\":[]}";
//                    send(sd, diskString.c_str(), diskString.size(), 0);
//                    usleep(500000);
//                    std::string requestString = "{\"command\":\"get_table\", \"name\":\"el nombre\"}";
//                    send(sd, requestString.c_str(), requestString.size(), 0);

                    break;
                }
            }
        } else if (j["identity"] == "queryExecutor"){
            if (queryExecutorSocket == 0){
                queryExecutorSocket = sd;
                printf("The query executor is %d\n", sd);
            } else {
                // reject the connection
                json rejectJSON;
                rejectJSON["command"] = "Go Fuck Yourself!!!";
                std::string rejectString = rejectJSON.dump();
                send(sd , rejectString.c_str() , rejectString.size() , 0 );
            }
        } else if (j["identity"] == "client"){
            for (i = 0; i < maxClients; i++)
            {
                //if position is empty
                if( clientSockets[i] == 0 )
                {
                    clientSockets[i] = sd;
                    printf("Adding to list of clients as %d\n" , i);

                    break;
                }
            }
        }
    } else if (j["command"]=="disconnect"){
        getpeername(sd , (struct sockaddr*)&address , \
                        (socklen_t*)&addrlen);
        printf("Host disconnected , ip %s , port %d \n" ,
               inet_ntoa(address.sin_addr) , ntohs(address.sin_port));

        //Close the socket and mark as 0 in list for reuse
        close( sd );
        allSockets[i] = 0;
        if (queryExecutorSocket == sd) {
            queryExecutorSocket = 0;
        }
        for (i = 0; i < maxClients; i++)
        {
            //if position is empty
            if( clientSockets[i] == sd )
            {
                clientSockets[i] = 0;
                printf("Removing client no. %d\n" , i);

                break;
            }
        }
        for (i = 0; i < maxDisks; i++)
        {
            //if position is empty
            if( diskSockets[i] == sd )
            {
                diskSockets[i] = 0;
                printf("Removing disk no. %d\n" , i);

                break;
            }
        }

    } else if (j["command"] == "answer_from_diskNode"){
        if (j["found"]){
            tables->addTable(JSONutils::jsonToTable(j));
            std::cout << tables->getTable(j["name"]).toString() << std::endl;
        }
    } else if (j["command"] == "create_table") {
        tables->addTable(JSONutils::jsonToTable(j));
        std::string toDisk = j.dump();
        for (i = 0; i < maxDisks; i++){
            if (diskSockets[i] != 0) {
                send(diskSockets[i],toDisk.c_str(), toDisk.size(), 0);
            }
        }
        std::cout << tables->tableList[0].toString() << std::endl;
        json returnCode;
        returnCode["command"] = "responseCode";
        returnCode["code"] = 1;
        returnCode["affected_entries"] = 0;
        returnCode["description"] = "Operación exitosa!!!";
        std::string returnStr = returnCode.dump();
        std::cout << returnStr << std::endl;
        send(currentClient,returnStr.c_str(),returnStr.size(),0);


    } else if (j["command"] == "insert"){
        resultCode code = operations->insert(j);
        if(code.getNumberOfRegisters()){
            json j1 = JSONutils::tableToJson(tables->getTable(j["name"]));
            j1["command"] = "update_table";
            std::string toDisk = j1.dump();
            for (i = 0; i < maxDisks; i++){
                if (diskSockets[i] != 0) {
                    send(diskSockets[i],toDisk.c_str(), toDisk.size(), 0);
                }
            }
            std::cout << tables->getTable(j["name"]).toString() << std::endl;

        }
        json responseCode;
        responseCode["command"] = "responseCode";
        responseCode["code"] = code.getCodeNumber();
        responseCode["affected_entries"] = code.getNumberOfRegisters();
        responseCode["description"] = code.getCodeDescription();
        std::string responseString = responseCode.dump();
        send(currentClient,responseString.c_str(), responseString.size(), 0);

    } else if (j["command"] == "parse"){
        currentClient = sd;
        std::string queryString = j["query"];
        send(queryExecutorSocket, queryString.c_str(), queryString.size(), 0);
    } else if (j["command"] == "drop_table"){
        resultCode code = operations->drop(j);
        if (code.getCodeNumber() == 1){
            std::string toDisk = JSONutils::tableToJson(tables->getTable(j["name"])).dump();
            for (i = 0; i < maxDisks; i++){
                if (diskSockets[i] != 0) {
                    send(diskSockets[i],toDisk.c_str(), toDisk.size(), 0);
                }
            }
        }
        json responseCode;
        responseCode["command"] = "answer_to_request";
        responseCode["code"] = code.getCodeNumber();
        responseCode["affected_entries"] = code.getNumberOfRegisters();
        responseCode["description"] = code.getCodeDescription();
        std::string responseString = responseCode.dump();
        send(currentClient,responseString.c_str(), responseString.size(), 0);
    }else if (j["command"] == "get_table"){
        currentClient = sd;
        sendTable(j["name"]);
    }else if (j["command"] == "update"){
        resultCode code = operations->update(j);
        if (code.getCodeNumber() == 1){
            json toDisk = JSONutils::tableToJson(tables->getTable(j["from"]));
            toDisk["command"] = "update_table";
            std::string toDiskStr = toDisk.dump();
            for (i = 0; i < maxDisks; i++){
                if (diskSockets[i] != 0) {
                    send(diskSockets[i],toDiskStr.c_str(), toDiskStr.size(), 0);
                }
            }
        }

        std::cout << tables->getTable(j["from"]).toString() << std::endl;

        json responseCode;
        responseCode["command"] = "responseCode";
        responseCode["code"] = code.getCodeNumber();
        responseCode["affected_entries"] = code.getNumberOfRegisters();
        responseCode["description"] = code.getCodeDescription();
        std::string responseString = responseCode.dump();
        send(currentClient,responseString.c_str(), responseString.size(), 0);
    } else if (j["command"] == "select"){
        Table resultTable = operations->select(j);
        json result;
        result["command"] = "table";
        result["table"] = JSONutils::tableToJson(resultTable);
        std::string str = result.dump();
        send(currentClient, str.c_str(), str.size(), 0);
    }
    else if(j["command"] == "delete"){
        resultCode code = operations->deleteT(j);
        if (code.getCodeNumber() == 1){
            json toDisk = JSONutils::tableToJson(tables->getTable(j["name"]));
            toDisk["command"] = "delete_table";
            std::string toDiskStr = toDisk.dump();
            for (i = 0; i < maxDisks; i++){
                if (diskSockets[i] != 0) {
                    send(diskSockets[i],toDiskStr.c_str(), toDiskStr.size(), 0);
                }
            }
        }

        std::cout << tables->getTable(j["name"]).toString() << std::endl;

        json responseCode;
        responseCode["command"] = "responseCode";
        responseCode["code"] = code.getCodeNumber();
        responseCode["affected_entries"] = code.getNumberOfRegisters();
        responseCode["description"] = code.getCodeDescription();
        std::string responseString = responseCode.dump();
        send(currentClient,responseString.c_str(), responseCode.size(), 0);
    }else if(j["command"] == "error"){
        json responseCode;
        responseCode["command"] = "responseCode";
        responseCode["code"] = 405;
        responseCode["affected_entries"] = 0;
        responseCode["description"] = j["description"];
        std::string responseString = responseCode.dump();
        send(currentClient,responseString.c_str(), responseCode.size(), 0);

    } else if (j["command"] == "getMetadata"){
        json meta;
        json k(getMetadata());
        meta["metadata"] = k;
        std::string str = meta.dump();
        std::cout << str << std::endl;
        send(currentClient, str.c_str(),str.size(),0);
    }


}
void ConnectionManager::sendTable(std::string name) {
    json answer;
    json command;
    command["command"] = "table";
    if (!tables->exists(name)){
        answer["command"] = "answer";
        answer["found"] = false;
    } else{
        answer = JSONutils::tableToJson(tables->getTable(name));
    }
    command["table"] = answer;
    std::string commandString = command.dump();
    std::cout << commandString << std::endl;
    send(currentClient, commandString.c_str(), commandString.size(), 0);


}


bool ConnectionManager::isClient(int fd) {
    for (i = 0; i < maxClients; i++){
        if (clientSockets[i] == fd)
            return true;
    }
    return false;
}

bool ConnectionManager::isDisk(int fd) {
    for (i = 0; i < maxDisks; i++){
        if (diskSockets[i] == fd)
            return true;
    }
    return false;
}

std::vector<std::string> ConnectionManager::getMetadata() {
    std::vector<std::string> result;
    for (int i = 0; i < tables->tableList.size(); i++) {
        result.push_back(JSONutils::tableToJson(tables->tableList[i]).dump());
    }
    return result;
}



