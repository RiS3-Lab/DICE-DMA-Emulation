#include "delay_effect.h"


float fx_buffer[ DELAY_LENGTH ];
int readIndex;
int writeIndex;

void delay_effect_init()
{
	int i;
	for(i = 0; i < DELAY_LENGTH; i++)
		fx_buffer[i] = 0;

	readIndex = 0;
	writeIndex = DELAY_LENGTH - 2;
}

float delayLine_readNext( float x )
{
    float ret = fx_buffer[readIndex];
    ++readIndex;
    if( readIndex >= DELAY_LENGTH )
        readIndex = 0;
    return ret;
}

void delayLine_writeNext( float x )
{
	fx_buffer[ writeIndex ] = x;
    ++writeIndex;
    if( writeIndex >= DELAY_LENGTH )
        writeIndex = 0;
}

float singleDelay( float x ) {
    delayLine_writeNext(x);
    return delayLine_readNext( x );
}

float singleEcho( float x, float g ) {
    delayLine_writeNext(x);
    return x + g * delayLine_readNext( x );
}

float echo( float x, float g ) {
   float ret = (1-g) * x + g * delayLine_readNext( x );
   delayLine_writeNext( ret );
   return ret;
}


float sdelay( float x, float g ) {
   float ret = (1-g) * x + g * delayLine_readNext( x );
   delayLine_writeNext( x );
   return ret;
}
