//
// Created by Hollan on 5/28/24.
//

#ifndef JASON_OPERATOREXCEPTION_H
#define JASON_OPERATOREXCEPTION_H

#include <iostream>

class OperatorException : public std::logic_error
{
public:
    explicit OperatorException(char oper, bool found = true) :
                        std::logic_error(found ?
                                         "The operator '" + std::string(1, oper) + "' is not allowed in this context." :
                                         "The operator '" + std::string(1, oper) + "' is not found.")
    {

    }
    OperatorException(char oper, const std::string& operand1, const std::string& operand2) :
                            std::logic_error("The operator '" + std::string(1, oper) +
                            "' is not allowed on '" + operand1 + "' and '" + operand2 + '\'')
    {

    }
    OperatorException(char oper, const std::string& operand1, const std::string& operand2, const std::string& why) :
            std::logic_error("The operator '" + std::string(1, oper) +
                             "' is not allowed on '" + operand1 + "' and '" + operand2 + "' because '" + why + '\'')
    {

    }
};

#endif //JASON_OPERATOREXCEPTION_H