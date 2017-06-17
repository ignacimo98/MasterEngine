//
// Created by Ignacio Mora on 6/16/17.
//

#include "jsonUtils.h"


json JSONutils::tableToJson(Table table)
{
    json result;
    result["command"] = "answer";
    result["found"] = true;
    result["name"] =  table.getName();
    json rows;
    for(const Row &row : table.getRows()) {
        json column;
        for(const std::string content : row.getContents()){
            column.push_back(content);
        }
        rows.push_back(column);
    }
    //json columnNames;
    std::vector<std::string> columnNames;
    std::vector<int> columnTypes;
    for(const ColumnProperties &property : table.getColumnProperties()) {
        //std::cout<<property.getName()<<"\n";
        columnNames.push_back(property.getName());
        columnTypes.push_back(property.getType());
    }
    result["columnNames"]= columnNames;
    result["columnTypes"]= columnTypes;
    result["rows"]= rows;
    //std::cout<<"JSON:  " <<result.dump()<<std::endl;
    //json *result2 = new json;
    //result2 = result;
    return result;
}

Table JSONutils::jsonToTable(json inputJson)
{


    Table result;
    result.setName(inputJson["name"]);

    for (int i = 0; i < inputJson["columnNames"].size(); i++) {
        result.insertColumn(inputJson["columnNames"][i],inputJson["columnTypes"][i] );
    }
    for (const auto &row : inputJson["rows"] ) {
        std::vector<std::string> newRow = row;
        result.insertRow(Row(newRow));
    }
    return result;



}

