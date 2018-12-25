/*!
    \file
    \brief task_4 configure colour of led.
    Switch small leds by button 0.
    Change intensity of big led colour.
    Colour the same as small led lit.
*/
#ifndef LEDS_AND_BUTTON_H
#define LEDS_AND_BUTTON_H

#include <stm32f4xx.h>
#include <my_lib.h>
#include <math.h>

/*!
    \brief Init buttons
    Init GPIOE Pins 0, 1 with In mode
 */
void init_button(void);

/*!
    \brief Init external interrupt for buttons 
    Init external interrupt for buttons.
    On EXTI_Line0 - GPIOE Pin 0.
    On EXTI_Line1 - GPIOE Pin 1.
 */
void init_external_interrupt(void);


/// Configured colour; 1 - red, 2 - green, 3 - blue.
uint8_t current_colour = 1;
/// Present intensity of red.
uint8_t colour_red_intensity = 1;
/// Present intensity of green.
uint8_t colour_green_intensity = 1;
/// Present intensity of blue.
uint8_t colour_blue_intensity = 1;

/*!
    \brief Button 0 interrupt handler.
    Change colour.
 */
void EXTI0_IRQHandler(void);

/*!
    \brief Button 1 interrupt handler
    Change intensity.
 */
void EXTI1_IRQHandler(void);

#endif  /* LEDS_AND_BUTTON_H */
