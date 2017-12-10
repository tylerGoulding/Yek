/**************************************************************************/
/*!
    @file     Adafruit_BluefruiLE_SPI.h
    @author   hathach, ktown (Adafruit Industries)

    @section LICENSE

    Software License Agreement (BSD License)

    Copyright (c) 2015, Adafruit Industries (adafruit.com)
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
    1. Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
    3. Neither the name of the copyright holders nor the
    names of its contributors may be used to endorse or promote products
    derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ''AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
/**************************************************************************/
#ifndef _ADAFRUIT_BLE_SPI_H_
#define _ADAFRUIT_BLE_SPI_H_

#include "Adafruit_BLE.h"
// #include <SPI.h>
#include "utility/Adafruit_FIFO.h"
#include "../../../../LUFA/Drivers/Peripheral/SPI.h"

#define SCK_P                       PB1
#define SCK_P_PORT                  PORTB
#define SCK_P_DDR                   DDRB
#define SCK_P_PIN                   PINB

#define MOSI_P                      PB2
#define MOSI_P_PORT                 PORTB
#define MOSI_P_DDR                  DDRB
#define MOSI_P_PIN                  PINB 

#define MISO_P                      PB3
#define MISO_P_PORT                 PORTB
#define MISO_P_DDR                  DDRB
#define MISO_P_PIN                  PINB

#define CS_P                        PB4
#define CS_P_PORT                   PORTB
#define CS_P_DDR                    DDRB
#define CS_P_PIN                    PINB

#define IRQ_P                       PE6
#define IRQ_P_PORT                  PORTE
#define IRQ_P_DDR                   DDRE
#define IRQ_P_PIN                   PINE


/* macro functions adapted from http://www.avrfreaks.net/forum/macros-ddr-and-port-pin-name */

#define PIN_HIGH(bit)             {bit ## _PORT |= (1 << bit);}
#define PIN_LOW(bit)              {bit ## _PORT &= ~(1 << bit);}

#define PIN_READ(bit)             (bit ## _PIN & (1 << bit))

#define SET_INPUT(bit)            {bit ## _DDR &= ~(1 << bit);}
#define SET_OUTPUT(bit)           {bit ## _DDR |= (1 << bit);}

#define SPI_CS_ENABLE()           PIN_LOW(CS_P)
#define SPI_CS_DISABLE()          PIN_HIGH(CS_P)

#define SPI_IGNORED_BYTE         0xFEu /**< SPI default character. Character clocked out in case of an ignored transaction. */
#define SPI_OVERREAD_BYTE         0xFFu /**< SPI over-read character. Character clocked out after an over-read of the transmit buffer. */
#define SPI_DEFAULT_Delay_MS         0.05

#define memclr(buffer, size)  memset(buffer, 0, size)


#define word(h,l) (h << 8 | l); 


class Adafruit_BluefruitLE_SPI : public Adafruit_BLE
{
  private:
    // Hardware Pin
    int8_t          m_cs_pin;
    int8_t          m_irq_pin;
    int8_t          m_rst_pin;

    // software SPI pins
    int8_t          m_sck_pin;
    int8_t          m_mosi_pin;
    int8_t          m_miso_pin;

    // TX
    uint8_t         m_tx_buffer[SDEP_MAX_PACKETSIZE];
    uint8_t         m_tx_count;

    // RX
    uint8_t         m_rx_buffer[BLE_BUFSIZE];
    Adafruit_FIFO   m_rx_fifo;

    // Low level transportation I/O functions
    bool    sendInitializePattern(void);
    bool    sendPacket(uint16_t command, const uint8_t* buffer, uint8_t count, uint8_t more_data);
    bool    getPacket(sdepMsgResponse_t* p_response);

    bool    getResponse(void);
    void    simulateSwitchMode(void);
//    bool    handleSwitchCmdInDataMode(uint8_t ch);

    uint8_t spixfer(uint8_t x);
    void spixfer(void *x, size_t len);

  public:
    // Constructor
    Adafruit_BluefruitLE_SPI(int8_t csPin, int8_t irqPin, int8_t rstPin = -1);
    Adafruit_BluefruitLE_SPI(int8_t clkPin, int8_t misoPin, int8_t mosiPin, int8_t csPin, int8_t irqPin, int8_t rstPin);

    // HW initialisation
    bool begin(bool v = false, bool blocking = true);
    void end(void);

    bool setMode(uint8_t new_mode);

    // Class Print virtual function Interface
    virtual size_t write(uint8_t c);
    virtual size_t write(const uint8_t *buffer, size_t size);

    // pull in write(str) and write(buf, size) from Print
    using Print::write;

    // Class Stream interface
    virtual int  available(void);
    virtual int  read(void);
    virtual void flush(void);
    virtual int  peek(void);
};

#endif
