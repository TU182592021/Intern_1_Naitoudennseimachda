/*
 * step04.c
 *
 *  Created on: 2023/08/21
 *      Author: X20308
 */
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
void main(void){
    volatile unsigned Loop;
    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,GPIO_PIN_2);
    while(-1){
        GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_2,GPIO_PIN_2);
        for(Loop=0;Loop<200000;Loop++);
        GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_2,0);
        for(Loop=0;Loop<200000;Loop++);
    }
}



