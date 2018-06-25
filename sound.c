#include "sound.h"

#define SOUND_DIR		P2DIR
#define SOUND_OUT		P2OUT
#define SOUND_SEL		P2SEL
#define SOUND_PIN		BIT5

typedef struct
{
	uint16_t total;
	uint16_t counter;
	sound_t *sound;
} sound_state_t;

static sound_state_t sound_state;
static volatile uint16_t periods;
static volatile uint8_t sound_active;

void sound_init()
{
	SOUND_DIR |= SOUND_PIN;
	SOUND_OUT &= ~SOUND_PIN;
	SOUND_SEL |= SOUND_PIN;
	TA2CCTL0 = CCIE;
	TA2EX0 = TAIDEX_5; // 1/6
	sound_active = 0;
}

// Assumes SMCLK @ 25.000.000 Hz
static inline void sound_timer_a2_play_tone()
{
	if(sound_state.sound->frequency > 0)
	{
		uint16_t freq_in_clk_cycles = 1041749L/sound_state.sound->frequency; // 25001984/24 = 1041749
		TA2CCR0 = freq_in_clk_cycles;
		TA2CCR2 = freq_in_clk_cycles >> 1;
		TA2CCTL2 = OUTMOD_7;
		periods = sound_state.sound->frequency * (sound_state.sound->duration_ms/1000.);
	} else
	{
		TA2CCTL2 = OUTMOD_0;
		TA2CCR0 = 1042; // 1kHz
		periods = sound_state.sound->duration_ms;
	}
	TA2CTL = TASSEL__SMCLK | MC__UP | ID__4 | TACLR; // 1/4
}

void sound_play(const sound_t *sound_effect, uint16_t n_notes)
{
	TA2CTL = MC__STOP;
	sound_state.total = n_notes;
	sound_state.counter = 0;
	sound_state.sound = (sound_t *)sound_effect;
	sound_timer_a2_play_tone();
	sound_active = 1;
}

static inline void sound_next_tone()
{
	TA2CTL = MC__STOP;
	sound_state.counter++;
	sound_state.sound++;
	if(sound_state.counter < sound_state.total)
	{
		sound_timer_a2_play_tone();
	} else
	{
		TA2CTL = MC__STOP;
		sound_active = 0;
	}
}

void sound_switch()
{
	SOUND_SEL ^= SOUND_PIN;
}

void sound_wait_until_finish()
{
	while(sound_active);
}

uint8_t sound_playing()
{
	return sound_active;
}

#pragma vector=TIMER2_A0_VECTOR
__interrupt void timer_a2_ccr0_isr()
{
	if(periods > 0)
	{
		periods--;
	} else
	{
		sound_next_tone();
	}
}
