


#include "button.h"
#include "pin_map.h"


int setupButtons(){
    DDRD &= ~((1<<DWN_PIN)|(1<<CTR_PIN)|(1<<UP_PIN));
    PORTD |= ((1<<DWN_PIN)|(1<<CTR_PIN)|(1<<UP_PIN));

    return 0;
}

void button_check(b_t button) {
    unsigned int value = (*(JOY_PORT) & BITVAL(button->pin)) ? 1 : 0;
    // unsigned int active = (obj->active_high == (value == 1));
    unsigned long now = time();
    //
    // check for debouncing
    if (button->previous_value == -1)
        button->previous_value = (button->active_high?0:1);
    if (value != button->previous_value) {

        if (now - button->last_change_time <= button->debounce_time) {
            return;
        }
        if (active) {
            if (button->press_cb)
                button->press_cb(button->priv);
        } else {
            button->long_pressed = false;
            if (now - button->last_change_time <= button->long_press_time || button->long_press_cb == NULL) {
                DEBUG_BUTTON("Short release, outside debounce\r\n");
                if (button->release_cb)
                    button->release_cb(button->priv);
            }
        }
        button->last_change_time = now;
        button->previous_value = value;
    }
}