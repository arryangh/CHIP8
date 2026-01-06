#include "exemple.h"
#include <stdio.h>
#include <stdint.h>
#include "ram/ram.h"
#include "rom/rom.h"
#include "misc/debug.h"
#include "testrom.h"
#include "processeur/processeur.h"
#include "keyboard/keyboard.h"
#include "speaker/speaker.h"




int main(int argc, char *argv[])
{
    RAM *ram = ram_create();

    rom_in_load(
        // "C:\\Users\\ARYAN\\CodeLiteProjects\\CHIP8-WORKSPACE\\.chip8files\\1-chip8-logo.ch8", //01
        // "C:\\Users\\ARYAN\\CodeLiteProjects\\CHIP8-WORKSPACE\\.chip8files\\2-ibm-logo.ch8", //02 
         "C:\\Users\\ARYAN\\CodeLiteProjects\\CHIP8-WORKSPACE\\.chip8files\\3-corax+.ch8",  //03
        // "C:\\Users\\ARYAN\\CodeLiteProjects\\CHIP8-WORKSPACE\\.chip8files\\4-flags.ch8", //04
        // "C:\\Users\\ARYAN\\CodeLiteProjects\\CHIP8-WORKSPACE\\.chip8files\\5-quirks.ch8", //05
        // "C:\\Users\\ARYAN\\CodeLiteProjects\\CHIP8-WORKSPACE\\.chip8files\\6-keypad.ch8", //06
        // "C:\\Users\\ARYAN\\CodeLiteProjects\\CHIP8-WORKSPACE\\.chip8files\\7-beep.ch8", //07
        // "C:\\Users\\ARYAN\\CodeLiteProjects\\CHIP8-WORKSPACE\\.chip8files\\8-scrolling.ch8", //08
        // "C:\\Users\\ARYAN\\CodeLiteProjects\\CHIP8-WORKSPACE\\.chip8files\\Maze (alt) [David Winter, 199x].ch8", //maze game
        // "C:\\Users\\ARYAN\\CodeLiteProjects\\CHIP8-WORKSPACE\\.chip8files\\games\\Tic-Tac-Toe [David Winter].ch8", 
        // "C:\\Users\\ARYAN\\CodeLiteProjects\\CHIP8-WORKSPACE\\.chip8files\\games\\Tetris [Fran Dachille, 1991].ch8",
        //"C:\\Users\\ARYAN\\CodeLiteProjects\\CHIP8-WORKSPACE\\.chip8files\\games\\Vertical Brix [Paul Robson, 1996].ch8",
        //"C:\\Users\\ARYAN\\CodeLiteProjects\\CHIP8-WORKSPACE\\.chip8files\\games\\ZeroPong [zeroZshadow, 2007].ch8",
        ram,
        DEBUT_MEMOIRE
    );

    struct Display display;
    Display_init(&display, 10);
    Display_set_colors(&display, 0, 0, 0, 0, 255, 0);
    
    struct Keyboard keyboard;
    Keyboard_init(&keyboard);
    
    //ajoute de speaker 
    struct Speaker speaker; 
    Speaker_init(&speaker);
    
    
    
    // instentiation CPU
    CPU cpu = {0};
    cpu.ram = ram;
    cpu.display = &display;
    cpu.PC = DEBUT_MEMOIRE;
    cpu.SP = 0;
    cpu.keyboard = &keyboard;
    cpu.speaker = &speaker; // pour le son

    int running = 1;
    SDL_Event event;
    
    int i = 0;
    
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
        
        // pour affichage propre des insturctions en hexadecimal dans le terminal
        i++;
        if (i%24 == 0){ // nouvelle ligne toutes les 24 instructions
            printf("\n");
        }
            

        /* ===== AFFICHAGE ===== */
        Display_update(&display);

        //SDL_Delay(1);
    }
    
    // nettoyage à la fin du programme (libération mémoire)

    Display_destroy(&display);
    Keyboard_destroy(&keyboard);
    ram_destroy(ram);

    return 0;
}

