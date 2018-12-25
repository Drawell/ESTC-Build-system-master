#include "my_lib.h"

uint8_t inited = 0;

void __ml_init_LEDs__(void) {
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

void __ml_init_tim1__(void){
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

void __ml_inint_small_LEDs__(void){
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

}

void __ml_switch_LED__(void){
    if (!inited)
        return;

}

void ml_init(void){
    __ml_init_LEDs__();
    __ml_init_tim1__();
    __ml_inint_small_LEDs__();
    inited = 1;
    return 1;
}

void ml_set_color(uint8_t red, uint8_t green, uint8_t blue){
    if (!inited)
        return;
        
    TIM_SetCompare1(TIM1, (red * 39)/10);
    TIM_SetCompare2(TIM1, (green * 39)/10);
    TIM_SetCompare3(TIM1, (blue * 39)/10);
}

uint16_t* array_of_small_LEDs_pins[] = {GPIO_Pin_12, GPIO_Pin_13, GPIO_Pin_14, GPIO_Pin_15};

void ml_turn_on_LED(uint8_t num){
    if (!inited)
        return;

    if (num < 4) {
        GPIO_SetBits(GPIOD, array_of_small_LEDs_pins[num]);
    }
        
}
void ml_turn_off_LED(uint8_t num){
    if (!inited)
        return;

    GPIO_ResetBits(GPIOD, array_of_small_LEDs_pins[num]);
}
