/*
      YEK - the Password Manager
   By Tyler Goulding and Fatema Almeshqab

   Source for our capstone 18500 undergraduate project

*/
#include <Arduino.h>
#include <Wire.h>
#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "FPS_GT511C3.h"
#include "Adafruit_FRAM_I2C.h"
#include "yek.h"
#include <U8x8lib.h>

/* these are all of our peripherals */
U8X8_SSD1306_128X32_UNIVISION_HW_I2C u8x8(OLED_RESET);
Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, -1);
FPS_GT511C3 fps;
Adafruit_FRAM_I2C fram = Adafruit_FRAM_I2C();

/* define our globals */
char account_format = TAB_F;
uint16_t framAddr = 0;
/* initialize values for our accounts */
uint8_t total_accounts = 0; /* number of accounts*/
uint8_t c_account = 0;
uint8_t o_account = 0;
uint8_t value;
uint8_t i;
uint8_t iByte;
char keys[BUFSIZE];
uint8_t mode = RUN;
char type [BUFSIZE];
char acc_name [BUFSIZE];
char acc_pass [BUFSIZE];
uint16_t memLoc = 0;
/* button macros */
int state_center_curr;            
int state_center_prev = HIGH;  
int state_up_curr;            
int state_up_prev = HIGH;
int state_down_curr;            
int state_down_prev = HIGH;  
int state_center_temp;
int state_down_temp;
int state_up_temp;
uint8_t flipState = 0;
unsigned int debounce_delay = 50;
unsigned int debounce_time_center = 0;  
unsigned int debounce_time_up = 0;  
unsigned int debounce_time_down = 0;  
uint8_t buttonState;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  setupScreen();
  fram.begin();
  /* grab the total number of account stored in the system */
//  fram.write8(0,0);
  total_accounts = fram.read8(0x0);
  delay(2000);
  fps.Open();         //send serial command to initialize fps
  setupBLE();
  DDRD &= 0b00011111;
  PORTD |= 0b11100000;
  populateArrays(c_account);
  delay(1500);
  u8x8.setFlipMode(flipState);
  updateScreen();
}

void loop() {
  buttonState = process_buttons();
  if (buttonState == CENTER_LONG_PRESSED) {
    if (mode == UPLOAD) {
      c_account = 0;  
      total_accounts = fram.read8(0x0);
      populateArrays(c_account);
      updateScreen();
      mode = RUN;
      return;
    }
    else if (mode == RUN){
      mode = UPLOAD;
      u8x8.clear();
      u8x8.drawString(4, 1, "Edit Info");
      // Serial.println(total_accounts);
      return;
    }
  }
  switch(mode) {
    case UPLOAD:   
      uint8_t e;
      iByte = -1;
      if (Serial.available() > 0) {
        iByte = Serial.read();
        
      }
      switch(iByte)
      {
        case '0': /* add account */
          u8x8.clear();
          u8x8.drawString(4, 1, "ADD ACC");   

          addAccount();
          u8x8.clear();
          u8x8.drawString(4, 1, "Edit Info");

          break;
        case '1': /* remove account */
          u8x8.clear();
          u8x8.drawString(4, 1, "REM ACC"); 
          removeAccount();
          u8x8.clear();
          u8x8.drawString(4, 1, "Edit Info");
          break;
        case '2': /* add fingerprint */
          u8x8.clear();
          u8x8.drawString(4, 1, "ADD FIN");   
          Enroll();
          u8x8.clear();
          u8x8.drawString(4, 1, "Edit Info");
          break;
        case '3': /* delete All fingerprint */
          u8x8.clear();
          u8x8.drawString(4, 1, "REM FIN");   
//          break; 
//         Serial.println("CANT DELETE, WE DONT HAVE A FPS");
          fps.DeleteAll();
          u8x8.clear();
          u8x8.drawString(4, 1, "Deleted"); 
          delay(500);
          u8x8.clear();
          u8x8.drawString(4, 1, "Edit Info");
          break;
        case '4':
          flipState = ~(flipState);
          u8x8.setFlipMode(flipState);
          u8x8.clear();
          u8x8.drawString(4, 1, "Edit Info");
        default:
          if (iByte != 255){
            // Serial.println(iByte);
          }
          break;
      }
      break;
    case RUN:
      if  (buttonState == UP_PRESSED) { /* (curr_up == LOW) && (old_up == HIGH) */
        c_account = (c_account + 1) % total_accounts;
        delay(50);
      }
      else if (buttonState == DOWN_PRESSED) { /* (curr_down == 0) && (old_down != 0) */
        if (c_account == 0){
          c_account = total_accounts-1;
        }
        else{
          c_account = (c_account - 1) % total_accounts;
        }
        delay(50);
      }
      else if  (buttonState == CENTER_SHORT_PRESSED) {
        
//        return;      
//        delay(100);
        fps.SetLED(true);
        u8x8.clear();
        u8x8.drawString(4, 1, "Press Finger");        
        /* Time out loop... */
        uint8_t badCount = 0;
        
        for (int count = 0; count < 100; count++) {
          if (fps.IsPressFinger()) {
            u8x8.drawString(4, 2, "is Finger");        
  
            fps.CaptureFinger(false);
            byte id = fps.Identify1_N();
            u8x8.drawString(4, 3, id);        

            if (id < 200)
            {
              ble.print("AT+BleKeyboard=");
              ble.println(acc_name); /*keys*/
              ble.waitForOK();
              delay(200);
              if (account_format == ENTER_F) {
                ble.print("AT+BleKeyboard=");
                ble.println("\\n\n\\r\n\n");  
                ble.waitForOK();
                delay(2500);

              } else {
                ble.print("AT+BleKeyboard=");
                ble.println("\\t");  
                ble.waitForOK();
              }
              ble.print("AT+BleKeyboard=");
              ble.println(acc_pass); /*keys*/
              ble.waitForOK();
              ble.print("AT+BleKeyboard=");
              ble.println("\\n\n\\r\n\n"); /*keys*/
              break;
            } 
            else { //if unable to recognize
              badCount++;
              if (badCount > 10) {
                // Serial.println("FAILED");
                break;
              }
            }         
          }
        }
      fps.SetLED(false);
      updateScreen();
      }
      break;
    default:
      /* how are you here?     */
      /* Congrat you broke it. */
      break;
  }
  if (c_account != o_account){
    memLoc = (c_account+1)*512;
    populateArrays(c_account);
    updateScreen();
  }
  o_account = c_account;
}

void Enroll() {
  fps.SetLED(true);
  Serial.println("in enroll");

  u8x8.clear();
  
  // find open enroll id
  int enrollid = 0;
  bool usedid = true;
  while (usedid == true)
  {
    usedid = fps.CheckEnrolled(enrollid);
    if (usedid == true) enrollid++;
  }
  fps.EnrollStart(enrollid);
  Serial.println("in enroll");
  u8x8.drawString(0, 1, "Press Finger");
  while (fps.IsPressFinger() == false) delay(100);
  bool bret = fps.CaptureFinger(true);
  int iret = 0;
  if (bret != false)
  {
    u8x8.clear();
    u8x8.drawString(0, 1, "Lift Finger");
    fps.Enroll1();
    while (fps.IsPressFinger() == true) delay(100);
    u8x8.clear();
    u8x8.drawString(0, 1, "Press Finger");
    while (fps.IsPressFinger() == false) delay(100);
    bret = fps.CaptureFinger(true);
    if (bret != false)
    {
      u8x8.clear();
      u8x8.drawString(0, 1, "Lift Finger");
      fps.Enroll2();
      while (fps.IsPressFinger() == true) delay(100);
      u8x8.clear();
      u8x8.drawString(0, 1, "Press Finger");
      while (fps.IsPressFinger() == false) delay(100);
      bret = fps.CaptureFinger(true);
      if (bret != false)
      {
        iret = fps.Enroll3();
        if (iret == 0)
        {
          u8x8.clear();
          u8x8.drawString(0, 1, "Fingerprint suc-");
          u8x8.drawString(0, 2, "cessfully added");
           Serial.println(0);
        }
        else
        {
          u8x8.clear();
          u8x8.drawString(0, 1, "Failed");
           Serial.println(1);

        }
      }
      else {
        u8x8.clear();
        u8x8.drawString(0, 1, "Failed");
         Serial.println(1);

      }
    }
    else {
      u8x8.clear();
      u8x8.drawString(0, 1, "Failed");
       Serial.println(1);

    }
  }
  else {
    u8x8.clear();
    u8x8.drawString(0, 1, "Failed");
     Serial.println(1);

  }
  fps.SetLED(false);

}

void updateScreen() {
    u8x8.clear();
    if (total_accounts == 0){
      u8x8.drawString(0,1, "No Accounts");
      delay(25);
      return;
    }
    u8x8.setFont(u8x8_font_chroma48medium8_r);
    u8x8.drawString(0,0, "Type:");
    u8x8.drawString(2, 1, type);
    u8x8.drawString(0,2, "Name:");

    u8x8.drawString(2, 3, acc_name);
    delay(25);
}

void populateArrays(uint8_t accountNum) {
  memset(type,0,BUFSIZE);
  memset(acc_name,0,BUFSIZE);
  memset(acc_pass,0,BUFSIZE);

  memLoc = (accountNum+1)*512;
  account_format = fram.read8(memLoc);
  Serial.println(account_format);
  for (i = 1; i < BUFSIZE; i++) {
      framAddr = memLoc + i;
      value = fram.read8(framAddr);
      if (value == '+'){
        break;
      }
      type[i-1] = value;
      
  }
  for (i = 0; i < BUFSIZE; i++) {
      framAddr = memLoc + i + BUFSIZE;
      value = fram.read8(framAddr);
      if (value == '+'){
        break;
      }
      acc_name[i] = value;
  }
  for (i = 0; i < BUFSIZE; i++) {
      framAddr = memLoc + i+ (2*BUFSIZE);
      value = fram.read8(framAddr);
      if (value == '+'){
        break;
      }
      acc_pass[i] = value;
  }  
}

void setupScreen() {
  u8x8.begin();
  u8x8.setPowerSave(0);
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.drawString(0, 0, "Yek");
  u8x8.drawString(0, 1, "18500");
  u8x8.drawString(0, 2, "Fatema, Matt,");
  u8x8.drawString(0, 3, "and Tyler");
}

void setupBLE() {
  ble.begin(VERBOSE_MODE);
  if ( FACTORYRESET_ENABLE )
  {
    /* Perform a factory reset to make sure everything is in a known state */
    // Serial.println(F("Performing a factory reset: "));
    if ( ! ble.factoryReset() ){
        // Serial.println(F("a"));
    }
  }
  ble.echo(false);
  ble.sendCommandCheckOK(F( "AT+GAPDEVNAME=Yek Keyboard" ));
  if ( ble.isVersionAtLeast(MINIMUM_FIRMWARE_VERSION) )
  {
    if ( !ble.sendCommandCheckOK(F( "AT+BleHIDEn=On" ))) {
        // Serial.println(F("b"));
    }
  }else
  {
    if (! ble.sendCommandCheckOK(F( "AT+BleKeyboardEn=On"  ))) {
        // Serial.println(F("c"));
    }
  }
  ble.reset();
}

void FAKE_addAccount() {
  memset(keys,0,BUFSIZE);
  /* Wait till there is some data on the line */
  while(Serial.available() == 0){}
  uint8_t e = Serial.readBytes(keys, BUFSIZE);
  if ((e == 0)||(total_accounts == 63)){ /* if nothing is read, send back 1 to computer */
     Serial.println(1);
    return;
  }
  /* update total account and store in memory */
  u8x8.clear();
  u8x8.drawString(0, 0,keys);
  Serial.println(0);
 
  memset(keys,0,BUFSIZE);
  while(Serial.available() == 0){}
  e = Serial.readBytes(keys, BUFSIZE);
  if (e == 0){
     Serial.println(1);
    return;
  }
  u8x8.clear();
  u8x8.drawString(0, 0,keys);
  Serial.println(0);

  memset(keys,0,BUFSIZE);
  while(Serial.available() == 0){}
  e = Serial.readBytes(keys, BUFSIZE);
  if (e == 0){
    Serial.println(1);
    return;
  }
  /* send password last - load into third 128 bytes of mem segment*/
  u8x8.clear();
  u8x8.drawString(0, 0,keys);
  Serial.println("0");
}

void addAccount() {
  memset(keys,0,BUFSIZE);
  /* Wait till there is some data on the line */
  while(Serial.available() == 0){}
  uint8_t e = Serial.readBytes(keys, BUFSIZE);
  if ((e == 0)||(total_accounts == 63)){ /* if nothing is read, send back 1 to computer */
     Serial.println("1");
    return;
  }
  /* update total account and store in memory */
  total_accounts = (total_accounts + 1)%64;
  fram.write8(0x0, total_accounts); 

  u8x8.clear();
  u8x8.drawString(0, 0,"Type Rec.");
  delay(200);
  for (i = 0; i<BUFSIZE; i++) {
    fram.write8((total_accounts)*512+i, keys[i]);
  }
  memset(keys,0,BUFSIZE);
  while(Serial.available() == 0){}
  e = Serial.readBytes(keys, BUFSIZE);
  if (e == 0){
     Serial.println(1);
    return;
  }
  u8x8.clear();
  u8x8.drawString(0, 0,"Name Rec.");
    delay(200);

  for (i = 0; i<BUFSIZE; i++) {
    fram.write8((total_accounts)*512+i+128, keys[i]);
  }
  Serial.println(0);
  memset(keys,0,BUFSIZE);
  while(Serial.available() == 0){}
  e = Serial.readBytes(keys, BUFSIZE);
  
  if (e == 0){
     Serial.println(1);
    return;
  }
  u8x8.clear();
  u8x8.drawString(0, 0,"Pass Rec.");
    delay(200);

  /* send password last - load into third 128 bytes of mem segment*/
  for (i = 0; i<BUFSIZE; i++) {
    fram.write8((total_accounts)*512+i+256, keys[i]);
  }  
  // Serial.print("password added: ");
  // Serial.println(keys);      
   Serial.println(0);
}

void removeAccount(){
  /* clear the buffer for good luck */
  memset(keys,0,BUFSIZE);
  /* spin till there is data on the Serial line */
  while(Serial.available() == 0){}
  uint8_t e = Serial.readBytes(keys, BUFSIZE);
  if (e == 0){ /* if nothing is read, send back 1 to computer */
    // Serial.println(1);
    return;
  }
  i = 0;
  uint8_t j = 0;
  /* search all account for a match */
  while (i < total_accounts) {
    memset(type,0,BUFSIZE);
    memLoc = (i+1)*512;
    /* populate type array */
    for (j = 1; j < BUFSIZE; j++) {
        framAddr = memLoc + j;
        value = fram.read8(framAddr);
        if (value == '+'){
          break;
        }
        type[j-1] = value;
    }
    /* check if names are the same */
    if (compareArray(type,keys)) {
      /* delete the account */
      value = deleteAcc(i);
      
      Serial.println(0);
    }
    i++;
  }
 Serial.println(1);

}

/*
 * deleteAcc(a) -> deletes the a'th account and move up the last account
 *
 */
int deleteAcc(uint8_t a){
  memLoc = (a+1)*512;
  // Serial.print("deleting: ");
  // Serial.println(a);
  for (uint16_t z = 0; z < 512; z++) {
    fram.write8(memLoc + z,0);
  }
  // Serial.println("Done clearing fram");
  /* lower total account and save to memory */
  total_accounts -= 1;
  fram.write8(0x0, total_accounts);
  // Serial.println("Total accounts is now: ");
  // Serial.print(total_accounts);

  /* we are deleting the last account so we are done */
  if (a == (total_accounts)){
    return 1;
  }
  /* we are not deleting the last account */
  else{ 
    /* grab the data from the current last account */
    populateArrays(total_accounts);
    memLoc = (a+1)*512;
    uint16_t tLoc = (total_accounts+1)*512;
    /* copy it into the recently cleared account 
     * This causes a change in order but prevents fragmentation
     */
    fram.write8(memLoc,tLoc);    
    for (i = 1; i < BUFSIZE; i++) {
        framAddr = memLoc + i;
        fram.write8(framAddr,type[i-1]);
    }
    for (i = 0; i < BUFSIZE; i++) {
        framAddr = memLoc + i;
        value = fram.read8(framAddr);
    }    
    for (i = 0; i < BUFSIZE; i++) {
        framAddr = memLoc + i + BUFSIZE;
        fram.write8(framAddr,acc_name[i]);
    }
    for (i = 0; i < BUFSIZE; i++) {
        framAddr = memLoc + i+ (2*BUFSIZE);
        fram.write8(framAddr,acc_pass[i]);
    } 
    return 0; 
  }
}


/*
 * compareArray(a,b) -> returns 1 if same, return 0 if different
 *
 */
int compareArray(char* a, char* b){
    for (uint8_t j = 0; j < BUFSIZE; j++) {
      if (a[j] != b[j]){
        return 0;
      }
    }
    return 1;  
}


uint8_t process_buttons() {

  uint8_t button_case = NONE_PRESSED; 
  //up 
  
  state_up_temp = !(PIND == 0b1111111); 
//  // Serial.println(PIND,BIN);
  if (state_up_temp != state_up_prev) {
    // reset the debouncing timer
    debounce_time_up = millis();
  }

  if ((millis() - debounce_time_up) > debounce_delay) {
    //// Serial.println("up is debounced");
    if ((state_up_curr != state_up_temp)) {
      state_up_curr = state_up_temp;
      //// Serial.println("up is debounced");
      if (state_up_curr == LOW) {
        button_case = UP_PRESSED;
        // Serial.println("UP PRESS");
      }
    }
  }
  state_up_prev = state_up_temp;
  //center 
  state_center_temp = !(PIND == 0b10111111); 
  
   if (state_center_temp != (state_center_prev)) {
    // reset the debouncing timer
    debounce_time_center = millis();
  }

  if ((millis() - debounce_time_center) > debounce_delay) {
    if ((state_center_curr != state_center_temp)) {
      state_center_curr = state_center_temp;

      int count = 0;
      if (state_center_curr == LOW) {
        while (state_center_curr == !(PIND == 0b10111111)) {
          delay(10);
          count++;
        }

        //// Serial.println(count);

        if (count > 100) {
          button_case = CENTER_LONG_PRESSED;
          // Serial.println("CENTER PRESS LONG");
        }

        else { 
          button_case = CENTER_SHORT_PRESSED;
          // Serial.println("CENTER PRESS");
        }
      }
    }
  }
  
  state_center_prev = state_center_temp;

  //down
  state_down_temp = !(PIND == 0b11011111); 
  
   if (state_down_temp != state_down_prev) {
    // reset the debouncing timer
    debounce_time_down = millis();
  }


  if ((millis() - debounce_time_down) > debounce_delay) {
    if ((state_down_curr != state_down_temp)) {
      state_down_curr = state_down_temp;

      if (state_down_curr == LOW) {
        button_case = DOWN_PRESSED;
        // Serial.println("DOWN PRESS");
      }
    }
  }
  
  state_down_prev = state_down_temp;
  
  return button_case;
}


