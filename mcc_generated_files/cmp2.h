/**
  CMP2 Generated Driver API Header File

  @Company
    Microchip Technology Inc.

  @File Name
    cmp2.h

  @Summary
    This is the generated header file for the CMP2 driver using PIC24 / dsPIC33 / PIC32MM MCUs

  @Description
    This header file provides APIs for driver for CMP2.
    Generation Information :
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.169.0
        Device            :  PIC24FJ64GC006
    The generated drivers are tested against the following:
        Compiler          :  XC16 v1.50
        MPLAB 	          :  MPLAB X v5.40
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

#ifndef _CMP2_H
#define _CMP2_H

/**
  Section: Included Files
*/

#include <xc.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif


/**
  @Summary
    This function initializes Comparator instance : 2

  @Description
    This routine initializes the Comparator driver instance for : 2, 
    making it ready for clients to open and use it. It also initializes any
    internal data structures.

  @Param
    None.

  @Returns 
    None
 
  @Comment
    
*/
void CMP2_Initialize(void);

/**
  @Summary
    This function returns the output of the comparator

  @Description
    This routine returns the output of the comparator. It returns a bool which 
    is the output of the comparator.

  @Param
    None.

  @Returns 
    None
 
*/
bool CMP2_OutputStatusGet(void);

        
/**
  @Summary
    This function returns the if the comparator event has occurred or not

  @Description
    This routine returns true if the comparator event has occurred. It returns false 
    if the comparator event has not occurred yet.

  @Param
    None.

  @Returns 
    bool.
 
*/
bool CMP2_EventStatusGet(void);

/**
  @Summary
    This function resets the event status bit of the comparator.

  @Description
    This routine resets the event status bit of the comparator after the comparator
    event has occurred.

  @Param
    None.

  @Returns 
    bool.
 
*/
void CMP2_EventStatusReset(void);

/**
  @Summary
    Callback for CMP2.

  @Description
    This routine is callback for CMP2

  @Param
    None.

  @Returns
    None
 
  @Example 
	Refer to CMP2_Initialize(); for an example
*/
void CMP2_CallBack(void);


/******************************************************************************
*                                                                             
*    Function:			CMP2_Enable
*    Description:       Enables the CMP module                                     
*      
*	 Parameters:		None                                       
*    Return Value:      None
******************************************************************************/
inline static void CMP2_Enable(void)
{
    CM2CONbits.CON = 1;
}

/******************************************************************************
*                                                                             
*    Function:			CMP2_Disable
*    Description:       Disables the CMP module                                     
*      
*	 Parameters:		None                                       
*    Return Value:      None
******************************************************************************/
inline static void CMP2_Disable(void)
{
    CM2CONbits.CON = 0;
}

#ifdef __cplusplus  // Provide C++ Compatibility

    }

#endif
    
#endif //_CMP2_H

/**
  End of File
*/
