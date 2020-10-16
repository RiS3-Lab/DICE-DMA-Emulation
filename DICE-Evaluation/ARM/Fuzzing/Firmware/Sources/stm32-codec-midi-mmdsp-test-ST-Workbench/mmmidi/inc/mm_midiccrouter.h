#ifndef MM_MIDICCROUTER_H
#define MM_MIDICCROUTER_H 

#ifdef __cplusplus
extern "C" {
#endif

#include "mm_midimsg.h" 

#define MIDI_CC_CB_Router_NUM_CBS 128

typedef struct __MIDI_CC_CB_Router MIDI_CC_CB_Router;

typedef void (*MIDI_CC_CB_Router_CB)(void *,MIDIMsg*);

typedef struct __MIDI_CC_CB_Info MIDI_CC_CB_Info;

/* stores a function pointer to a callback you want to call given a certain
 * control change message and also data to be given to the callback. One nice
 * thing about this is you can have many objects, some which share callbacks,
 * others that don't, use the same interface. For example:
 *
 * Obj1 { ... }
 * Obj1Cb(void *obj1instance, MIDIMsg_Byte_t val) { ... }
 *
 * Obj2 { ... }
 * Obj2Cb(void *obj2instance, MIDIMsg_Byte_t val) { ... }
 *
 * Obj1 obj11 obj12;
 * Obj2 obj21 obj22;
 *
 * MIDI_CC_CB_Router_regFunc(router,&obj11,Obj1Cb);
 * MIDI_CC_CB_Router_regFunc(router,&obj12,Obj1Cb);
 * MIDI_CC_CB_Router_regFunc(router,&obj21,Obj2Cb);
 * MIDI_CC_CB_Router_regFunc(router,&obj22,Obj2Cb);
 *
 * Calling MIDI_CC_CB_Router_route is now more flexible.
 */
struct __MIDI_CC_CB_Info {
    MIDI_CC_CB_Router_CB callback;
    void *data;
};

#define MIDI_CC_CB_Info_call(cbinfo,val) cbinfo.callback(cbinfo.data,val)

struct __MIDI_CC_CB_Router {
    MIDI_CC_CB_Info cbInfos[MIDI_CC_CB_Router_NUM_CBS];
};

/* Route the control change message to the correct callback and call it with val.
 * Router gets casted to *MIDI_CC_CB_Router.*/
void MIDI_CC_CB_Router_route(void *router, MIDIMsg *msg);
/* Add the cb and its data to the cc number of router */
void MIDI_CC_CB_Router_addCB(MIDI_CC_CB_Router *router, MIDIMsg_Byte_t cc, MIDI_CC_CB_Router_CB cb, void *data);
/* Initialize cc router to null so there are no problems when trying to call a
 * non existing control change callback. */
void MIDI_CC_CB_Router_init(MIDI_CC_CB_Router *router);

#ifdef __cplusplus
}
#endif  

#endif /* MM_MIDICCROUTER_H */
