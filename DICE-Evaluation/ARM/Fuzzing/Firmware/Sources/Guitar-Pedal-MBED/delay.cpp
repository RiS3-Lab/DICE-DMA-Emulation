#include "delay.h"
#include "math.h"

#define MAX_BUF_SIZE 1

/*****************************************************************************
*       Fractional delay line implementation in C:
*
*                    ---------[d_mix]--------------------------
*                    |                                         |
*                    |                                         |
*                    |x1                                       v
*     xin ------>[+]----->[z^-M]--[interp.]----[d_fw]-------->[+]-----> yout
*                 ^                         |
*                 |                         |
*                 |----------[d_fb]<--------|
*******************************************************************************/

double d_buffer[MAX_BUF_SIZE];

/*
This interface defines the delay object
*/
struct fract_delay {
    double d_mix;       /*delay blend parameter*/
    short d_samples;	/*delay duration in samples*/
    double d_fb;	    /*feedback volume*/
    double d_fw;	    /*delay tap mix volume*/
    double n_fract;     /*fractional part of the delay*/
    double *rdPtr;      /*delay read pointer*/
    double *wrtPtr;     /*delay write pointer*/
};

static struct fract_delay del;

/*
This function is used to initialize the delay object
*/
void Delay_Init(double delay_samples,double dfb,double dfw, double dmix) {
	Delay_set_delay(delay_samples);
	Delay_set_fb(dfb);
	Delay_set_fw(dfw);
	Delay_set_mix(dmix);
	del.wrtPtr = &d_buffer[MAX_BUF_SIZE-1];
}

/*
These functions are used as interface to the delay object,
so there's not direct access to the delay object from
external modules
*/
void Delay_set_fb(double val) {
    del.d_fb = val;
}

void Delay_set_fw(double val) {
    del.d_fw = val;
}

void Delay_set_mix(double val) {
    del.d_mix = val;
}

void Delay_set_delay(double n_delay) {
    /*Get the integer part of the delay*/
    del.d_samples = (short)floor(n_delay);

    /*gets the fractional part of the delay*/
    del.n_fract = (n_delay - del.d_samples);
}

double Delay_get_fb(void) {
    return del.d_fb;
}

double Delay_get_fw(void) {
    return del.d_fw;
}

double Delay_get_mix(void) {
    return del.d_mix;
}

/*
This is the main delay task,
*/
double Delay_task(double xin) {
	double yout;
	double * y0;
	double * y1;
	double x1;
	double x_est;

    /*Calculates current read pointer position*/
	del.rdPtr = del.wrtPtr - (short)del.d_samples;

	/*Wraps read pointer*/
	if (del.rdPtr < d_buffer) {
		del.rdPtr += MAX_BUF_SIZE-1;
	}

	/*Linear interpolation to estimate the delay + the fractional part*/
	y0 = del.rdPtr-1;
	y1 = del.rdPtr;

	if (y0 < d_buffer) {
	    y0 += MAX_BUF_SIZE-1;
	}

	x_est = (*(y0) - *(y1))*del.n_fract + *(y1);

    /*Calculate next value to store in buffer*/
    x1 = xin + x_est*del.d_fb;

	/*Store value in buffer*/
	*(del.wrtPtr) = x1;

    /*Output value calculation*/
	yout = x1*del.d_mix + x_est*del.d_fw;

    /*Increment delat write pointer*/
	del.wrtPtr++;

    /*Wraps delay write pointer*/
	if ((del.wrtPtr-&d_buffer[0]) > MAX_BUF_SIZE-1) {
		del.wrtPtr = &d_buffer[0];
	}
	return yout;
}

/*****USAGE EXAMPLE****************************************/


/*void main(void) {
    double xin;
    double yout;
    Delay_Init(85.6,0.7,0.7,1);

    while(1) {
        if (new_sample_flag()) {

            xin = read_sample();

            yout = Delay_task(xin);

            write_output(yout);
        }
    }
}
*/
