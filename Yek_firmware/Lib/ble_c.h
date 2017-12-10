#ifndef _BLE_C_H_
#define _BLE_C_H_

// #include "Adafruit_ATParser.h"
#define VERBOSE_MODE                   true  // If set to 'true' enables debug output
#define BLUEFRUIT_SPI_CS               8
#define BLUEFRUIT_SPI_IRQ              7
#define BLUEFRUIT_SPI_RST              4    // Optional but recommended, set to -1 if unused
#define FACTORYRESET_ENABLE         0
#define MINIMUM_FIRMWARE_VERSION    "0.6.6"


#ifdef __cplusplus
extern "C" {
#endif




typedef struct Adafruit_BluefruitLE_SPI Adafruit_BluefruitLE_SPI;

void ble_New(void);
void ble_print(char *s);
void ble_println_array(char a[]);
void ble_println_str(char *s);
void ble_waitForOK(void);
void ble_begin(bool v);
bool ble_factoryReset(void);
void ble_echo(bool v);
bool ble_setName(void);
bool ble_setHIDEn(void);
bool ble_setKeyboardOn(void);
bool ble_isVersionAtLeast(void);
void ble_reset(void);
#ifdef __cplusplus
}
#endif


#endif