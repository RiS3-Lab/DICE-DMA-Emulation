#include "tremolo2.h"

static double dep;
static short counter_limit;
static short control;
static short mod;
static double offset;

void Tremolo_init(short effect_rate,double depth) {
    dep     = depth;
    control = 1;
    mod     = 0;
    counter_limit = effect_rate;
    offset  = 1 - dep;
}

double Tremolo_process(double xin) {
    double yout;
    double m;

    m = (double)mod*dep/counter_limit;
    yout = (m + offset)*xin;
    return yout;
}

void Tremolo_sweep(void) {

	    mod += control;

	    if (mod > counter_limit) {
	        control = -1;
            }
            else if(!mod) {
	        control = 1;
            }
}
