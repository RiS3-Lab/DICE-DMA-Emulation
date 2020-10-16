#include "mm_midirouter.h"
#include <string.h> 

/* The msg is not freed! To avoid memory leaks this should be done somewhere if
 * the message was allocated with malloc. */
int MIDI_Router_handleMsg(MIDI_Router *router, MIDIMsg *msg)
{
    if((!msg) || (!msg->data)) {
        return MIDI_Router_Err_NULL;
    }
    if (MIDIMSG_IS_SYS_COMMON(msg->data[0])) {
        if (router->systemCommon.callback) { 
            MIDI_CB_Info_call(router->systemCommon,msg);
        }
        return MIDI_Router_Err_GOOD;
    }
    MIDIMsg_Byte_t chan = MIDIMSG_GET_CHANNEL(msg->data[0]);
    switch (MIDIMSG_GET_STATUS(msg->data[0])) {
        /* for all cases, it is not required that data have been stored, only a
         * callback (data can be ignored by the callback) */
        case MIDIMSG_NOTE_OFF :
            if (router->cbSets[chan].noteOff.callback) { 
                MIDI_CB_Info_call(router->cbSets[chan].noteOff,msg);
            }
            break;
        case MIDIMSG_NOTE_ON :
            if (router->cbSets[chan].noteOn.callback) { 
                MIDI_CB_Info_call(router->cbSets[chan].noteOn,msg);
            }
            break;
        case MIDIMSG_POLY_PRS :
            if (router->cbSets[chan].aftertouch.callback) { 
                MIDI_CB_Info_call(router->cbSets[chan].aftertouch,msg);
            }
            break;
        case MIDIMSG_CNTRL_CHNG :
            if (router->cbSets[chan].controlChange.callback) { 
                MIDI_CB_Info_call(router->cbSets[chan].controlChange,msg);
            }
            break;
        case MIDIMSG_PRGRM_CHNG :
            if (router->cbSets[chan].programChange.callback) { 
                MIDI_CB_Info_call(router->cbSets[chan].programChange,msg);
            }
            break;
        case MIDIMSG_CHN_PRS :
            if (router->cbSets[chan].channelPressure.callback) { 
                MIDI_CB_Info_call(router->cbSets[chan].channelPressure,msg);
            }
            break;
        case MIDIMSG_PCH_BND :
            if (router->cbSets[chan].pitchBendChange.callback) { 
                MIDI_CB_Info_call(router->cbSets[chan].pitchBendChange,msg);
            }
            break;
        default :
            return MIDI_Router_Err_NOTIMP;
    }
    return MIDI_Router_Err_GOOD;
}

/* Add the cb and its data to the cc number of router */
MIDI_Router_Err MIDI_Router_addCB(MIDI_Router *router, MIDIMsg_Byte_t type, MIDIMsg_Byte_t chan, MIDI_Router_CB cb, void *data)
{
    switch (type) {
        case MIDIMSG_NOTE_OFF :
            router->cbSets[chan].noteOff.callback = cb;
            router->cbSets[chan].noteOff.data = data;
            break;
        case MIDIMSG_NOTE_ON :
            router->cbSets[chan].noteOn.callback = cb;
            router->cbSets[chan].noteOn.data = data;
            break;
        case MIDIMSG_POLY_PRS :
            router->cbSets[chan].aftertouch.callback = cb;
            router->cbSets[chan].aftertouch.data = data;
            break;
        case MIDIMSG_CNTRL_CHNG :
            router->cbSets[chan].controlChange.callback = cb;
            router->cbSets[chan].controlChange.data = data;
            break;
        case MIDIMSG_PRGRM_CHNG :
            router->cbSets[chan].programChange.callback = cb;
            router->cbSets[chan].programChange.data = data;
            break;
        case MIDIMSG_CHN_PRS :
            router->cbSets[chan].channelPressure.callback = cb;
            router->cbSets[chan].channelPressure.data = data;
            break;
        case MIDIMSG_PCH_BND :
            router->cbSets[chan].pitchBendChange.callback = cb;
            router->cbSets[chan].pitchBendChange.data = data;
            break;
        case MIDIMSG_SYS_COMMON :
            router->systemCommon.callback = cb;
            router->systemCommon.data = data;
            break;
        default :
            return MIDI_Router_Err_NOTIMP;
    }
    return MIDI_Router_Err_GOOD;
}

void MIDI_Router_init(MIDI_Router *router)
{
    size_t chan;
    for (chan = 0; chan < MIDI_Router_NUM_CHANS; chan++) {
        memset(&router->cbSets[chan],0,sizeof(MIDI_Router_CB_Set));
    }
}
