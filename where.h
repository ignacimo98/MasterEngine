//
// Created by jimena on 16/06/17.
//

#ifndef MASTERENGINE_WHERE_H
#define MASTERENGINE_WHERE_H

#include <iostream>
#include <vector>

class Comparator{
public:
    Comparator(std::string tableName, std::string columName, std::string operatorType, std::string value);
private:
    std::string tableName;
    std::string columnName;
    std::string operatorType;
    std::string value;
};

Comparator::Comparator(std::string tableName, std::string columName, std::string operatorType, std::string value) {
    this->tableName = tableName;
    this->columnName = columName;
    this->operatorType = operatorType;
    this->value = value;
}

class Where{
public:
    Where(std::string condition, std::vector<Comparator>);
private:
    std::string condition;
    std::vector<Comparator> comparators;
};
#endif //MASTERENGINE_WHERE_H
