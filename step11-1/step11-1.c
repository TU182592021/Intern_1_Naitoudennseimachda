/*
 * step11-1.c
 *
 *  Created on: 2023/08/23
 *      Author: X20308
 */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"
#include "driverlib/adc.h"
#include "driverlib/pin_map.h"
#include "driverlib/pwm.h"

volatile long g_cnt=0;

void SysTickHandler(void){
    g_cnt--;
}

void DelayMs(unsigned long ms){
    g_cnt=ms;
    SysTickEnable();
    while(g_cnt>0);
        SysTickDisable();
}

void lcd_RS(unsigned char value){
    if(value!=0)
        value=0xFF;
    GPIOPinWrite(GPIO_PORTE_BASE,GPIO_PIN_1,value);
}

void lcd_command(){
    GPIOPinWrite(GPIO_PORTE_BASE,GPIO_PIN_0,0xFF);
    GPIOPinWrite(GPIO_PORTE_BASE,GPIO_PIN_0,0x00);
}

void lcd_transmit8(unsigned char data){
    data=data>>2;
    GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5,data);
    lcd_command();
}

void lcd_transmit4(unsigned char data){
    unsigned char temp;
    temp=data>>2;
    GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5,temp);
    lcd_command();
    temp=data<<2;
    GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5,temp);
    lcd_command();
}

void lcd_init(void){
    lcd_RS(0);
    DelayMs(15);
    lcd_transmit8(0x30);
    DelayMs(5);
    lcd_transmit8(0x30);
    DelayMs(1);
    lcd_transmit8(0x30);
    DelayMs(5);
    lcd_transmit8(0x20);
    DelayMs(1);
    lcd_transmit4(0x28);
    DelayMs(1);
    lcd_transmit4(0x0C);
    DelayMs(1);
    lcd_transmit4(0x06);
    DelayMs(1);
}

void lcd_clear(void){
    lcd_RS(0);
    lcd_transmit4(0x01);
    DelayMs(1);
}

void lcd_disp(char* s){
    lcd_RS(1);
    while(*s){
        lcd_transmit4(*s++);
        DelayMs(1);
    }
}

void lcd_NewLine(void){
    lcd_RS(0);
    lcd_transmit4(0xC0);
    DelayMs(1);

}

void lcd_Home(void){
    lcd_RS(0);
    lcd_transmit4(0x02);
    DelayMs(1);
}

void main(void){
    uint32_t data;
    unsigned long ulPeriod;
    volatile unsigned long Loop;
    char temp[2]=" ";
    char text_1[17];
    char text_2[17];

    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);

    GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE,GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5);
    GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE,GPIO_PIN_0 | GPIO_PIN_1);
    GPIOPinTypeADC(GPIO_PORTE_BASE,GPIO_PIN_3);

    ADCSequenceConfigure(ADC0_BASE,3,ADC_TRIGGER_PROCESSOR,0);
    ADCSequenceStepConfigure(ADC0_BASE,3,0,ADC_CTL_CH0 | ADC_CTL_IE | ADC_CTL_END);
    ADCSequenceEnable(ADC0_BASE,3);

    GPIOPinTypePWM(GPIO_PORTE_BASE,GPIO_PIN_4 |GPIO_PIN_5);
    GPIOPinConfigure(GPIO_PE4_M0PWM4);
    GPIOPinConfigure(GPIO_PE5_M0PWM5);

    ulPeriod=SysCtlClockGet()/1000;
    PWMGenConfigure(PWM0_BASE,PWM_GEN_2,PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC);
    PWMGenPeriodSet(PWM0_BASE,PWM_GEN_2,ulPeriod);
    PWMPulseWidthSet(PWM0_BASE,PWM_OUT_4,ulPeriod*9/10);
    PWMPulseWidthSet(PWM0_BASE,PWM_OUT_5,ulPeriod*9/10);
    PWMOutputState(PWM0_BASE,PWM_OUT_4_BIT,true);
    PWMOutputState(PWM0_BASE,PWM_OUT_5_BIT,false);
    PWMGenEnable(PWM0_BASE,PWM_GEN_2);
    SysTickPeriodSet(SysCtlClockGet()/1000);
    SysTickIntRegister(SysTickHandler);
    lcd_init();
    lcd_clear();
    while(1){
        strcpy(text_1,"A/D ");
        strcpy(text_2,"Duty ");
        ADCProcessorTrigger(ADC0_BASE,3);
        ADCSequenceDataGet(ADC0_BASE,3,&data);
        PWMPulseWidthSet(PWM0_BASE,PWM_OUT_4,ulPeriod*data/4096);
        temp[0]=0x30+(data/1000);
        strcat(text_1,temp);
        temp[0]=0x30+((data%1000)/100);
        strcat(text_1,temp);
        temp[0]=0x30+((data%100)/10);
        strcat(text_1,temp);
        temp[0]=0x30+(data%10);
        strcat(text_1,temp);
        temp[0]=0x30+((data*100/4096)/1000);
        strcat(text_2,temp);
        temp[0]=0x30+(((data*1000/4096)%100)/10);
        strcat(text_2,temp);
        temp[0]=0x30+((data*100/4096)%10);
        strcat(text_2,temp);
        temp[0]='%';
        strcat(text_2,temp);
        lcd_disp(text_1);
        lcd_NewLine();
        lcd_disp(text_2);
        lcd_Home();


    }
}
