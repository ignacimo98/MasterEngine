#include "join.h"


Join::Join(std::string externalTable, std::string externalColumn, std::string internalColumn) {
    this->externalTable = externalTable;
    this->externalColumn = externalColumn;
    this->internalColumn = internalColumn;
}

std::string Join::getExternalTable() const
{
    return externalTable;
}

void Join::setExternalTable(const std::string &value)
{
    externalTable = value;
}

std::string Join::getExternalColumn() const
{
    return externalColumn;
}

void Join::setExternalColumn(const std::string &value)
{
    externalColumn = value;
}

std::string Join::getInternalColumn() const
{
    return internalColumn;
}

void Join::setInternalColumn(const std::string &value)
{
    internalColumn = value;
}

