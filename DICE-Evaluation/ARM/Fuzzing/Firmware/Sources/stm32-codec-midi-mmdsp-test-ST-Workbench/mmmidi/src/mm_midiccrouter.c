#include "mm_midiccrouter.h" 
#include <string.h> 

/* Route the control change message to the correct callback and call it with val */
void MIDI_CC_CB_Router_route(void *router, MIDIMsg *msg)
{
    if (((MIDI_CC_CB_Router*)router)->cbInfos[msg->data[1]].callback) {
        MIDI_CC_CB_Info_call(((MIDI_CC_CB_Router*)router)->cbInfos[msg->data[1]],
                msg);
    }
}

/* Add the cb and its data to the cc number of router */
void MIDI_CC_CB_Router_addCB(MIDI_CC_CB_Router *router, MIDIMsg_Byte_t cc, MIDI_CC_CB_Router_CB cb, void *data)
{
    router->cbInfos[cc].callback = cb;
    router->cbInfos[cc].data = data;
}

/* Initialize all cb infos to NULL */
void MIDI_CC_CB_Router_init(MIDI_CC_CB_Router *router)
{
    size_t cc;
    for (cc = 0; cc < MIDI_CC_CB_Router_NUM_CBS; cc++) {
        memset(&router->cbInfos[cc],0,sizeof(MIDI_CC_CB_Info));
    }
}
