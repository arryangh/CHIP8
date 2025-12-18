#include "processeur/processeur.h"

void cpu_cycle(CPU *cpu)
{
    uint8_t high, low;

    /* ================= FETCH ================= */
    high = ram_read(cpu->ram, cpu->PC);
    low  = ram_read(cpu->ram, cpu->PC + 1);

    uint16_t opcode = ((uint16_t)high << 8) | low;
    // printf("opcode= %4X\n", opcode);

    /* instruction suivante par défaut */
    cpu->PC += 2;

    /* ================= DECODE / EXECUTE ================= */
    switch (opcode & 0xF000) {

        /* ===== 00E0 : CLS ===== */
        case 0x0000:{
            if (opcode == 0x00E0) {
                Display_CLS(cpu->display);
            } else if (opcode == 0x00EE) {
                /* RET - Retour de sous-programme */
                cpu->SP--;
                cpu->PC = cpu->stack[cpu->SP];
            }
            break;
        }


        /* ===== 6xnn : LD Vx, byte ===== */
        case 0x6000: {
            uint8_t x  = (opcode & 0x0F00) >> 8;
            uint8_t nn =  opcode & 0x00FF;
            cpu->Vx[x] = nn;
            break;
        }

        /* ===== Annn : LD I, addr ===== */
        case 0xA000:{
            cpu->I = opcode & 0x0FFF;
            break;
        }
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
        
        /* ===== 1nnn : JP addr ===== */
        case 0x1000:
            cpu->PC = opcode & 0x0FFF;
            break;
            
        
        /* ===== 7xnn : ADV Vx, nn ===== */ //registre Vx recoit Vx+nn
        case 0x7000: {
            uint8_t x  = (opcode & 0x0F00) >> 8;
            uint8_t nn =  opcode & 0x00FF;
            cpu->Vx[x] += nn; //très important le "+=": Vx = Vx += nn
            break;
        }
        
        
        /* ===== 3xnn : SEQ Vx,kk ===== */ //"skip if equal" = saute la prochaine instruction si le Vx[x]==kk
        case 0x3000: {
            uint8_t x  = (opcode & 0x0F00) >> 8;
            uint8_t nn =  opcode & 0x00FF;
            if (cpu->Vx[x] == nn){
                cpu->PC+=2; //instruction = 16bit = 2octets on saut alors 2 octets
            }
            break;
        }


        /* ===== 4xnn : SNE Vx,kk ===== */ //"skip if not equal" = saute la prochaine instruction si le Vx[x]!=kk
        case 0x4000: {
            uint8_t x  = (opcode & 0x0F00) >> 8;
            uint8_t nn =  opcode & 0x00FF;
            if (cpu->Vx[x] != nn){
                cpu->PC+=2; //instruction = 16bit = 2octets on saut alors 2 octets
            }
            break;
        }


        /* ===== 5xy0 : SRE Vx,Vy ===== */ //"skip if registers equal" =  saute laprochaine instruction si le registre Vx contient la même valeur que le registre Vy
        case 0x5000: {
            if ((opcode & 0x000F) != 0) break; // sécurité

            uint8_t x = (opcode & 0x0F00) >> 8;
            uint8_t y = (opcode & 0x00F0) >> 4;

            if (cpu->Vx[x] == cpu->Vx[y]) {
                cpu->PC += 2;
            }
            break;
        }

        
        /* ===== 9xy0 : SRN Vx,Vy ===== */ //"skip if registers not equal" =  saute la prochaine instruction si le registre Vx != Vy
        case 0x9000: {
            if ((opcode & 0x000F) != 0) break; // sécurité

            uint8_t x = (opcode & 0x0F00) >> 8;
            uint8_t y = (opcode & 0x00F0) >> 4;

            if (cpu->Vx[x] != cpu->Vx[y]) {
                cpu->PC += 2;
            }
            break;
        }

        /* ===== 2nnn : CAL addr ===== */
        case 0x2000:
            cpu->stack[cpu->SP] = cpu->PC;  // sauvegarde adresse de retour
            cpu->SP++;
            cpu->PC = opcode & 0x0FFF;      // saut vers le sous-programme
            break;
            
            
            
        /* ===== 8xy_ : SRE Vx,Vy ===== */ //"skip if registers equal" =  saute laprochaine instruction si le registre Vx contient la même valeur que le registre Vy
        case 0x8000: {
            uint8_t x = (opcode & 0x0F00) >> 8;
            uint8_t y = (opcode & 0x00F0) >> 4;
            uint8_t n = (opcode & 0x000F); //distinguer 8xy0, 8xy1, 8xy2, ...
            
            switch (n){
                case 0: { //8xy0 = LRR Vx,Vy = load register in register Vx prend la valeur de Vy
                    cpu->Vx[x] = cpu->Vx[y];
                    break;
                }
                
                case 1: { //8xy1 = OR Vx,Vy = Vx prend "Vx | Vy"
                    cpu->Vx[x] |= cpu->Vx[y];
                    break;
                }
                
                case 2: { //8xy2 = AND Vx,Vy = Vx prend "Vx & Vy"
                    cpu->Vx[x] &= cpu->Vx[y];
                    break;
                }
                
                case 3: { //8xy3 = XOR Vx,Vy = Vx prend "Vx & Vy"
                    cpu->Vx[x] ^= cpu->Vx[y];
                    break;
                }
                
                case 4: { //8xy4 = ADD Vx,Vy = Vx prend "Vx + Vy"
                    cpu->Vx[x] += cpu->Vx[y];
                    break;
                }   
             
                case 5: { //8xy5 = SUB Vx,Vn = Vx prend "Vx + Vy"
                    cpu->Vx[x] -= cpu->Vx[y];
                    break;
                }   
 
                case 6: { //8xy6 = SHR Vx = shift right: décallage du bit de registre Vx à droite, Vf prend le bit pérdu
                    uint8_t x = (opcode & 0x0F00) >> 8; //Le bit de poids faible est sauvegardé dans VF
                    cpu->Vx[0xF] = cpu->Vx[x] & 0x01;  // VF = bit LSB (bit 0)
                    cpu->Vx[x] = cpu->Vx[x] >> 1; // Décalage à droite de Vx 
                    break;
                }  
                
                case 7: { //8xy7 = SBU Vx,Vy = Vx prend "Vy - Vx"
                    cpu->Vx[0xF] = (cpu->Vx[y] >= cpu->Vx[x]);
                    cpu->Vx[x] = cpu->Vx[y] - cpu->Vx[x];
                    break;
                }  
                
                case 0xE: { //8xyE = SHL Vx = = shift left: décallage du bit de registre Vx à gauche, Vf prend le bit pérdu
                    cpu->Vx[0xF] = (cpu->Vx[x] & 0x80) >> 7;
                    cpu->Vx[x] <<= 1;
                    break;
                }  
                
                default:
                    break;
            }

            break;
        }
        
        case 0xF000: {
            uint8_t x = (opcode & 0x0F00) >> 8;
            uint8_t n = (opcode & 0x00FF); //distinguer Fx65, Fx55, Fx55, Fx33, Fx1E
            
            
            switch(n){
                case 0x65: { //Fx65 = LIR Vx = load from I to register 
                    for (int i=0; i<=x; i++){
                        cpu->Vx[i] = ram_read(cpu->ram, cpu->I + i); //de Vx[0] à Vx[x] est chargé par ce qu'il y a dans mémoire à l'adresse I+0 à I+x
                    }
                    
                    break;
                }
                
                case 0x55: { //Fx55 = LRI Vx = load register at location I
                    for (int i=0; i<=x; i++){
                        ram_write(cpu->ram, cpu->I + i, cpu->Vx[i]); //de Vx[0] à Vx[x] est chargé par ce qu'il y a dans mémoire à l'adresse I+0 à I+x
                    }                    
                    
                    break;
                }
                
                case 0x33: { //Fx33 = CRD Vx = convert register in decimal. ex: 11111111 => 256
                    uint8_t value = cpu->Vx[x];  // Valeur à convertir
    
                    ram_write(cpu->ram, cpu->I + 0, value / 100); // centaines
                    ram_write(cpu->ram, cpu->I + 1, (value / 10) % 10); // dixaines
                    ram_write(cpu->ram, cpu->I + 2, value % 10);  // unités
                    break;
                }
                
                case 0x1E: { //Fx1E = ARI Vx = add register to I: fait I + valeur(Vx[x])
                    cpu->I += cpu->Vx[x];
                    break;
                }           

     
                default:
                    break;
            }
            
            
            break;
        }
        
        
        
        
        
        
        
        default:
            break;   
    }

}
