

#include "ble_c.h"
#include "Adafruit_BluefruitLE_SPI.h"
// #include "Adafruit_BLE.h"

#ifdef __cplusplus
extern "C" {
#endif

static Adafruit_BluefruitLE_SPI *ble = NULL;


void ble_New() {
    if (ble == NULL) {
        // throw it on the stack we dont need heap runtime issues
        ble = new Adafruit_BluefruitLE_SPI(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, -1);
    }
}

void ble_print(char *s){
    ble->print(s);
}
void ble_println_array(char a[]){
    ble->print(a);
}
void ble_println_str(char *s){
    ble->println(s);
}
void ble_waitForOK(void){
    ble->waitForOK();
}
void ble_begin(bool v){
    ble->begin(v);
}
bool ble_factoryReset(){
    return ble->factoryReset();

}
void ble_echo(bool v){
    ble->echo(v);
}

bool ble_setName() {
  return ble->sendCommandCheckOK(F( "AT+GAPDEVNAME=Yek Keyboard" ));
    
}
bool ble_setHIDEn() {
  return ble->sendCommandCheckOK(F( "AT+BleHIDEn=On" ));

}
bool ble_setKeyboardOn() {
  return ble->sendCommandCheckOK(F( "AT+BleKeyboardEn=On"  ));
}
bool ble_isVersionAtLeast(){
  return ble->isVersionAtLeast(MINIMUM_FIRMWARE_VERSION);
}

void ble_reset(void){
  ble->reset();
}


#ifdef __cplusplus
}
#endif

