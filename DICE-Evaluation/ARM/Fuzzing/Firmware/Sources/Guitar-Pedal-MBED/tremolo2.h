#ifndef __TREMOLO_H__
#define __TREMOLO_H__

extern void Tremolo_init(short effect_rate,double depth);
extern double Tremolo_process(double xin);
extern void Tremolo_sweep(void);

#endif
