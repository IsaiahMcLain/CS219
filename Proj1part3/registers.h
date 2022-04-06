#ifndef REGISTERS_H
#define REGISTERS_H

#include<iostream>

//Object used to act as registers for assembly code
struct Registers{
    uint32_t r0{0};
    uint32_t r1{0};
    uint32_t r2{0};
    uint32_t r3{0};
    uint32_t r4{0};
    uint32_t r5{0};
    uint32_t r6{0};
    uint32_t r7{0};
};

#endif //REGISTERS_H