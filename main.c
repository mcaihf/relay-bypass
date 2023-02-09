#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <xc.h>  
#include "header.h" // Include the header (configuration bits)

#define shift_reg_clk 50		// Wait time between shift register updates for debouncing (default = 50)
#define short_threshold 0xFFF9	// default = 0b1111 1111 1111 1001 : hold ftsw for 100ms to activate short press
#define long_threshold 	0x01	// default = 0b0000 0000 0000 0001 : hold ftsw for 800ms to activate long press
/*
Footswitch is active low (0). Debouncing works but looking for a one (ftsw release) after a set number of zeros
e.g. 0b1111 1100 0000 0001 will give a required button hold time of 9*shift_reg_clk (9 zeros * 50ms = 450ms)
*/

void main(void) { // main loop
    setup();
	
    uint8_t mode = 0; // variable that will define the pedal's state: on or off
    uint16_t debounce_0 = 0xFFFF; // shift register for debouncing GPIO0
	
    while (1) {
        debounce_0 = 2 * debounce_0 + GP0; // debounce GP0
		// shift bits left and add a 0 if ftsw is pressed, add 1 if not pressed
		if (debounce_0 == short_threshold) {
			if (mode == 0)
				GP5 ^= 1;	// toggle bypass relay
			else
				GP4 ^= 1;	// toggle octave relay
		}
		if (debounce_0 == long_threshold) {
			mode ^= 1;		// if long press, switch between bypass and octave mode
		}
		_delay_ms(shift_reg_clk);
    }
}

void setup(void) {
    ANSEL = 0; // no analog GPIO
    CMCON = 0x07; // comparator off
    ADCON0 = 0; // ADC and DAC converters off
	
    TRISIO0 = 1; // Footswitch, GPIO 0 is an input
    TRISIO1 = 1; // Ext Switch
    TRISIO2 = 1; // Ext Sense
    TRISIO3 = 0; // Not connected, GPIO 3 is an output
    TRISIO4 = 0; // Oct bypass
    TRISIO5 = 0; // Main bypass

    GPIO = 0; // Initially, all GPIOs are in a low state
}