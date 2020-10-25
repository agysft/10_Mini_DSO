/**
  PIN MANAGER Generated Driver File

  @Company:
    Microchip Technology Inc.

  @File Name:
    pin_manager.h

  @Summary:
    This is the generated manager file for the PIC24 / dsPIC33 / PIC32MM MCUs device.  This manager
    configures the pins direction, initial state, analog setting.
    The peripheral pin select, PPS, configuration is also handled by this manager.

  @Description:
    This source file provides implementations for PIN MANAGER.
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

#ifndef _PIN_MANAGER_H
#define _PIN_MANAGER_H
/**
    Section: Includes
*/
#include <xc.h>

/**
    Section: Device Pin Macros
*/
/**
  @Summary
    Sets the GPIO pin, RE0, high using LATE0.

  @Description
    Sets the GPIO pin, RE0, high using LATE0.

  @Preconditions
    The RE0 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RE0 high (1)
    IO_RE0_SetHigh();
    </code>

*/
#define IO_RE0_SetHigh()          (_LATE0 = 1)
/**
  @Summary
    Sets the GPIO pin, RE0, low using LATE0.

  @Description
    Sets the GPIO pin, RE0, low using LATE0.

  @Preconditions
    The RE0 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RE0 low (0)
    IO_RE0_SetLow();
    </code>

*/
#define IO_RE0_SetLow()           (_LATE0 = 0)
/**
  @Summary
    Toggles the GPIO pin, RE0, using LATE0.

  @Description
    Toggles the GPIO pin, RE0, using LATE0.

  @Preconditions
    The RE0 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Toggle RE0
    IO_RE0_Toggle();
    </code>

*/
#define IO_RE0_Toggle()           (_LATE0 ^= 1)
/**
  @Summary
    Reads the value of the GPIO pin, RE0.

  @Description
    Reads the value of the GPIO pin, RE0.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    uint16_t portValue;

    // Read RE0
    postValue = IO_RE0_GetValue();
    </code>

*/
#define IO_RE0_GetValue()         _RE0
/**
  @Summary
    Configures the GPIO pin, RE0, as an input.

  @Description
    Configures the GPIO pin, RE0, as an input.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RE0 as an input
    IO_RE0_SetDigitalInput();
    </code>

*/
#define IO_RE0_SetDigitalInput()  (_TRISE0 = 1)
/**
  @Summary
    Configures the GPIO pin, RE0, as an output.

  @Description
    Configures the GPIO pin, RE0, as an output.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RE0 as an output
    IO_RE0_SetDigitalOutput();
    </code>

*/
#define IO_RE0_SetDigitalOutput() (_TRISE0 = 0)
/**
  @Summary
    Sets the GPIO pin, RG2, high using LATG2.

  @Description
    Sets the GPIO pin, RG2, high using LATG2.

  @Preconditions
    The RG2 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RG2 high (1)
    IO_RG2_SetHigh();
    </code>

*/
#define IO_RG2_SetHigh()          (_LATG2 = 1)
/**
  @Summary
    Sets the GPIO pin, RG2, low using LATG2.

  @Description
    Sets the GPIO pin, RG2, low using LATG2.

  @Preconditions
    The RG2 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RG2 low (0)
    IO_RG2_SetLow();
    </code>

*/
#define IO_RG2_SetLow()           (_LATG2 = 0)
/**
  @Summary
    Toggles the GPIO pin, RG2, using LATG2.

  @Description
    Toggles the GPIO pin, RG2, using LATG2.

  @Preconditions
    The RG2 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Toggle RG2
    IO_RG2_Toggle();
    </code>

*/
#define IO_RG2_Toggle()           (_LATG2 ^= 1)
/**
  @Summary
    Reads the value of the GPIO pin, RG2.

  @Description
    Reads the value of the GPIO pin, RG2.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    uint16_t portValue;

    // Read RG2
    postValue = IO_RG2_GetValue();
    </code>

*/
#define IO_RG2_GetValue()         _RG2
/**
  @Summary
    Configures the GPIO pin, RG2, as an input.

  @Description
    Configures the GPIO pin, RG2, as an input.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RG2 as an input
    IO_RG2_SetDigitalInput();
    </code>

*/
#define IO_RG2_SetDigitalInput()  (_TRISG2 = 1)
/**
  @Summary
    Configures the GPIO pin, RG2, as an output.

  @Description
    Configures the GPIO pin, RG2, as an output.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RG2 as an output
    IO_RG2_SetDigitalOutput();
    </code>

*/
#define IO_RG2_SetDigitalOutput() (_TRISG2 = 0)
/**
  @Summary
    Sets the GPIO pin, RG3, high using LATG3.

  @Description
    Sets the GPIO pin, RG3, high using LATG3.

  @Preconditions
    The RG3 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RG3 high (1)
    IO_RG3_SetHigh();
    </code>

*/
#define IO_RG3_SetHigh()          (_LATG3 = 1)
/**
  @Summary
    Sets the GPIO pin, RG3, low using LATG3.

  @Description
    Sets the GPIO pin, RG3, low using LATG3.

  @Preconditions
    The RG3 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RG3 low (0)
    IO_RG3_SetLow();
    </code>

*/
#define IO_RG3_SetLow()           (_LATG3 = 0)
/**
  @Summary
    Toggles the GPIO pin, RG3, using LATG3.

  @Description
    Toggles the GPIO pin, RG3, using LATG3.

  @Preconditions
    The RG3 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Toggle RG3
    IO_RG3_Toggle();
    </code>

*/
#define IO_RG3_Toggle()           (_LATG3 ^= 1)
/**
  @Summary
    Reads the value of the GPIO pin, RG3.

  @Description
    Reads the value of the GPIO pin, RG3.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    uint16_t portValue;

    // Read RG3
    postValue = IO_RG3_GetValue();
    </code>

*/
#define IO_RG3_GetValue()         _RG3
/**
  @Summary
    Configures the GPIO pin, RG3, as an input.

  @Description
    Configures the GPIO pin, RG3, as an input.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RG3 as an input
    IO_RG3_SetDigitalInput();
    </code>

*/
#define IO_RG3_SetDigitalInput()  (_TRISG3 = 1)
/**
  @Summary
    Configures the GPIO pin, RG3, as an output.

  @Description
    Configures the GPIO pin, RG3, as an output.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RG3 as an output
    IO_RG3_SetDigitalOutput();
    </code>

*/
#define IO_RG3_SetDigitalOutput() (_TRISG3 = 0)

/**
    Section: Function Prototypes
*/
/**
  @Summary
    Configures the pin settings of the PIC24FJ64GC006
    The peripheral pin select, PPS, configuration is also handled by this manager.

  @Description
    This is the generated manager file for the PIC24 / dsPIC33 / PIC32MM MCUs device.  This manager
    configures the pins direction, initial state, analog setting.
    The peripheral pin select, PPS, configuration is also handled by this manager.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    void SYSTEM_Initialize(void)
    {
        // Other initializers are called from this function
        PIN_MANAGER_Initialize();
    }
    </code>

*/
void PIN_MANAGER_Initialize (void);



#endif
