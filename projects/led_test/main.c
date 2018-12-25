#include "main.h"


void init_leds(void)
{
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);           
}

void init_button(void)
{
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_Init(GPIOE, &GPIO_InitStructure);
}

void init_external_interrupt(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource0);
    
    EXTI_InitTypeDef EXTI_InitStruct;
    EXTI_InitStruct.EXTI_Line = EXTI_Line0;
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;
    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_Init(&EXTI_InitStruct);

    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);

    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource1);
    EXTI_InitStruct.EXTI_Line = EXTI_Line1;
    EXTI_Init(&EXTI_InitStruct);

    NVIC_InitStruct.NVIC_IRQChannel = EXTI1_IRQn;
    NVIC_Init(&NVIC_InitStruct);
}

void init_tim2(void)
{
    TIM_TimeBaseInitTypeDef tim_struct;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
    tim_struct.TIM_Period = 200 - 1;
    tim_struct.TIM_Prescaler = 42000 - 1;
    tim_struct.TIM_ClockDivision = 0;
    tim_struct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &tim_struct);
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM2, ENABLE);

    NVIC_InitTypeDef nvic_struct;
    nvic_struct.NVIC_IRQChannel = TIM2_IRQn;
    nvic_struct.NVIC_IRQChannelPreemptionPriority = 0;
    nvic_struct.NVIC_IRQChannelSubPriority = 1;
    nvic_struct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic_struct);
}


const uint16_t arr_of_colours[6]  = {GPIO_Pin_8, GPIO_Pin_8 | GPIO_Pin_9, GPIO_Pin_9,
                GPIO_Pin_9 | GPIO_Pin_10, GPIO_Pin_10, GPIO_Pin_8 | GPIO_Pin_10};

uint8_t size = 6;
uint8_t direction = 1;
uint8_t current_colour = 0;
void switch_LED(){
    GPIO_SetBits(GPIOA, arr_of_colours[current_colour]);
    current_colour = (current_colour + 1 + (size - 2) * direction) % size;
    GPIO_ResetBits(GPIOA, arr_of_colours[current_colour]);
}

uint8_t speed = 0;
uint8_t iteration = 0;
void TIM2_IRQHandler (void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {
        if (2 * speed <= iteration){
            switch_LED();
            iteration = 0;
        }
        else{
            iteration++;
        }
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }
}

void EXTI0_IRQHandler(void) {
    if (EXTI_GetITStatus(EXTI_Line0)!= RESET) {
        direction = !direction;
        EXTI_ClearITPendingBit(EXTI_Line0);
    }
}

void EXTI1_IRQHandler(void){
    if (EXTI_GetITStatus(EXTI_Line1) != RESET) {
        speed = (speed + 1) % 3;
        iteration = 0;
        EXTI_ClearITPendingBit(EXTI_Line1);
    }
}

int main(void)
{
    init_leds();
    init_external_interrupt();
    init_button();
    init_tim2();
    
    while (1)
    {
    }
}
