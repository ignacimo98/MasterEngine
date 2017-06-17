#ifndef TABLEUTILS_H
#define TABLEUTILS_H
#include "table.h"

namespace TableUtils {
Table OR(Table table1, Table table2);
Table AND(Table table1, Table table2);
Table equalTo(Table table, std::string columnName, std::string value);
Table lessThan(Table table, std::string columnName, std::string value);
Table greaterThan(Table table, std::string columnName, std::string value);
}

#endif // TABLEUTILS_H
