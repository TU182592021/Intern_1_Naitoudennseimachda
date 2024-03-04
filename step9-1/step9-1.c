/*
 * step9-1.c
 *
 *  Created on: 2023/08/22
 *      Author: X20308
 */
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/adc.h"

void main(void){
    uint32_t data;
    char State;

    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);

    GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE,0x0F);
    GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE,0xF0);

    GPIOPinTypeADC(GPIO_PORTE_BASE,GPIO_PIN_3);

    ADCSequenceConfigure(ADC0_BASE,3,ADC_TRIGGER_PROCESSOR,0);
    ADCSequenceStepConfigure(ADC0_BASE,3,0,ADC_CTL_CH0 | ADC_CTL_IE | ADC_CTL_END);
    ADCSequenceEnable(ADC0_BASE,3);

    while(1){
        ADCProcessorTrigger(ADC0_BASE,3);
        ADCSequenceDataGet(ADC0_BASE,3,&data);
        if(data<512){
            State=0x01;
        }else if(data<1024){
            State=0x03;
        }else if(data<1536){
            State=0x07;
        }else if(data<2048){
            State=0x0F;
        }else if(data<2560){
            State=0x1F;
        }else if(data<3072){
            State=0x3F;
        }else if(data<3584){
            State=0x7F;
        }else{
            State=0XFF;
        }
        GPIOPinWrite(GPIO_PORTD_BASE,0x0F,~State);
        GPIOPinWrite(GPIO_PORTC_BASE,0xF0,~State);

    }
}



