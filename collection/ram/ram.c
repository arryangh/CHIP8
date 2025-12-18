#include "ram/ram.h"
#include <stdlib.h> 
#include <stdint.h>

RAM* ram_create(){
    //err1: création de RAM échoue
    RAM* ram = malloc(RAM_SIZE * sizeof(uint8_t)); //RAM_SIZE: constante défini dans ram.h
    if (ram == NULL){
        printf("ram_create(): malloc échoué pour créer le struct RAM");
        return NULL;
    }
    
    //err2: création de memory pour RAM échoue
    ram->memory = malloc(RAM_SIZE);
    if (ram->memory == NULL){
        free(ram); //remonter et libérer struct RAM
        printf("ram_create(): malloc échoué pour créer le memory pour struct RAM");
    }
    
    //Si tout va bien; remplir le tableau "memory" avec que des 0
    for (int i=0; i< RAM_SIZE; i++){
        ram->memory[i] = 0; //chaque uint8_t = 0 => 0000 0000
    }
    
    return ram; //RAM* ram
}


void ram_destroy(RAM *ram){
    if (ram){ // si le struct ram existe
        free(ram->memory); // libérer d'abord son memory
        free(ram); // puis struct ram lui même
    }
}


int ram_write(RAM *ram, uint16_t address, uint8_t value){
    ram->memory[address] = value;
    return 0;
}

uint8_t ram_read(RAM *ram, uint16_t address){
    return ram->memory[address];
}