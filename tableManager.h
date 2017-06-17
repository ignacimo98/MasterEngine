//
// Created by Ignacio Mora on 6/16/17.
//

#ifndef MASTERENGINE_TABLEMANAGER_H
#define MASTERENGINE_TABLEMANAGER_H


#include <vector>

#include "table.h"

class TableManager {
public:
    TableManager();
    Table getTable(std::string name);
    bool exists(std::string name);
    bool addTable(Table table);

private:
    std::vector<Table> tableList;

};


#endif //MASTERENGINE_TABLEMANAGER_H
