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

void main(void){
    int Loop;

    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB));

    GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE,GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
    GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE,GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
    GPIOPinTypeGPIOInput(GPIO_PORTB_BASE,GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5);

    while(1){
        GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7,OFF);
        GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3,OFF);
        if(GPIOPinRead(GPIO_PORTB_BASE,GPIO_PIN_2)==0){
            GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7,OFF);
            GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3,OFF);
            for(Loop=0;Loop<20000;Loop++);
        }else if(GPIOPinRead(GPIO_PORTB_BASE,GPIO_PIN_3)==0){
            GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3,ON);
            GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7,ON);
            for(Loop=0;Loop<20000;Loop++);
        }else if(GPIOPinRead(GPIO_PORTB_BASE,GPIO_PIN_4)==0){
            GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3,ON);
            GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7,OFF);
            for(Loop=0;Loop<20000;Loop++);
        }else if(GPIOPinRead(GPIO_PORTB_BASE,GPIO_PIN_5)==0){
            GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3,OFF);
            GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7,ON);
            for(Loop=0;Loop<20000;Loop++);
        }
    }
}
