#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <xc.h>  
#include "header.h" // Include the header (configuration bits)

void main(void) { // main loop
    setup();
    
    uint8_t state = 0; // variable that will define the pedal's state: on or off
    uint8_t mode = 0; // variable that will define the pedal's state: on or off
            
    while (1) {
                if (GP0 == 0) { // if the switch is pressed
            __delay_ms(15); // debounce
            if (GP0 == 0) {
                __delay_ms(200); // switch released
                if (GP0 == 1) { // if short press toggle relay bypass based on mode
                    if (mode == 0) { // bypass mode relay toggle
                        if (state == 1) { // if the effect is on
                            state = 0; // ...turn it off
                            __delay_ms(10);
                            GP5 = 0; // inactivate the relay 
                        }
                        if (state == 0) { // if the pedal is off
                            state = 1; // ...turn if on
                            __delay_ms(10);
                            GP5 = 1; // activate the relay
                        }
                    }
                    else { // octave mode relay toggle
                        if (state == 1) { // if the effect is on
                            state = 0; // ...turn it off
                            __delay_ms(10);
                            GP4 = 0; // inactivate the relay 
                        }
                        if (state == 0) { // if the pedal is off
                            state = 1; // ...turn if on
                            __delay_ms(10);
                            GP4 = 1; // activate the relay
                        }
                    }
                }
                else { // if ftsw is still held, toggle mode
                        if (mode == 1) {
                            mode = 0; // set to bypass mode
                            __delay_ms(10);
                        }
                        if (mode == 0) {
                            mode = 1; // set to octave mode
                            __delay_ms(10);
                        }
                }
            }
        }
        __delay_ms(10); // we wait for the PIC to change the state of each GPIO
         */
    }
}

void setup(void) {
    ANSEL = 0; // no analog GPIO
    CMCON = 0x07; // comparator off
    ADCON0 = 0; // ADC and DAC converters off
    TRISIO0 = 1; // Foot switch, GPIO 0 is an input
    TRISIO1 = 1; // Ext Switch
    TRISIO2 = 1; // Ext Sense
    TRISIO3 = 0; // Not connected, GPIO 3 is an output
    TRISIO4 = 0; // Oct bypass
    TRISIO5 = 0; // Main bypass

    GPIO = 0; // Initially, all GPIOs are in a low state
}