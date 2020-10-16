#include "mm_midieventbuilder.h" 


void MIDIEventBuilder_init(MIDIEventBuilder *meb)
{
    meb->state = MIDIEventBuilder_State_WAIT_STATUS;
    meb->waitByte = 0;
    meb->event = NULL;
}

static void MIDIEventBuilder_updateIfSTATUS(MIDIEventBuilder *meb, MIDIMsg_Byte_t byte, uint64_t time)
{
    if (!(meb->event = MIDIEvent_newFromStatus(byte, time))) {
        /* couldn't make event, don't change state */
        return;
    }
    meb->state = MIDIEventBuilder_State_WAIT_DATA;
    meb->waitByte = 1;
}

static void MIDIEventBuilder_updateIfCOMPLETE(MIDIEventBuilder *meb,
        MIDIEventBuilder_OnComplete_CB_t on_complete)
{
    /* MIDIEvent ready to move on (get stored or whatever) */
    on_complete(meb);
    /* return builder to initial state */
    MIDIEventBuilder_init(meb);
}


/* Function usually called when a byte has been received (over UART for
 * example). meb should have been initialized already, byte is the byte that was
 * just received, get_time is a function that will return the time, most likely
 * at which the byte was received, and on_complete is a callback to call if meb
 * is complete after the byte is appended to it.
 */
void MIDIEventBuilder_update(MIDIEventBuilder *meb,
                             MIDIMsg_Byte_t byte,
                             MIDIEventBuilder_GetTime_CB_t get_time,
                             MIDIEventBuilder_OnComplete_CB_t on_complete)
{

    switch (meb->state) {
        case MIDIEventBuilder_State_WAIT_STATUS :
            if (MIDIMSG_IS_STATUS(byte)) {
                MIDIEventBuilder_updateIfSTATUS(meb, byte, get_time());
            }
            /* return if not a status byte (don't change state) */
            return;
        case MIDIEventBuilder_State_WAIT_DATA :
            if (MIDIMSG_IS_STATUS(byte)) {
                /* old event is now garbage, start over again */
                free(meb->event);
                MIDIEventBuilder_init(meb);
                MIDIEventBuilder_updateIfSTATUS(meb, byte, get_time());
            } else if (MIDIMSG_IS_DATA(byte)) {
                MIDIEventBuilder_appendByte(meb, byte);
                if (MIDIMSG_GET_LENGTH(MIDIEventBuilder_getMsg(meb).data[0]) == meb->waitByte) {
                    MIDIEventBuilder_updateIfCOMPLETE(meb, on_complete);
                }
            }
            return;
    }
}

