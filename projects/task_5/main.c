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

void init_external_iterrupt() {    
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

void change_intensity(void)
{
    // 1-red ; 2-green; 3-blue
    uint8_t bright_count = 11;
    uint32_t k = 1000 / (bright_count - 1);

    switch (current_colour){
    case 1:
        colour_red_intensity = (colour_red_intensity + 1) % bright_count;
        break;
    case 2:
        colour_green_intensity = (colour_green_intensity + 1) % bright_count;
        break;
    case 3:
        colour_blue_intensity = (colour_blue_intensity + 1) % bright_count;
        break;
    default:
        break;
    }

    ml_set_color(23 * colour_red_intensity, 23 * colour_green_intensity, 23 * colour_blue_intensity);   
}


void EXTI0_IRQHandler(void) {
    if (EXTI_GetITStatus(EXTI_Line0)!= RESET) {
        
        ml_turn_off_LED(current_colour);
        current_colour = (current_colour + 1) % 3;
        ml_turn_on_LED(current_colour);

        NVIC_DisableIRQ(EXTI0_IRQn);
        for(int i = 0; i < 10000; i++);
        NVIC_EnableIRQ(EXTI0_IRQn);
        EXTI_ClearITPendingBit(EXTI_Line0);
    }
}

void EXTI1_IRQHandler(void){
    if (EXTI_GetITStatus(EXTI_Line1) != RESET) {
        change_intensity();        
        EXTI_ClearITPendingBit(EXTI_Line1);
    }
}

int main(void)
{
    init_Buttons();
    init_external_iterrupt();
    ml_init();
    ml_turn_off_LED(current_colour);
    
    while (1)
    {
    }
}
