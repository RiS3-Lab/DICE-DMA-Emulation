#ifndef MM_MIDIROUTER_STANDARD_H
#define MM_MIDIROUTER_STANDARD_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mm_midirouter.h"
#include "mm_midiccrouter.h" 

typedef struct __MIDI_Router_Standard MIDI_Router_Standard;

struct __MIDI_Router_Standard {
    MIDI_Router router;
    MIDI_CC_CB_Router cbRouters[MIDI_Router_NUM_CHANS];
};

void MIDI_Router_Standard_init(MIDI_Router_Standard *mr);

#ifdef __cplusplus
}
#endif  
#endif /* MM_MIDIROUTER_STANDARD_H */
