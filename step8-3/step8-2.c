/*
 * step8-2.c
 *
 *  Created on: 2023/08/22
 *      Author: X20308
 */
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_ints.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"

#define ON 0x00
#define OFF 0xFF

char State;
int speed=1;
int Loop;

void Timer0IntHandler(void){
    TimerIntClear(TIMER0_BASE,TIMER_TIMA_TIMEOUT);
    State=~State;
    GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_0,State);
}

void main(void){
    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);

    GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE,GPIO_PIN_0);
    GPIOPinTypeGPIOInput(GPIO_PORTB_BASE,GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5);

    TimerConfigure(TIMER0_BASE,TIMER_CFG_PERIODIC);
    TimerLoadSet(TIMER0_BASE,TIMER_A,SysCtlClockGet()/speed);
    TimerIntRegister(TIMER0_BASE,TIMER_A,Timer0IntHandler);
    IntEnable(INT_TIMER0A);
    TimerIntEnable(TIMER0_BASE,TIMER_TIMA_TIMEOUT);
    TimerEnable(TIMER0_BASE,TIMER_A);
    State=OFF;
    GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_0,State);
    while(1){
        if(GPIOPinRead(GPIO_PORTB_BASE,GPIO_PIN_2)==0){
                speed++;
                TimerLoadSet(TIMER0_BASE,TIMER_A,SysCtlClockGet()/speed);
                for(Loop=0;Loop<200000;Loop++);
            }
            if(GPIOPinRead(GPIO_PORTB_BASE,GPIO_PIN_3)==0){
                if(speed>1){
                    speed--;
                    TimerLoadSet(TIMER0_BASE,TIMER_A,SysCtlClockGet()/speed);
                    for(Loop=0;Loop<200000;Loop++);
                }else{
                    for(Loop=0;Loop<200000;Loop++);
                }
            }
    }
}




