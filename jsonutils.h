
#ifndef JSONUTILS_H
#define JSONUTILS_H
//#include "JSON Library/json.hpp"
#include "JSON library/json.hpp"
#include "table.h"
#include "where.h"
#include "join.h"
using json = nlohmann::json;

namespace JSONutils {
json tableToJson(Table table);
Table jsonToTable(json inputJson);
Where jsonToWhere(json inputJson);
Join jsonToJoin(json inputJson);
Comparator jsonToComparator(json inputJson);
}

#endif // JSONUTILS_H
