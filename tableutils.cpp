#include "tableutils.h"

Table extractTemplate(Table table){
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
