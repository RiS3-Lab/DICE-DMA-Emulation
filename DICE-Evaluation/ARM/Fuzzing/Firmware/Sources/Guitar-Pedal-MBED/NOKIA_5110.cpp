// Project: Nokia5110 - Controlling a NK5110 display from an NXP LPC1768
// File: NOKIA_5110.cpp
// Author: Chris Yan
// Created: January, 2012
// Revised: January, 2014
//  Desc: Supporting code for the NokiaLcd class

#include "NOKIA_5110.h"
#include "mbed.h"

NokiaLcd::NokiaLcd(LcdPins pinout)
{
    // SPI
    LcdSpi = new SPI(pinout.mosi, pinout.miso, pinout.sclk);
    LcdSpi->format(LCD_SPI_BITS, LCD_SPI_MODE);
    LcdSpi->frequency(LCD_FREQ);

    // Control Pins
    Pins = new DigitalOut*[3];
    Pins[PIN_RST]   = new DigitalOut(pinout.rst);
    Pins[PIN_SCE]   = new DigitalOut(pinout.sce);
    Pins[PIN_DC]    = new DigitalOut(pinout.dc);

    // Initial Command Instructions, note the initial command mode
    FunctionSet.V   = CMD_FS_HORIZONTAL_MODE;
    FunctionSet.H   = CMD_FS_EXTENDED_MODE;
    FunctionSet.PD  = CMD_FS_ACTIVE_MODE;
    FunctionChar    = CreateFunctionChar();

    TempControlChar = CMD_TC_TEMP_2;
    DispControlChar = CMD_DC_NORMAL_MODE;
    BiasChar        = CMD_BI_MUX_48;
    VopChar         = CMD_VOP_7V38;
}

void NokiaLcd::ShutdownLcd()
{
    FunctionSet.PD  = CMD_FS_POWER_DOWN_MODE;

    ClearLcdMem();
    SendFunction( CMD_DC_CLEAR_DISPLAY );
    SendFunction( CreateFunctionChar() );
}

void NokiaLcd::ClearLcdMem()
{
    for(int tick = 0; tick <= 503; tick++)
        LcdSpi->write(0x00);
}

void NokiaLcd::TestLcd(char test_pattern)
{
    for(int tick = 0; tick <= 503; tick++)
        LcdSpi->write(test_pattern);         // Command gets sent
}

void NokiaLcd::InitLcd()
{
    ResetLcd();
    Pins[PIN_SCE]->write(0);     // Chip Select goes low

    // Redefine the FunctionChar in case it has changed
    FunctionSet.V   = CMD_FS_HORIZONTAL_MODE;
    FunctionSet.H   = CMD_FS_EXTENDED_MODE;
    FunctionSet.PD  = CMD_FS_ACTIVE_MODE;
    SendFunction( CreateFunctionChar() );   // Extended CMD set
    SendFunction( VopChar );                // | Vop
    SendFunction( TempControlChar );        // | Temp
    SendFunction( BiasChar );               // | Bias

    FunctionSet.H   = CMD_FS_BASIC_MODE;
    SendFunction( CreateFunctionChar() );   // Basic CMD set
    SendFunction( DispControlChar );        // | Display Mode

    ClearLcdMem();
    Pins[PIN_DC]->write(1);     // Data/CMD goes back to Data mode
}

void NokiaLcd::ResetLcd()
{
    Pins[PIN_RST]->write(0);    // Reset goes low
    Pins[PIN_RST]->write(1);    // Reset goes high
}

char NokiaLcd::CreateFunctionChar()
{
    return ( 0x20 | FunctionSet.PD | FunctionSet.V | FunctionSet.H );
}

void NokiaLcd::SendDrawData(char data)
{
    LcdSpi->write(data);         // Command gets sent
}

void NokiaLcd::DrawChar(char character)
{
    for( int i = 0; i < 6; i++)
        SendDrawData( FONT_6x6[ ((character - 32)*6) + i] );
}

void NokiaLcd::DrawString(char* s)
{
    char len = strlen(s);
    for( int idx = 0; idx < len; idx++ )
    {
        for( int i = 0; i < 6; i++)
            SendDrawData( FONT_6x6[ ((s[idx] - 32)*6) + i] );
    }
}

void NokiaLcd::DrawFrameChar(char character)
{
    for( int i = 0; i < 6; i++)
        SendDrawData((( FONT_6x6[ ((character - 32)*6) + i]  ) << 1 ) | 0x81);
}

void NokiaLcd::DrawNegFrameChar(char character)
{
    for( int i = 0; i < 6; i++)
        SendDrawData(~(( FONT_6x6[ ((character - 32)*6) + i]  ) << 1 ) | 0x81);
}

char* NokiaLcd::NumToStr(int num)
{
    if(num <= 0)
        return "0";

    double length = 0;
    int tlen = 0;
    int temp = 1;
    char c;

    // Get number of digits
    while( temp <= num )
    {
        temp *= 10;
        length++;
    }
    tlen = length;
    char* numString = new char[tlen+1];

    // Convert each place in number to a stand-alone representative number
    temp = 0;
    for(int idx = pow(10, length); idx>1; idx = (idx/10))
    {
        c = (char)( ((num % idx)-(num % (idx/10)))/(idx/10) + 48);
        numString[temp] = c;
        temp++;
    }
    numString[temp] = '\0';
    return numString;
}

void NokiaLcd::SetXY(char x, char y)
{
    if( (x > 83) || (y > 5) )
        return;

    SendFunction( x | 0x80 );
    SendFunction( y | 0x40 );
}

void NokiaLcd::SendFunction(char cmd) //TODO:Detection of what H should be
{
    Pins[PIN_DC]->write(0);     // Data/CMD goes low
    LcdSpi->write(cmd);         // Command gets sent
    Pins[PIN_DC]->write(1);     // Data/CMD goes back to Data mode
}

NokiaLcd::~NokiaLcd()
{
    ShutdownLcd();
}
