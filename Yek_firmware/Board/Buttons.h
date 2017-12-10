/* button Macros */
#define NONE_PRESSED 0
#define UP_PRESSED 1
#define CENTER_LONG_PRESSED 2 
#define CENTER_SHORT_PRESSED 3
#define DOWN_PRESSED 4
#define LOW 0
#define HIGH 1



static inline void Buttons_Init(void)
{
    // Initialize the appropriate port pins as an inputs here, with pull-ups
    // See Atmel doc 7799, section 12.2.3
    DDRD &= 0b00011111;
    PORTD |= 0b11100000;
}

static inline void Buttons_Disable(void)
{
    // Clear the appropriate port pins as high impedance inputs here
  DDRD &= ~0b00011111;
  PORTD &= ~0b11100000;

}

uint8_t Buttons_GetStatus(void) {
  return 0;
  /*
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
          Delay_MS(10);
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
  */
}