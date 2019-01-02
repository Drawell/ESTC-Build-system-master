#include "main.h"

void init_Buttons() {
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_Init(GPIOE, &GPIO_InitStructure);
}

void init_external_iterrupt()
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
    tim_struct.TIM_Period = 100 - 1;
    tim_struct.TIM_Prescaler = 42000 - 1;
    tim_struct.TIM_ClockDivision = 0;
    tim_struct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &tim_struct);
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);    

    NVIC_InitTypeDef nvic_struct;
    nvic_struct.NVIC_IRQChannel = TIM2_IRQn;
    nvic_struct.NVIC_IRQChannelPreemptionPriority = 0;
    nvic_struct.NVIC_IRQChannelSubPriority = 1;
    nvic_struct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic_struct);
}

void change_intensity(void)
{
    // 1-red ; 2-green; 3-blue
    uint8_t bright_count = 5;
    uint8_t k = 255 / bright_count;

    switch (current_colour){
    case 0:
        colour_red_intensity = (colour_red_intensity + 1) % bright_count;
        break;
    case 1:
        colour_green_intensity = (colour_green_intensity + 1) % bright_count;
        break;
    case 2:
        colour_blue_intensity = (colour_blue_intensity + 1) % bright_count;
        break;
    default:
        break;
    }

    ml_set_color(k * colour_red_intensity, k * colour_green_intensity, k * colour_blue_intensity);   
}

void TIM2_IRQHandler (void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {
        mask_flag = 1;
        TIM_Cmd(TIM2, DISABLE); 
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }
}

void EXTI0_IRQHandler(void) {
    if (EXTI_GetITStatus(EXTI_Line0)!= RESET) {
        if (mask_flag){
            ml_turn_off_LED(current_colour);
            current_colour = (current_colour + 1) % 3;
            ml_turn_on_LED(current_colour);

            mask_flag = 0;
            TIM_Cmd(TIM2, ENABLE);
        }
        EXTI_ClearITPendingBit(EXTI_Line0);
    }
}

void EXTI1_IRQHandler(void){
    if (EXTI_GetITStatus(EXTI_Line1) != RESET) {
        if (mask_flag){
            change_intensity();

            mask_flag = 0;
            TIM_Cmd(TIM2, ENABLE);            
        }
        EXTI_ClearITPendingBit(EXTI_Line1);
    }
}

int main(void)
{
    init_Buttons();
    init_external_iterrupt();
    init_tim2();
    ml_init();
    ml_turn_on_LED(current_colour);

    ml_set_color(0,0,0);
    while (1)
    {
    }
}
