#ifndef TABLEUTILS_H
#define TABLEUTILS_H
#include "table.h"

namespace TableUtils {
Table tableUnion(Table table1, Table table2);
Table tableIntersection(Table table1, Table table2);
Table tableDifference(Table tableMain, Table tableToRemove);

Table equalTo(Table table, std::string columnName, std::string value);
Table lessThan(Table table, std::string columnName, std::string value);
Table greaterThan(Table table, std::string columnName, std::string value);
Table subTable(Table table, std::vector<std::string> columnsNames =  std::vector<std::string>(), std::vector<int> rowsIndexes = std::vector<int>());
Table extractTemplate(Table table);
}

#endif // TABLEUTILS_H
