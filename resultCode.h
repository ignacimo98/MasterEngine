//
// Created by jimena on 16/06/17.
//

#ifndef MASTERENGINE_RESULTCODE_H
#define MASTERENGINE_RESULTCODE_H
#include "iostream"


class resultCode{
private:

    int codeNumber;
    int numberOfRegisters;
    std::string codeDescription;

public:
    resultCode(int codeNumber, int numberOfRegisters, std::string codeDescription);

    int getCodeNumber() const {
        return codeNumber;
    }

    void setCodeNumber(int codeNumber) {
        resultCode::codeNumber = codeNumber;
    }

    int getNumberOfRegisters() const {
        return numberOfRegisters;
    }

    void setNumberOfRegisters(int numberOfRegisters) {
        resultCode::numberOfRegisters = numberOfRegisters;
    }

    const std::string &getCodeDescription() const {
        return codeDescription;
    }

    void setCodeDescription(const std::string &codeDescription) {
        resultCode::codeDescription = codeDescription;
    }

};

#endif //MASTERENGINE_RESULTCODE_H
