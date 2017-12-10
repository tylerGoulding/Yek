#ifndef FRAM_I2C_H
#define FRAM_I2C_H
#include <avr/io.h>
#include <stdint.h> 
#include <stdbool.h>
#include "../../../../../LUFA/Drivers/Peripheral/TWI.h"

void fram_begin(void);
void fram_write8 (uint16_t framAddr, uint8_t value);
uint8_t fram_read8(uint16_t framAddr);

#endif