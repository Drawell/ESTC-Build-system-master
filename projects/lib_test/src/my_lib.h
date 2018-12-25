/*!
    \file
    \brief my_lib.
    init buttons, small leds, big led, tim1
*/
#ifndef LEDS_AND_BUTTON_LIB_H
#define LEDS_AND_BUTTON_LIB_H

#include <stm32f4xx.h>
#include <math.h>

/*!
    \brief Init all
    Init GPIOA Pins 8, 9, 10 with AF mode
    Init GPIOD Pins 12, 13, 14 with Out mode
    Init timer tim1 as PWM.
 */
void ml_init(void);

/*!
    \brief Set color of big led
    \param[in] red Intensity of red colour
    \param[in] green Intensity of green colour
    \param[in] blue Intensity of blue colour
 */
void ml_set_color(uint8_t red, uint8_t green, uint8_t blue);

/*!
    \brief Turn ON small led by number
    \param[in] num Number of led. 1 - red, 2 - green, 3 - blue, 4 - orange.
 */
void ml_turn_on_LED(uint8_t num);

/*!
    \brief Turn OFF small led by number
    \param[in] num Number of led. 1 - red, 2 - green, 3 - blue, 4 - orange.
 */
void ml_turn_off_LED(uint8_t num);



#endif  /* LEDS_AND_BUTTON_H */
