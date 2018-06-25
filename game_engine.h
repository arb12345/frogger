#ifndef GAME_ENGINE_H_
#define GAME_ENGINE_H_

#include <msp430.h>
#include <stdint.h>
#include "joystick.h"
#include "sprites.h"
#include "lcd.h"
#include "timer.h"
#include "buttons.h"
#include "random.h"
#include "sound.h"

typedef struct
{
	int16_t x;
	int16_t y;
	const sprite_t *image;
} game_engine_entity_t;

typedef struct
{
	game_engine_entity_t entity;
	uint16_t speed;
} game_engine_car_entity_t;

#define ANIMATION_SPRITES_MAX		2
typedef struct
{
	game_engine_entity_t entity;
	const sprite_t *animation_up[ANIMATION_SPRITES_MAX];
	const sprite_t *animation_down[ANIMATION_SPRITES_MAX];
	const sprite_t *animation_left[ANIMATION_SPRITES_MAX];
	const sprite_t *animation_right[ANIMATION_SPRITES_MAX];
} game_engine_frog_entity_t;

typedef struct
{
	game_engine_entity_t entity;
	uint8_t visible;
} game_engine_strawberry_entity_t;

typedef enum
{
	GAME_ENGINE_MV_DIR_STOPPED = 0,
	GAME_ENGINE_MV_DIR_UP,
	GAME_ENGINE_MV_DIR_DOWN,
	GAME_ENGINE_MV_DIR_LEFT,
	GAME_ENGINE_MV_DIR_RIGHT
} game_engine_mv_dir_t;

#define GAME_ENGINE_INITIAL_SCORE		0
#define GAME_ENGINE_INITIAL_LIVES		3
#define GAME_ENGINE_INITIAL_TIME 		200
#define GAME_ENGINE_TIME_TOTAL_STEPS	15
#define GAME_ENGINE_TIME_STEP			(GAME_ENGINE_INITIAL_TIME/GAME_ENGINE_TIME_TOTAL_STEPS)
#define GAME_ENGINE_TIME_STEP_THRESHOLD	(GAME_ENGINE_TIME_STEP/2)

void game_engine_screen_draw_all_entities();
void game_engine_move_all_cars();
void game_engine_periodic_interrupt_start();
void game_engine_periodic_interrupt_stop();
void game_engine_screen_draw_scenario();
void game_engine_frog_action();
uint8_t game_engine_any_car_run_over_frog();
uint8_t game_engine_frog_crossed_the_road();
void game_engine_reset_entities();
void game_engine_screen_draw_score();
void game_engine_reset_score();
void game_engine_inc_score();
void game_engine_screen_draw_lives();
void game_engine_reset_lives();
void game_engine_dec_lives();
void game_engine_inc_lives();
uint8_t game_engine_no_more_lives();
void game_engine_wait_on_any_button_press();
void game_engine_screen_game_over();
void game_engine_play_sound_car_run_over_frog();
void game_engine_play_sound_frog_crossed_road();
void game_engine_play_sound_frog_jump();
void game_engine_play_sound_game_over();
void game_engine_play_sound_frog_ate_strawberry();
void game_engine_screen_draw_muted();
void game_engine_sound_muted_reset();
void game_engine_sound_mute();
void game_engine_strawberry_disable();
void game_engine_strawberry_random_enable();
uint8_t game_engine_frog_ate_strawberry();
void game_engine_reset_time();
void game_engine_dec_time();
uint8_t game_engine_no_more_time();
void game_engine_screen_draw_time();

#endif /* GAME_ENGINE_H_ */
