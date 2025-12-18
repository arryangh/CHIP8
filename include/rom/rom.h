#ifndef ROM_H 
#define ROM_H 

#include <stdint.h> 
#include <ram/ram.h> 

/**
 * @brief 
 * @param filename
 * @param ram
 * @param start_adress
 * @return 
 */
int rom_in_load(const char *filename, RAM *ram, uint16_t start_address);

#endif 
