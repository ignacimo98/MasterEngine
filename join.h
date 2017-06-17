//
// Created by jimena on 16/06/17.
//

#ifndef MASTERENGINE_JOIN_H
#define MASTERENGINE_JOIN_H

#include <iostream>

class Join{
public:
    Join(std::string externalTable, std::string externalColumn, std::string internalColumn);

    std::string getExternalTable() const;
    void setExternalTable(const std::string &value);

    std::string getExternalColumn() const;
    void setExternalColumn(const std::string &value);

    std::string getInternalColumn() const;
    void setInternalColumn(const std::string &value);

private:
    std::string externalTable;
    std::string externalColumn;
    std::string internalColumn;
};

#endif //MASTERENGINE_JOIN_H
