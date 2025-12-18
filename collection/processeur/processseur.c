#include "processeur/processeur.h"

void cpu_cycle(CPU *cpu)
{
    uint8_t high, low;

    /* ================= FETCH ================= */
    high = ram_read(cpu->ram, cpu->PC);
    low  = ram_read(cpu->ram, cpu->PC + 1);

    uint16_t opcode = ((uint16_t)high << 8) | low;
    printf("opcode= %4X\n", opcode);

    /* instruction suivante par défaut */
    cpu->PC += 2;

    /* ================= DECODE / EXECUTE ================= */
    switch (opcode & 0xF000) {

        /* ===== 00E0 : CLS ===== */
        case 0x0000:
            if (opcode == 0x00E0) {
                Display_CLS(cpu->display);
            }
            break;

        /* ===== 1nnn : JP addr ===== */
        case 0x1000:
            cpu->PC = opcode & 0x0FFF;
            break;

        /* ===== 6xnn : LD Vx, byte ===== */
        case 0x6000: {
            uint8_t x  = (opcode & 0x0F00) >> 8;
            uint8_t nn =  opcode & 0x00FF;
            cpu->Vx[x] = nn;
            break;
        }

        /* ===== Annn : LD I, addr ===== */
        case 0xA000:
            cpu->I = opcode & 0x0FFF;
            break;

        /* ===== Dxyn : DRW ===== */
        case 0xD000: {
            uint8_t x = (opcode & 0x0F00) >> 8;
            uint8_t y = (opcode & 0x00F0) >> 4;
            uint8_t n =  opcode & 0x000F;

            struct Sprite sprite;
            Sprite_init(&sprite, n);

            for (uint8_t i = 0; i < n; i++) {
                uint8_t byte = ram_read(cpu->ram, cpu->I + i);
                Sprite_add(&sprite, byte);
            }

            cpu->Vx[0xF] = 0;
            Display_DRW(cpu->display,
                        &sprite,
                        cpu->Vx[x],
                        cpu->Vx[y],
                        &cpu->Vx[0xF]);

            Sprite_destroy(&sprite);
            break;
        }

        /* ===== opcode ignoré ===== */
        default:
            /* volontairement vide */
            break;
    }
}
