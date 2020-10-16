#ifndef MM_MIDIROUTER_H
#define MM_MIDIROUTER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mm_midimsg.h" 

#define  MIDI_Router_NUM_CHANS 16

typedef enum {
    MIDI_Router_Err_GOOD,
    MIDI_Router_Err_NOTIMP, /* Output when something is not implemented, such as the kind of message */
    MIDI_Router_Err_NULL    /* Something was NULL, like perhaps the message that was passed */
} MIDI_Router_Err;


typedef struct __MIDI_Router MIDI_Router;
typedef struct __MIDI_Router_CB_Set MIDI_Router_CB_Set;
typedef struct __MIDI_Router_CB_Info MIDI_Router_CB_Info;
typedef void (*MIDI_Router_CB)(void*,MIDIMsg*);

#define MIDI_CB_Info_call(cbinfo,msg) cbinfo.callback(cbinfo.data,msg)

struct __MIDI_Router_CB_Info {
    MIDI_Router_CB callback;
    void *data;
};

struct __MIDI_Router_CB_Set {
    MIDI_Router_CB_Info noteOff;
    MIDI_Router_CB_Info noteOn;
    MIDI_Router_CB_Info aftertouch;
    MIDI_Router_CB_Info controlChange;
    MIDI_Router_CB_Info programChange;
    MIDI_Router_CB_Info channelPressure;
    MIDI_Router_CB_Info pitchBendChange;
};

struct __MIDI_Router {
    MIDI_Router_CB_Set cbSets[MIDI_Router_NUM_CHANS];
    /* System common messages have no channel. */
    MIDI_Router_CB_Info systemCommon;
};

/* Call the appropriate callback based on what's in msg */
int MIDI_Router_handleMsg(MIDI_Router *router, MIDIMsg *msg);
/* Add the cb and its data to the cc number of router */
MIDI_Router_Err MIDI_Router_addCB(MIDI_Router *router, MIDIMsg_Byte_t type, MIDIMsg_Byte_t chan, MIDI_Router_CB cb, void *data);
/* Set all the cbsets to null so we know not to call unset callbacks */
void MIDI_Router_init(MIDI_Router *router);

#ifdef __cplusplus
}
#endif  
#endif /* MM_MIDIROUTER_H */
