//
// Created by jimena on 16/06/17.
//
#include "operationManager.h"
#include "where.h"
#include "join.h"

bool isInteger(const std::string & s)
{
    if(s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false ;

    char * p ;
    strtol(s.c_str(), &p, 10) ;

    return (*p == 0) ;
}
bool isDouble(const std::string& s)
{
    int nb_point=0;
    for (int i=0; i<s.length();i++)
    {
        if (s[i]=='.')
        {
            nb_point++;
        }
        else if (!isdigit(s[i])) {
            return false;
        }
    }
    if (nb_point==1)
    {
        return true;
    }
    else
    {
        return false;
    }
}

OperationManager::OperationManager(TableManager *tableManager) {
    this->tables = tableManager;

}

Table OperationManager::selectAux(std::string tableName, std::vector<std::string> columns, Where whereObject, Join joinObject){


}
/*
{
    "command": "execute",
    "type": "select",
    "what": ["id ", "cedula", "*"],
    "from": "estudiantes",
    "where": {
        "cmd": "AND",
        "comparators": [{
            "table": "estudiantes",
            "column": "id",
            "operator": "=",
            "value": 5
        }, {
            "table": "estudiantes",
            "column": "cedula",
            "operator": "=",
            "value": 2000222
        }]
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

    //std::cout<<"operator "<< comparator.getOperatorType()<< "    "<<comparator.getColumnName() << "\n___________________________\n"<<result.toString()<<"_______________________________________\n";

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
            result = TableUtils::tableUnion(result, subResult);
            //std::cout<<"union \n____________________________________\n"<<result.toString()<<"____________________________________\n";
        }
    }
    else{
        if(where.getComparators().size()>0){
            result = applyComparator(table, where.getComparators()[0]);
        }

    }
   // std::cout<<"where "<< where.getCondition() << "___________________________\n"<<result.toString();

    return result;
}
//where sin join

Table OperationManager::selectAux(std::string tableName, std::vector<std::string> columns, Where whereObject){
    Table table = tables->getTable(tableName);
    //Table result = TableUtils::extractTemplate(table);
    Table result = applyWhere(table, whereObject);
    result = TableUtils::subTable(result, columns);
    //whereObject.
    return result;
}

//join sin where
Table OperationManager::selectAux(std::string tableName, std::vector<std::string> columns, Join joinObject){


}
//columnas
Table OperationManager::selectAux(std::string tableName, std::vector<std::string> columns){
    Table table = tables->getTable(tableName);
    Table result = TableUtils::subTable(table, columns);
    return result;
    /*if(columns.size()>0 && columns[0]=="*"){
        result = table;
    }
    else{
    Table result = TableUtils::subTable(table, columns);
    }*/
}


resultCode OperationManager::insert(json j){
    Table workingTable;
    if(tables->exists(j["name"])){
        workingTable = tables->getTable(j["name"]);
        std::vector<std::string> toAdd;
        for (int i = 0; i < workingTable.getTotalColumns(); i++){
            bool found = false;
            for (int k = 0; k < j["column_names"].size(); k++){
                if (workingTable.getColumnProperties()[i].getName() == j["column_names"][k]){
                    found = true;
                    if (workingTable.getColumnProperties()[i].getType() == 0){
                        toAdd.push_back(j["values"][k]);
                        break;
                    }
                    else if (workingTable.getColumnProperties()[i].getType() == 1){
                        if (isInteger(j["values"][k])){
                            toAdd.push_back(j["values"][k]);
                            break;
                        }
                        else{
                            return resultCode(69,0,"Hay un valor inválido!");
                        }
                    }
                    else if (workingTable.getColumnProperties()[i].getType() == 2){
                        if (isDouble(j["values"][k])){
                            toAdd.push_back(j["values"][k]);
                            break;
                        }
                        else{
                            return resultCode(69,0,"Hay un valor inválido!");
                        }
                    }
                }
            }
            if (!found){
                toAdd.push_back("");
            }
        }

        workingTable.insertRow(toAdd);

        for (int i = 0; i < tables->tableList.size(); i++){
            if (tables->tableList[i].getName() == workingTable.getName()){
                tables->tableList[i] = workingTable;
                break;
            }
        }

        return resultCode(1,1,"Operación exitosa!");
    }
    return resultCode(404,0,"Table not found!!!");


}



resultCode OperationManager::drop(json j) {
    std::cout<<"se esta haciendo drop\n";
    int affectedRegisters = tables->deleteTable(j["name"]);
    if (affectedRegisters > 1){
        return resultCode(1, affectedRegisters, "Operación exitosa!");
    }
    return resultCode(404, 0, "Tabla no existe");
}



Table OperationManager::select(json inputJson){
    Table result;
    //columnas
    if(inputJson["where"] == "" && inputJson["join"] == ""){
        result = selectAux(inputJson["from"], inputJson["what"]);
    }
    //join sin where
    else if(inputJson["where"] == "" && inputJson["join"] != "") {
        Join joinObject =  JSONutils::jsonToJoin(inputJson["join"]);
        result = selectAux(inputJson["from"], inputJson["what"], joinObject);
    }
    //where sin join
    else if(inputJson["where"] != "" && inputJson["join"] == ""){
        Where whereObject = JSONutils::jsonToWhere(inputJson["where"]);
        result = selectAux(inputJson["from"], inputJson["what"], whereObject);
    }
    //where con join
    else{
        Where whereObject = JSONutils::jsonToWhere(inputJson["where"]);
        Join joinObject =  JSONutils::jsonToJoin(inputJson["join"]);
        // result = selectAux(inputJson["from"], inputJson["what"], whereObject, joinObject);
    }
    std::cout<<"\nSELECT \n____________________________________\n"<<result.toString()<<"____________________________________\n";
    return result;
}
resultCode OperationManager::deleteT(json j){
    resultCode result(404,0,"Table not found");
    if(tables->exists(j["name"])) {
        if(j["where"] != "") {
            Where whereObject = JSONutils::jsonToWhere(j["where"]);
            result = deleteAux(j["name"], whereObject);
        }
        else{
            result = drop(j);
        }
    }
    return result;
}


resultCode OperationManager::update(json j) {

    resultCode result(404,0,"Table not found");
    if(tables->exists(j["from"])) {
        if(j["where"] != "") {
            Where whereObject = JSONutils::jsonToWhere(j["where"]);
            result = updateAux(j["from"], j["columns"], j["values"], whereObject);
        }
        else{
            result = updateAux(j["from"], j["columns"], j["values"]);
        }
    }
    return result;
}

resultCode
OperationManager::updateAux(std::string tableName, std::vector<std::string> columns, std::vector<std::string> values) {
    Table workingTable = tables->getTable(tableName);
    for (int i = 0; i < columns.size(); ++i) {
        std::cout<<" "<<columns[i];
    }
    for (int i = 0; i < values.size(); ++i) {
        std::cout<<" "<<values[i];
    }


    workingTable = TableUtils::updateColumns(workingTable, values, columns);

    for (int i = 0; i < tables->tableList.size(); i++){
        if (tables->tableList[i].getName() == workingTable.getName()){
            tables->tableList[i] = workingTable;
            break;
        }
    }
    std::cout<<"\nUPDATE \n____________________________________\n"<<workingTable.toString()<<"____________________________________\n";
    return resultCode(1, workingTable.getTotalRows(), "Operacion Exitosa!");
}

resultCode
OperationManager::updateAux(std::string tableName, std::vector<std::string> columns, std::vector<std::string> values,
                            Where whereObject) {

    Table workingTable = tables->getTable(tableName);

    Table subTable = applyWhere(workingTable, whereObject);

    int affectedRegisters = subTable.getTotalRows();

    subTable = TableUtils::updateColumns(subTable, values, columns);
    //std::cout<<"subtable \n____________________________________\n"<<subTable.toString()<<"____________________________________\n";

    Table tableToSend = TableUtils::updateRows(workingTable, subTable);
    //std::cout<<"tableto \n____________________________________\n"<<tableToSend.toString()<<"____________________________________\n";

    for (int i = 0; i < tables->tableList.size(); i++){
        if (tables->tableList[i].getName() == tableToSend.getName()){
            tables->tableList[i] = tableToSend;
            break;
        }
    }
    std::cout<<"\nUPDATE \n____________________________________\n"<<tableToSend.toString()<<"____________________________________\n";

    return resultCode(1, affectedRegisters, "Operacion Exitosa!");
}

resultCode OperationManager::deleteAux(std::string tableName, Where whereObject)
{
     Table workingTable = tables->getTable(tableName);
     Table subTable = applyWhere(workingTable, whereObject);


     Table resultTable = TableUtils::tableDifference(workingTable, subTable);
     int affectedRegisters = subTable.getTotalRows();

     //std::cout << resultTable.toString()<<std::endl;

     for (int i = 0; i < tables->tableList.size(); i++){
         if (tables->tableList[i].getName() == resultTable.getName()){
             tables->tableList[i] = resultTable;
             break;
         }
     }
     std::cout<<"\nDELETE \n____________________________________\n"<<resultTable.toString()<<"____________________________________\n";
     std::cout<< resultTable.toString() <<"\n";
     return resultCode(1, affectedRegisters, "Operacion Exitosa!");

}










