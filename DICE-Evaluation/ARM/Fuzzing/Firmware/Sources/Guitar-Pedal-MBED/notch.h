#ifndef __BR_IIR_H__
#define __BR_IIR_H__

/*
Notch filter coefficients object
*/
struct br_coeffs {
	double e;
	double p;
	double d[3];
};

/*
Notch filter object
*/
struct br_filter {
	double e;
	double p;
	double d[3];
	double x[3];
	double y[3];
};

void br_iir_init(double fsfilt,double gb,double Q,double fstep, short fmin);
void br_iir_setup(struct br_filter * H,int index);
double br_iir_filter(double yin,struct br_filter * H);

#endif
