/**********************************************
    Program for CS219, takes assembly code from file and processes the action
    the assembly code wishes to execute and outputs the result to the user
    @file main.cpp 
    @author Isaiah McLain 
    @version 1.0 1/25/2022
**********************************************/

#include <iostream>
#include <string>
#include <fstream> 
#include <sstream>
#include <iomanip> 

//Struct that takes the command and two hexadecimal values
struct AssemblyCode {
    std::string command; 
    std::string operand1;
    std::string operand2; 
};

//Struct that holds the hexadeimal operands as decimals for later manipulation
struct HexDigits {
    uint32_t hexVal1;
    uint32_t hexVal2;
};

//Global Constant 
const int AssemblyCodeSize = 50; 

//Prototypes 
uint32_t add(HexDigits &hex);
HexDigits converter(std::string tempOperand1, std::string tempOperand2);
std::string backToHex(uint32_t answer);

/**********************************************
    This function is the main driver of the program, for now it handles
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
    while(!inFile.eof() && size < 50){
        inFile >> commands[size].command >> commands[size].operand1
        >> commands[size].operand2; 
        ++size;
    } 

    inFile.close();

    //Creates HexDigts struct array that will be equal to amount of lines in assembly code
    HexDigits hexes[size]; 

    //Loops through to fill hexes struct with converted hex to decimal values
    for(int i = 0; i < size; i++){
        std::string temp = commands[i].operand1;
        std::string temp2 = commands[i].operand2; 

       hexes[i] = converter(temp, temp2);
    }

    //Loop that will run through each command of file and determine what function
    //Should be called based on that command, handles output as well (for now)
    for(int i = 0; i < size; i++)
        if(commands[i].command == "ADD"){
            HexDigits hex; 
            hex.hexVal1 = hexes[i].hexVal1;
            hex.hexVal2 = hexes[i].hexVal2;
            
            uint32_t answer = add(hex);
            std::string hexAnswer = backToHex(answer);

            std::cout << "-------------------------------------------------------" << std::endl;
            std::cout << "Added hex values: " << commands[i].operand1
            << " + " << commands[i].operand2 << " = " << hexAnswer 
            << std::endl;
            std::cout << std::endl; 
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
HexDigits converter(std::string tempOperand1, std::string tempOperand2){
    //Removes 0x formatting
    tempOperand1.erase(0,2);
    tempOperand2.erase(0,2);

    //Gets length of operands, used in loops
    int length1 = tempOperand1.size();
    int length2 = tempOperand2.size(); 

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

    //Reset values to be used for second operand
    tempHexValue = 0;
    base = 1;

    for(int i = length2 - 1; i>=0; i--){
        if(tempOperand2[i]>='0' && tempOperand2[i]<= '9'){
            tempHexValue += (tempOperand2[i] - 48) * base;
            base = base * 16;
        }
        else{
            tempHexValue += (tempOperand2[i] - 55) * base;
            base = base * 16;
        }
    } 

    hex.hexVal2 = tempHexValue;

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
    
    //Readds hex format of 0x
    std::string hexAnswer = "0x" + temp; 
    return hexAnswer; 
}
