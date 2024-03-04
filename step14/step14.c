/*
 * step14.c
 *
 *  Created on: 2023/08/28
 *      Author: X20308
 */
/*
 * step13.c
 *
 *  Created on: 2023/08/24
 *      Author: X20308
 */
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
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
#include "driverlib/uart.h"
#include "driverlib/timer.h"

#define ON 0x00
#define OFF 0xFF
char transmitTemp[5]={0,0,0,0x0d};

void Timer0IntHandler(void){
    TimerIntClear(TIMER0_BASE,TIMER_TIMA_TIMEOUT);
    UARTCharPut(UART0_BASE, transmitTemp[0]);
    UARTCharPut(UART0_BASE, transmitTemp[1]);
    UARTCharPut(UART0_BASE, transmitTemp[2]);
    UARTCharPut(UART0_BASE, transmitTemp[3]);
}

volatile long g_cnt=0;

uint32_t Cycle_temp(uint32_t num){
    uint32_t STemp;
    int Loop;
    if(num==1){
        STemp=24;
    }else if(num==2){
        STemp=25;
    }else if(num==3){
        STemp=26;
    }else if(num==4){
        STemp=27;
    }else if(num==5){
        STemp=28;
    }else{

    }
    for(Loop=0;Loop<200000;Loop++);
    return STemp;
}

uint32_t AirFlow(uint32_t num){
    uint32_t duty;
    int Loop;
    if(num==1){
        duty=0;
    }else if(num==2){
        duty=3;
    }else if(num==3){
        duty=7;
    }else if(num==4){
        duty=9;
    }
    for(Loop=0;Loop<200000;Loop++);
    return duty;
}

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
    uint32_t i=1,average_data,sum_data,data_1,data_2,setting_Temp,setting_flow,switch_1=1,switch_2=1;
    bool tempValue;
    unsigned long ulPeriod;
    volatile unsigned long Loop;
    char temp[2]=" ",text_1[17],text_2[17];

    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);

    //UARTとポートへのクロック供給(必須)
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    //UARTとRX、TXの設定
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    //通信速度、通信仕様の設定
    UARTConfigSetExpClk(UART0_BASE,SysCtlClockGet(),38400,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

    //switch
    GPIOPinTypeGPIOInput(GPIO_PORTB_BASE,GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5);

    //LCD
    GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE,GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5);
    GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE,GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 |GPIO_PIN_5);

    //LED
    GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE,0x0F);
    GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE,0xF0);

    //A/D
    GPIOPinTypeADC(GPIO_PORTE_BASE,GPIO_PIN_3);
    ADCSequenceConfigure(ADC0_BASE,3,ADC_TRIGGER_PROCESSOR,0);
    ADCSequenceStepConfigure(ADC0_BASE,3,0,ADC_CTL_CH0 | ADC_CTL_IE | ADC_CTL_END);
    ADCSequenceEnable(ADC0_BASE,3);

    //PWM
    GPIOPinTypePWM(GPIO_PORTE_BASE,GPIO_PIN_4 |GPIO_PIN_5);
    GPIOPinConfigure(GPIO_PE4_M0PWM4);
    ulPeriod=SysCtlClockGet()/1000;
    PWMGenConfigure(PWM0_BASE,PWM_GEN_2,PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC);
    PWMGenPeriodSet(PWM0_BASE,PWM_GEN_2,ulPeriod);
    PWMPulseWidthSet(PWM0_BASE,PWM_OUT_4,ulPeriod*9/10);
    PWMOutputState(PWM0_BASE,PWM_OUT_4_BIT,true);
    PWMOutputState(PWM0_BASE,PWM_OUT_5_BIT,false);
    PWMGenEnable(PWM0_BASE,PWM_GEN_2);

    SysTickPeriodSet(SysCtlClockGet()/1000);
    SysTickIntRegister(SysTickHandler);
    lcd_init();
    lcd_clear();

    //Timer
    TimerConfigure(TIMER0_BASE,TIMER_CFG_PERIODIC);
    TimerLoadSet(TIMER0_BASE,TIMER_A,SysCtlClockGet());
    TimerIntRegister(TIMER0_BASE,TIMER_A,Timer0IntHandler);
    IntEnable(INT_TIMER0A);
    TimerIntEnable(TIMER0_BASE,TIMER_TIMA_TIMEOUT);
    TimerEnable(TIMER0_BASE,TIMER_A);

    while(1){
        //設定温度切り替え
        if(GPIOPinRead(GPIO_PORTB_BASE,GPIO_PIN_2)==0){
                    if(switch_1<5)
                         switch_1+=1;
                    else
                        switch_1=1;
                    setting_Temp=Cycle_temp(switch_1);
                }

       //強度切り替え
        if(GPIOPinRead(GPIO_PORTB_BASE,GPIO_PIN_3)==0){
                    if(switch_2<4){
                        switch_2+=1;
                    }

                    else{
                        switch_2=1;
                    }
                    setting_flow=AirFlow(switch_2);
                }

        //平均初期化
        if(GPIOPinRead(GPIO_PORTB_BASE,GPIO_PIN_4)==0){
                    i=1;
                    sum_data=0;
                }

        //強度切り替え上書き
        setting_flow=AirFlow(switch_2);

        //条件的初期化
        if(i>300){
            i=1;
            sum_data=0;
        }

        //A/D値取得及びノイズ除去
        if(i==1){
        ADCProcessorTrigger(ADC0_BASE,3);
        ADCSequenceDataGet(ADC0_BASE,3,&data_1);
        average_data=data_1;
        sum_data=data_1;
        }else{
            ADCProcessorTrigger(ADC0_BASE,3);
            ADCSequenceDataGet(ADC0_BASE,3,&data_2);
            sum_data+=data_2;
            average_data=(sum_data)/i;
        }

        //値渡し
        if(i!=1){
            data_1=data_2;
        }

        //A/D値
        average_data=floor(average_data*0.806);

        //LCD
        strcpy(text_1,"S_TEMP ");
        temp[0]=0x30+(setting_Temp/10);
        strcat(text_1,temp);
        temp[0]=0x30+(setting_Temp%10);
        strcat(text_1,temp);
        temp[0]=0xDF;
        strcat(text_1,temp);
        temp[0]=0x63;
        strcat(text_1,temp);
        temp[0]=0xA0;
        strcat(text_1,temp);
        temp[0]=0x30+(i/100);
        strcat(text_1,temp);
        temp[0]=0x30+(i%100/10);
        strcat(text_1,temp);
        temp[0]=0x30+(i%10);
        strcat(text_1,temp);

        strcpy(text_2,"TM&MD ");
        temp[0]=0x30+(((average_data-600)/10)/100);
        transmitTemp[0]=0x30+(((average_data-600)/10)/100);
        strcat(text_2,temp);
        temp[0]=0x30+((((average_data-600)/10)%100)/10);
        transmitTemp[1]=0x30+((((average_data-600)/10)%100)/10);
        strcat(text_2,temp);
        temp[0]=0x30+(((average_data-600)/10)%10);
        transmitTemp[2]=0x30+(((average_data-600)/10)%10);
        strcat(text_2,temp);
        temp[0]=0xDF;
        strcat(text_2,temp);
        temp[0]=0x63;
        strcat(text_2,temp);
        temp[0]=0xA0;
        strcat(text_2,temp);
        temp[0]=0x26;
        strcat(text_2,temp);
        temp[0]=0xA0;
        strcat(text_2,temp);
        temp[0]=0x30+(setting_flow/10);
        strcat(text_2,temp);
        temp[0]=0x30+(setting_flow%10);
        strcat(text_2,temp);

        if((average_data-600)/10>setting_Temp){
            tempValue=true;
        }else{
            tempValue=false;
        }

        //Motor
        if(tempValue){
            setting_flow=AirFlow(switch_2);
        }else{
            setting_flow=0;
        }
        PWMPulseWidthSet(PWM0_BASE,PWM_OUT_4,ulPeriod*setting_flow/10);

        lcd_disp(text_1);
        lcd_NewLine();
        lcd_disp(text_2);
        lcd_Home();

        //インクリメント
        i++;
    }
}




