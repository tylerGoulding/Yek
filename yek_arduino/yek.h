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


void setupScreen();

void setupBLE();

void updateScreen();
void populateArrays(uint8_t pos);
void Enroll();
void addAccount();
void removeAccount();
int deleteAcc(uint8_t a);
int compareArray(char* a, char* b);
uint8_t process_buttons();
