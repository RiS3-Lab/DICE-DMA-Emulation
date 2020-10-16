#ifndef __PHASER_H__
#define __PHASER_H__

extern void Phaser_init(short effect_rate,short sampling,short maxf,short minf,short Q,double gainfactor,double mix_volume,short freq_step, double dmix);
extern double Phaser_process(double xin);
extern void Phaser_sweep(void);

#endif
