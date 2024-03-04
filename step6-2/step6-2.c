/*
 * step6-1.c
 *
 *  Created on: 2023/08/22
 *      Author: X20308
 *      LEDŠÔŠu“_“”
 */
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"

#define ON 0x0000
#define OFF 0xFF

void main(void){
    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);

    GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE,GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
    GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE,GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);

    while(1){
        GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_0 | GPIO_PIN_2,ON);
        GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_4 | GPIO_PIN_6,ON);

        /*
        GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3,ON);
        GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7,ON);
        */
    }
}




