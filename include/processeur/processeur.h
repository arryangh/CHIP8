#ifndef CPU_H
#define CPU_H

#include <stdio.h>
#include <stdint.h> //pour uint8_t
#include "ram/ram.h"
#include "display/display.h"
#include "keyboard/keyboard.h"
#include "speaker/speaker.h"



#define VX_SIZE 16
#define STACK_SIZE 16

typedef struct {
    uint8_t  Vx[16]; //registres généraux (16 * 16bit)
    uint16_t I; // registre d'adresse (16bits)
    uint16_t PC; // compteur de programme (16bits)
    uint8_t DT; // Delay timer (8bits)
    uint8_t ST; // Sound timer (8bits)
    uint16_t stack[STACK_SIZE]; // Pile (16 * 16bits)
    uint8_t  SP;

    RAM *ram; // RAM (4096 * 8bits)
    
    // périphériques
    struct Display *display;
    struct Keyboard *keyboard;
    struct Speaker *speaker; 
} CPU;


void cpu_cycle(CPU *cpu);


#endif