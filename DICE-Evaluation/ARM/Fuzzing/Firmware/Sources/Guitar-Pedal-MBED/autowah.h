#ifndef __AUTOWAH_H__
#define __AUTOWAH_H__

extern void AutoWah_init(short effect_rate,short sampling,short maxf,short minf,short Q,double gainfactor,short freq_step);
extern double AutoWah_process(int xin);
extern void AutoWah_sweep(void);

#endif
