/*
 * step6-1.c
 *
 *  Created on: 2023/08/21
 *      Author: X20308
 *      LED“_“”
 */
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"

#define ON 0x00
#define OFF 0xFF

void main(void){
    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);

    GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE,GPIO_PIN_0);

    while(1){
        GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_0,ON);
    }
}


