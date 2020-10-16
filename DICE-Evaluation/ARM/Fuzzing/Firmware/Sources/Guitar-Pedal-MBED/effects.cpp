#include "effects.h"
#include <math.h>
#include "tremolo2.h"
#include "autowah.h"
#include "autowah2.h"
#include "delay.h"
#include "delay_effect.h"

int mod[MAX_MOD_COUNT];


void init_effects()
{
	//Flanger_init(short effect_rate,short sampling,short maxd,short mind,double fwv,double stepd,double fbv)
	 Flanger_init(8000,1500,200,0.5,0.3,4,0.1);
	 Tremolo_init(4000,1);

	 //Delay_Init(double delay_samples,double dfb,double dfw, double dmix)
	    //double d_fb;	    /*feedback volume*/
	    //double d_fw;	    /*delay tap mix volume*/
	 //    double d_mix;       /*delay blend parameter*/
	 //Delay_Init(800, 0.1, 2, 0.9);
	 Delay_Init(300,0.7,0.1,1);

	 delay_effect_init();
	 //AutoWah_init2(2000, 3000,60,1,1,1,10);
	 // AutoWah_init2(short effect_rate,short sampling,short maxf,short minf,short Q,double gainfactor,short freq_step) {
	 //AutoWah_init(2000, 16000, 1000, 500, 4, 0.707, 10 );

	 int i;
	 for(i=0;i<MAX_MOD_COUNT; i++)
		 mod[i] = 0;
	 set_modifications_default();

}

float clean(float x)
{
	x-= 0.5f;
	x *= mod[0]/25.0f;  //(float)(mod1/10.0f);
	float out_ = x +0.5;
	return out_;
}

float ez_distort(float x)
{
	x-= 0.5f;
	x *= mod[0]/10.0f;

    int adjust = 0.01f + (mod[0]/20.0f);

    float out_ = x;
    if(out_ > adjust)
    {
        out_ = adjust;
    }
    else if(out_ < -adjust){
        out_ = -adjust;
    }

    out_ += 0.5;
    return out_;
}

float ez_distort2(float x)
{
	x-= 0.5f;
	x *= mod[0]/2.0f;

    int adjust = 0.01f + (mod[0]/20.0f);

    float out_ = x;
    if(out_ > adjust)
    {
        out_ = adjust;
    }
    else if(out_ < -adjust){
        out_ = -adjust;
    }

    out_ += 0.5;
    return out_;
}

float advanced_distort(float x)
{
    return x;
}

float phasor = 0;
float crusher(float x, float normfreq, int bits )
{
    //wpm http://www.musicdsp.org/archive.php?classid=4
    float step = 1.0f/(1<<bits);
    float last = 0;

    phasor = phasor + normfreq;
    if (phasor >= 1.0f)
    {
      phasor = phasor - 1.0f;
      last = step * floor( x/step + 0.5f );
    }
    return last;
}

float overdrive(float x)
{
	x-= 0.5f; x *= 3;
     //return (3 - ((2-3*x)*(2-3*x)))/3.0f;
    //x = x*2.0f;

	float y = x;
     if(abs(x) > 0.2f)
        y = 2*x;
     else
     {
    	y =  x*x;
    	if(x<0)
    		y = -y;
     }

     if(y>0.4)
    	 y=0.4;
     if(y < -0.4)
    	 y = -0.4;


    return y + 0.5;
}
/*
float abs(float x)
{
	if(x < 0)
		return -x;
	return x;
}
*/
unsigned int index = 0;
float tremolo(float x)
{
	x-=0.5f;x*=4;

    float pi = 3.1415;
    float Fs = 2500.0f  + (mod[0] - 25) * 100;
    float Fc = 1.0f;
    float alpha = 0.5f + 0.5f*(mod[0] / 50.0f);
    float trem=(0.5+ alpha*sin(2*pi*index*(Fc/Fs)));
    float y = trem * x;
    index++;
    return y + 0.5;
}


/*unsigned int index = 0;
float tremolo(float x)
{
	x-=0.5f;x*=10; x+= 0.5f;

    float pi = 3.1415;
    float Fs = 2500.0f  + (mod1 - 25) * 100;
    float Fc = 1.0f;
    float alpha = 0.5f + 0.5f*(mod2 / 50.0f);
    float trem=(0.5+ alpha*sin(2*pi*index*(Fc/Fs)));
    float y = trem * x;
    index++;
    return y;
}*/

int is_init = 0;
float delay_line(float input)
{
	input -= 0.5;
	input *= 3;
    return Delay_task(input) + 0.5f;
}


float flanger(float val)
{
	val -= 0.5f;
	val *= 4;
	float yout = (float)Flanger_process(0.7*val);

	Flanger_sweep();

	yout += 0.5f;
	return(yout);
}

float tremolo2(float x)
{
	x -= 0.5f;


	if(x< 0.0001 && x > - 0.0001)
		x = 0;


	float yout = x;
	x *= 4;


	float trem = Tremolo_process(0.7f*x);


		x *= 6.0f;
		yout = trem;



	Tremolo_sweep();

	return yout + 0.5f;
}

float wahwah(float x)
{
        float yout = (float)AutoWah_process(x);

        AutoWah_sweep();

        return yout;
}

float fx_avg = 0;
float delay(float in)
{
	in -= 0.5f;

	float yout = sdelay(in, 0.9f);

	return yout + 0.5f;
}

void set_modifications_default()
{
	   mod[0] = 25;
	   mod[1] = 10;
	   mod[2] = 10;
	   mod[3] = 10;
	   mod[4] = 10;
}
