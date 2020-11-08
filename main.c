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
int DisplayOrientation = 0;
/* colors */
#define ColorBlack      0x0000
#define ColorBlue       0x001f
#define ColorGreen      0x07e0
#define ColorRed        0xf800
#define ColorCyan       0x07ff
#define ColorMagenta    0xf81f
#define ColorYellow     0xffe0
#define ColorWhite      0xffff
#define ColorScale      0b1000010000010000  //0bRRRRRGGGGGGBBBBB
/* Attenuator ON-OFF */
#define CH1ATT  PORTGbits.RG7   // 0=ATT-ON(15V), 1=ATT-OFF(1.25V)
#define CH2ATT  PORTGbits.RG8   // 0=ATT-ON(15V), 1=ATT-OFF(1.25V)

/* Rotary Encoder */
int rotData, rotDir, swPos; float rotVal; float rotValMag = 0.5;
/* Wave Data */
static int originalWavedata[2][LCD_WIDTH*3];
uint8_t byteWavedata[2][LCD_WIDTH];
uint8_t prev_byteWavedata[2][LCD_WIDTH];
/* Value for the switch pushing time */
int SWREValue = 0, SW1Value = 0, SW2Value = 0, SW3Value = 0, SW4Value = 0;
/* Time axis setting table */
    /*                      PR2
     *  16us/div	3       0x3
     *  20us/div	4   	0x4
     *  40us/div    9       0x9
     *  100us/div	24      0x18
     *  200us/div	49  	0x31
     *  500us/div	124     0x7C
     *  1ms/div     249 	0x0F9
     *  2ms/div     499     0x1F3
     *  5ms/div     1249	0x4E1
     *  10ms/div	2499	0x9C3
     */
uint8_t TimeAxisTableIndex = 2;
uint16_t TimeAxisTable[]={3, 4, 9, 0x18, 0x31, 0x7c, 0xf9, 0x1f3, 0x4e1, 0x9c3};
char TimeAxisTable_s[10][11]={
    " 16us/div\0",
    " 20us/div\0",
    " 40us/div\0",
    "100us/div\0",
    "200us/div\0",
    "500us/div\0",
    "  1ms/div\0",
    "  2ms/div\0",
    "  5ms/div\0",
    " 10ms/div\0"};
/* Voltage axis setting table */
    /*                
     *  0.25V/div	1.0009
     *  0.5V/div	2.0018
     *  1V/div      4.0036
     *  2V/div      8.0072
     *  5V/div      20.018
     *  10V/div     40.036
     */
int VoltageAxisTableIndex[2] = {2, 2};
float VoltageAxisTable[2][6]={
    {1.0009, 2.0018, 4.0036, 8.0072, 20.018, 40.036},
    {1.0009, 2.0018, 4.0036, 8.0072, 20.018, 40.036}
};
char VoltageAxisTable_s[2][6][11]={
    {"0.25V/div\0",
    " 0.5V/div\0",
    "   1V/div\0",
    "   2V/div\0",
    "   5V/div\0",
    "  10V/div\0"},
    {"0.25V/div\0",
    " 0.5V/div\0",
    "   1V/div\0",
    "   2V/div\0",
    "   5V/div\0",
    "  10V/div\0"}
};
int VoltageOffset[2] = {120, 24};   // Display offset CH1=120, CH2=24

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
    /* for 2.4inch ILI9341 */
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
    /* for 2inch ST7789 */
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
void GLCD_DisplayOrientation(int n){
    if (n==0){
        GLCD_COM(0x36); // Memory Access Control
        GLCD_DAT(0b10101000); // MV=1,MX=0,MY=1   MY MX MV ML BGR MH 0 0
    //	GLCD_DAT(0b01101000); // MV=1,MX=1,MY=0   MY MX MV ML BGR MH 0 0
    } else {
        GLCD_COM(0x36); // Memory Access Control
    //  GLCD_DAT(0b10101000); // MV=1,MX=0,MY=1   MY MX MV ML BGR MH 0 0
    	GLCD_DAT(0b01101000); // MV=1,MX=1,MY=0   MY MX MV ML BGR MH 0 0
    }
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
void GLCD_ClearCharacterArea(){
    int i;
    for ( i=0; i<15; i++){
        GLCD_LineHrz(0,i,320,ColorBlack);
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
    // Interrupt occurs every 5ms; 
    //  for detection of the rotary encoder and
    //  for detection of the pressed time of the SW
    if ( SWRotaryEncoder == 0 ) { SWREValue++; } else { SWREValue = 0; }
    if ( SW1 == 0 ) { SW1Value++; } else { SW1Value = 0; }
    if ( SW2 == 0 ) { SW2Value++; } else { SW2Value = 0; }
    if ( SW3 == 0 ) { SW3Value++; } else { SW3Value = 0; }
    if ( SW4 == 0 ) { SW4Value++; } else { SW4Value = 0; }
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
void DispWave(int CH, float WaveDiv, uint16_t WaveOffset, int DetectPosition, uint16_t WaveColor){
    int i, tempData;
    uint16_t gx, prev_gx, prev_prev_gx;

    // prepare data for display
    for (i=0;i<LCD_WIDTH;i++){
        prev_byteWavedata[CH][i] = byteWavedata[CH][i];
        tempData = originalWavedata[CH][i+DetectPosition] / WaveDiv + WaveOffset;
        if (tempData > 239) tempData = 239;
        if (tempData < 0) tempData = 0;
        byteWavedata[CH][i] = (uint8_t)(tempData);  //0..4095 -> 0..240
    }
    // display
    prev_gx = byteWavedata[CH][0];
    prev_prev_gx =  prev_byteWavedata[CH][0];
    for (i=0;i<LCD_WIDTH;i++){
        gx = prev_byteWavedata[CH][i];
        GLCD_LineHL(prev_prev_gx, gx, i, ColorBlack);   // clear previous waveform
        prev_prev_gx = gx;
        gx = byteWavedata[CH][i];
        GLCD_LineHL(prev_gx, gx, i, WaveColor);    // display waveform
        prev_gx = gx;
    }
}
void DispScale(){
    int x,y;
    GLCD_LineHrz(0,16,LCD_WIDTH,ColorScale);
    GLCD_LineHrz(0,8*16,LCD_WIDTH,ColorScale);
    GLCD_LineHrz(0,LCD_HEIGHT-1,LCD_WIDTH,ColorScale);
    for ( y = 0; y < 8; y++){
        for ( x = 1; x < 10; x++){
            GLCD_LineHrz( x*32, y*32+32, 2, ColorScale);
        }
    }
    GLCD_LineVrt(0,16,LCD_HEIGHT-1-16,ColorScale);
    GLCD_LineVrt(160,16,LCD_HEIGHT-1-16,ColorScale);
    GLCD_LineVrt(319,16,LCD_HEIGHT-1-16,ColorScale);
    for ( y = 0; y < 16; y++) GLCD_LineHrz( 158, y*16+16, 5, ColorScale);
    for ( y = 0; y < 16; y++) GLCD_LineHrz( 0, y*16+16, 3, ColorScale);
    for ( y = 0; y < 16; y++) GLCD_LineHrz( 316, y*16+16, 3, ColorScale);
    for ( x = 1; x < 20; x++) GLCD_LineVrt( x*16, 126, 5, ColorScale);
    for ( x = 1; x < 20; x++) GLCD_LineVrt( x*16, 17, 3, ColorScale);
    for ( x = 1; x < 20; x++) GLCD_LineVrt( x*16, 236, 3, ColorScale);
}
int DetectEdge(int CH1orCH2, int DetectPosition, int ThresholdLevel, int Hysteresis, int Rising_or_Falling_edge, int DetectionInterval){
    /*
     *  edge detect = seek rising/falling edge
     *
     *      CH1orCH2:        CH1=1, CH2=2
     *      DetectPosition:  Left = 0, Center = 160, Right = 320
     *      ThresholdLevel:  +-0..+-2047
     *      Hysteresis:      noise suppress 1<
     *      Rising_or_Falling_edge:  Rising = 1, Falling = -1
     *      DetectionInterval:   interval of time axis 1..2
     *      return:  edge position 
     */
    int i;
    if (CH1orCH2 > 2) return -1;
    CH1orCH2 -= 1;
    for ( i = DetectPosition; i < (LCD_WIDTH*2 -DetectionInterval +DetectPosition ); i++ ){
        if ( (Rising_or_Falling_edge * ( originalWavedata[CH1orCH2][i + DetectionInterval] - ThresholdLevel - Hysteresis ) > 0 ) 
                && (Rising_or_Falling_edge * ( ThresholdLevel - Hysteresis - originalWavedata[CH1orCH2][i] ) > 0 ) ) break;
    }
    if ( i > LCD_WIDTH*2 ) {
        i = DetectPosition;
    } else {
        i -= DetectPosition;
    }
    return i;
}
/*
                         Main application
 */
int main(void)
{
    int t;
    char pString[32]="Hello World !";// font16.h 29char/line
    float prev_rotVal;
    int SelectedCH = 1; int prevSelectedCH = 1; 
    int OperationMode = 1; int prevOperationMode = 1;
    
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

    /*** DMA Setting ****/
    DMACON = 0;
	DMACONbits.DMAEN = 1;               // DMA Enable
	DMACONbits.PRSSEL = 1;              // 0: Fixed Priority, 1: Round-robin
	DMAH = 8192;                        // Upper Limit 8k byte
	DMAL = 0x0800;                      // Lower Limit Excluding SFR Space
    
    // DMA CH0 Setting
	DMACH0 = 0;                         // Stop Channel
	DMACH0bits.RELOAD = 1;              // Reload DMASRC, DMADST, DMACNT
	DMACH0bits.TRMODE = 0b00;           // Oneshot    
	DMACH0bits.SAMODE = 0b00;              // Source Addrs 0b11:PIA mode --0b00:No Increment--
	DMACH0bits.DAMODE = 1;              // Dist Addrs Increment
	DMACH0bits.SIZE = 0;                // Word Mode(16bit)
	DMASRC0 = (unsigned int)&ADRES0;    // From ADC Buf0 select
	DMADST0 = (unsigned int)originalWavedata[0];  // To Buffer select
	DMACNT0 = LCD_WIDTH*3;                      // DMA Counter
	DMAINT0 = 0;                        // All Clear
	DMAINT0bits.CHSEL = 0x2F;           // Select Pipeline ADC
	DMACH0bits.CHEN = 0;                // Channel DisableEnable
	IFS0bits.DMA0IF = 0;                // Flag Reset

    // DMA CH1 Setting
	DMACH1 = 0;                         // Stop Channel
	DMACH1bits.RELOAD = 1;              // Reload DMASRC, DMADST, DMACNT
	DMACH1bits.TRMODE = 0b00;           // Oneshot    
	DMACH1bits.SAMODE = 0b00;              // Source Addrs 0b11:PIA mode --0b00:No Increment--
	DMACH1bits.DAMODE = 1;              // Dist Addrs Increment
	DMACH1bits.SIZE = 0;                // Word Mode(16bit)
	DMASRC1 = (unsigned int)&ADRES1;    // From ADC Buf0 select
	DMADST1 = (unsigned int)originalWavedata[1];  // To Buffer select
	DMACNT1 = LCD_WIDTH*3;                      // DMA Counter
	DMAINT1 = 0;                        // All Clear
	DMAINT1bits.CHSEL = 0x2F;           // Select Pipeline ADC
	DMACH1bits.CHEN = 0;                // Channel DisableEnable
	IFS0bits.DMA1IF = 0;                // Flag Reset

    T2CONbits.TON = 0;                  // stop Timer2

    /* Select ATT 0/1 = 15V/1.5V */
    CH1ATT = 1; Nop();
    CH2ATT = 1;

    /* UART */
    printf("SYSTEM Initialized\n");
    
    /* Change Sampling frequency by Rotary Encoder */
    rotVal = (float) TimeAxisTableIndex;
    rotValMag = 0.2;
    prev_rotVal = rotVal;

    while (1)
    {
        // Add your application code
        /* Capture two waves */
        IEC1bits.T5IE = false;  // disable TMR5 interrupt; stop detection of the rotary encoder
        ADCON1bits.ADON = 1;    // ADC Enable
        TMR2 = 0;               // reset Timer2
        ADSTATLbits.SL0IF = 0;  // ADC Flag Clear
        T2CONbits.TON = 1;		// start Timer2 = start ADC
        IFS0bits.DMA0IF = 0;    // DMA0 Interrupt Flag Reset
        IFS0bits.DMA1IF = 0;    // DMA1 Interrupt Flag Reset
        DMACH0bits.CHEN = 1;    // DMA0 Channel Enable & Start
        DMACH1bits.CHEN = 1;    // DMA1 Channel Enable & Start
        while( ( !IFS0bits.DMA0IF ) | ( !IFS0bits.DMA1IF ) );	// Wait Max_Size sampling
        IFS0bits.DMA0IF = 0;	// Clear DMA0 Interrupt Flag
        IFS0bits.DMA1IF = 0;	// Clear DMA1 Interrupt Flag
        DMACH0bits.CHEN = 0;    // DMA0 Channel Disable & Stop
        DMACH1bits.CHEN = 0;    // DMA1 Channel Disable & Stop
        IEC1bits.T5IE = true;   // enable TMR5 interrupt; start detection of the rotary encoder

        t = DetectEdge(SelectedCH, 160, 50, 1, 1, 1);
        DispScale();    // Display Scale
        DispWave(0, VoltageAxisTable[0][VoltageAxisTableIndex[0]], VoltageOffset[0], t, ColorCyan);  // display CH1 wave
        DispWave(1, VoltageAxisTable[1][VoltageAxisTableIndex[1]], VoltageOffset[1], t, ColorYellow);   // display CH2 wave

        if ( SW1Value > 0){     // when SW1 clicked
            while ( SW1Value != 0 ){}   // Wait SW1 released
            SelectedCH = 1; 
            LEDRotaryEncoderBlue =1;Nop();
            LEDRotaryEncoderOrange=0;
            OperationMode += 1;
            if ( OperationMode > 3 ) OperationMode = 1;
        } 
        if ( SW2Value > 0){     // when SW2 clicked
            while ( SW2Value != 0 ){}   // Wait SW2 released
            SelectedCH = 2;
            LEDRotaryEncoderOrange=1;Nop();
            LEDRotaryEncoderBlue=0;
            OperationMode += 1;
            if ( OperationMode > 3 ) OperationMode = 1;
        }
        if ( SW3Value > 0){     // when SW3 clicked
            while ( SW3Value != 0 ){}   // Wait SW3 released
            SelectedCH = 3; 
            LEDRotaryEncoderOrange=1;Nop();
            LEDRotaryEncoderBlue=1;
        }
        if ( SW4Value > 0){     // when SW4 clicked
            while ( SW4Value != 0 ){
                if ( SW4Value > 600 ){  // Press for 3 seconds
                    DisplayOrientation = ~DisplayOrientation;
                    GLCD_DisplayOrientation(DisplayOrientation);
                    GLCD_Clear(0);
                    while ( SW4Value != 0 ){}
                }
            }   // Wait SW4 released
            SelectedCH = 4; 
        }
        prevSelectedCH = SelectedCH;
        
        switch( OperationMode ){
            case 1:{    // Select Time Axis
                if ( OperationMode != prevOperationMode ){
                    /* Change Sampling frequency by Rotary Encoder */
                    rotVal = (float) TimeAxisTableIndex;
                    rotValMag = 0.2;
                    prev_rotVal = rotVal;
                    GLCD_ClearCharacterArea();
                }   
                if(prev_rotVal != rotVal){
                    GLCD_ClearCharacterArea();
                    if (rotVal < 0) rotVal = 0;
                    if (rotVal > 9) rotVal = 9;
                    TimeAxisTableIndex = (int)rotVal;
                    PR2 = TimeAxisTable[TimeAxisTableIndex];
                }
                sprintf(pString,"%s",TimeAxisTable_s[TimeAxisTableIndex]);
                GLCD_DrawString(0, 0, pString, ColorWhite);
                prev_rotVal = rotVal;
                break;
            }
            case 2:{    // Voltage Axis
                if ( OperationMode != prevOperationMode ){
                    /* Change Voltage Axis by Rotary Encoder */
                    rotVal = (float) VoltageAxisTableIndex[SelectedCH-1];
                    rotValMag = 0.2;
                    prev_rotVal = rotVal;
                    GLCD_ClearCharacterArea();
                }
                if(prev_rotVal != rotVal){
                    GLCD_ClearCharacterArea();
                    if (rotVal < 0) rotVal = 0;
                    if (rotVal > 5) rotVal = 5;
                    VoltageAxisTableIndex[SelectedCH-1] = (int)rotVal;
                }
                sprintf(pString,"CH%d %s",SelectedCH,VoltageAxisTable_s[SelectedCH-1][(int)rotVal]);
                GLCD_DrawString(0, 0, pString, ColorWhite);
                prev_rotVal = rotVal;
                break;
            }
            case 3:{    // // Voltage Offset
                if ( OperationMode != prevOperationMode ){
                    /* Change Voltage Offset by Rotary Encoder */
                    rotVal = (float) VoltageOffset[SelectedCH-1];
                    rotValMag = 0.2;
                    prev_rotVal = rotVal;
                    GLCD_ClearCharacterArea();
                    sprintf(pString,"CH%d Offset",SelectedCH);
                    GLCD_DrawString(0, 0, pString, ColorWhite);
                }
                VoltageOffset[SelectedCH-1] = (int)rotVal;
            }
        }
        prevOperationMode = OperationMode;
    }

    return 1;
}
/**
 End of File
*/

