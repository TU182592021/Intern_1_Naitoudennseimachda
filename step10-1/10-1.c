/*
 * 10-1.c
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
#include "driverlib/timer.h"
#include "driverlib/adc.h"
#include "driverlib/pin_map.h"
#include "driverlib/pwm.h"

void main(void){
    uint32_t data;
    unsigned long ulPeriod;
    volatile unsigned long Loop;

    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);

    GPIOPinTypeADC(GPIO_PORTE_BASE,GPIO_PIN_3);
    ADCSequenceConfigure(ADC0_BASE,3,ADC_TRIGGER_PROCESSOR,0);
    ADCSequenceStepConfigure(ADC0_BASE,3,0,ADC_CTL_CH0 | ADC_CTL_IE | ADC_CTL_END);
    ADCSequenceEnable(ADC0_BASE,3);

    GPIOPinTypePWM(GPIO_PORTD_BASE,GPIO_PIN_0);
    GPIOPinConfigure(GPIO_PD0_M0PWM6);
    ulPeriod=SysCtlClockGet()/1000;
    PWMGenConfigure(PWM0_BASE,PWM_GEN_3,PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC);
    PWMGenPeriodSet(PWM0_BASE,PWM_GEN_3,ulPeriod);
    PWMPulseWidthSet(PWM0_BASE,PWM_OUT_6,ulPeriod*9/10);
    PWMOutputState(PWM0_BASE,PWM_OUT_6_BIT,true);
    PWMGenEnable(PWM0_BASE,PWM_GEN_3);

    while(1){
        ADCProcessorTrigger(ADC0_BASE,3);
        ADCSequenceDataGet(ADC0_BASE,3,&data);
        PWMPulseWidthSet(PWM0_BASE,PWM_OUT_6,ulPeriod*(4096-data)/4097);
    }



}



