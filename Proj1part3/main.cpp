/**********************************************
    Program for CS219, takes assembly code from file and processes the action
    the assembly code wishes to execute and outputs the result to the user
    @file main.cpp 
    @author Isaiah McLain 
    @version 3.0 3/28/2022
**********************************************/

#include <fstream> 
#include <sstream>
#include <iomanip> 
#include <algorithm>

#include "assemblyoperands.h"
#include "hexdigits.h"
#include "registers.h"

//Global Constant 
const int AssemblyCodeSize = 100; 

//Prototypes 
uint32_t add(HexDigits &hex);
uint32_t sub(HexDigits &hex);
uint32_t invert(HexDigits &hex);
uint32_t ands(HexDigits &hex);
uint32_t ord(HexDigits &hex);
uint32_t xord(HexDigits &hex);
uint32_t lsr(HexDigits &hex);
uint32_t lsl(HexDigits &hex);
uint32_t asr(HexDigits &hex);
void mov(HexDigits &hex, Registers &registers, std::string registercode1);
HexDigits converter(std::string tempOperand1);
std::string backToHex(uint32_t answer);
uint32_t fetcher(std::string register1, Registers &registers);


/**********************************************
    This function is the main driver of the program, it handles
    the loading of the file into the program as well as the function calls
    that manipulate the hexadecimal functions. 
    @param none
    @return int that tells program executed properly 
**********************************************/
int main() {
    std::ifstream inFile("assembly.txt"); 

    if(!inFile){
        std::cerr << "Error opening file." << std::endl; 
    }
    std::cout << "File opended successfully" << std::endl;

    //Creates array of struct to hold multiple operations and operands
    AssemblyCode commands[AssemblyCodeSize];
    int size{}; 

    //Loads file into struct, stops once end of file is reached or size is = to 50.
    while(!inFile.eof() && size < AssemblyCodeSize){
        inFile >> commands[size].command;
        if(commands[size].command == "LSR" || commands[size].command == "NOT"
        || commands[size].command == "LSL" || commands[size].command == "ASR"
        || commands[size].command == "MOV")
        {
            inFile >> commands[size].operand0 >> commands[size].operand1; 
        }
        else{
            inFile >> commands[size].operand0 >> commands[size].operand1 >> commands[size].operand2;
        }
        ++size;
    } 

    inFile.close();

    //Creates HexDigts struct array that will be equal to amount of lines in assembly code
    HexDigits hexes[size]; 

    // //Loops through to fill hexes struct with converted hex to decimal values
    // for(int i = 0; i < size; i++){
    //     std::string temp = commands[i].operand1;
    //     std::string temp2 = commands[i].operand2; 

    //    hexes[i] = converter(temp, temp2);
    // }

    //Creates uninitilized registers object which contains r0-r7 registers to store
    //values and act as voltaile memory 
    Registers registers; 

    //Loop that will run through each command of file and determine what function
    //Should be called based on that command, handles output as well (for now)
    for(int i = 0; i < size; i++)
        if(commands[i].command == "MOV"){
            HexDigits hex; 
            std::string register1; 
            register1 = commands[i].operand0;
            
            std::for_each(register1.begin(), register1.end(), [](char &iTemp){
            iTemp = ::toupper(iTemp);
            });
            
            std::string temp = commands[i].operand1;
            hexes[i] = converter(temp);
            hex.hexVal1 = hexes[i].hexVal1;

            mov(hex, registers, register1);

            std::cout << "-------------------------------------------------------" << std::endl;
            std::cout << "Value moved into register: " << commands[i].operand1
            << " moved to register " << commands[i].operand0 << std::endl;
            std::cout << std::endl; 

        }
        else if(commands[i].command == "ADD"){
            HexDigits hex; 
            
            std::string registerstore = commands[i].operand0;
            std::string register1 = commands[i].operand1;
            std::string register2 = commands[i].operand2;

            std::for_each(register1.begin(), register1.end(), [](char &iTemp){
            iTemp = ::toupper(iTemp);
            });

            std::for_each(register2.begin(), register2.end(), [](char &iTemp){
            iTemp = ::toupper(iTemp);
            });

            std::for_each(registerstore.begin(), registerstore.end(), [](char &iTemp){
            iTemp = ::toupper(iTemp);
            });

            hex.hexVal1 = fetcher(register1, registers); 
            hex.hexVal2 = fetcher(register2, registers); 
            

            uint32_t answer = add(hex);
            hex.hexVal1 = answer; 
            mov(hex, registers, registerstore); 
            std::string hexAnswer = backToHex(answer);

            std::cout << "-------------------------------------------------------" << std::endl;
            std::cout << "Added hex values: " << register1
            << " + " << register2 << " = " << hexAnswer << " in " << registerstore
            << std::endl;
            std::cout << std::endl; 
        }
        else if(commands[i].command == "AND"){
            HexDigits hex; 
            std::string registerstore = commands[i].operand0;
            std::string register1 = commands[i].operand1;
            std::string register2 = commands[i].operand2;

            std::for_each(register1.begin(), register1.end(), [](char &iTemp){
            iTemp = ::toupper(iTemp);
            });

            std::for_each(register2.begin(), register2.end(), [](char &iTemp){
            iTemp = ::toupper(iTemp);
            });

            std::for_each(registerstore.begin(), registerstore.end(), [](char &iTemp){
            iTemp = ::toupper(iTemp);
            });

            hex.hexVal1 = fetcher(register1, registers); 
            hex.hexVal2 = fetcher(register2, registers); 
            
            uint32_t answer = ands(hex);
            hex.hexVal1 = answer; 
            mov(hex, registers, registerstore);
            std::string hexAnswer = backToHex(answer);

            std::cout << "-------------------------------------------------------" << std::endl;
            std::cout << "Anded hex value: " << register1 << " & " << register2
            << " = " << hexAnswer << " in " << registerstore << std::endl;
            std::cout << std::endl; 
        }
        else if(commands[i].command == "ORR"){
            HexDigits hex; 
            std::string registerstore = commands[i].operand0;
            std::string register1 = commands[i].operand1;
            std::string register2 = commands[i].operand2;

            std::for_each(register1.begin(), register1.end(), [](char &iTemp){
            iTemp = ::toupper(iTemp);
            });

            std::for_each(register2.begin(), register2.end(), [](char &iTemp){
            iTemp = ::toupper(iTemp);
            });

            std::for_each(registerstore.begin(), registerstore.end(), [](char &iTemp){
            iTemp = ::toupper(iTemp);
            });

            hex.hexVal1 = fetcher(register1, registers); 
            hex.hexVal2 = fetcher(register2, registers); 
            
            uint32_t answer = ord(hex);
            hex.hexVal1 = answer; 
            mov(hex, registers, registerstore);
            std::string hexAnswer = backToHex(answer);
            std::cout << "-------------------------------------------------------" << std::endl;
            std::cout << "Or'd hex value: " << commands[i].operand1 << " | " << commands[i].operand2 
            << " = " << hexAnswer << " in " << registerstore << std::endl;
            std::cout << std::endl; 
        }
         else if(commands[i].command == "XOR"){
            HexDigits hex; 
            std::string registerstore = commands[i].operand0;
            std::string register1 = commands[i].operand1;
            std::string register2 = commands[i].operand2;

            std::for_each(register1.begin(), register1.end(), [](char &iTemp){
            iTemp = ::toupper(iTemp);
            });

            std::for_each(register2.begin(), register2.end(), [](char &iTemp){
            iTemp = ::toupper(iTemp);
            });

            std::for_each(registerstore.begin(), registerstore.end(), [](char &iTemp){
            iTemp = ::toupper(iTemp);
            });

            hex.hexVal1 = fetcher(register1, registers); 
            hex.hexVal2 = fetcher(register2, registers);
            
            uint32_t answer = xord(hex);
            hex.hexVal1 = answer; 
            mov(hex, registers, registerstore);
            std::string hexAnswer = backToHex(answer);
            std::cout << "-------------------------------------------------------" << std::endl;
            std::cout << "Xor'd hex value: " << commands[i].operand1 << " ^ " << commands[i].operand2 
            << " = " << hexAnswer <<  " in " << registerstore << std::endl;
            std::cout << std::endl; 
        }
        else if(commands[i].command == "NOT"){
            HexDigits hex; 
            std::string registerstore = commands[i].operand0;
            std::string register1 = commands[i].operand1;

            std::for_each(register1.begin(), register1.end(), [](char &iTemp){
            iTemp = ::toupper(iTemp);
            });

            std::for_each(registerstore.begin(), registerstore.end(), [](char &iTemp){
            iTemp = ::toupper(iTemp);
            });

            hex.hexVal1 = fetcher(register1, registers);
            
            uint32_t answer = invert(hex);
            hex.hexVal1 = answer; 
            mov(hex, registers, registerstore);
            std::string hexAnswer = backToHex(answer);

            std::cout << "-------------------------------------------------------" << std::endl;
            std::cout << "Inverted hex value: " << commands[i].operand1
            << " = " << hexAnswer << " in " << registerstore << std::endl;
            std::cout << std::endl; 
        }
        else if(commands[i].command == "LSR"){
            HexDigits hex; 
            std::string registerstore = commands[i].operand0;
            std::string register1 = commands[i].operand1;

            std::for_each(register1.begin(), register1.end(), [](char &iTemp){
            iTemp = ::toupper(iTemp);
            });

            std::for_each(registerstore.begin(), registerstore.end(), [](char &iTemp){
            iTemp = ::toupper(iTemp);
            });

            hex.hexVal1 = fetcher(register1, registers);
            
            uint32_t answer = lsr(hex);
            hex.hexVal1 = answer; 
            mov(hex, registers, registerstore);
            std::string hexAnswer = backToHex(answer);

            std::cout << "-------------------------------------------------------" << std::endl;
            std::cout << "Logical shift right hex value: " << commands[i].operand1
            << " = " << hexAnswer << " in " << registerstore << std::endl;
            std::cout << std::endl; 
        }
        else if(commands[i].command == "LSL"){
            HexDigits hex; 
            std::string registerstore = commands[i].operand0;
            std::string register1 = commands[i].operand1;

            std::for_each(register1.begin(), register1.end(), [](char &iTemp){
            iTemp = ::toupper(iTemp);
            });

            std::for_each(registerstore.begin(), registerstore.end(), [](char &iTemp){
            iTemp = ::toupper(iTemp);
            });

            hex.hexVal1 = fetcher(register1, registers);
            
            uint32_t answer = lsl(hex);
            hex.hexVal1 = answer; 
            mov(hex, registers, registerstore);
            std::string hexAnswer = backToHex(answer);

            std::cout << "-------------------------------------------------------" << std::endl;
            std::cout << "Logical shift left hex value: " << commands[i].operand1
            << " = " << hexAnswer << " in " << registerstore << std::endl;
            std::cout << std::endl; 
        }
        else if(commands[i].command == "ASR"){
            HexDigits hex; 
            std::string registerstore = commands[i].operand0;
            std::string register1 = commands[i].operand1;

            std::for_each(register1.begin(), register1.end(), [](char &iTemp){
            iTemp = ::toupper(iTemp);
            });

            std::for_each(registerstore.begin(), registerstore.end(), [](char &iTemp){
            iTemp = ::toupper(iTemp);
            });

            hex.hexVal1 = fetcher(register1, registers);
            
            uint32_t answer = asr(hex);
            hex.hexVal1 = answer; 
            mov(hex, registers, registerstore);
            std::string hexAnswer = backToHex(answer);

            std::cout << "-------------------------------------------------------" << std::endl;
            std::cout << "Arithmetic shift right hex value: " << commands[i].operand1
            << " = " << hexAnswer << " in " << registerstore << std::endl;
            std::cout << std::endl; 
        }
        else if(commands[i].command == "SUB"){
            HexDigits hex; 
            std::string registerstore = commands[i].operand0;
            std::string register1 = commands[i].operand1;
            std::string register2 = commands[i].operand2;

            std::for_each(register1.begin(), register1.end(), [](char &iTemp){
            iTemp = ::toupper(iTemp);
            });

            std::for_each(register2.begin(), register2.end(), [](char &iTemp){
            iTemp = ::toupper(iTemp);
            });

            std::for_each(registerstore.begin(), registerstore.end(), [](char &iTemp){
            iTemp = ::toupper(iTemp);
            });

            hex.hexVal1 = fetcher(register1, registers); 
            hex.hexVal2 = fetcher(register2, registers);
            
            uint32_t answer = sub(hex);
            hex.hexVal1 = answer; 
            mov(hex, registers, registerstore);
            std::string hexAnswer = backToHex(answer);

            std::cout << "-------------------------------------------------------" << std::endl;
            std::cout << "Subtracted hex values: " << commands[i].operand2
            << " - " << commands[i].operand1 << " = " << hexAnswer << " in " << registerstore 
            << std::endl;
            std::cout << std::endl; 
        }
        else{
            std::cout << "Command not recognized" << std::endl;
        }
}


/**********************************************
    This function converts the loaded hexadecimal string into a struct of uint32_t
    so that the program can add the decimal values before converting them back
    to hex. 
    @param std::string tempOperand1, std::string tempOperand2, these are both the 
    operands taken from one line of the file currently in hexadecimal form
    @return HexDigits hex, returns a struct containing both uint32_t decimal values
**********************************************/
HexDigits converter(std::string tempOperand1){
    //Removes 0x formatting
    tempOperand1.erase(0,2);
    //tempOperand2.erase(0,2);

    //Gets length of operands, used in loops
    int length1 = tempOperand1.size();
    //int length2 = tempOperand2.size(); 

    //Used to track what 16^x for conversion, x is base
    int base = 1;
    uint32_t tempHexValue = 0; 

    //For loop steps through each char in string and determines if it's a decimal
    //Or a char and then converts the string into it's decimal counterpart. The -48
    //and -55 are needed as they are used to convert the string's ASCII value into a
    //decimal. The base then scales up increasing the power that the hex is raised to. 
    for(int i = length1 - 1; i>=0; i--){
        if(tempOperand1[i]>='0' && tempOperand1[i]<= '9'){
            tempHexValue += (tempOperand1[i] - 48) * base;
            base = base * 16;
        }
        else{
            tempHexValue += (tempOperand1[i] - 55) * base;
            base = base * 16;
        }
    }

    HexDigits hex; 
    hex.hexVal1 = tempHexValue;

    // //Reset values to be used for second operand
    // tempHexValue = 0;
    // base = 1;

    // for(int i = length2 - 1; i>=0; i--){
    //     if(tempOperand2[i]>='0' && tempOperand2[i]<= '9'){
    //         tempHexValue += (tempOperand2[i] - 48) * base;
    //         base = base * 16;
    //     }
    //     else{
    //         tempHexValue += (tempOperand2[i] - 55) * base;
    //         base = base * 16;
    //     }
    // } 

    // hex.hexVal2 = tempHexValue;

    return hex; 
}

/**********************************************
    This function adds the two hex values together 
    @param HexDigits &hex, takes a struct containing the hexadecimal values
    in decimal form
    @return uint32_t, added decimal value  
**********************************************/
uint32_t add(HexDigits &hex){
    return hex.hexVal1 + hex.hexVal2;
}

/**********************************************
    This function subtracts the two hex values 
    @param HexDigits &hex, takes a struct containing the hexadecimal values
    in decimal form
    @return uint32_t, subtracted decimal value  
**********************************************/
uint32_t sub(HexDigits &hex){
    return hex.hexVal2 - hex.hexVal1;
}

/**********************************************
    This function inverts the hex value
    @param HexDigits &hex, takes a struct containing the hexadecimal value
    in decimal form
    @return uint32_t, inverted decimal value  
**********************************************/
uint32_t invert(HexDigits &hex){
    return ~hex.hexVal1;
}

/**********************************************
    This function does a logical shift right by 1 bit on the hex value
    @param HexDigits &hex, takes a struct containing the hexadecimal value
    in decimal form
    @return uint32_t, logical shift right by 1 bit decimal value  
**********************************************/
uint32_t lsr(HexDigits &hex){
    hex.hexVal1 = hex.hexVal1 >> 1; 
    return hex.hexVal1;
}

/**********************************************
    This function does a logical shift left by 1 bit on the hex value
    @param HexDigits &hex, takes a struct containing the hexadecimal value
    in decimal form
    @return uint32_t, logical shift left by 1 bit decimal value  
**********************************************/
uint32_t lsl(HexDigits &hex){
    hex.hexVal1 = hex.hexVal1 << 1; 
    return hex.hexVal1;
}

/**********************************************
    This function does a arithmetic shift right by 1 bit on the hex value
    @param HexDigits &hex, takes a struct containing the hexadecimal value
    in decimal form
    @return uint32_t, arithmetic shift right by 1 bit decimal value  
**********************************************/
uint32_t asr(HexDigits &hex){
    std::string hexValue{};
    std::string MSB{};
    
    //Takes decial and puts it into string format so I can save MSB
    hexValue = std::to_string(hex.hexVal1); 
    MSB = hexValue.at(0); 

    //Logical Shift right is conducted
    int temp = std::stoi(hexValue); 
    temp = temp >> 1; 

    //The shifted value is put back into string format so I can append the MSB onto the 
    //front so the value is unchanged like an ASR operation does,  
    hexValue = std::to_string(temp); 
    hexValue.at(0) = MSB.at(0); 

    //Put value back into struct so it can be returned
    hex.hexVal1 = std::stoi(hexValue); 
    
    return hex.hexVal1;
}

/**********************************************
    This function ors the two hex values together 
    @param HexDigits &hex, takes a struct containing the hexadecimal values
    in decimal form
    @return uint32_t, ord decimal value  
**********************************************/
uint32_t ord(HexDigits &hex){
    return hex.hexVal1 | hex.hexVal2;
}

/**********************************************
    This function xors the two hex values together 
    @param HexDigits &hex, takes a struct containing the hexadecimal values
    in decimal form
    @return uint32_t, xord decimal value  
**********************************************/
uint32_t xord(HexDigits &hex){
    return hex.hexVal1 ^ hex.hexVal2;
}

/**********************************************
    This function ANDS the two hex values together 
    @param HexDigits &hex, takes a struct containing the hexadecimal values
    in decimal form
    @return uint32_t, anded decimal value  
**********************************************/
uint32_t ands(HexDigits &hex){
    return hex.hexVal1 & hex.hexVal2;
}


/**********************************************
    This function MOVs a hex value into a register
    @param HexDigits &hex, Registers &registers
    @return void (none)
**********************************************/
void mov(HexDigits &hex, Registers &registers, std::string registercode1){
    if(registercode1 == "R0"){
        registers.r0 = hex.hexVal1; 
    }
    else if(registercode1 == "R1"){
        registers.r1 = hex.hexVal1; 
    }
    else if(registercode1 == "R2"){
        registers.r2 = hex.hexVal1; 
    }
    else if(registercode1 == "R3"){
        registers.r3 = hex.hexVal1; 
    }
    else if(registercode1 == "R4"){
        registers.r4 = hex.hexVal1; 
    }
    else if(registercode1 == "R5"){
        registers.r5 = hex.hexVal1; 
    }
    else if(registercode1 == "R6"){
        registers.r6 = hex.hexVal1; 
    }
    else if(registercode1 == "R7"){
        registers.r7 = hex.hexVal1; 
    }
    else{
        std::cout << "That register doesn't exist" << std::endl; 
    }
}

/**********************************************
    This function takes the decimal answer and converts it back to hexadecimal, 
    it also adds the 0x formatting back into the string
    @param uint32_t answer, takes the decimal version of the answer 
    @return std::string hexAnswer, returns the properly formatted hexadecimal answer 
**********************************************/
std::string backToHex(uint32_t answer){
    std::stringstream ss; 
    
    //Turns answer into hex value
    ss << std::hex << answer; 
    std::string temp = ss.str(); 
    
    //Uses for each iterator STL function to step through string and make all 
    //hex digits capital
    std::for_each(temp.begin(), temp.end(), [](char &iTemp){
        iTemp = ::toupper(iTemp);
    });
    
    //Readds hex format of 0x
    std::string hexAnswer = "0x" + temp; 
    return hexAnswer; 
}

/**********************************************
    This function takes the register taken from the file read and then returns
    the assocaited value with that register. 
    @param std::string register1, Registers &registers
    @return uint32_t registerValue
**********************************************/
uint32_t fetcher(std::string register1, Registers &registers){
     if(register1 == "R0"){
        return registers.r0; 
    }
    else if(register1 == "R1"){
        return registers.r1; 
    }
    else if(register1 == "R2"){
        return registers.r2; 
    }
    else if(register1 == "R3"){
        return registers.r3;  
    }
    else if(register1 == "R4"){
        return registers.r4; 
    }
    else if(register1 == "R5"){
        return registers.r5; 
    }
    else if(register1 == "R6"){
        return registers.r6; 
    }
    else if(register1== "R7"){
        return registers.r7;  
    }
    else{
        std::cout << "That register doesn't exist" << std::endl; 
        return 1;
    }
}

