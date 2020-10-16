#ifndef AUTO_WAH_ENV
#define AUTO_WAH_ENV

void AutoWah_sweep2(double xin);
double AutoWah_process2(double xin);
void AutoWah_init2(short effect_rate,short sampling,short maxf,short minf,short Q,double gainfactor,short freq_step);

#endif
