#ifndef MIDI_LOWLEVEL_H
#define MIDI_LOWLEVEL_H 

#include "stm32f4xx.h" 

#define MIDI_DMA_STRUCT DMA1_Stream5 

/* This should be automated according to current clock speed and dividers, keep
 * it simple for now */
#define MIDI_TIMER_PERIOD_TICKS 45000 
#define MIDI_TIMER_PERIOD_MS    1

#define MIDI_BAUD_RATE 31250 
/* The number of bytes per midi timer period, rounded up and then multiplied by
 * two for some extra space */
#define MIDI_BUF_SIZE ((uint32_t)(2 * (MIDI_BAUD_RATE / 1000 / 8 + 1) * MIDI_TIMER_PERIOD_MS))

#define MIDI_TIMER_INTERRUPT() do { \
    extern int MIDITimeToProcessBuffer; \
    MIDITimeToProcessBuffer = 1; \
} while (0)

/* This function must be defined by the user for successful linking */
void MIDI_process_byte(char byte);

/* This function must be called repeatedly in main or a system tick thread */
void MIDI_process_buffer(void);

void MIDI_low_level_setup(void); 
void MIDI_low_level_setup_nolib(void);


#endif /* MIDI_LOWLEVEL_H */
