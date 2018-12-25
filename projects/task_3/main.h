/*!
    \file
    \brief task_3 switch lolour of led
    Change colour colour by external interrupt by timer.
    Change direction of round by button 0.
    Change speed of changing by button 1.
*/
#ifndef LEDS_AND_BUTTON_H
#define LEDS_AND_BUTTON_H

#include <stm32f4xx.h>
#include <my_lib.h>
#include <math.h>

/*!
    \brief Init LEDs
    Init GPIOA Pins 8, 9, 10 with Out mode
 */
void init_leds(void);

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

/*!
    \brief Init tim2 
    Init timer tim2 with 0.2 sec period
    and external interrupt for it
 */
void init_tim2(void);

/// array of colours
const uint16_t arr_of_colours[6]  = {GPIO_Pin_8, GPIO_Pin_8 | GPIO_Pin_9, GPIO_Pin_9,
                GPIO_Pin_9 | GPIO_Pin_10, GPIO_Pin_10, GPIO_Pin_8 | GPIO_Pin_10};

/// count of colours
uint8_t size = 6;

/// Direction of round.
uint8_t direction = 1;
/// Index of currently lit colour
uint8_t current_colour = 0;
/// Speed of changing
uint8_t speed = 0;
/// Number of iteration
uint8_t iteration = 0;

/*!
    \brief Switch colours.
 */
void switch_LED();

/*!
    \brief Tim2 interrupt handler.
    Incriment iteration variable or 
    if 2 * speed variable <= iteration switch colour
    and set iteration 0.
 */
void TIM2_IRQHandler (void);

/*!
    \brief Button 0 interrupt handler.
    Change direction of round.
 */
void EXTI0_IRQHandler(void);

/*!
    \brief Button 1 interrupt handler
    Incriment speed with mod 3.
    Set iteration 0.
 */
void EXTI1_IRQHandler(void);

#endif  /* LEDS_AND_BUTTON_H */
