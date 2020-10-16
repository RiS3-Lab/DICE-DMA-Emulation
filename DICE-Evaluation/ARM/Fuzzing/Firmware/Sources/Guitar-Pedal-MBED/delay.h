#ifndef __DELAY_H__
#define __DELAY_H__

void Delay_Init(double delay_samples,double dfb,double dfw, double dmix);
void Delay_set_fb(double val);
void Delay_set_fw(double val);
void Delay_set_mix(double val);
void Delay_set_delay(double n_delay);
double Delay_get_fb(void);
double Delay_get_fw(void);
double Delay_get_mix(void);
double Delay_task(double xin);



#endif
