#include "processeur/processeur.h"

void cpu_cycle(CPU *cpu)
{
    uint8_t high, low;

    /* ===== FETCH ===== */
    high = ram_read(cpu->ram, cpu->PC);
    low = ram_read(cpu->ram, cpu->PC + 1);

    uint16_t opcode = ((uint16_t)high << 8) | low;
    printf("opcode= %4X\n", opcode);

    /* instruction suivante par défaut */
    cpu->PC += 2;

    /* ===== DECODE / EXECUTE ===== */
    switch (opcode & 0xF000) {

    /* ================= 0x00E0 / 0x00EE ================= */
    case 0x0000:
        if (opcode == 0x00E0) {                 /* CLS */
            Display_CLS(cpu->display);
        }
        else if (opcode == 0x00EE) {            /* RET */
            cpu->PC = cpu->stack[--cpu->SP];
        }
        break;

    /* ================= 1nnn : JP ================= */
    case 0x1000:
        cpu->PC = opcode & 0x0FFF;
        break;

    /* ================= 2nnn : CALL ================= */
    case 0x2000:
        cpu->stack[cpu->SP++] = cpu->PC;
        cpu->PC = opcode & 0x0FFF;
        break;

    /* ================= 3xkk : SE ================= */
    case 0x3000: {
        uint8_t x  = (opcode & 0x0F00) >> 8;
        uint8_t kk =  opcode & 0x00FF;
        if (cpu->Vx[x] == kk)
            cpu->PC += 2;
        break;
    }

    /* ================= 6xkk : LD ================= */
    case 0x6000: {
        uint8_t x = (opcode & 0x0F00) >> 8;
        cpu->Vx[x] = opcode & 0x00FF;
        break;
    }

    /* ================= 7xkk : ADD ================= */
    case 0x7000: {
        uint8_t x = (opcode & 0x0F00) >> 8;
        cpu->Vx[x] += opcode & 0x00FF;
        break;
    }

    /* ================= Annn : LD I ================= */
    case 0xA000:
        cpu->I = opcode & 0x0FFF;
        break;

    /* ================= Dxyn : DRW ================= */
    case 0xD000: {
        
        uint8_t x = (opcode & 0x0F00) >> 8;
        uint8_t y = (opcode & 0x00F0) >> 4;
        uint8_t n =  opcode & 0x000F;

        struct Sprite sprite;
        Sprite_init(&sprite, n);

        for (uint8_t i = 0; i < n; i++) {
            uint8_t byte;
            byte = ram_read(cpu->ram, cpu->I + i);
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

    /* ================= Fx29 : FONT ================= */
    case 0xF000: {
        uint8_t x = (opcode & 0x0F00) >> 8;
        if ((opcode & 0x00FF) == 0x29) {
            cpu->I = cpu->Vx[x] * 5;   /* 5 octets par sprite */
        }
        break;
    }

    default:
        printf("inscruction ignore\n");
        /* instruction ignorée */
        break;
    }
}
