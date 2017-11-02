#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "Adafruit_BluefruitLE_UART.h"
#include "FPS_GT511C3.h"

#include "BluefruitConfig.h"

#if SOFTWARE_SERIAL_AVAILABLE
  #include <SoftwareSerial.h>
#endif


/* BUTTONS */
#define OLED_RESET 4

#define UP 6
#define CTR 5
#define DOWN 9
#define maxSize 128
#define BUFSIZE 128
#define FACTORYRESET_ENABLE         0
#define MINIMUM_FIRMWARE_VERSION    "0.6.6"
static char *account_type[3] = {"gmail", "twitter", "test_type"};
static char *account_name[3] = {"15500capstone@gmail.com","15500cap","test_name"};
static char *account_pass[3] = {"=yL]-MYN%{P8D>X.%M7BE8#%9[p4hGEjswKse<4G","3>y-GdHw/Fk`2}%\\GRt2*GK#g^","test_password"};

Adafruit_SSD1306 display(OLED_RESET);

Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ,-1);
FPS_GT511C3 fps; // (Arduino SS_RX = pin 4, Arduino SS_TX = pin 5)

int pos = 0;
int old_pos = 0;
char keys[BUFSIZE+1];
int old_up = HIGH;
int old_down = HIGH;
int old_ctr = HIGH;

void error(const __FlashStringHelper*err) {
  Serial.println(err);
  while (1);
}

void setup() {
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
  display.display();
  delay(2000);
  pinMode(UP,INPUT);
  pinMode(CTR,INPUT);  
  pinMode(DOWN,INPUT);
  // Clear the buffer.
  display.clearDisplay();
  display.display();
  fps.Open();         //send serial command to initialize fps

   if ( !ble.begin(VERBOSE_MODE) )
  {
    error(F("Couldn't find Bluefruit, make sure it's in CoMmanD mode & check wiring?"));
  }
  Serial.println( F("OK!") );

  if ( FACTORYRESET_ENABLE )
  {
    /* Perform a factory reset to make sure everything is in a known state */
    Serial.println(F("Performing a factory reset: "));
    if ( ! ble.factoryReset() ){
      error(F("Couldn't factory reset"));
    }
  }

  /* Disable command echo from Bluefruit */
  ble.echo(false);

  Serial.println("Requesting Bluefruit info:");
  /* Print Bluefruit information */
  ble.info();

  /* Change the device name to make it easier to find */
  Serial.println(F("Setting device name to 'Bluefruit Keyboard': "));
  if (! ble.sendCommandCheckOK(F( "AT+GAPDEVNAME=Bluefruit Keyboard" )) ) {
    error(F("Could not set device name?"));
  }

  /* Enable HID Service */
  Serial.println(F("Enable HID Service (including Keyboard): "));
  if ( ble.isVersionAtLeast(MINIMUM_FIRMWARE_VERSION) )
  {
    if ( !ble.sendCommandCheckOK(F( "AT+BleHIDEn=On" ))) {
      error(F("Could not enable Keyboard"));
    }
  }else
  {
    if (! ble.sendCommandCheckOK(F( "AT+BleKeyboardEn=On"  ))) {
      error(F("Could not enable Keyboard"));
    }
  }

  /* Add or remove service requires a reset */
  Serial.println(F("Performing a SW reset (service changes require a reset): "));
  if (! ble.reset() ) {
    error(F("Couldn't reset??"));
  }

  pos = 0;
}



void loop() {
  int curr_up = digitalRead(UP);
  int curr_down = digitalRead(DOWN);
  int curr_ctr = digitalRead(CTR);
  if  ((curr_up == LOW) && (old_up == HIGH)) {
    pos = (pos+1) % 3;
    Serial.println(pos);
    delay(100);
    
  } 
  else if  ((curr_down == LOW) && (old_down == HIGH)) { 
    pos = (pos-1) % 3;
    if (pos < 0){
      pos += 3;
    }
    Serial.println(pos);
    delay(100);

  }
  else if  ((curr_ctr == LOW) && (old_ctr == HIGH)) { 
    fps.SetLED(true);
    delay(1000);
    int badCount = 0;
    for (int count = 0; count < 5000;count++) {
      if (fps.IsPressFinger()) {
        fps.CaptureFinger(false);
        int id = fps.Identify1_N();
        
        if (id <200)
        {//if the fingerprint matches, provide the matching template ID
          memset(keys, 0, maxSize);
      //    sprintf(keys, "%s\t%s\r\n", account_name[pos], account_pass[pos]);
          sprintf(keys, "%s\t", account_name[pos]);
          Serial.print("\nSending ");
          Serial.println(keys);
          ble.print("AT+BleKeyboard=");
          ble.println(keys);
          if( ble.waitForOK() )
          {
            Serial.println( F("OK!") );
          }else
          {
            Serial.println( F("FAILED!") );
          }
          memset(keys, 0, maxSize);
          sprintf(keys, "%s", account_pass[pos]);
          ble.print("AT+BleKeyboard=");
          ble.println(keys);
          memset(keys, 0, maxSize);
          sprintf(keys, "\\n\n\\r\n\n");
          Serial.print("\nSending ");
          Serial.println(keys);
          ble.print("AT+BleKeyboard=");
          ble.println(keys);
          break;
        }
        else
        {//if unable to recognize
          if (badCount++ >15){
            // we've failed 15 times.. probs not there
            break;  
          }
          
          Serial.println("Finger not found");
        }
      }      
  }
  fps.SetLED(false);
}

  old_up = curr_up;
  old_down = curr_down;
  old_ctr = curr_ctr;
  updateScreen();
  old_pos = pos;
}

void updateScreen(){
  if (pos != old_pos){
    display.clearDisplay();
    delay(10);
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.println(account_type[pos]);
    display.println(account_name[pos]);
    display.display();
    delay(100);
  }
}
