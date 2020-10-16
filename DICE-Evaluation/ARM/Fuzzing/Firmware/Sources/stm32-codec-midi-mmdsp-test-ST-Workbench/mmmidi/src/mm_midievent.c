#include "mm_midievent.h"

MIDIEvent *MIDIEvent_new(size_t length, uint64_t time)
{
    if (length == 0) { return NULL; }
    MIDIEvent *result = (MIDIEvent *)malloc(sizeof(MIDIEvent)
            + MIDIMsg_sizeof(length));
    result->time = time;
    return result;
}

MIDIEvent *MIDIEvent_newFromStatus(size_t status, uint64_t time)
{
    MIDIEvent *result;
    if (!(result = MIDIEvent_new(MIDIMsg_lengthFromStatus(status),
                    time))) {
        return NULL;
    }
    MIDIMsg_init(&(result->msg), 1, status);
    return result;
}
