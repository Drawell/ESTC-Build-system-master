/*!
    \file
    \brief task_2 switch lit led
    Off lit led, on next led and wait for timer
*/
#ifndef LEDS_AND_BUTTON_H
#define LEDS_AND_BUTTON_H

#include <stm32f4xx.h>
#include <my_lib.h>
#include <math.h>

/*!
    \brief Init LEDs
    Init GPIOD Pins 12, 13, 14, 15 with Out mode
 */
void init_leds(void);

/*!
    \brief Init button
    Init GPIOA Pin 0 with In mode
 */
void init_button(void);

/*!
    \brief Init timer
    Init timer tim2 with 1 sec period
 */
void init_tim2(void);

/// Is 1 if button pressed, 0 when not. 
uint8_t button_pushed = 0;
/// Direction of round.
uint8_t direction = 1;

/*!
    \brief wait tim2
    Wait while tim2 counter not 0.
    Set button_pushed variable is 1 when button pressed.
    Change direction when button released    .
*/
void wait_for_tim2(void);


#endif  /* LEDS_AND_BUTTON_H */
