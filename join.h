//
// Created by jimena on 16/06/17.
//

#ifndef MASTERENGINE_JOIN_H
#define MASTERENGINE_JOIN_H

#include <iostream>

class Join{
public:
    Join(std::string externalTable, std::string externalColumn, std::string internalColumn);

private:
    std::string externalTable;
    std::string externalColumn;
    std::string internalColumn;
};

Join::Join(std::string externalTable, std::string externalColumn, std::string internalColumn) {
    this->externalTable = externalTable;
    this->externalColumn = externalColumn;
    this->internalColumn = internalColumn;
}

#endif //MASTERENGINE_JOIN_H
