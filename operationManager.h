//
// Created by jimena on 16/06/17.
//

#ifndef MASTERENGINE_OPERATIONMANAGER_H
#define MASTERENGINE_OPERATIONMANAGER_H

#include "table.h"
#include "resultCode.h"
#include "connectionManager.h"

class OperationManager{
public:
    static Table select(json json);
    static resultCode create();
    static resultCode update();
    static resultCode drop();
    static resultCode deleteT();
    static resultCode insert();
    static resultCode createIndex();
};



#endif //MASTERENGINE_OPERATIONMANAGER_H
