/*
 * step6-1.c
 *
 *  Created on: 2023/08/22
 *      Author: X20308
 *      LEDイルミネーション
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
    int GPIO_PIN_i[]={GPIO_PIN_0,GPIO_PIN_1,GPIO_PIN_2,GPIO_PIN_3,GPIO_PIN_4,GPIO_PIN_5,GPIO_PIN_6,GPIO_PIN_7};
    int Loop;
    int LED_Number;
    int time=0;

    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);

    GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE,GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
    GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE,GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
while(1){
    if(time%2==0){
        for(LED_Number=0;LED_Number<8;LED_Number++){
                    if(LED_Number<4){
                        GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_i[LED_Number],ON);
                        for(Loop=0;Loop<200000;Loop++);
                        GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_i[LED_Number],OFF);
                    }else{
                        GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_i[LED_Number],ON);
                        for(Loop=0;Loop<200000;Loop++);
                        GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_i[LED_Number],OFF);
                    }
        }
        time++;
    }else if(time%2!=0){
        for(LED_Number=7;LED_Number>=0;LED_Number--){
                                            if(LED_Number<4){
                                                GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_i[LED_Number],ON);
                                                for(Loop=0;Loop<200000;Loop++);
                                                GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_i[LED_Number],OFF);
                                            }else{
                                                GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_i[LED_Number],ON);
                                                for(Loop=0;Loop<200000;Loop++);
                                                GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_i[LED_Number],OFF);
                                            }
                }
            time++;
        }
    }
}




