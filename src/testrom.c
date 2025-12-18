#include <stdio.h>
#include <stdint.h>

#include "ram/ram.h"
#include "rom/rom.h"
#include "misc/debug.h"

#define START_ADDRESS 0x200     // adresse standard CHIP-8 pour charger les ROM
#define MAX_ROM_SIZE  4096 - START_ADDRESS

int testrom()
{
    printf("=== Test chargement ROM CHIP-8 ===\n");

    // 1) Création de la RAM
    RAM *ram = ram_create();
    if (ram == NULL) {
        printf("Erreur : impossible de creer la RAM\n");
        return 1;
    }

    // 2) Charger la ROM
    const char *rom_path = "C:\\Users\\ARYAN\\CodeLiteProjects\\CHIP8-WORKSPACE\\1-chip8-logo.ch8";   // adresse absolu du fichier binaire .ch8

    if (rom_in_load(rom_path, ram, START_ADDRESS) != 0) {
        printf("Erreur lors du chargement de la ROM\n");
        ram_destroy(ram);
        return 1;
    }
    
    printf("ROM chargee depuis : %s\n\n", rom_path);


    // 3) Lecture et affichage des instructions
    printf("--- Instructions decodees ---\n\n");
    printf("@mem| instruction en hexadecimal|        decodage provided\n"); //titre des colonnes

    for (uint16_t addr = START_ADDRESS; addr < 4096; addr += 2)
    {
        uint8_t hi = ram_read(ram, addr);
        uint8_t lo = ram_read(ram, addr + 1);

        // si la ROM est terminée (00 00 → instruction vide), on arrête
        if (hi == 0 && lo == 0)
            break;

        uint16_t instr = (hi << 8) | lo; //décalage vers gauche de hi de 8 bits pour avoir "hi et lo" sur 16 octé côté à côté 

        const char *decoded = instruction_as_str(instr);

        
        printf("%03X |           %04X            | %s\n", //%X pour afficher les uint16_t en hexadecimal 
               addr,
               instr,
               decoded);
    }

    // 4) Nettoyage
    ram_destroy(ram);

    printf("\n=== Fin du programme ===\n");

    return 0;
}
