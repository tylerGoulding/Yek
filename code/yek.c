/* Includes: */
#include "yek.h"

/* loop break conditon */
static bool running = true;
volatile char sendBuffer[BUFSIZE+1];
static uint8_t PrevKeyboardHIDReportBuffer[sizeof(USB_KeyboardReport_Data_t)];
/** LUFA HID Class driver interface configuration and state information. */
USB_ClassInfo_HID_Device_t Keyboard_HID_Interface =
    {
        .Config =
            {
                .InterfaceNumber              = 1,
                .ReportINEndpoint             =
                    {
                        .Address              = KEYBOARD_EPADDR,
                        .Size                 = KEYBOARD_EPSIZE,
                        .Banks                = 1,
                    },
                .PrevReportINBuffer           = PrevKeyboardHIDReportBuffer,
                .PrevReportINBufferSize       = sizeof(PrevKeyboardHIDReportBuffer),
            },
    };
SSD1306 display(SCREEN_RESET);

int main{
    /* init */
    setupHarware();
    setupBLE();
    setupSettings();

    while(1){
        checkButtons();
        updateScreen();
        // first transmit via USB
        if (mode == SETUP_MODE){
            HID_Device_USBTask(&Keyboard_HID_Interface);
            USB_USBTask();
        }
        // wireless implies delay so its ok to put it second
        int accountSelected = 1;
        if (accountSelected){
            if (populateBuff(sendBuffer,BUFSIZE) != 0){
                //error setting up Buffer
            }
            if (TRANSMIT_BLE){

                memset(buffer, 0, BUFSIZE);

                ble.print("AT+BleKeyboard=");
                ble.println(keys);
                if( ble.waitForOK() ) {

                }
                else {
                    // idk what to do if it fails?
                    // retransmit?
                    // display fail screen?
                }
            }            
        }




    }
}


int populateBuff(char buffer[], uint8_t maxSize) {
    // TODO: have this look through the lookup table
    memset(buffer, 0, maxSize);
    char *accountInfo = "tgouldin\rpassword\n";
    strcpy(buffer, accountInfo);
    return 0;

}

/** Event handler for the library USB Connection event. */
void EVENT_USB_Device_Connect(void)
{
    /* Uploading Passwords so lets get into setup mode */
    mode = SETUP_MODE;
}

void setupHarware(void){
    /* Disable watchdog if enabled by bootloader/fuses */
    MCUSR &= ~(1 << WDRF);
    /* TODO: may be able to use the watch dog to cause a reset if needed... */
    wdt_disable();
    Serial_Init(9600, true);
    setupButtons();
    USB_Init();
}