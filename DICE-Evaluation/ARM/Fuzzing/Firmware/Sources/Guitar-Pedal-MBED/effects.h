#ifndef EFFECTS_H
#define EFFECTS_H

#include "math.h"
#include "flanger.h"

#define MAX_MOD_COUNT 5


extern int mod[];

void init_effects(void);
float clean(float x);
float ez_distort(float x);
float ez_distort2(float x);
float advanced_distort(float x);
float crusher(float x, float normfreq, int bits );
float overdrive(float x);
float tremolo(float x);
float delay_line(float input);
float fuzz_exp(float x);
float flanger(float val);
float tremolo2(float x);
float wahwah(float x);
//float abs(float x);
float delay(float in);
void set_modifications_default();

#endif
