#ifndef __FLANGER_H__
#define __FLANGER_H__

extern void Flanger_init(short effect_rate,short sampling,short maxd,short mind,double fwv,double stepd,double fbv);
extern double Flanger_process(double xin);
extern void Flanger_sweep(void);

#endif
