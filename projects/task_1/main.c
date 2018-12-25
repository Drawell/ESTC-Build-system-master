#include "main.h"

#define SWITCH_DELAY 400000

/*!
    \brief Init LEDs
    Init GPIOD Pins 12, 13, 14, 15 with Out mode
 */
void init_leds(void)
{
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOD, &GPIO_InitStructure);            
}

/*!
    \brief Init button
    Init GPIOA Pin 0 with in mode
 */
void init_button(void)
{
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(GPIOD, &GPIO_InitStructure); 
}

/*!
    \brief wait for some time
    Wait for some time.
    Set button_pushed variable is 1 when button pressed.
    Change direction when button released    
*/
void delay(uint32_t delay_ticks){
    for (uint32_t i = 0; i < delay_ticks; i++){
        if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) != RESET){
            button_pushed = 1;
        }
        else if (button_pushed) {
            button_pushed = 0;
            direction = !direction;
            for (uint16_t j = 0; j < 20000; j++);
            return;
        }
    }
}


/*!
    \brief switch lit led
    Off lit led, on next led and call delay function
*/
int main(void)
{
    init_leds();
    init_button();

    const uint16_t arr_of_leds[4] = { GPIO_Pin_12, GPIO_Pin_13, GPIO_Pin_14, GPIO_Pin_15 };
    uint8_t current_led = 0;

    while (1)
    {
        GPIO_ResetBits(GPIOD, arr_of_leds[current_led]);
        current_led = (current_led + 1 + 2 * direction) % 4;
        GPIO_SetBits(GPIOD, arr_of_leds[current_led]);
        delay(SWITCH_DELAY);
    }
}
