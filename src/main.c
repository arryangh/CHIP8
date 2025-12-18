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
        "C:\\Users\\ARYAN\\CodeLiteProjects\\CHIP8-WORKSPACE\\1-chip8-logo.ch8",
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

    while (1) {
        cpu_cycle(&cpu);
        Display_update(&display);
        SDL_Delay(2);
    }
    
    
    // testrom();
    return 0;
}


