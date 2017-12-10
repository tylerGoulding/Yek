# YEK
Yek is a prototype of an on-hand password manager that redefines how users interact with account safety and passwords on a laptop computer. It is an embedded device that acts as your personal butler to memorize passwords. With a yek device and a fingerprint scan, you can login to any of your accounts by connecting to any computer device through a bluetooth connection. Yek  acts as a keyboard and types in your login information (username and password) to the selected textbox of a login page, so that you do not have to remember your unique and strong passwords. It can fit in your pocket just like a physical keychain, and yet it can hold 63 of your accounts through a computer application.

## Team
- Fatema Almeshqab
- Tyler Goulding
- Matt Chu

## Code
When prototyping, we developed in Arduino then ported it to AVR C. We have included both for your convenience.

##PCB
The pcb was designed in Eagle. There is one error in the PCB if you are planning to send it out (or printing it in house if that's your slice of pie). The pins on the Battery JST plug are flipped. This is not a major issue but change it if you would like. We did not so that the professors could see our the equivalent PCB we printed.
## Computer Application
The computer application was written in Java and is available as a runnable .jar file that works on Windows, Mac, or Linux, and just requires that Java 8 be installed. The app, uses a library called jSerialComm to communicate with the Yek through a serial port connection.

## Building the Yek
### If you want to use Arduino
To build with Arduino:
Download the following libraries:

U8g2 Screen Library:
> https://github.com/olikraus/U8g2_Arduino

FRAM Library: 

> https://github.com/adafruit/Adafruit_FRAM_I2C

Fingerprint scanner:
> https://github.com/sparkfun/Fingerprint_Scanner-TTL

Bluefruit library:
> https://github.com/adafruit/Adafruit_BluefruitLE_nRF51

Open Arduino and compile the .ino in the folder Yek_arduino

### If you want to use GCC
Use the code in Yek_firmware folder

To build with GCC:

- install avr-gcc (GCC) 7.2.0 using your favorite package manager (this will take 2 hours... sorry)
- download LUFA
- update the path to lufa into makefile
- run make in the folder


## Acknowledgements
* Dean Camera, for LUFA.
* Adafruit (author hathach), for their parts and their BLE library
* Josh Hawley, for his FPS library
* Arduino, for their Print class and Stream class
* Olikraus, for the screen library and compressed fonts
* Zak Kemble, for a AVR implementation of millis
* Mike DiBacco, for helping us lasercut the case
* Some random dude at the Makerspace who helped me 3d print

