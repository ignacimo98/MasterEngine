//
// Created by jimena on 16/06/17.
//
#include "resultCode.h"

resultCode::resultCode(int codeNumber, int numberOfRegisters, std::string codeDescription) {
    this->codeNumber = codeNumber;
    this->codeDescription = codeDescription;
    this->numberOfRegisters = numberOfRegisters;
}

