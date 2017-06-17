#include "where.h"

Comparator::Comparator()
{

}

Comparator::Comparator(std::string tableName, std::string columName, std::string operatorType, std::string value) {
    this->tableName = tableName;
    this->columnName = columName;
    this->operatorType = operatorType;
    this->value = value;
}

std::string Comparator::getValue() const
{
    return value;
}

void Comparator::setValue(const std::string &newValue)
{
    value = newValue;
}

std::string Comparator::getOperatorType() const
{
    return operatorType;
}

void Comparator::setOperatorType(const std::string &value)
{
    operatorType = value;
}

std::string Comparator::getColumnName() const
{
    return columnName;
}

void Comparator::setColumnName(const std::string &value)
{
    columnName = value;
}

std::string Comparator::getTableName() const
{
    return tableName;
}

void Comparator::setTableName(const std::string &value)
{
    tableName = value;
}


Where::Where()
{

}

Where::Where(std::string condition, std::vector<Comparator> comparators)
{
    setCondition(condition);
    setComparators(comparators);
}

std::string Where::getCondition() const
{
    return condition;
}

void Where::setCondition(const std::string &value)
{
    condition = value;
}

std::vector<Comparator> Where::getComparators() const
{
    return comparators;
}

void Where::setComparators(const std::vector<Comparator> &value)
{
    comparators = value;
}
