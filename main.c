/**
  Generated main.c file from MPLAB Code Configurator

  @Company
    Microchip Technology Inc.

  @File Name
    main.c

  @Summary
    This is the generated main.c using PIC24 / dsPIC33 / PIC32MM MCUs.

  @Description
    This source file provides main entry point for system initialization and application code development.
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

/**
  Section: Included Files
*/
#include "mcc_generated_files/system.h"
#define FCY 16000000UL
#include <libpic30.h>
#include "font16.h"
#include "mcc_generated_files/spi1.h"
#include "mcc_generated_files/tmr5.h"
#include <stdio.h>

/* LEDs and Switches */
#define LED1 PORTEbits.RE0
#define LEDRotaryEncoderOrange PORTGbits.RG3
#define LEDRotaryEncoderBlue PORTGbits.RG2
#define SWRotaryEncoder PORTDbits.RD7
#define SW1 PORTEbits.RE7
#define SW2 PORTEbits.RE6
#define SW3 PORTEbits.RE5
#define SW4 PORTEbits.RE1
/* SPI LCD */
#define LCD_2_4inch   //2.4inch LCD, No define = 2inch LCD
#define GLCDReset   PORTDbits.RD5
#define GLCDCS      PORTDbits.RD3
#define GLCDDC      PORTDbits.RD4
#define GLCDBL      PORTDbits.RD6
#define LCD_WIDTH   320 //LCD width
#define LCD_HEIGHT  240 //LCD height
/* color */
#define ColorBlack      0x0000
#define ColorBlue       0x001f
#define ColorGreen      0x07e0
#define ColorRed        0xf800
#define ColorCyan       0x07ff
#define ColorMagenta    0xf81f
#define ColorYellow     0xffe0
#define ColorWhite      0xffff
/* Attenuator ON-OFF */
#define CH1ATT  PORTGbits.RG7   // 0=ATT-ON(15V), 1=ATT-OFF(1.25V)
#define CH2ATT  PORTGbits.RG8   // 0=ATT-ON(15V), 1=ATT-OFF(1.25V)

// Rotary Encoder
int pressedTime = 0; int rotData, rotDir, swPos; float rotVal; float rotValMag = 0.5;

void GLCD_COM(uint8_t acommand){
    GLCDDC = 0;     //0:command
    while( SPI1STATbits.SPITBF == true ){}
    SPI1BUF = acommand;
    while ( SPI1STATbits.SRXMPT == true);
    acommand = SPI1BUF;
}
void GLCD_DAT(uint8_t acommand){
    GLCDDC = 1;     //1:data
    while( SPI1STATbits.SPITBF == true ){}
    SPI1BUF = acommand;
    while ( SPI1STATbits.SRXMPT == true);
    acommand = SPI1BUF;
}
void GLCD_DAT16(uint16_t acommand){
    GLCDDC = 1;     //1:data
    uint8_t tmpcommand;
    while( SPI1STATbits.SPITBF == true ){}
    SPI1BUF = (acommand>>8) & 0xff;
    while ( SPI1STATbits.SRXMPT == true);
    tmpcommand = SPI1BUF;
    while( SPI1STATbits.SPITBF == true ){}
    SPI1BUF = acommand & 0xff;
    while ( SPI1STATbits.SRXMPT == true);
    acommand = SPI1BUF;
}
uint16_t GLCD_Read16(uint16_t X, uint16_t Y){
    GLCD_COM(0x2a); // Column Address Set
    GLCD_DAT16(X);
	GLCD_COM(0x2b); // Page Address Set
    GLCD_DAT16(Y);
	GLCD_COM(0x2E); // Memory read
    return SPI1BUF;
}
void GLCD_Init(){
    __delay_ms(200);
    GLCDReset = 0;  //RESET
    __delay_ms(200);
    GLCDReset = 1;
    __delay_ms(200);
    GLCDCS = 0;     //CS=enable

#ifdef LCD_2_4inch
    /* for 2.4inch */
	GLCD_COM(0x11); //Sleep out
	
	GLCD_COM(0xCF);
	GLCD_DAT(0x00);
	GLCD_DAT(0xC1);
	GLCD_DAT(0X30);
	GLCD_COM(0xED);
	GLCD_DAT(0x64);
	GLCD_DAT(0x03);
	GLCD_DAT(0X12);
	GLCD_DAT(0X81);
	GLCD_COM(0xE8);
	GLCD_DAT(0x85);
	GLCD_DAT(0x00);
	GLCD_DAT(0x79);
	GLCD_COM(0xCB);
	GLCD_DAT(0x39);
	GLCD_DAT(0x2C);
	GLCD_DAT(0x00);
	GLCD_DAT(0x34);
	GLCD_DAT(0x02);
	GLCD_COM(0xF7);
	GLCD_DAT(0x20);
	GLCD_COM(0xEA);
	GLCD_DAT(0x00);
	GLCD_DAT(0x00);
	GLCD_COM(0xC0); //Power control
	GLCD_DAT(0x1D); //VRH[5:0]
	GLCD_COM(0xC1); //Power control
	GLCD_DAT(0x12); //SAP[2:0];BT[3:0]
	GLCD_COM(0xC5); //VCM control
	GLCD_DAT(0x33);
	GLCD_DAT(0x3F);
	GLCD_COM(0xC7); //VCM control
	GLCD_DAT(0x92);
	GLCD_COM(0x3A); // Memory Access Control
	GLCD_DAT(0x55);
	GLCD_COM(0x36); // Memory Access Control
//	GLCD_DAT(0x08);
	GLCD_DAT(0b10101000); // MV=1,MX=0,MY=1   MY MX MV ML BGR MH 0 0
//	GLCD_DAT(0b01101000); // MV=1,MX=1,MY=0   MY MX MV ML BGR MH 0 0
	GLCD_COM(0xB1);
	GLCD_DAT(0x00);
	GLCD_DAT(0x12);
	GLCD_COM(0xB6); // Display Function Control
	GLCD_DAT(0x0A);
	GLCD_DAT(0xA2);

	GLCD_COM(0x44);
	GLCD_DAT(0x02);

	GLCD_COM(0xF2); // 3Gamma Function Disable
	GLCD_DAT(0x00);
	GLCD_COM(0x26); //Gamma curve selected
	GLCD_DAT(0x01);
	GLCD_COM(0xE0); //Set Gamma
	GLCD_DAT(0x0F);
	GLCD_DAT(0x22);
	GLCD_DAT(0x1C);
	GLCD_DAT(0x1B);
	GLCD_DAT(0x08);
	GLCD_DAT(0x0F);
	GLCD_DAT(0x48);
	GLCD_DAT(0xB8);
	GLCD_DAT(0x34);
	GLCD_DAT(0x05);
	GLCD_DAT(0x0C);
	GLCD_DAT(0x09);
	GLCD_DAT(0x0F);
	GLCD_DAT(0x07);
	GLCD_DAT(0x00);
	GLCD_COM(0XE1); //Set Gamma
	GLCD_DAT(0x00);
	GLCD_DAT(0x23);
	GLCD_DAT(0x24);
	GLCD_DAT(0x07);
	GLCD_DAT(0x10);
	GLCD_DAT(0x07);
	GLCD_DAT(0x38);
	GLCD_DAT(0x47);
	GLCD_DAT(0x4B);
	GLCD_DAT(0x0A);
	GLCD_DAT(0x13);
	GLCD_DAT(0x06);
	GLCD_DAT(0x30);
	GLCD_DAT(0x38);
	GLCD_DAT(0x0F);
	GLCD_COM(0x29); //Display on
#else
    /* for 2inch */
	GLCD_COM(0x36);
	GLCD_DAT(0xA0);

	GLCD_COM(0x3A); 
	GLCD_DAT(0x05);

	GLCD_COM(0x21); 

	GLCD_COM(0x2A);
	GLCD_DAT(0x00);
	GLCD_DAT(0x01);
	GLCD_DAT(0x00);
	GLCD_DAT(0x3F);

	GLCD_COM(0x2B);
	GLCD_DAT(0x00);
	GLCD_DAT(0x00);
	GLCD_DAT(0x00);
	GLCD_DAT(0xEF);

	GLCD_COM(0xB2);
	GLCD_DAT(0x0C);
	GLCD_DAT(0x0C);
	GLCD_DAT(0x00);
	GLCD_DAT(0x33);
	GLCD_DAT(0x33);

	GLCD_COM(0xB7);
	GLCD_DAT(0x35); 

	GLCD_COM(0xBB);
	GLCD_DAT(0x1F);

	GLCD_COM(0xC0);
	GLCD_DAT(0x2C);

	GLCD_COM(0xC2);
	GLCD_DAT(0x01);

	GLCD_COM(0xC3);
	GLCD_DAT(0x12);   

	GLCD_COM(0xC4);
	GLCD_DAT(0x20);

	GLCD_COM(0xC6);
	GLCD_DAT(0x0F); 

	GLCD_COM(0xD0);
	GLCD_DAT(0xA4);
	GLCD_DAT(0xA1);

	GLCD_COM(0xE0);
	GLCD_DAT(0xD0);
	GLCD_DAT(0x08);
	GLCD_DAT(0x11);
	GLCD_DAT(0x08);
	GLCD_DAT(0x0C);
	GLCD_DAT(0x15);
	GLCD_DAT(0x39);
	GLCD_DAT(0x33);
	GLCD_DAT(0x50);
	GLCD_DAT(0x36);
	GLCD_DAT(0x13);
	GLCD_DAT(0x14);
	GLCD_DAT(0x29);
	GLCD_DAT(0x2D);

	GLCD_COM(0xE1);
	GLCD_DAT(0xD0);
	GLCD_DAT(0x08);
	GLCD_DAT(0x10);
	GLCD_DAT(0x08);
	GLCD_DAT(0x06);
	GLCD_DAT(0x06);
	GLCD_DAT(0x39);
	GLCD_DAT(0x44);
	GLCD_DAT(0x51);
	GLCD_DAT(0x0B);
	GLCD_DAT(0x16);
	GLCD_DAT(0x14);
	GLCD_DAT(0x2F);
	GLCD_DAT(0x31);
	GLCD_COM(0x21);

	GLCD_COM(0x11);

	GLCD_COM(0x29);
#endif
    GLCDBL = 1;  //BL On
}
void GLCD_SetWindow(uint16_t Xstart, uint16_t Ystart, uint16_t Xend, uint16_t Yend){ 
	GLCD_COM(0x2a);
	GLCD_DAT(0x00);
	GLCD_DAT(Xstart & 0xff);
	GLCD_DAT((Xend - 1) >> 8);
	GLCD_DAT((Xend - 1) & 0xff);

	GLCD_COM(0x2b);
	GLCD_DAT(0x00);
	GLCD_DAT(Ystart & 0xff);
	GLCD_DAT((Yend - 1) >> 8);
	GLCD_DAT((Yend - 1) & 0xff);

	GLCD_COM(0x2C);
}
void GLCD_SetCursor(uint16_t X, uint16_t Y){ 
	GLCD_COM(0x2a);
    GLCD_DAT16(X);
	GLCD_COM(0x2b);
    GLCD_DAT16(Y);
	GLCD_COM(0x2C);
}
void GLCD_Clear(uint16_t Color){
	unsigned int i,j;  	
	GLCD_SetWindow(0, 0, LCD_WIDTH, LCD_HEIGHT);
	for(i = 0; i < LCD_WIDTH; i++){
		for(j = 0; j < LCD_HEIGHT; j++){
			GLCD_DAT((Color>>8)&0xff);
			GLCD_DAT(Color);
		}
	}
}
void GLCD_DrawPaint(uint16_t x, uint16_t y, uint16_t Color){
	GLCD_SetCursor(x, y);
	GLCD_DAT16(Color); 	    
}
void GLCD_LineHrz(uint16_t x, uint16_t y, uint16_t l, uint16_t color){
    int i;
    GLCD_SetCursor(x, y);
    for ( i=0; i< l; i++ ){
        GLCD_DAT16(color);
    }
}
void GLCD_LineVrt(uint16_t x, uint16_t y, uint16_t l, uint16_t color){
    int i;
    GLCD_SetCursor(x, y);
    for ( i=0; i< l; i++ ){
        GLCD_DAT16(color);
        GLCD_SetCursor(x, y+i);
    }
}
void GLCD_LineHL(uint16_t xH, uint16_t xL, uint16_t t, uint16_t color){
    /* Draw a vertical line from XL to XH */
    uint16_t i;
    uint16_t xT;
    if (xH < xL){
        xT = xH;
        xH = xL;
        xL = xT;
    }
    if ((xH | xL) == 0) xH = 1;
    for ( i= LCD_HEIGHT-xH; i <= LCD_HEIGHT-xL; i++){
        GLCD_SetCursor(t, i);
        GLCD_DAT16(color);
    }
}
void GLCD_DrawChar(uint16_t Xpoint, uint16_t Ypoint, const char Acsii_Char, uint16_t Color_Foreground){
    uint16_t Page, Column, i;
    i = (Acsii_Char - ' ') * 32 ;
    for ( Page = 0; Page < 16; Page ++ ) {
        for ( Column = 0; Column < 11; Column ++ ) {
            //To determine whether the font background color and screen background color is consistent
            if (Font16_Table[i] & (0x80 >> (Column % 8))){
                    GLCD_DrawPaint(Xpoint + Column, Ypoint + Page, Color_Foreground );
            }
            //One pixel is 8 bits
            if (Column % 8 == 7) {
                i++;
            }
        }/* Write a line */
        i++;
    }/* Write all */
}
void GLCD_DrawString(uint16_t Xpoint, uint16_t Ypoint, const char * pString, uint16_t Color_Foreground){
    while (* pString != '\0') {
        GLCD_DrawChar(Xpoint, Ypoint, * pString, Color_Foreground);
        pString ++;
        Xpoint += 11;
    }
}
void TMR5_int(){
    // Interrupt occurs every 20ms; 
    //  for detection of the rotary encoder and
    //  for detection of the pressed time of the SW
    if ( SWRotaryEncoder == 0 ) pressedTime++;
    rotData = ((rotData & 0x3) << 2) | (PORTF & 0x3);
    switch(rotData){
        case 0b0010 :
        case 0b1011 :
        case 0b1101 :
        case 0b0100 : 
            rotVal-= rotValMag;
            rotDir = -1;
            return;

        case 0b0001 :
        case 0b0111 :
        case 0b1110 :
        case 0b1000 : 
            rotVal+= rotValMag;
            rotDir = 1;
            return;

        default: 
            rotDir = 0;
            return;
    }
}
/*
                         Main application
 */
int main(void)
{
    int i;
    char pString[32]="Hello World !";// font16.h 29char/line
    
    // initialize the device
    DSCON = 0x0000; // must clear RELEASE bit after Deep Sleep
    DSCON = 0x0000; // must be write same command twice
    SYSTEM_Initialize();
    TMR5_SetInterruptHandler(TMR5_int);
    GLCD_Init();
    GLCD_Clear(0);

    /* set OP AMP 1 */
    ANSGbits.ANSELG6 = 1;   TRISGbits.TRISG6 = 1;    //RG9(8 pin) = Analog (Input)
    ANSBbits.ANSELB5 = 1;   TRISBbits.TRISB5 = 1;    //RB5(11 pin) = Analog (Output)
    AMP1CON = 0;
    AMP1CONbits.AMPEN = 1;  AMP1CONbits.AMPOE = 1;
    AMP1CONbits.SPDSEL = 1;     // High Power
    AMP1CONbits.NINSEL = 0b110; // 0b110 = voltage follower
    AMP1CONbits.PINSEL = 0b010; // 0b010 = OAxP1
    
    /* set OP AMP 2 */
    ANSBbits.ANSELB1 = 1;   TRISBbits.TRISB1 = 1;    //RB1(15 pin) = Analog (Input)
    ANSBbits.ANSELB3 = 1;   TRISBbits.TRISB3 = 1;    //RB5(13 pin) = Analog (Output)
    AMP2CON = 0;
    AMP2CONbits.AMPEN = 1;  AMP2CONbits.AMPOE = 1;
    AMP2CONbits.SPDSEL = 1;     // High Power
    AMP2CONbits.NINSEL = 0b110; // 0b110 = voltage follower
    AMP2CONbits.PINSEL = 0b010; // 0b010 = OAxP1

    GLCD_DrawString(0,0,"1.2V is output to C26/C27",0xffff);

    /* Select ATT 0/1 = 15V/1.25V */
    CH1ATT = 1; Nop();
    CH2ATT = 1;
    GLCD_DrawString(0,20,"U3 Pin 9 and 11 will be High",0xffff);
    
    while (1)
    {
        // Add your application code

    }

    return 1;
}
/**
 End of File
*/

