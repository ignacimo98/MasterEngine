#include "jsonutils.h"



json JSONutils::tableToJson(Table table)
{
    json result;
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
    result["primaryKey"]= table.getPrimaryKey();
    //std::cout<<"JSON:  " <<result.dump()<<std::endl;
    //json *result2 = new json;
    //result2 = result;
    return result;
}

Table JSONutils::jsonToTable(json inputJson)
{

    Table result;
    result.setName(inputJson["name"]);
    result.setPrimaryKey(inputJson["primaryKey"]);

    for (int i = 0; i < inputJson["columnNames"].size(); i++) {
        result.insertColumn(inputJson["columnNames"][i],inputJson["columnTypes"][i] );
   }
    for (const auto &row : inputJson["rows"] ) {
        std::vector<std::string> newRow = row;
        result.insertRow(Row(newRow));
    }
    return result;



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


Where JSONutils::jsonToWhere(json inputJson)
{
    Where result;
    result.setCondition(inputJson["cmd"]);
    std::vector<Comparator> compV;
    for(const auto &comparator : inputJson["comparators"]){
        compV.push_back(jsonToComparator(comparator));
    }
    result.setComparators(compV);
    return result;

}

Join JSONutils::jsonToJoin(json inputJson)
{
    Join result;
    result.setExternalColumn(inputJson["externalColumn"]);
    result.setExternalTable(inputJson["externalTable"]);
    result.setInternalColumn(inputJson["internalColumn"]);
    return result;
}

Comparator JSONutils::jsonToComparator(json inputJson)
{
    Comparator result;
    result.setValue(inputJson["value"]);
    result.setColumnName(inputJson["column"]);
    result.setOperatorType(inputJson["operator"]);
    result.setTableName(inputJson["table"]);
    return result;
}
