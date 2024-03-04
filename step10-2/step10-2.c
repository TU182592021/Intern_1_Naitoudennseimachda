/*
 * 10-2.c
 *
 *  Created on: 2023/08/23
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

#define ON 0x00
#define OFF 0xFF

void main(void){
    uint32_t data;
    unsigned long ulPeriod;
    volatile unsigned long Loop;

    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);

    GPIOPinTypeADC(GPIO_PORTE_BASE,GPIO_PIN_3);
    ADCSequenceConfigure(ADC0_BASE,3,ADC_TRIGGER_PROCESSOR,0);
    ADCSequenceStepConfigure(ADC0_BASE,3,0,ADC_CTL_CH0 | ADC_CTL_IE | ADC_CTL_END);
    ADCSequenceEnable(ADC0_BASE,3);

    GPIOPinTypePWM(GPIO_PORTE_BASE,GPIO_PIN_4 |GPIO_PIN_5);
    GPIOPinConfigure(GPIO_PE4_M0PWM4);
    ulPeriod=SysCtlClockGet()/1000;
    PWMGenConfigure(PWM0_BASE,PWM_GEN_2,PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC);
    PWMGenPeriodSet(PWM0_BASE,PWM_GEN_2,ulPeriod);
    PWMPulseWidthSet(PWM0_BASE,PWM_OUT_4,ulPeriod*9/10);
    PWMOutputState(PWM0_BASE,PWM_OUT_4_BIT,true);
    PWMOutputState(PWM0_BASE,PWM_OUT_5_BIT,false);
    PWMGenEnable(PWM0_BASE,PWM_GEN_2);

    while(1){
        ADCProcessorTrigger(ADC0_BASE,3);
        ADCSequenceDataGet(ADC0_BASE,3,&data);
        PWMPulseWidthSet(PWM0_BASE,PWM_OUT_4,ulPeriod*9/10);
        for(Loop=0;Loop<200000;Loop++);
        PWMPulseWidthSet(PWM0_BASE,PWM_OUT_4,ulPeriod*0);
        for(Loop=0;Loop<200000;Loop++);

    }

}



