#ifndef ASSEMBLYOPERANDS_H
#define ASSEMBLYOPERANDS_H

#include <string>

//Struct that takes the command and two hexadecimal values
struct AssemblyCode {
    std::string command; 
    std::string operand0;
    std::string operand1;
    std::string operand2; 
};

#endif //ASSEMBLYOPERANDS_H