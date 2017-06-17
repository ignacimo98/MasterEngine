#include "tableutils.h"

//Table extractTemplate(Table table){
Table TableUtils::extractTemplate(Table table){
    Table result;
    result.setName(table.getName());
    result.setPrimaryKey(table.getPrimaryKey());
    result.setColumnProperties(table.getColumnProperties());
    return result;
}

bool existOnTable(Row row, Table table){
    bool result = false;
    int keyIndex = table.getPrimaryKeyIndex();
    for(const Row &tableRow : table.getRows()){
        if(tableRow.getContents()[keyIndex]==row.getContents()[keyIndex]){
            result = true;
            break;
        }
    }
    return result;
}

Table TableUtils::OR(Table table1, Table table2)
{
    Table result = extractTemplate(table1);
    int keyIndex = table1.getPrimaryKeyIndex();
    result.setRows(table1.getRows());
    for(const Row &row : table2.getRows() ){
        if(!existOnTable(row, result)){
            result.insertRow(row);
        }
    }
    return result;
}

Table TableUtils::AND(Table table1, Table table2)
{
    Table result = extractTemplate(table1);
    int keyIndex = table1.getPrimaryKeyIndex();
    for(const Row &row : table1.getRows() ){
        if(existOnTable(row, table2)){
            result.insertRow(row);
        }
    }
    return result;
}

Table TableUtils::equalTo(Table table, std::string columnName, std::string value)
{
    Table result = extractTemplate(table);
    int index = result.getColumnIndex(columnName);
    if(index != -1){
        for(const Row &row: table.getRows()){
            if(row.getContents()[index]==value){
                result.insertRow(row);
            }
        }
    }
    return result;
}





Table lessThanInt(Table table, std::string columnName, std::string value)
{
    int valueI;
    Table result = TableUtils::extractTemplate(table);

    int index = result.getColumnIndex(columnName);
    if(index != -1){

        for(const Row &row: table.getRows()){
            if( std::stoi(row.getContents()[index]) < valueI ){
                result.insertRow(row);
            }
        }
    }
    return result;


}
Table lessThanDouble(Table table, std::string columnName, std::string value)
{
    double valueD;
    Table result = TableUtils::extractTemplate(table);

    int index = result.getColumnIndex(columnName);
    if(index != -1){
        for(const Row &row: table.getRows()){
            if( std::stod(row.getContents()[index]) < valueD ){
                result.insertRow(row);
            }
        }
    }
    return result;
}
Table lessThanString(Table table, std::string columnName, std::string value)
{
    Table result = TableUtils::extractTemplate(table);

    int index = result.getColumnIndex(columnName);
    if(index != -1){
        for(const Row &row: table.getRows()){
            if( row.getContents()[index].compare(value) < 0 ){
                result.insertRow(row);
            }
        }
    }
    return result;
}

Table TableUtils::lessThan(Table table, std::string columnName, std::string value)
{
    Table result;
    int index = table.getColumnIndex(columnName);
    if(index != -1){
        int type = table.getColumnProperties()[index].getType();
        if(type == INT)
            result = lessThanInt( table,  columnName,  value);
        else if (type == DOUBLE)
            result = lessThanDouble( table, columnName,  value);
        else if (type == STRING)
            result = lessThanString( table,  columnName,  value);

    }
    return result;
}




Table greaterThanInt(Table table, std::string columnName, std::string value)
{
    int valueI;
    Table result = TableUtils::extractTemplate(table);

    int index = result.getColumnIndex(columnName);
    if(index != -1){

        for(const Row &row: table.getRows()){
            if( std::stoi(row.getContents()[index]) > valueI ){
                result.insertRow(row);
            }
        }
    }
    return result;


}
Table greaterThanDouble(Table table, std::string columnName, std::string value)
{
    double valueD;
    Table result = TableUtils::extractTemplate(table);

    int index = result.getColumnIndex(columnName);
    if(index != -1){
        for(const Row &row: table.getRows()){
            if( std::stod(row.getContents()[index]) > valueD ){
                result.insertRow(row);
            }
        }
    }
    return result;
}
Table greaterThanString(Table table, std::string columnName, std::string value)
{
    Table result = TableUtils::extractTemplate(table);

    int index = result.getColumnIndex(columnName);
    if(index != -1){
        for(const Row &row: table.getRows()){
            if( row.getContents()[index].compare(value) > 0 ){
                result.insertRow(row);
            }
        }
    }
    return result;
}



Table TableUtils::greaterThan(Table table, std::string columnName, std::string value)
{
    Table result;
    int index = table.getColumnIndex(columnName);
    if(index != -1){
        int type = table.getColumnProperties()[index].getType();
        if(type == INT)
            result = greaterThanInt( table,  columnName,  value);
        else if (type == DOUBLE)
            result = greaterThanDouble( table, columnName,  value);
        else if (type == STRING)
            result = greaterThanString( table,  columnName,  value);

    }
    return result;
}

Table TableUtils::subTable(Table table, std::vector<std::string> columnsNames, std::vector<int> rowsIndexes)
{
    Table result = extractTemplate(table);
    if(!rowsIndexes.empty()){
        result.setRows(table.getRows(rowsIndexes));
    }
    else{
        result.setRows(table.getRows());
    }

    if(!columnsNames.empty()){
        for(const ColumnProperties &columnProperty : result.getColumnProperties()){
            bool exists= false;
            for(const std::string &columnName: columnsNames){
                if(columnProperty.getName()==columnName){
                    exists=true;
                    break;
                }
            }
            if(!exists){
                result.removeColumn(columnProperty.getName());
            }
        }
    }
    return result;
}
