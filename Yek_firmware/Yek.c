/*

        Yek: Password Manager
        18-500 ECE Capstone
        Tyler Goulding, Fatema Almeshqab, Matthew Chu

  This is the main source file for Yek. This file contains the main tasks of
  the demo and is responsible for the initial application hardware configuration.

  We relied heavily on LUFA CDC example for our USB stack. 

  Alot of the libraries used were intended for Arduino so while they are all still
  the code of the respective owners, we had to refactor alot of the function calls.


  Yek stores an offboard non-volatile memory and relays them via BLE to a users computer.


  
  We need to include the below copyright for using the LUFA library.

  We would also like to thank: Adafruit (author hathach), for their parts and their BLE library
                               Josh Hawley, for his FPS library
                               Arduino, for their Print class and Stream class
                               Olikraus, for the screen library and compressed fonts
                               Zak Kemble, for a AVR implementation of millis


*/



// u8x8 COPYRIGHT
/*
Copyright (c) 2016, olikraus@gmail.com
All rights reserved.
*/

// LUFA COPYRIGHT
/*
             LUFA Library
     Copyright (C) Dean Camera, 2017.

  dean [at] fourwalledcubicle [dot] com
           www.lufa-lib.org
*/

/** \file
 *
 *  Main source file for the Yek. This file contains the main tasks of
 *  the demo and is responsible for the initial application hardware configuration.
 */

#include "Yek.h"

/** LUFA CDC Class driver interface configuration and state information. This structure is
 *  passed to all CDC Class driver functions, so that multiple instances of the same class
 *  within a device can be differentiated from one another.
 */
			USB_ClassInfo_CDC_Device_t VirtualSerial_CDC_Interface =
				{
					.Config =
						{
							.ControlInterfaceNumber   = INTERFACE_ID_CDC_CCI,
							.DataINEndpoint           =
								{
									.Address          = CDC_TX_EPADDR,
									.Size             = CDC_TXRX_EPSIZE,
									.Banks            = 1,
								},
							.DataOUTEndpoint =
								{
									.Address          = CDC_RX_EPADDR,
									.Size             = CDC_TXRX_EPSIZE,
									.Banks            = 1,
								},
							.NotificationEndpoint =
								{
									.Address          = CDC_NOTIFICATION_EPADDR,
									.Size             = CDC_NOTIFICATION_EPSIZE,
									.Banks            = 1,
								},
						},
				};

/** Standard file stream for the CDC interface when set up, so that the virtual CDC COM port can be
 *  used like any regular character stream in the C APIs.
 */
static FILE USBSerialStream;
// U8X8_SSD1306_128X32_UNIVISION_HW_I2C u8x8(OLED_RESET);
u8x8_t u8x8;
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
int state_center_prev = 1;  
int state_up_curr;            
int state_up_prev = 1;
int state_down_curr;            
int state_down_prev = 1;  
int state_center_temp;
int state_down_temp;
int state_up_temp;

unsigned int debounce_delay = 50;
unsigned int debounce_time_center = 0;  
unsigned int debounce_time_up = 0;  
unsigned int debounce_time_down = 0;  
uint8_t flipState = 0;
uint8_t buttonState;

/** Main program entry point. This routine contains the overall program flow, including initial
 *  setup of all components and the main program loop.
 */
int main(void)
{
	SetupHardware();
	/* Create a regular character stream for the interface so that it can be used with the stdio.h functions */
	CDC_Device_CreateStream(&VirtualSerial_CDC_Interface, &USBSerialStream);
	GlobalInterruptEnable();
	for (;;)
	{
		buttonState = process_buttons();
		if (buttonState == CENTER_LONG_PRESSED) {
			if (mode == UPLOAD) {
			  c_account = 0;  
			  total_accounts = fram_read8(0x0);
			  populateArrays(c_account);
			  updateScreen();
			  mode = RUN;
			  break;
			}
			else if (mode == RUN){
			  mode = UPLOAD;
			  u8x8_ClearDisplay(&u8x8);
			  u8x8_DrawString(&u8x8, 0, 0, "Switch to UPLOAD");
			  break;
			}
		}
	switch(mode) {
	    case UPLOAD:   
	      iByte = -1;
    		if (CDC_Device_BytesReceived(&VirtualSerial_CDC_Interface) > 0) {
	        iByte = CDC_Device_ReceiveByte(&VirtualSerial_CDC_Interface);
	      }
	      switch(iByte)
      	{
        case '0': /* add account */
          u8x8_ClearDisplay(&u8x8);
          u8x8_DrawString(&u8x8, 4, 1, "ADD ACC");   
          addAccount();
          u8x8_ClearDisplay(&u8x8);
          u8x8_DrawString(&u8x8, 4, 1, "Edit Info");
          break;
        case '1': /* remove account */
          u8x8_ClearDisplay(&u8x8);
          u8x8_DrawString(&u8x8, 4, 1, "REM ACC"); 
          removeAccount();
          u8x8_ClearDisplay(&u8x8);
          u8x8_DrawString(&u8x8, 4, 1, "Edit Info");
          break;
        case '2': /* add fingerprint */
          u8x8_ClearDisplay(&u8x8);
          u8x8_DrawString(&u8x8, 4, 1, "ADD FIN");   
          Enroll();
          u8x8_ClearDisplay(&u8x8);
          u8x8_DrawString(&u8x8, 4, 1, "Edit Info");
          break;
        case '3': /* delete All fingerprint */
          u8x8_ClearDisplay(&u8x8);
          u8x8_DrawString(&u8x8, 4, 1, "REM FIN");   
          fps_DeleteAll();
          u8x8_ClearDisplay(&u8x8);
          u8x8_DrawString(&u8x8, 4, 1, "Edit Info");
          break;
        case '4':
          flipState = ~(flipState);
          u8x8_SetFlipMode(&u8x8, flipState);
          u8x8_ClearDisplay(&u8x8);
          u8x8_DrawString(&u8x8, 4, 1, "Edit Info");
        default:
          break;
      }
      break;
	    case RUN:
	      if  (buttonState == UP_PRESSED) { /* (curr_up == LOW) && (old_up == HIGH) */
	        c_account = (c_account + 1) % total_accounts;
	        Delay_MS(25);
	      }
	      else if (buttonState == DOWN_PRESSED) { /* (curr_down == 0) && (old_down != 0) */
	        if (c_account == 0){
	          c_account = total_accounts-1;
	        }
	        else{
	          c_account = (c_account - 1) % total_accounts;
	        }
	        Delay_MS(25);
	      }
	      else if  (buttonState == CENTER_SHORT_PRESSED) {
	        fps_SetLED(true);
	        u8x8_ClearDisplay(&u8x8);
	        u8x8_DrawString(&u8x8, 4, 1, "Press Finger");        
	        /* Time out loop... */
	        uint8_t badCount = 0;
	        for (int count = 0; count < 150; count++) {
	          if (fps_IsPressFinger()) {
	            fps_CaptureFinger(false);
	            uint8_t id = fps_Identify1_N();
	            if (id < 200)
	            {
	              ble_print("AT+BleKeyboard=");
	              ble_println_array(acc_name); /*keys*/
	              Delay_MS(200);
	              ble_print("AT+BleKeyboard=");
	              ble_println_str("\t");
	              ble_waitForOK();
	              ble_print("AT+BleKeyboard=");
	              ble_println_array(acc_pass); /*keys*/
	              ble_waitForOK();
	              ble_print("AT+BleKeyboard=");
	              ble_println_str("\\n\n\\r\n\n"); /*keys*/
	              break;
	            } 
	            else { //if unable to recognize
	              badCount++;
	              if (badCount > 10) {
	                break;
	              }
	            }         
	          }
	        }
	      fps_SetLED(false);
	      updateScreen();
	      }
	      break;
	    default:
	      /* how are you here?     */
	      /* Congrat you broke it. */
	      break;
	  }
		/* Must throw away unused bytes from the host, or it will lock up while waiting for the device */
		CDC_Device_ReceiveByte(&VirtualSerial_CDC_Interface);
		CDC_Device_USBTask(&VirtualSerial_CDC_Interface);
		USB_USBTask();
	}
}

/** Configures the board hardware and chip peripherals for the demo's functionality. */
void SetupHardware(void)
{
	/* Disable watchdog if enabled by bootloader/fuses */
	MCUSR &= ~(1 << WDRF);
	wdt_disable();
	/* Disable clock division */
	clock_prescale_set(clock_div_1);

	/* Hardware Initialization */
	millis_init();
	Buttons_Init();
	setupScreen();
	fram_begin();
	/* grab the total number of account stored in the system */
	total_accounts = fram_read8(0x0);
  fps_New();
	fps_Open();         //send serial command to initialize fps
	setupBLE();
	USB_Init();
}
uint8_t u8x8_hw_i2c(U8X8_UNUSED u8x8_t *u8x8, U8X8_UNUSED uint8_t msg, U8X8_UNUSED uint8_t arg_int, U8X8_UNUSED void *arg_ptr)
{
#ifdef U8X8_HAVE_HW_I2C
  switch(msg)
  {
    case U8X8_MSG_BYTE_SEND:
      TWI_SendByte((uint8_t)*data);  
      break;
    case U8X8_MSG_BYTE_INIT:
      TWI_Init(TWI_BIT_PRESCALE_1, TWI_BITLENGTH_FROM_FREQ(1, 100000));
      break;
    case U8X8_MSG_BYTE_SET_DC:
      break;
    case U8X8_MSG_BYTE_START_TRANSFER:
      TWI_StartTransmission (0x3C, 10);
      break;
    case U8X8_MSG_BYTE_END_TRANSFER:
      TWI_StopTransmission();
      break;
    default:
      return 0;
  }
#endif
  return 1;
}
uint8_t u8x8_no_CB(U8X8_UNUSED u8x8_t *u8x8, U8X8_UNUSED uint8_t msg, U8X8_UNUSED uint8_t arg_int, U8X8_UNUSED void *arg_ptr)
{
  /* the dummy callback will not handle any message and will fail for all messages */
  return 0;
}

void setupScreen() {
  u8x8_Setup(&u8x8, u8x8_d_ssd1306_128x32_univision, u8x8_cad_ssd13xx_i2c, u8x8_hw_i2c, u8x8_no_CB);
  u8x8_InitDisplay(&u8x8);
  u8x8_SetPowerSave(&u8x8, 0);
  u8x8_SetFont(&u8x8,u8x8_font_chroma48medium8_r);
  u8x8_DrawString(&u8x8, 0, 0, "Yek");
  u8x8_DrawString(&u8x8, 0, 1, "18500");
  u8x8_DrawString(&u8x8, 0, 2, "Fatema, Matt,");
  u8x8_DrawString(&u8x8, 0, 3, "and Tyler");
}
void setupBLE() {
  ble_New();

  ble_begin(VERBOSE_MODE);
  if ( FACTORYRESET_ENABLE )
  {
    /* Perform a factory reset to make sure everything is in a known state */
    if ( ! ble_factoryReset() ){
    }
  }
  ble_echo(false);
  ble_setName();
  if (ble_isVersionAtLeast() )
  {
    if ( !ble_setHIDEn()) {
    }
  }else
  {
    if (! ble_setKeyboardOn()) {
    }
  }
  ble_reset();
}

void populateArrays(uint8_t accountNum) {
  memset(type,0,BUFSIZE);
  memset(acc_name,0,BUFSIZE);
  memset(acc_pass,0,BUFSIZE);

  memLoc = (accountNum+1)*512;
  account_format = fram_read8(memLoc);
  // Serial.println(account_format);
  for (i = 1; i < BUFSIZE; i++) {
      framAddr = memLoc + i;
      value = fram_read8(framAddr);
      if (value == '+'){
        break;
      }
      type[i-1] = value;
      
  }
  for (i = 0; i < BUFSIZE; i++) {
      framAddr = memLoc + i + BUFSIZE;
      value = fram_read8(framAddr);
      if (value == '+'){
        break;
      }
      acc_name[i] = value;
  }
  for (i = 0; i < BUFSIZE; i++) {
      framAddr = memLoc + i+ (2*BUFSIZE);
      value = fram_read8(framAddr);
      if (value == '+'){
        break;
      }
      acc_pass[i] = value;
  }  
}

uint8_t process_buttons(void) {

  uint8_t button_case = NONE_PRESSED; 
  //up 
  
  state_up_temp = !(PIND == 0b1111111); 
  if (state_up_temp != state_up_prev) {
    // reset the debouncing timer
    debounce_time_up = millis();
  }

  if ((millis() - debounce_time_up) > debounce_delay) {
    if ((state_up_curr != state_up_temp)) {
      state_up_curr = state_up_temp;
      if (state_up_curr == LOW) {
        button_case = UP_PRESSED;
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
          Delay_MS(10);
          count++;
        }

        if (count > 100) {
          button_case = CENTER_LONG_PRESSED;
        }

        else { 
          button_case = CENTER_SHORT_PRESSED;
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
      }
    }
  }
  
  state_down_prev = state_down_temp;
  
  return button_case;
}

void addAccount() {
  memset(keys,0,BUFSIZE);
  /* Wait till there is some data on the line */
  fread(keys, BUFSIZE,1,&USBSerialStream);
  /* update total account and store in memory */
  total_accounts = (total_accounts + 1)%64;
  fram_write8(0x0, total_accounts); 
  u8x8_ClearDisplay(&u8x8);
  u8x8_DrawString(&u8x8, 0, 0,"Type Rec.");
  Delay_MS(200);
  for (i = 0; i<BUFSIZE; i++) {
    fram_write8((total_accounts)*512+i, keys[i]);
  }
  memset(keys,0,BUFSIZE);
  fread(keys, BUFSIZE,1,&USBSerialStream);
  u8x8_ClearDisplay(&u8x8);
  u8x8_DrawString(&u8x8, 0, 0,"Name Rec.");
    Delay_MS(200);

  for (i = 0; i<BUFSIZE; i++) {
    fram_write8((total_accounts)*512+i+128, keys[i]);
  }
  memset(keys,0,BUFSIZE);
  fread(keys, BUFSIZE,1,&USBSerialStream);

  u8x8_ClearDisplay(&u8x8);
  u8x8_DrawString(&u8x8, 0, 0,"Pass Rec.");
  Delay_MS(200);

  /* send password last - load into third 128 bytes of mem segment*/
  for (i = 0; i<BUFSIZE; i++) {
    fram_write8((total_accounts)*512+i+256, keys[i]);
  }  
}

void removeAccount(){
  /* clear the buffer for good luck */
  memset(keys,0,BUFSIZE);

  /* this is non-blocking but we never had an issue */  
  fread(keys, BUFSIZE,1,&USBSerialStream);

  i = 0;
  uint8_t j = 0;
  /* search all account for a match */
  while (i < total_accounts) {
    memset(type,0,BUFSIZE);
    memLoc = (i+1)*512;
    /* populate type array */
    for (j = 1; j < BUFSIZE; j++) {
        framAddr = memLoc + j;
        value = fram_read8(framAddr);
        if (value == '+'){
          break;
        }
        type[j-1] = value;
    }
    /* check if names are the same */
    if (compareArray(type,keys)) {
      /* delete the account */
      value = deleteAcc(i);
      }
    i++;
  }
}

/*
 * deleteAcc(a) -> deletes the a'th account and move up the last account
 *
 */
int deleteAcc(uint8_t a){
  memLoc = (a+1)*512;
  for (uint16_t z = 0; z < 512; z++) {
    fram_write8(memLoc + z,0);
  }
  /* lower total account and save to memory */
  total_accounts -= 1;
  fram_write8(0x0, total_accounts);
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
    fram_write8(memLoc,tLoc);    
    for (i = 1; i < BUFSIZE; i++) {
        framAddr = memLoc + i;
        fram_write8(framAddr,type[i-1]);
    }
    for (i = 0; i < BUFSIZE; i++) {
        framAddr = memLoc + i;
        value = fram_read8(framAddr);
    }    
    for (i = 0; i < BUFSIZE; i++) {
        framAddr = memLoc + i + BUFSIZE;
        fram_write8(framAddr,acc_name[i]);
    }
    for (i = 0; i < BUFSIZE; i++) {
        framAddr = memLoc + i+ (2*BUFSIZE);
        fram_write8(framAddr,acc_pass[i]);
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


void Enroll() {
  fps_SetLED(true);

  u8x8_ClearDisplay(&u8x8);
  
  // find open enroll id
  int enrollid = 0;
  bool usedid = true;
  while (usedid == true)
  {
    usedid = fps_CheckEnrolled(enrollid);
    if (usedid == true) enrollid++;
  }
  fps_EnrollStart(enrollid);
  u8x8_DrawString(&u8x8, 0, 1, "Press Finger");
  while (fps_IsPressFinger() == false) Delay_MS(100);
  bool bret = fps_CaptureFinger(true);
  int iret = 0;
  if (bret != false)
  {
    u8x8_ClearDisplay(&u8x8);
    u8x8_DrawString(&u8x8, 0, 1, "Lift Finger");
    fps_Enroll1();
    while (fps_IsPressFinger() == true) Delay_MS(100);
    u8x8_ClearDisplay(&u8x8);
    u8x8_DrawString(&u8x8, 0, 1, "Press Finger");
    while (fps_IsPressFinger() == false) Delay_MS(100);
    bret = fps_CaptureFinger(true);
    if (bret != false)
    {
      u8x8_ClearDisplay(&u8x8);
      u8x8_DrawString(&u8x8, 0, 1, "Lift Finger");
      fps_Enroll2();
      while (fps_IsPressFinger() == true) Delay_MS(100);
      u8x8_ClearDisplay(&u8x8);
      u8x8_DrawString(&u8x8, 0, 1, "Press Finger");
      while (fps_IsPressFinger() == false) Delay_MS(100);
      bret = fps_CaptureFinger(true);
      if (bret != false)
      {
        iret = fps_Enroll3();
        if (iret == 0)
        {
          u8x8_ClearDisplay(&u8x8);
          u8x8_DrawString(&u8x8, 0, 1, "Fingerprint suc-");
          u8x8_DrawString(&u8x8, 0, 2, "cessfully added");
        }
        else
        {
          u8x8_ClearDisplay(&u8x8);
          u8x8_DrawString(&u8x8, 0, 1, "Failed");
        }
      }
      else {
        u8x8_ClearDisplay(&u8x8);
        u8x8_DrawString(&u8x8, 0, 1, "Failed");

      }
    }
    else {
      u8x8_ClearDisplay(&u8x8);
      u8x8_DrawString(&u8x8, 0, 1, "Failed");

    }
  }
  else {
    u8x8_ClearDisplay(&u8x8);
    u8x8_DrawString(&u8x8, 0, 1, "Failed");

  }
  fps_SetLED(false);

}


void updateScreen() {
    u8x8_ClearDisplay(&u8x8);
    if (total_accounts == 0){
      u8x8_DrawString(&u8x8, 0, 1, "No Accounts");
      Delay_MS(25);
      return;
    }
    u8x8_DrawString(&u8x8, 0,0, "Type:");
    u8x8_DrawString(&u8x8, 2, 1, type);
    u8x8_DrawString(&u8x8, 0,2, "Name:");
    u8x8_DrawString(&u8x8, 2, 3, acc_name);
    Delay_MS(25);
}

/** Event handler for the library USB Connection event. */
void EVENT_USB_Device_Connect(void)
{
}

/** Event handler for the library USB Disconnection event. */
void EVENT_USB_Device_Disconnect(void)
{
}

/** Event handler for the library USB Configuration Changed event. */
void EVENT_USB_Device_ConfigurationChanged(void)
{
	bool ConfigSuccess = true;

	ConfigSuccess &= CDC_Device_ConfigureEndpoints(&VirtualSerial_CDC_Interface);

	// LEDs_SetAllLEDs(ConfigSuccess ? LEDMASK_USB_READY : LEDMASK_USB_ERROR);
}

/** Event handler for the library USB Control Request reception event. */
void EVENT_USB_Device_ControlRequest(void)
{
	CDC_Device_ProcessControlRequest(&VirtualSerial_CDC_Interface);
}

/** CDC class driver callback function the processing of changes to the virtual
 *  control lines sent from the host..
 *
 *  \param[in] CDCInterfaceInfo  Pointer to the CDC class interface configuration structure being referenced
 */
void EVENT_CDC_Device_ControLineStateChanged(USB_ClassInfo_CDC_Device_t *const CDCInterfaceInfo)
{
	/* You can get changes to the virtual CDC lines in this callback; a common
	   use-case is to use the Data Terminal Ready (DTR) flag to enable and
	   disable CDC communications in your application when set to avoid the
	   application blocking while waiting for a host to become ready and read
	   in the pending data from the USB endpoints.
	*/
	bool HostReady = (CDCInterfaceInfo->State.ControlLineStates.HostToDevice & CDC_CONTROL_LINE_OUT_DTR) != 0;
  (void) HostReady;
}

