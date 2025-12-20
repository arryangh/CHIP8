#ifndef CPU_H
#define CPU_H

#include <stdio.h>
#include <stdint.h> //pour uint8_t
#include "ram/ram.h"
#include "display/display.h"
#include "keyboard/keyboard.h"
#include "speaker/speaker.h"

void processeurFonction();

#define VX_SIZE 16
#define STACK_SIZE 16

typedef struct {
    uint8_t  Vx[16];
    uint16_t I;
    uint16_t PC;
    
    uint8_t DT;
    uint8_t ST;

    uint16_t stack[STACK_SIZE];
    uint8_t  SP;

    RAM *ram;
    struct Display *display;
    struct Keyboard *keyboard;
    struct Speaker *speaker; //ajoute de speaker
} CPU;


void cpu_cycle(CPU *cpu);


#endif