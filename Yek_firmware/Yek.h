/*
             LUFA Library
     Copyright (C) Dean Camera, 2017.

  dean [at] fourwalledcubicle [dot] com
           www.lufa-lib.org
*/

/*
  Copyright 2017  Dean Camera (dean [at] fourwalledcubicle [dot] com)

  Permission to use, copy, modify, distribute, and sell this
  software and its documentation for any purpose is hereby granted
  without fee, provided that the above copyright notice appear in
  all copies and that both that the copyright notice and this
  permission notice and warranty disclaimer appear in supporting
  documentation, and that the name of the author not be used in
  advertising or publicity pertaining to distribution of the
  software without specific, written prior permission.

  The author disclaims all warranties with regard to this
  software, including all implied warranties of merchantability
  and fitness.  In no event shall the author be liable for any
  special, indirect or consequential damages or any damages
  whatsoever resulting from loss of use, data or profits, whether
  in an action of contract, negligence or other tortious action,
  arising out of or in connection with the use or performance of
  this software.
*/

/** \file
 *
 *  Header file for VirtualSerial.c.
 */

#ifndef _YEK_H_
#define _YEK_H_

	/* Includes: */
		#include <avr/io.h>
		#include <avr/wdt.h>
		#include <avr/power.h>
		#include <avr/interrupt.h>
		#include <string.h>
		#include <stdio.h>

		#include "Descriptors.h"
        #include "Lib/fram.h"
        #include "Lib/millis/millis.h"
        #include "Lib/fps_c.h"
        #include "Lib/ble_c.h"
        #include "Lib/uLib/u8x8.h"
        #include "Board/Buttons.h"
		#include <LUFA/Drivers/Board/LEDs.h>
		#include <LUFA/Drivers/Board/Joystick.h>
		#include <LUFA/Drivers/USB/USB.h>
		#include <LUFA/Platform/Platform.h>

	/* Macros: */
		/** LED mask for the library LED driver, to indicate that the USB interface is not ready. */
		#define LEDMASK_USB_NOTREADY      LEDS_LED1

		/** LED mask for the library LED driver, to indicate that the USB interface is enumerating. */
		#define LEDMASK_USB_ENUMERATING  (LEDS_LED2 | LEDS_LED3)

		/** LED mask for the library LED driver, to indicate that the USB interface is ready. */
		#define LEDMASK_USB_READY        (LEDS_LED2 | LEDS_LED4)

		/** LED mask for the library LED driver, to indicate that an error has occurred in the USB interface. */
		#define LEDMASK_USB_ERROR        (LEDS_LED1 | LEDS_LED3)


/* BLUETOOTH macros*/
#define BUFSIZE                        128   // Size of the read buffer for incoming data
#define VERBOSE_MODE                   true  // If set to 'true' enables debug output
#define BLUEFRUIT_SPI_CS               8
#define BLUEFRUIT_SPI_IRQ              7
#define BLUEFRUIT_SPI_RST              4    // Optional but recommended, set to -1 if unused

#define UPLOAD 0
#define RUN 1
#define OLED_RESET 4
#define UP 7
#define CTR 6
#define DOWN 5
#define maxSize 128
#define BUFSIZE 128
#define FACTORYRESET_ENABLE         0
#define MINIMUM_FIRMWARE_VERSION    "0.6.6"

/* button Macros */
#define NONE_PRESSED 0
#define UP_PRESSED 1
#define CENTER_LONG_PRESSED 2 
#define CENTER_SHORT_PRESSED 3
#define DOWN_PRESSED 4

#define TAB_F '0' //91 equals 0
#define ENTER_F '1' //92 equals 1


void setupScreen(void);

void setupBLE(void);

void updateScreen(void);
void populateArrays(uint8_t pos);
void Enroll(void);
void addAccount(void);
void removeAccount(void);
int deleteAcc(uint8_t a);
int compareArray(char* a, char* b);
uint8_t process_buttons(void);






	/* Function Prototypes: */
		void SetupHardware(void);

		void EVENT_USB_Device_Connect(void);
		void EVENT_USB_Device_Disconnect(void);
		void EVENT_USB_Device_ConfigurationChanged(void);
		void EVENT_USB_Device_ControlRequest(void);





#endif

