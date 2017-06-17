//
// Created by Ignacio Mora on 6/16/17.
//

#include "tableManager.h"


TableManager::TableManager() {

}

Table TableManager::getTable(std::string name) {
    int i;
    for (i = 0; i < tableList.size(); i++){
        if (tableList[i].getName() == name)
            return tableList[i];
    }
    return Table();
}

bool TableManager::addTable(Table table) {
    if (!exists(table.getName())) {
        tableList.push_back(table);
        return true;
    }
    return false;
}

bool TableManager::exists(std::string name) {
    int i;
    for (i = 0; i < tableList.size(); i++){
        if (tableList[i].getName() == name)
            return true;
    }
    return false;
}

int TableManager::deleteTable(std::string name) {
    int rowsAffected = 0;
    if (exists(name)){
        int i;
        for(i = 0; i < tableList.size(); i++){
            if (tableList[i].getName() == name){
                break;
            }
        }
        rowsAffected = tableList[i].getTotalRows();
        tableList.erase(tableList.begin() + i);
    }
    return rowsAffected;
}

void TableManager::updateRow(std::string name, std::vector<std::string> row) {
    getTable(name).insertRow(row);
}



