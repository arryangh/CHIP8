#ifndef CPU_H
#define CPU_H

#include <stdio.h>
#include <stdint.h> //pour uint8_t
#include "ram/ram.h"
#include "display/display.h"

void processeurFonction();

#define VX_SIZE 16
#define STACK_SIZE 16

typedef struct {
    uint8_t  Vx[16];
    uint16_t I;
    uint16_t PC;

    uint16_t stack[STACK_SIZE];
    uint8_t  SP;

    RAM *ram;
    struct Display *display;
} CPU;


void cpu_cycle(CPU *cpu);


#endif