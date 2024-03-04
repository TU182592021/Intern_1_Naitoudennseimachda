/*
 * step6-1.c
 *
 *  Created on: 2023/08/22
 *      Author: X20308
 *      LEDスイッチ点灯
 */
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"

#define ON 0x00
#define OFF 0xFF

void Cycle_temp(int num){
    int Loop;
    int LED_num_1;
    int LED_num_2;
    if(num==1){
        LED_num_1=GPIO_PIN_3;
        LED_num_2=GPIO_PIN_1;
    }else if(num==2){
        LED_num_1=GPIO_PIN_1;
        LED_num_2=GPIO_PIN_2;
    }else if(num==3){
        LED_num_1=GPIO_PIN_2;
        LED_num_2=GPIO_PIN_3;
    }
    GPIOPinWrite(GPIO_PORTD_BASE,LED_num_1,OFF);
    GPIOPinWrite(GPIO_PORTD_BASE,LED_num_2,ON);
    for(Loop=0;Loop<300000;Loop++);
}

void main(void){
    int i=1;

    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

    GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE,GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
    GPIOPinTypeGPIOInput(GPIO_PORTB_BASE,GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5);

    while(1){
        if(GPIOPinRead(GPIO_PORTB_BASE,GPIO_PIN_2)==0){
            if(i<3)
                 i+=1;
            else
                i=1;
            Cycle_temp(i);
        }
    }
}
