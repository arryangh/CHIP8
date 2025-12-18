#ifndef RAM_H //condition pour assurer un include unique
#define RAM_H



#include <stdio.h>
#include <stdint.h> //pour uint8_t



#define RAM_SIZE 4096       // 4 KB de RAM
#define DEBUT_MEMOIRE 0x200 // 512


typedef struct{
    uint8_t *memory;
}RAM;

/**
 * @brief initialise la RAM à vide
 * @param ram
 */
RAM* ram_create(); //crée un renvoi un RAM (liste de 4096 * uint8_t 



/**
 * @brief écrire dans le ram, à l'adresse unit16_t, la valeur uint8_t
 * @param ram
 * @param address
 * @param value
 */
int ram_write(RAM *ram, uint16_t address, uint8_t value); //écrire dans le RAM (0 si succès, -1 si écheque) 




/**
 * @brief 
 * @param ram
 * @param address
 * @param value
 * @return 
 */
uint8_t ram_read(RAM *ram, uint16_t address);


void ram_destroy(RAM *ram);

#endif


