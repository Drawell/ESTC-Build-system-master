#include "main.h"

#define SWITCH_DELAY 400000

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

void init_tim2(void)
{
    TIM_TimeBaseInitTypeDef tim_struct;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
    tim_struct.TIM_Period = 1000 - 1;
    tim_struct.TIM_Prescaler = 42000 - 1;
    tim_struct.TIM_ClockDivision = 0;
    tim_struct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &tim_struct);
    TIM_Cmd(TIM2, ENABLE);
}

void wait_for_tim2(void)
{
    uint8_t i_dont_know_but_it_dont_work_without_this_line = 1;
    while(1){
        if (TIM_GetCounter(TIM2) == 0) {
            return;
        }

        if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) != RESET){
            button_pushed = 1;
        }
        else if (button_pushed) {
            button_pushed = 0;
            direction = !direction;
            for (uint32_t i; i < 2000; i++);
        }
    }
}
int main(void)
{
    init_leds();
    init_button();
    init_tim2();

    const uint16_t arr_of_leds[4] = { GPIO_Pin_12, GPIO_Pin_13, GPIO_Pin_14, GPIO_Pin_15 };
    uint8_t current_led = 0;

    while (1)
    {   
        GPIO_ResetBits(GPIOD, arr_of_leds[current_led]);
        current_led = (current_led + 1 + 2 * direction) % 4;
        GPIO_SetBits(GPIOD, arr_of_leds[current_led]);
        wait_for_tim2();
    }
}
