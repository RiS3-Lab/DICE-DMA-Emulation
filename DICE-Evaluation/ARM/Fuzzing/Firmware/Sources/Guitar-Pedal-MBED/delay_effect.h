#ifndef __DELAY_EFFECT_H__
#define __DELAY_EFFECT_H__

#define DELAY_LENGTH 1800

float delayLine_readNext( float x );
void delayLine_writeNext( float x );
float singleDelay( float x );
float singleEcho( float x, float g );
float echo( float x, float g );
float sdelay( float x, float g );
void delay_effect_init();

#endif
