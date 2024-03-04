/*
 * step16.c
 *
 *  Created on: 2023/08/29
 *      Author: X20308
 */
/*
 * step15.c
 *
 *  Created on: 2023/08/29
 *      Author: X20308
 */

#include "driverlib/ssi.c"
#include <stdio.h>
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


void main(void){
    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
    char *pcChars = "SSI Master send data.";
    int32_t i32Idx;
    //
    // Enable the SSI0 peripheral
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI0);
    //
    // Wait for the SSI0 module to be ready.
    //
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_SSI0))
    {
    }
    //
    // Configure the SSI.
    //
    SSIConfigSetExpClk(SSI0_BASE, SysCtlClockGet(), SSI_FRF_MOTO_MODE0,
    SSI_MODE_MASTER, 2000000, 8);
    //
    // Enable the SSI module.
    //
    SSIEnable(SSI0_BASE);
    //
    // Send some data.
    //
    i32Idx = 0;
    while(pcChars[i32Idx])
    {
    SSIDataPut(SSI0_BASE, pcChars[i32Idx]);
    i32Idx++;
    }

}







