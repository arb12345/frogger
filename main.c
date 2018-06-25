#include <msp430.h>
#include <stdint.h>
#include "wdt.h"
#include "random.h"
#include "pmm.h"
#include "clock.h"
#include "lcd.h"
#include "timer.h"
#include "sprites.h"
#include "joystick.h"
#include "buttons.h"
#include "sound.h"
#include "game_engine.h"

#define DEBUG 0

#if DEBUG
	#include "logger.h"
#endif

int main(void) {
	wdt_off();
	random_init();
	pmm_vcore(1);
	pmm_vcore(2);
	pmm_vcore(3);
	clock_init();
	__enable_interrupt();
	lcd_init();
	joystick_init();
	buttons_init();
	sound_init();
#if DEBUG
	logger_init();
#endif
	game_engine_reset_score();
	game_engine_reset_lives();
	game_engine_sound_muted_reset();
	game_engine_strawberry_disable();
	game_engine_reset_time();
	game_engine_reset_entities();
	game_engine_periodic_interrupt_start();
	while(1)
	{
		__low_power_mode_0();
#if DEBUG
		logger_time_capture_start();
#endif
		game_engine_input_read();
		game_engine_frog_action();
		game_engine_sound_mute();
		game_engine_move_all_cars();
		game_engine_screen_draw_scenario();
		game_engine_screen_draw_all_entities();
		game_engine_screen_draw_score();
		game_engine_screen_draw_lives();
		game_engine_screen_draw_time();
		game_engine_screen_draw_muted();
		lcd_display_buffer();
		if(game_engine_frog_ate_strawberry())
		{
			game_engine_play_sound_frog_ate_strawberry();
			game_engine_inc_lives();
			game_engine_strawberry_disable();
		}

		if(game_engine_any_car_run_over_frog() || game_engine_no_more_time())
		{
			game_engine_periodic_interrupt_stop();
			game_engine_play_sound_car_run_over_frog();
			sound_wait_until_finish();
			game_engine_periodic_interrupt_start();
			if(game_engine_no_more_lives())
			{
				game_engine_periodic_interrupt_stop();
				game_engine_play_sound_game_over();
				game_engine_screen_game_over();
				lcd_display_buffer();
				sound_wait_until_finish();
				game_engine_wait_on_any_button_press();
				game_engine_reset_score();
				game_engine_reset_lives();
				game_engine_strawberry_disable();
				game_engine_periodic_interrupt_start();
			} else
			{
				game_engine_dec_lives();
				game_engine_strawberry_random_enable();
			}
			game_engine_reset_time();
			game_engine_reset_entities();
		} else if (game_engine_frog_crossed_the_road())
		{
			game_engine_periodic_interrupt_stop();
			game_engine_play_sound_frog_crossed_road();
			sound_wait_until_finish();
			game_engine_periodic_interrupt_start();
			game_engine_reset_time();
			game_engine_reset_entities();
			game_engine_inc_score();
			game_engine_strawberry_random_enable();
		} else
		{
			game_engine_dec_time();
		}
#if DEBUG
		logger_time_capture_stop();
		logger_time_capture_print_info();
#endif
	}
}
