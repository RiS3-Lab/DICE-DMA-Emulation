/*******************FLANGER.C******************************/

#include "delay.h"
#include "flanger.h"

static short samp_freq;
static double var_delay;
static short counter;
static short counter_limit;
static short control;
static short max_delay;
static short min_delay;
static double mix_vol;
static double delay_step;

/*
This is the initialization function, basically
it passes the initialization parameters to the delay block
and initializes the flanger control variables.
*/
void Flanger_init(short effect_rate,short sampling,short maxd,short mind,double fwv,double stepd,double fbv) {
	Delay_Init(2,fbv,fwv,1);

	samp_freq = sampling;
	counter = effect_rate;
	control = 1;
	var_delay = mind;

	//User Parameters
	counter_limit = effect_rate;
	max_delay =  maxd;
	min_delay = mind;
	mix_vol = 1;
	delay_step = stepd;
}

/*This is the flanging process task
that uses the delay task inside*/
double Flanger_process(double xin) {
	double yout;

	yout = Delay_task(xin);
	return yout;
}

/*
This sweep function creates a slow frequency
ramp that will go up and down changing the
delay value at the same time. The counter
variable is a counter of amount of samples that
the function waits before it can change the delay.
*/
void Flanger_sweep(void) {
	if (!--counter) {
	    var_delay+=control*delay_step;

	    if (var_delay > max_delay) {
	        control = -1;
            }

            if (var_delay < min_delay) {
	        control = 1;
            }

	    Delay_set_delay(var_delay);
	    counter = counter_limit;
	}
}


/*****USAGE EXAMPLE****************************************/
/*#include "Flanger.h"

void main(void) {
    double xin;
    double yout;
    Flanger_init(500,16000,70,2,0.3,1,0.3);

    while(1) {
        if (new_sample_flag()) {

            xin = read_sample();

            yout = Flanger_process(0.7*xin);


            write_output(yout);

            Flanger_sweep();
        }
    }
}
*/
