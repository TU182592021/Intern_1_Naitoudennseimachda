/*
 * step9-2.c
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
#include "driverlib/adc.h"

#define ON 0x00
#define OFF 0xFF

char State;
int speed=1;
uint32_t data;

void Timer0IntHandler(void){
    static long period;
    TimerIntClear(TIMER0_BASE,TIMER_TIMA_TIMEOUT);
    State=~State;
    GPIOPinWrite(GPIO_PORTD_BASE,0x0F,State);
    GPIOPinWrite(GPIO_PORTC_BASE,0xF0,State);
    period=data/400;
    if(period<1)
        period=1;
    TimerLoadSet(TIMER0_BASE,TIMER_A,SysCtlClockGet()/period);
}

void main(void){


    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);

    GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE,0x0F);
    GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE,0xF0);
    GPIOPinTypeADC(GPIO_PORTE_BASE,GPIO_PIN_3);

    ADCSequenceConfigure(ADC0_BASE,3,ADC_TRIGGER_PROCESSOR,0);
    ADCSequenceStepConfigure(ADC0_BASE,3,0,ADC_CTL_CH0 | ADC_CTL_IE | ADC_CTL_END);
    ADCSequenceEnable(ADC0_BASE,3);

    TimerConfigure(TIMER0_BASE,TIMER_CFG_PERIODIC);
    TimerLoadSet(TIMER0_BASE,TIMER_A,SysCtlClockGet());
    TimerIntRegister(TIMER0_BASE,TIMER_A,Timer0IntHandler);
    IntEnable(INT_TIMER0A);
    TimerIntEnable(TIMER0_BASE,TIMER_TIMA_TIMEOUT);
    TimerEnable(TIMER0_BASE,TIMER_A);
    State=OFF;
    GPIOPinWrite(GPIO_PORTD_BASE,0x0F,State);
    GPIOPinWrite(GPIO_PORTC_BASE,0xF0,State);
    while(1){
                ADCProcessorTrigger(ADC0_BASE,3);
                ADCSequenceDataGet(ADC0_BASE,3,&data);
    }
}









