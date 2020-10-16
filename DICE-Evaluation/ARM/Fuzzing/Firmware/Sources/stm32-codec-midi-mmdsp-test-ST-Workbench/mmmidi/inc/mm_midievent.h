#ifndef MM_MIDIEVENT_H
#define MM_MIDIEVENT_H 

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h> 
#include <stdint.h> 
#include "mm_midimsg.h" 



typedef uint32_t MIDIEvent_Time_t;

typedef struct __MIDIEvent {
    uint64_t time;
    MIDIMsg msg;
} MIDIEvent;

MIDIEvent *MIDIEvent_new(size_t length, uint64_t time);
MIDIEvent *MIDIEvent_newFromStatus(size_t status, uint64_t time);

#ifdef __cplusplus
}
#endif  
#endif /* MM_MIDIEVENT_H */
