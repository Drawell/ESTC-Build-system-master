#include "main.h"


void init_leds(void)
{
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_TIM1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_TIM1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_TIM1);
    
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure);           
}

void init_small_leds(void)
{
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    // 12 - red; 13 - green; 14 - blue;
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
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
}

void init_external_interrupt(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    
    EXTI_InitTypeDef EXTI_InitStruct;
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;
    EXTI_InitStruct.EXTI_Mode    = EXTI_Mode_Interrupt;
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;    

    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;

    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource0);
    EXTI_InitStruct.EXTI_Line = EXTI_Line0;
    EXTI_Init(&EXTI_InitStruct);
    NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn;
    NVIC_Init(&NVIC_InitStruct);

    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource1);
    EXTI_InitStruct.EXTI_Line = EXTI_Line1;
    EXTI_Init(&EXTI_InitStruct);
    NVIC_InitStruct.NVIC_IRQChannel = EXTI1_IRQn;
    NVIC_Init(&NVIC_InitStruct);

}

void init_tim1()
{
    TIM_TimeBaseInitTypeDef tim_struct;    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    tim_struct.TIM_Period = 1000 - 1;
    tim_struct.TIM_Prescaler = 168 - 1;
    tim_struct.TIM_ClockDivision = 0;
    tim_struct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM1, &tim_struct);
    TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);

    TIM_OCInitTypeDef OC_struct = {0};
    OC_struct.TIM_OutputState = TIM_OutputState_Enable;
    OC_struct.TIM_OCPolarity = TIM_OCPolarity_Low;
    OC_struct.TIM_OCMode = TIM_OCMode_PWM1;   

    OC_struct.TIM_Pulse = 0;
    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
    TIM_OC1Init(TIM1, &OC_struct);

    OC_struct.TIM_Pulse = 0;
    TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
    TIM_OC2Init(TIM1, &OC_struct);
    
    OC_struct.TIM_Pulse = 0;
    TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);
    TIM_OC3Init(TIM1, &OC_struct);
    
    TIM_CtrlPWMOutputs(TIM1, ENABLE);
    TIM_Cmd(TIM1, ENABLE);
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
    uint32_t k = 1000 / (bright_count - 1);

    switch (current_colour){
    case 0:
        colour_red_intensity = (colour_red_intensity + 1) % bright_count;
        TIM_SetCompare1(TIM1, colour_red_intensity  * k);
        break;
    case 1:
        colour_green_intensity = (colour_green_intensity + 1) % bright_count;
        TIM_SetCompare2(TIM1, colour_green_intensity  * k);
        break;
    case 2:
        colour_blue_intensity = (colour_blue_intensity + 1) % bright_count;
        TIM_SetCompare3(TIM1, colour_blue_intensity  * k);
        break;
    default:
        break;
    }
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
            const uint16_t arr_of_leds[3] = {GPIO_Pin_14, GPIO_Pin_12, GPIO_Pin_15};        
            GPIO_ResetBits(GPIOD, arr_of_leds[current_colour]);
            current_colour = (current_colour + 1) % 3;
            GPIO_SetBits(GPIOD, arr_of_leds[current_colour]);

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
            mask_flag = 1;
            TIM_Cmd(TIM2, ENABLE);
        }
        EXTI_ClearITPendingBit(EXTI_Line1);
    }
}

int main(void)
{
    init_leds();
    init_small_leds();
    init_button();
    init_external_interrupt();    
    init_tim1();
    init_tim2();

    GPIO_SetBits(GPIOD, GPIO_Pin_14);
    
    while (1)
    {
    }
}
