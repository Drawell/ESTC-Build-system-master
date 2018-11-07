#include "main.h"

#define SWITCH_DELAY 400000

int abs(int x)
{
    if (x < 0)
        return -x;
    else
        return x;

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
   

    
    int i, j = 0;
    int* a[] = {GPIO_Pin_8, GPIO_Pin_8 | GPIO_Pin_9, GPIO_Pin_9,
                GPIO_Pin_9 | GPIO_Pin_10, GPIO_Pin_10, GPIO_Pin_8 | GPIO_Pin_10};
    /*int* a[] = {GPIO_Pin_8, GPIO_Pin_9, GPIO_Pin_10};*/
    
    char click_flag = 0;
    GPIO_ResetBits(GPIOA, a[0]);  
    while (1)
    {

        if (! GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_0)) {
            click_flag = 1;
        }
        if (! GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_0)) {
            click_flag = 2;
        }

        else if (click_flag) {
            i = click_flag == 1 ? 1 : -1;
            j = abs(j + i) % 6;

            GPIO_SetBits(GPIOA, GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10); 
            j = (j + 1) % 6; //size(a) / size(a[0]);            
            GPIO_ResetBits(GPIOA, a[j]);
            click_flag = 0;
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
