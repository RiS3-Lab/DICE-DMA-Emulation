#include "mm_midirouter_standard.h" 

void MIDI_Router_Standard_init(MIDI_Router_Standard *mr)
{
    MIDI_Router_init(&mr->router);
    MIDIMsg_Byte_t chan;
    for (chan = 0; chan < MIDI_Router_NUM_CHANS; chan++) {
        MIDI_CC_CB_Router_init(&mr->cbRouters[chan]);
        /* Add control change routers */
        MIDI_Router_addCB(&mr->router, MIDIMSG_CNTRL_CHNG, chan, MIDI_CC_CB_Router_route,
                (void*)&mr->cbRouters[chan]);
    }
}
