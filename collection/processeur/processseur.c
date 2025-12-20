#include "processeur/processeur.h"



static uint64_t last_timer_tick = 0;





void cpu_cycle(CPU *cpu)
{
    uint8_t high, low;

    /* ================= FETCH ================= */
    high = ram_read(cpu->ram, cpu->PC);
    low  = ram_read(cpu->ram, cpu->PC + 1);

    uint16_t opcode = ((uint16_t)high << 8) | low;
    //printf("opcode= %4X\n", opcode);
    printf("%4X ", opcode);

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


        /* ===== 6xkk : LD Vx, byte ===== */
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
        case 0xD000: { // DRW
            uint8_t x = (opcode & 0x0F00) >> 8;
            uint8_t y = (opcode & 0x00F0) >> 4;
            uint8_t n =  opcode & 0x000F;

            // Attendre le prochain V-Blank (60 Hz)
            static uint64_t last_draw = 0;
            uint64_t now = SDL_GetTicks64();
            
            if (now - last_draw < 16) {  // 16ms = 60 Hz
                SDL_Delay(16 - (now - last_draw));
            }
            last_draw = SDL_GetTicks64();

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
                    cpu->Vx[0XF] = 0; // pour valider vF RESET TEST du rom6 
                    break;
                }
                
                case 2: { //8xy2 = AND Vx,Vy = Vx prend "Vx & Vy"
                    cpu->Vx[x] &= cpu->Vx[y];
                    cpu->Vx[0XF] = 0; // pour valider vF RESET TEST du rom6
                    break;
                }
                
                case 3: { //8xy3 = XOR Vx,Vy = Vx prend "Vx & Vy"
                    cpu->Vx[x] ^= cpu->Vx[y];
                    cpu->Vx[0XF] = 0; // pour valider vF RESET TEST du rom6
                    break;
                }
                
                case 4: { // 8xy4 : ADD Vx, Vy
                    uint8_t vx = cpu->Vx[x];
                    uint8_t vy = cpu->Vx[y];
                    uint16_t sum = vx + vy;
                    cpu->Vx[x]   = sum & 0xFF;      // Résultat d'abord
                    cpu->Vx[0xF] = (sum > 0xFF);    // Flag ensuite
                    break;
                }
  
             
                case 5: { // 8xy5 : SUB Vx, Vy
                    uint8_t vx = cpu->Vx[x];
                    uint8_t vy = cpu->Vx[y];
                    cpu->Vx[x]   = vx - vy;         // Résultat d'abord
                    cpu->Vx[0xF] = (vx >= vy);      // NOT borrow ensuite
                    break;
                }   
 
                case 6: { //8xy6 = SHR Vx, Vy = shift right (CHIP-8 original)
                    uint8_t lsb = cpu->Vx[y] & 0x01;  // Bit perdu de Vy
                    cpu->Vx[x] = cpu->Vx[y] >> 1;     // Vx = Vy décalé à droite
                    cpu->Vx[0xF] = lsb;                // Flag avec le bit perdu
                }  
                
                case 7: {
                    uint8_t vx = cpu->Vx[x];
                    uint8_t vy = cpu->Vx[y];
                    cpu->Vx[x]   = vy - vx;         // Résultat d'abord
                    cpu->Vx[0xF] = (vy >= vx);      // NOT borrow ensuite
                    break;
                } 
                
                case 0xE: { //8xyE = SHL Vx, Vy = shift left (CHIP-8 original)
                    uint8_t msb = (cpu->Vx[y] & 0x80) >> 7;  // Bit perdu de Vy
                    cpu->Vx[x] = cpu->Vx[y] << 1;             // Vx = Vy décalé à gauche
                    cpu->Vx[0xF] = msb;                       // Flag avec le bit perdu
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
                    cpu->I += x + 1; //pour valider MEMORY TEST de rom6
                    
                    break;
                }
                
                case 0x55: { //Fx55 = LRI Vx = load register at location I
                    for (int i=0; i<=x; i++){
                        ram_write(cpu->ram, cpu->I + i, cpu->Vx[i]); //de Vx[0] à Vx[x] est chargé par ce qu'il y a dans mémoire à l'adresse I+0 à I+x
                        
                    }                    
                    cpu->I += x + 1; //pour valider MEMORY TEST de rom6
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
                
                case 0x0A: { // Fx0A = LD Vx, K
                    uint8_t key;

                    if (Keyboard_wait(cpu->keyboard, &key) == 0) { //bloque le thread et les timers qui se mettent à jours en bas sont gelés
                        cpu->Vx[x] = key;
                    }

                    break;
                }
                
                        
                case 0x18: // Fx18 : LRS Vx = Load Vx into Sound Timer 
                    cpu->ST = cpu->Vx[x];
                    break;
                    
                case 0x07: // Fx07 : LTR Vx = Load Delay Timer into Vx 
                    cpu->Vx[x] = cpu->DT;
                    break;
                    
                case 0x15: // Fx15 : LRT Vx = Load Vx into Delay Timer 
                    cpu->DT = cpu->Vx[x];
                    break;

                case 0x29: //Fx29 : LSI Vx = Load sprite address for digit Vx into I
                    cpu->I = cpu->Vx[x] * 5;  // chaque sprite 5 octets (0-F)
                    break;
     
     
                default:
                    break;
            }
            
            
            break;
        }
        
        case 0xE000: {
            uint8_t x = (opcode & 0x0F00) >> 8;
            uint8_t key = cpu->Vx[x];
            int state;

            switch (opcode & 0x00FF) {

                /* Ex9E : SKP Vx */
                case 0x9E:
                    if (Keyboard_get(cpu->keyboard, key, &state) == 0 &&
                        state == KEY_DOWN) {
                        cpu->PC += 2;
                    }
                    break;

                /* ExA1 : SKNP Vx */
                case 0xA1:
                    if (Keyboard_get(cpu->keyboard, key, &state) == 0 &&
                        state == KEY_UP) {
                        cpu->PC += 2;
                    }
                    break;

                default:
                    break;
            }
            break;
        }
        
        case 0xC000: {  // RND Vx, kk
            uint8_t x  = (opcode & 0x0F00) >> 8;
            uint8_t kk = opcode & 0x00FF;

            uint8_t rnd = rand() % 256;  // générer un entier aléatoire 0..255
            cpu->Vx[x] = rnd & kk;

            break;
        }
        
        /* ===== Bnnn : JP V0, addr ===== */
        case 0xB000: {
            cpu->PC = (opcode & 0x0FFF) + cpu->Vx[0];
            break;
        }
        
        
        
        
        
        default:
            break;   
    }
    
    
    
    
     // pour le timer, 60Hz, on décremente CPU.DT 60 fois par seconde
    uint64_t now = SDL_GetTicks64();
    
    
    if (now - last_timer_tick >= 14) { // ≈ 60 Hz
        if (cpu->DT > 0)
            cpu->DT--;

        if (cpu->ST > 0)
            cpu->ST--;

        last_timer_tick = now;
    }

}
