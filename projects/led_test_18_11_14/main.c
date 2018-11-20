#include "main.h"

#define SWITCH_DELAY 400000

int* a[] = {GPIO_Pin_8, GPIO_Pin_8 | GPIO_Pin_9, GPIO_Pin_9,
                GPIO_Pin_9 | GPIO_Pin_10, GPIO_Pin_10, GPIO_Pin_8 | GPIO_Pin_10};
    /*int* a[] = {GPIO_Pin_8, GPIO_Pin_9, GPIO_Pin_10};*/
int size = 6;
char direction = 1;
char index = 0;
void switch_LED(){
    index = direction ? (index + 1) % size : (index + size - 1) % size;

    GPIO_SetBits(GPIOA, GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10);
    GPIO_ResetBits(GPIOA, a[index]);
}

int speed = 0;
int iteration = 0;

void TIM2_IRQHandler (void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {
        if (2 * speed == iteration){
            switch_LED();
            iteration = 0;
        }
        else{
            iteration++;
        }

        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }
}

void EXTI1_IRQHandler(void) {
    if(EXTI_GetITStatus(EXTI_Line1)!= RESET){
        /* Do stuff */
        direction = !direction;

        EXTI_ClearITPendingBit(EXTI_LineX);
    }
}

int main(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* LEDs array to toggle between them */
  /* LED to toggle during iteration */
  uint8_t  current_led = 0;

  /* Enable peripheral clock for LEDs port */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

  /* Init LEDs */
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0 | GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
   
  TIM_TimeBaseInitTypeDef tim_struct;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
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

    speed = 0;
    iteration = 0;
    char click_flag_1 = 0;
    char click_flag_2 = 0;
    GPIO_ResetBits(GPIOA, a[0]);  
    while (1)
    {
        /*
        if (! GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_0)) {
            click_flag_1 = 1;
        }
        else if (click_flag_1) {
            direction = !direction;
            click_flag_1 = 0;
        }*/
        
        if (! GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_1)) {
            click_flag_2 = 1;
        }
        else if (click_flag_2) {
            speed = (speed + 1) % 4;
            iteration = 0;
            click_flag_2 = 0;
        }
        
        
        /*
        for (j = 0; j < 6; j++){
            if (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_0)) 
                is_on = 1;
            if (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_1)) 
                is_on = 0;

            if (is_on){
                GPIO_ResetBits(GPIOA, a[j]);
                for (i = 0; i < 2 * SWITCH_DELAY; i++);    	
                GPIO_SetBits(GPIOA, a[j]);
            }
        }	 */   
    
    
    /* Switch the LED on */
    
    /*GPIO_ResetBits(GPIOD, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14);
    for (i = 0; i < SWITCH_DELAY / 2; i++);*/

    /* Switch the LED off */
    /*GPIO_SetBits(GPIOD, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14);
    for (i = 0; i < SWITCH_DELAY; i++);*/
    
  }
}
