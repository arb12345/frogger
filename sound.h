#ifndef SOUND_H_
#define SOUND_H_

#include <stdint.h>
#include <msp430.h>
#include "timer.h"

#define SOUND_PAUSE			0
#define SOUND_NOTE_C3		130
#define SOUND_NOTE_C4		261
#define SOUND_NOTE_D4		294
#define SOUND_NOTE_E4		330
#define SOUND_NOTE_F4		349
#define SOUND_NOTE_G4		392
#define SOUND_NOTE_A4		440
#define SOUND_NOTE_B4		494
#define SOUND_NOTE_C5		523
#define SOUND_NOTE_D5		587
#define SOUND_NOTE_E5		659
#define SOUND_NOTE_F5		698
#define SOUND_NOTE_G5		784
#define SOUND_NOTE_A5		880
#define SOUND_NOTE_B5		988

typedef struct
{
	uint16_t frequency;
	uint16_t duration_ms;
} sound_t;

void sound_init();
void sound_play(const sound_t *sound_effect, uint16_t n_notes);
void sound_switch();
void sound_wait_until_finish();
uint8_t sound_playing();

#endif /* SOUND_H_ */
