#include "exemple.h"
#include <stdio.h>
#include <stdint.h>
#include "ram/ram.h"
#include "rom/rom.h"
#include "misc/debug.h"
#include "testrom.h"
#include "processeur/processeur.h"



int main(int argc, char *argv[])
{
    RAM *ram = ram_create();

    rom_in_load(
        //"C:\\Users\\ARYAN\\CodeLiteProjects\\CHIP8-WORKSPACE\\.chip8files\\1-chip8-logo.ch8", 01
        // "C:\\Users\\ARYAN\\CodeLiteProjects\\CHIP8-WORKSPACE\\.chip8files\\2-ibm-logo.ch8", 02 
        //"C:\\Users\\ARYAN\\CodeLiteProjects\\CHIP8-WORKSPACE\\.chip8files\\3-corax+.ch8",  03
        "C:\\Users\\ARYAN\\CodeLiteProjects\\CHIP8-WORKSPACE\\.chip8files\\4-flags.ch8",
        ram,
        DEBUT_MEMOIRE
    );

    struct Display display;
    Display_init(&display, 10);
    Display_set_colors(&display, 0, 0, 0, 0, 255, 0);

    CPU cpu = {0};
    cpu.ram = ram;
    cpu.display = &display;
    cpu.PC = DEBUT_MEMOIRE;
    cpu.SP = 0;

    int running = 1;
    SDL_Event event;
    
    //int i = 0;
    
    while (running) {

        /* ===== GESTION DES ÉVÉNEMENTS ===== */
        
        /*
         * le gestionnaire d'évènement SDL attend un évènement 
         * */
        while (SDL_PollEvent(&event)) { // pour éviter le bug de curseur de la souris (logo chargement)
            if (event.type == SDL_QUIT) {
                running = 0;
            }
        }

        /* ===== CPU ===== */
        cpu_cycle(&cpu);
        //i++;
        // printf("i=%d", i);

        /* ===== AFFICHAGE ===== */
        Display_update(&display);

        SDL_Delay(2);
    }

    /* ===== NETTOYAGE ===== */
    Display_destroy(&display);
    ram_destroy(ram);

    return 0;
}

