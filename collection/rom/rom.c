#include <stdio.h> 
#include <stdint.h> 
#include "rom/rom.h"
#include "ram/ram.h"



int rom_in_load(const char *filename, RAM *ram, uint16_t start_address){
    FILE *f = fopen(filename, "rb");
    if (!f){
        perror("rom_in_load(): Erreur d'ouverture de ROM");
        return -1;
    }
    
    uint16_t addr = start_address;
    int byte;

    while ((byte = fgetc(f)) != EOF) {
        ram_write(ram, addr, (uint8_t)byte);
        addr++;
    }

    fclose(f);
    return 0;
    
    
    
}