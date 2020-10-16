#include "notch.h"
#include "autowah.h"

static short center_freq;
static short samp_freq;
static short counter;
static short counter_limit;
static short control;
static short max_freq;
static short min_freq;
static short f_step;
static br_filter H;

/*
This is the auto wah effect initialization function.
This initializes the band pass filter and the effect control variables
*/
void AutoWah_init(short effect_rate,short sampling,short maxf,short minf,short Q,double gainfactor,short freq_step) {
	double C;

	/*Process variables*/
	center_freq = 0;
	samp_freq = sampling;
	counter = effect_rate;
	control = 0;

	/*User Parametters*/
	counter_limit = effect_rate;

	/*Convert frequencies to index ranges*/
	min_freq = 0;
	max_freq = (maxf - minf)/freq_step;

    br_iir_init(sampling,gainfactor,Q,freq_step,minf);

    f_step = freq_step;
}

/*
This function generates the current output value
Note that if the input and output signal are integer
unsigned types, we need to add a half scale offset
*/
double AutoWah_process(int xin) {
	double yout;

	yout = br_iir_filter(xin,&H);
	#ifdef INPUT_UNSIGNED
		yout += 32767;
	#endif

	return yout;
}

/*
This function will emulate a LFO that will vary according
to the effect_rate parameter set in the AutoWah_init function.
*/
void AutoWah_sweep(void) {
	double yout;

	if (!--counter) {
		if (!control) {
			br_iir_setup(&H,(center_freq+=f_step));
			if (center_freq > max_freq) {
				control = 1;
			}
		}
		else if (control) {
			br_iir_setup(&H,(center_freq-=f_step));
			if (center_freq == min_freq) {
				control = 0;
			}
		}

		counter = counter_limit;
	}
}
