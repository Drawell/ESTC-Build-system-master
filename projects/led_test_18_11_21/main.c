#include "main.h"

#define SWITCH_DELAY 400000

int* a[] = {GPIO_Pin_8, GPIO_Pin_8 | GPIO_Pin_9, GPIO_Pin_9,
                GPIO_Pin_9 | GPIO_Pin_10, GPIO_Pin_10, GPIO_Pin_8 | GPIO_Pin_10};
    /*int* a[] = {GPIO_Pin_8, GPIO_Pin_9, GPIO_Pin_10};*/
int size = 6;
char direction = 1;
char index = 0;
void switch_LED() {
    index = direction ? (index + 1) % size : (index + size - 1) % size;

    GPIO_SetBits(GPIOA, GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10);
    GPIO_ResetBits(GPIOA, a[index]);
}

int speed = 0;
int iteration = 0;
void TIM2_IRQHandler (void) {
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {
        if (2 * speed == iteration){
            switch_LED();
            iteration = 0;
        }
        else {
            iteration++;
        }

        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }
}

void EXTI0_IRQHandler(void) {
    if (EXTI_GetITStatus(EXTI_Line0)!= RESET) {

        EXTI_ClearITPendingBit(EXTI_Line0);
    }    
}

void EXTI1_IRQHandler(void){
    if (EXTI_GetITStatus(EXTI_Line1) != RESET) {
        speed = (speed + 1) % 5;
        //iteration = 0;
        //switch_LED();


        TIM_SetCompare1(TIM1, speed * 200);
        TIM_SetCompare2(TIM1, speed * 200);
        TIM_SetCompare3(TIM1, speed * 200);

        EXTI_ClearITPendingBit(EXTI_Line1);
    }
}

void init_LEDs() {
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

void init_tim2() {
    TIM_TimeBaseInitTypeDef tim_struct;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    tim_struct.TIM_Period = 125 - 1;
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

void init_external_iterrupt() {
    //
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

void init_tim1(){
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

void inint_small_LEDs(){


}

int main(void)
{ 

  /* LEDs array to toggle between them */
  /* LED to toggle during iteration */
  uint8_t  current_led = 0;

    
    init_Buttons();
    //init_tim2();
    init_external_iterrupt();
    init_LEDs();
    init_tim1();

    speed = 0;
    iteration = 0;
    
    while (1)
    {

    }
}
