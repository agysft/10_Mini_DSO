
/**
  CMP2 Generated Driver File 

  @Company
    Microchip Technology Inc.

  @File Name
    cmp2.c

  @Summary
    This is the generated driver implementation file for the CMP2 driver using PIC24 / dsPIC33 / PIC32MM MCUs

  @Description
    This header file provides implementations for driver APIs for CMP2. 
    Generation Information : 
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.169.0
        Device            :  PIC24FJ64GC006
    The generated drivers are tested against the following:
        Compiler          :  XC16 v1.50
        MPLAB             :  MPLAB X v5.40
*/

/*
    (c) 2020 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

/**
  Section: Included Files
*/

#include "cmp2.h"

/**
  Section: Driver Interface
*/

void CMP2_Initialize(void)
{   
    IEC1bits.CMIE = 0;
    
    // CMIDL disabled; 
    CMSTAT = 0x00;
    // CON enabled; CPOL Not Inverted; EVPOL Any Change; COE disabled; CCH C2INB; CREF CVREF; CEVT disabled; 
    CM2CON = 0x80D0 & ~(0x8000);
    
    // Clearing IF flag before enabling the interrupt.
    IFS1bits.CMIF = 0;
    // Enabling CMP2 interrupt.
    IEC1bits.CMIE = 1;

    CMP2_Enable();
}

bool CMP2_OutputStatusGet(void)
{
    return (CM2CONbits.COUT);
}
bool CMP2_EventStatusGet(void)
{   
    return (CM2CONbits.CEVT);      
}
void CMP2_EventStatusReset(void)
{
    CM2CONbits.CEVT = 0;
}

void __attribute__ ((weak)) CMP2_CallBack(void)
{
    // Add your custom callback code here
}

void __attribute__ ( ( interrupt, no_auto_psv ) ) _CompInterrupt(void)
{
    // CMP2 callback function 
    CMP2_CallBack();

    // Clear the CEVT bit to enable further interrupts
    CMP2_EventStatusReset();
	
    // clear the CMP2 interrupt flag
    IFS1bits.CMIF = 0;
}

/**
  End of File
*/
