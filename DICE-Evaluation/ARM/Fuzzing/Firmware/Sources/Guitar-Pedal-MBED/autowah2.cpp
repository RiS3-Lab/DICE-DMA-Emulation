/*
 * Code obtained from ...
 *
 *
 */

#include "notch.h"
#include "autowah.h"
#include <math.h>

static short center_freq;
static short samp_freq;
static short counter;
static short counter_limit;
static short control;
static short max_freq;
static short min_freq;
static short f_step;
static br_filter H;
static double a[3];
static double b[3];
double x[3],y[3];

void AutoWah_init2(short effect_rate,short sampling,short maxf,short minf,short Q,double gainfactor,short freq_step) {
	double C;

	//Process variables
	center_freq = 0;
	samp_freq = sampling;
	counter = effect_rate;
	control = 0;

	//User Parametters
	counter_limit = effect_rate;

	//Convert frequencies to index ranges
	min_freq = 0;
	max_freq = (maxf - minf)/freq_step;

    br_iir_init(sampling,gainfactor,Q,freq_step,minf);
	f_step = freq_step;

	//Lowpass filter parameters
	/*
	   C = 1/TAN(PI*Fc/Fs)
	*/
	C = 1018.59;

	b[0] = 1/(1+2*0.7071*C+pow(C,2));
	b[1] = 2*b[0];
	b[2] = b[0];

	a[1] = 2*b[0]*(1-pow(C,2));
	a[2] = b[0]*(1-2*0.7071*C+pow(C,2));
}

double AutoWah_process2(double xin) {
	double yout;

	yout = br_iir_filter(xin,&H);
	#ifdef INPUT_UNSIGNED
		yout += 32767;
	#endif

	return yout;
}

void AutoWah_sweep2(double xin) {
	unsigned int filter_index;
	double yout;
        double detect;

	/*The input is 16 bit unsigned so it
	has to be centered to 0*/
	detect = (xin - 32767.0);
	x[0] = x[1];
	x[1] = x[2];
	/*Here the input to the filter
	is half rectified*/
	x[2] = (detect > 0)?detect:0;

	y[0] = y[1];
	y[1] = y[2];

	y[2] =    b[0]*x[2];
	y[2] +=   b[1]*x[1];
	y[2] +=   b[2]*x[0];
	y[2] -=   a[1]*y[1];
	y[2] -=   a[2]*y[0];

	if (!--counter) {
		/*The output of the LPF (y[2]) is scaled by 0.1
		in order to be used as a LFO to control the band pass filter
		*/
	    filter_index = (double)min_freq + y[2]*0.1;

            /*The scaling value is determined as a value
            that would keep the filter index within the
            range of max_freq
	    */
            if(filter_index > max_freq) {
                filter_index = max_freq;
            }
            if(filter_index < 0) {
                filter_index = 0;
            }

	    br_iir_setup(&H,filter_index);

	    counter = counter_limit;
	}
}
