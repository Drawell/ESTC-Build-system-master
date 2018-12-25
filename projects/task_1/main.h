/*!
    \file
    \brief task_1 switch lit led
    Off lit led, on next led and call delay function
*/
#ifndef LEDS_AND_BUTTON_H
#define LEDS_AND_BUTTON_H

#include <stm32f4xx.h>
#include <my_lib.h>


/*!
    \brief Init LEDs
    Init GPIOD Pins 12, 13, 14, 15 with Out mode
 */
void init_leds(void);

/*!
    \brief Init button
    Init GPIOA Pin 0 with in mode
 */
void init_button(void);

/// Is 1 if button pressed, 0 when not. 
uint8_t button_pushed = 0; 
/// Direction of round
uint8_t direction = 1; 

/*!
    \brief wait for some time
    Wait for some time.
    Set button_pushed variable is 1 when button pressed.
    Change direction when button released    
*/
void delay(uint32_t delay_ticks);



#include <math.h>

#endif  /* LEDS_AND_BUTTON_H */
