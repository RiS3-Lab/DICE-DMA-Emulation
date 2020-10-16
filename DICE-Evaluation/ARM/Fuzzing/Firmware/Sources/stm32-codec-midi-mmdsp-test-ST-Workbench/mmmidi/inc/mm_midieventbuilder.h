#ifndef MM_MIDIEVENTBUILDER_H
#define MM_MIDIEVENTBUILDER_H 

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdint.h> 
#include "mm_midievent.h" 

typedef enum
{
    MIDIEventBuilder_State_WAIT_STATUS = 0x00,
    MIDIEventBuilder_State_WAIT_DATA   = 0x01,
} MIDIEventBuilder_State_t;

typedef struct __MIDIEventBuilder {
    MIDIEventBuilder_State_t state;
    size_t waitByte;
    MIDIEvent *event;
} MIDIEventBuilder;

typedef void(*MIDIEventBuilder_OnComplete_CB_t)(MIDIEventBuilder *);
typedef MIDIEvent_Time_t(*MIDIEventBuilder_GetTime_CB_t)(void);

#define MIDIEventBuilder_getMsg(meb) (meb->event->msg)
#define MIDIEventBuilder_appendByte(meb,byte) MIDIEventBuilder_getMsg(meb).data[meb->waitByte++] = byte


#ifdef __cplusplus
}
#endif  
#endif /* MM_MIDIEVENTBUILDER_H */
