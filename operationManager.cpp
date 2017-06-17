//
// Created by jimena on 16/06/17.
//
#include "operationManager.h"
#include "where.h"
#include "join.h"

OperationManager::OperationManager(TableManager *tableManager) {
    this->tables = tableManager;

}

Table OperationManager::selectAux(std::string tableName, std::vector<std::string> columns, Where whereObject, Join joinObject){

}

Table OperationManager::selectAux(std::string tableName, std::vector<std::string> columns, Where whereObject){

}

Table OperationManager::selectAux(std::string tableName, std::vector<std::string> columns, Join joinObject){

}

Table OperationManager::selectAux(std::string tableName, std::vector<std::string> columns){

}


resultCode OperationManager::insert(json j){
    if(tables->exists(j["name"])){
        std::vector<std::string> toAdd;
        for (int i = 0; i < tables->getTable(j["name"]).getTotalColumns(); i++){
            for (int k = 0; k < j["column_names"].size(); k++){
                if (tables->getTable(j["name"]).getColumnProperties()[i].getName() == j["column_names"][k]){

                }
            }
        }

    }
    return resultCode(0,0,"");
}

Table OperationManager::select(json json){
    //columnas
    if(json["where"] == "" && json["join"] == ""){
        selectAux(json["from"], json["what"]);
    }
    //join sin where
    else if(json["where"] == "" && json["join"] != "") {
        //Join joinObject(json["join"]);
        //selectAux(json["from"], json["what"], joinObject);
    }
    //where sin join
    else if(json["where"] != "" && json["join"] == ""){
        //Where whereObject(json["where"]);
        //selectAux(json["from"], json["what"], whereObject);
    }
    //where con join
    else{
        //Where whereObject(json["where"]);
        //Join joinObject(json["join"]);
        //selectAux(json["from"], json["what"], whereObject, joinObject);
    }
}








