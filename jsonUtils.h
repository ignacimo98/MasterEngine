//
// Created by Ignacio Mora on 6/16/17.
//

#ifndef MASTERENGINE_JSONUTILS_H
#define MASTERENGINE_JSONUTILS_H

#include "JSON Library/json.hpp"
#include "table.h"
using json = nlohmann::json;

namespace JSONutils {
    json tableToJson(Table table);
    Table jsonToTable(json inputJson);
}

#endif //MASTERENGINE_JSONUTILS_H
