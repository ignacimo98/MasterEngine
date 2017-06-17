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
    std::string getValue() const;
    void setValue(const std::string &newValue);

    std::string getOperatorType() const;
    void setOperatorType(const std::string &value);

    std::string getColumnName() const;
    void setColumnName(const std::string &value);

    std::string getTableName() const;
    void setTableName(const std::string &value);

private:
    std::string tableName;
    std::string columnName;
    std::string operatorType;
    std::string value;
};


class Where{
public:
    Where(std::string condition, std::vector<Comparator> comparators);
    std::string getCondition() const;
    void setCondition(const std::string &value);

    std::vector<Comparator> getComparators() const;
    void setComparators(const std::vector<Comparator> &value);

private:
    std::string condition;
    std::vector<Comparator> comparators;
};
#endif //MASTERENGINE_WHERE_H
