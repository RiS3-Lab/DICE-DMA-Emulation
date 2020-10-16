#ifndef MM_MIDIEVENTBUILDER_H
#define MM_MIDIEVENTBUILDER_H 

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdint.h> 
#include "mm_midimsg.h" 

#define MIDI_MSG_BUILDER_MAX_MSG_LEN 12 

typedef enum
{
    MIDIMsgBuilder_State_WAIT_STATUS,
    MIDIMsgBuilder_State_WAIT_DATA,
    MIDIMsgBuilder_State_WAIT_DATA_SYSEX,
    MIDIMsgBuilder_State_COMPLETE
} MIDIMsgBuilder_State_t;

typedef enum
{
    MIDIMsgBuilder_Action_NONE,
    MIDIMsgBuilder_Action_RCVD_SYSEX_START,
    MIDIMsgBuilder_Action_RCVD_SYSEX_DATUM,
    MIDIMsgBuilder_Action_RCVD_SYSEX_END,
    MIDIMsgBuilder_Action_MSG_COMPLETE
} MIDIMsgBuilder_Action_t;

typedef struct __MIDIMsgBuilder {
    MIDIMsgBuilder_State_t state;
    size_t waitByte;
    int n_data_desired;
    MIDIMsg *msg;
    uint8_t canary3[128];
    MIDIMsg_Byte_t msgData[MIDI_MSG_BUILDER_MAX_MSG_LEN]; /* Space to store msg */
    uint8_t canary4[128];
} MIDIMsgBuilder;

typedef void(*MIDIMsgBuilder_OnComplete_CB_t)(MIDIMsgBuilder *);

#define MIDIMsgBuilder_getMsg(mmb) (mmb->msg)
#define MIDIMsgBuilder_appendByte(mmb,byte) MIDIMsg_setByte(MIDIMsgBuilder_getMsg(mmb),mmb->waitByte++,byte)

void MIDIMsgBuilder_init(MIDIMsgBuilder *mmb);
MIDIMsgBuilder_Action_t MIDIMsgBuilder_update(MIDIMsgBuilder *mmb, MIDIMsg_Byte_t byte);

#ifdef __cplusplus
}
#endif  
#endif /* MM_MIDIEVENTBUILDER_H */
