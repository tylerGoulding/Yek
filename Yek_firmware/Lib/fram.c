
#include "fram.h"

void fram_begin(void){
  // activate internal pullups for twi.
  // probably not really necessary considering we have external but doesnt hurt
  PORTD |= 0b00000011;
  // initialize the twi for communication
  TWI_Init(TWI_BIT_PRESCALE_1, TWI_BITLENGTH_FROM_FREQ(1, 100000));
}

void fram_write8 (uint16_t framAddr, uint8_t value)
{
  TWI_StartTransmission (0x50, 10);
  TWI_SendByte(framAddr >> 8);
  TWI_SendByte(framAddr & 0xFF);
  TWI_SendByte(value);
  TWI_StopTransmission();
}
uint8_t fram_read8(uint16_t framAddr)
{
  uint8_t Byte1;
  TWI_StartTransmission (0x50, 10);
  TWI_SendByte(framAddr >> 8);
  TWI_SendByte(framAddr & 0xFF);
  TWI_StopTransmission();
  if (TWI_StartTransmission(0xA0 | TWI_ADDRESS_WRITE, 10) == TWI_ERROR_NoError) {
    // Read three bytes, acknowledge after the third byte is received
    TWI_ReceiveByte(&Byte1, true);
    // Must stop transmission afterwards to release the bus
    TWI_StopTransmission();
  }
  return Byte1;
}
