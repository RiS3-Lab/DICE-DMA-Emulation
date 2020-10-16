/* Includes ------------------------------------------------------------------*/
#include <stdlib.h> 
#include "stm32f4xx.h" 
#include "leds.h" 
#include "midi_lowlevel.h" 
#include "mm_midimsgbuilder.h" 
#include "mm_midirouter_standard.h" 

#define MIDI_MSG_DEBUG 
#define NUM_MIDI_MSGS 10 

extern char midiBuffer[]; /* for debugging */

static MIDIMsgBuilder_State_t lastState;
static MIDIMsgBuilder midiMsgBuilder;
static MIDI_Router_Standard midiRouter;

void MIDI_note_on_do(void *data, MIDIMsg *msg)
{
    if ((MIDIMSG_GET_CHANNEL(msg->data[0]) == 1)
            && (msg->data[1] == 60)) {
        LEDs_greenSet();
    }
    if ((MIDIMSG_GET_CHANNEL(msg->data[0]) == 1)
            && (msg->data[1] == 67)) {
        LEDs_redSet();
    }
    //MIDIMsg_free(msg); //Bug identified by DICE
}

void MIDI_note_off_do(void *data, MIDIMsg *msg)
{
    if ((MIDIMSG_GET_CHANNEL(msg->data[0]) == 1)
            && (msg->data[1] == 60)) {
        LEDs_greenReset();
    }
    if ((MIDIMSG_GET_CHANNEL(msg->data[0]) == 1)
            && (msg->data[1] == 67)) {
        LEDs_redReset();
    }
    MIDIMsg_free(msg); //Bug identified by DICE
}

int main(void)
{
	startForkserver(0);
	/* Initialize MIDI Message builder */
    MIDIMsgBuilder_init(&midiMsgBuilder);
    int dummy;
    
    /* Initialize the MIDI router */
    MIDI_Router_Standard_init(&midiRouter);
    MIDI_Router_addCB(&midiRouter.router, MIDIMSG_NOTE_ON, 1, MIDI_note_on_do, &dummy); 
    MIDI_Router_addCB(&midiRouter.router, MIDIMSG_NOTE_OFF, 1, MIDI_note_off_do, &dummy); 

    /* Enable LEDs so we can toggle them */

    LEDs_Init();
    
    /* Set up midi */
    MIDI_low_level_setup_nolib();

    while (1) {
        MIDI_process_buffer();
    }
}

void do_stuff_with_msg(MIDIMsg *msg)
{
    MIDI_Router_handleMsg(&midiRouter.router, msg);
}

void MIDI_process_byte(char byte)
{
    switch (MIDIMsgBuilder_update(&midiMsgBuilder,byte)) {
        case MIDIMsgBuilder_State_WAIT_STATUS:
            break;
        case MIDIMsgBuilder_State_WAIT_DATA:
            break;
        case MIDIMsgBuilder_State_COMPLETE:
            do_stuff_with_msg(midiMsgBuilder.msg);
            MIDIMsgBuilder_init(&midiMsgBuilder); /* reset builder */
            break;
        default:
            break;
    }
    lastState = midiMsgBuilder.state;
}
