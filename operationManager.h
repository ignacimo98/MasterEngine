//
// Created by jimena on 16/06/17.
//

#ifndef MASTERENGINE_OPERATIONMANAGER_H
#define MASTERENGINE_OPERATIONMANAGER_H

#include "table.h"
#include "resultCode.h"
#include "connectionManager.h"
#include "tableManager.h"
#include "JSON library/json.hpp"
#include "where.h"
#include "join.h"
using json = nlohmann::json;

class OperationManager{
public:
    OperationManager(TableManager* tableManager);
    Table select(json json);
    resultCode create();
    resultCode update();
    resultCode drop();
    resultCode deleteT();
    resultCode insert(json j);
    resultCode createIndex();
    TableManager* tables;
private:
    Table selectAux(std::string tableName, std::vector<std::string> columns, Where whereObject, Join joinObject);
    Table selectAux(std::string tableName, std::vector<std::string> columns, Where whereObject);
    Table selectAux(std::string tableName, std::vector<std::string> columns, Join joinObject);
    Table selectAux(std::string tableName, std::vector<std::string> columns);

};



#endif //MASTERENGINE_OPERATIONMANAGER_H
