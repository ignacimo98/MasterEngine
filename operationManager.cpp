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

/*
{
    "type": "select",
    "what": ["id ", "cedula", "*"],
    "from": "estudiantes",
    "where": {
        "table": "estudiantes",
        "column": "id",
        "operator": "=",
        "value": 5
    },
    "join": {
        "externalTable": "cursos",
        "externalColumn": "idest",
        "internalColumn": "id"
    }

}
 */

Table applyComparator(Table table, Comparator comparator ){
    Table result;
    if(comparator.getOperatorType() == "="){
        result = TableUtils::equalTo(table,comparator.getColumnName(), comparator.getValue());
    }
    else if (comparator.getOperatorType()== "<") {
        result = TableUtils::lessThan(table,comparator.getColumnName(), comparator.getValue());

    }
    else if (comparator.getOperatorType() == ">") {
       result =  TableUtils::greaterThan(table,comparator.getColumnName(), comparator.getValue());
    }
    return result;
}
Table applyWhere(Table table, Where where){
    Table result;
    if(where.getCondition()=="AND"){
        result = table;
        for(const Comparator &comparator : where.getComparators()){
            result = applyComparator(result, comparator);
            //result = TableUtils::AND(result, subResult);
        }
    }
    else if(where.getCondition()=="OR"){
        result = TableUtils::extractTemplate(table);
        for(const Comparator &comparator : where.getComparators()){
            Table subResult = applyComparator(table, comparator);
            result = TableUtils::OR(result, subResult);
        }
    }
    else{
        if(where.getComparators().size()>0){
            result = applyComparator(table, comparator);
        }

    }
    return result;
}
//where sin join

Table OperationManager::selectAux(std::string tableName, std::vector<std::string> columns, Where whereObject){
    Table table = tables->getTable(tableName);
    Table result = TableUtils::subTable(table, columns);
    //whereObject.
    return result;
}
Table OperationManager::selectAux(std::string tableName, std::vector<std::string> columns, Join joinObject){
}
//columnas
Table OperationManager::selectAux(std::string tableName, std::vector<std::string> columns){
    Table table = tables->getTable(tableName);
    Table result = TableUtils::subTable(table, columns);
    return result;
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
    Table result;
    //columnas
    if(json["where"] == "" && json["join"] == ""){
        result = selectAux(json["from"], json["what"]);
    }
    //join sin where
    else if(json["where"] == "" && json["join"] != "") {
        //Join joinObject(json["join"]);
        // result = selectAux(json["from"], json["what"], joinObject);
    }
    //where sin join
    else if(json["where"] != "" && json["join"] == ""){
        Where whereObject(json["where"]);
        result = selectAux(json["from"], json["what"], whereObject);
    }
    //where con join
    else{
        //Where whereObject(json["where"]);
        //Join joinObject(json["join"]);
        // result = selectAux(json["from"], json["what"], whereObject, joinObject);
    }
    return result;
}








