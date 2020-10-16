#include "mm_midimsgbuilder.h" 


void MIDIMsgBuilder_init(MIDIMsgBuilder *mmb)
{
    mmb->state = MIDIMsgBuilder_State_WAIT_STATUS;
    mmb->waitByte = 0;
    mmb->n_data_desired = 0;
    mmb->msg = (MIDIMsg*)mmb->msgData;
}

/* Appends byte. If there is room, returns 0, otherwise returns -1 and byte
 * wasn't appended. */
int MIDIMsgBuilder_append_byte(MIDIMsgBuilder *mmb, MIDIMsg_Byte_t byte)
{
    if (mmb->waitByte < MIDI_MSG_BUILDER_MAX_MSG_LEN) {
        mmb->msgData[mmb->waitByte] = byte;
        mmb->waitByte++;
        return 0;
    }
    return -1;
}

static void MIDIMsgBuilder_updateIfSTATUS(MIDIMsgBuilder *mmb, MIDIMsg_Byte_t byte)
{
    if (!(mmb->msg = MIDIMsg_newFromStatus(byte))) {
        /* couldn't make event, don't change state */
        return;
    }
    mmb->state = MIDIMsgBuilder_State_WAIT_DATA;
    mmb->waitByte = 1;
}

static void MIDIMsgBuilder_updateIfCOMPLETE(MIDIMsgBuilder *mmb)
{
    mmb->state = MIDIMsgBuilder_State_COMPLETE;
}

MIDIMsgBuilder_Action_t MIDIMsgBuilder_update(MIDIMsgBuilder *mmb,
                                              MIDIMsg_Byte_t byte)
{
    switch(mmb->state) {
        case MIDIMsgBuilder_State_WAIT_DATA_SYSEX:
            if (MIDIMSG_IS_DATA(byte)) {
                /* Caller must itself pass byte to routine reading sysex bytes */
                return MIDIMsgBuilder_Action_RCVD_SYSEX_DATUM;
            } else if (MIDIMSG_IS_SYSEX_END(byte)) {
                mmb->state = MIDIMsgBuilder_State_WAIT_STATUS;
                return MIDIMsgBuilder_Action_RCVD_SYSEX_END;
            }
            /* Otherwise continue as if we just received a status byte we were
             * waiting for. */
        case MIDIMsgBuilder_State_WAIT_DATA:
            if (MIDIMSG_IS_DATA(byte)) {
                (void)MIDIMsgBuilder_append_byte(mmb,byte);
                mmb->n_data_desired--;
                if (mmb->n_data_desired == 0) {
                    /* Builder contains a full message */
                    mmb->state = MIDIMsgBuilder_State_WAIT_STATUS;
                    return MIDIMsgBuilder_Action_MSG_COMPLETE;
                }
                return MIDIMsgBuilder_Action_NONE;
            }
            /* Otherwise continue as if we just received a status byte we were
             * waiting for. */
        case MIDIMsgBuilder_State_WAIT_STATUS:
            if (MIDIMSG_IS_STATUS(byte)) {
                /* Reset builder. */
                MIDIMsgBuilder_init(mmb);
                (void)MIDIMsgBuilder_append_byte(mmb,byte);
                mmb->n_data_desired = MIDIMsg_numDataBytes(byte);
                if (mmb->n_data_desired == 0) {
                    /* No data needed */
                    mmb->state = MIDIMsgBuilder_State_WAIT_STATUS;
                    return MIDIMsgBuilder_Action_MSG_COMPLETE;
                } else if (mmb->n_data_desired == -1) {
                    /* Undefined number of data, which is for sysex */
                    mmb->state = MIDIMsgBuilder_State_WAIT_DATA_SYSEX;
                    return MIDIMsgBuilder_Action_RCVD_SYSEX_START;
                }
                mmb->state = MIDIMsgBuilder_State_WAIT_DATA;
                return MIDIMsgBuilder_Action_NONE;
            }
    }
}

