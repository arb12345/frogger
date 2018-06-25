#include "game_engine.h"

static uint16_t game_engine_time;

#define GAME_ENGINE_CARS_MAX	4
static game_engine_car_entity_t game_engine_cars[GAME_ENGINE_CARS_MAX];

static game_engine_frog_entity_t game_engine_frog =
{
		{
				{ 27 },
				{ 56 },
				{ &sprite_frog0_up }
		},
		{
				{ &sprite_frog1_up },
				{ &sprite_frog0_up }
		},
		{
				{ &sprite_frog1_down },
				{ &sprite_frog0_down }
		},
		{
				{ &sprite_frog1_left },
				{ &sprite_frog0_left }
		},
		{
				{ &sprite_frog1_right },
				{ &sprite_frog0_right }
		},
};

static game_engine_strawberry_entity_t game_engine_strawberry =
{
		{
				{ 0 },
				{ 0 },
				{ &sprite_strawberry0 }
		},
		{ 0 }
};

static uint16_t game_engine_score;
static uint16_t game_engine_lives;
static uint8_t game_engine_muted = 0;

#define GAME_ENGINE_SOUND_FROG_CROSSED_ROAD_QUATER_NOTE_DURATION 				125
static const sound_t game_engine_sound_frog_crossed_road [] =
{
		{
				{ SOUND_NOTE_C5 },
				{ GAME_ENGINE_SOUND_FROG_CROSSED_ROAD_QUATER_NOTE_DURATION }
		},
		{
				{ SOUND_NOTE_F5 },
				{ GAME_ENGINE_SOUND_FROG_CROSSED_ROAD_QUATER_NOTE_DURATION }
		},
		{
				{ SOUND_NOTE_G5 },
				{ GAME_ENGINE_SOUND_FROG_CROSSED_ROAD_QUATER_NOTE_DURATION }
		},
};

#define GAME_ENGINE_SOUND_GAME_OVER_QUATER_NOTE_DURATION						250
static const sound_t game_engine_sound_game_over[] =
{
		{
				{ SOUND_NOTE_G4 },
				{ GAME_ENGINE_SOUND_GAME_OVER_QUATER_NOTE_DURATION }
		},
		{
				{ SOUND_NOTE_C4 },
				{ GAME_ENGINE_SOUND_GAME_OVER_QUATER_NOTE_DURATION * 2 }
		},
};

#define GAME_ENGINE_SOUND_CAR_RUN_OVER_FROG_QUATER_NOTE_DURATION						250
static const sound_t game_engine_sound_car_run_over_frog[] =
{
		{
				{ SOUND_NOTE_C3 },
				{ GAME_ENGINE_SOUND_CAR_RUN_OVER_FROG_QUATER_NOTE_DURATION }
		},
		{
				{ SOUND_NOTE_C3 },
				{ GAME_ENGINE_SOUND_CAR_RUN_OVER_FROG_QUATER_NOTE_DURATION }
		},
};

#define GAME_ENGINE_SOUND_FROG_JUMP_QUATER_NOTE_DURATION						33
static const sound_t game_engine_sound_frog_jump[] =
{
		{
				{ SOUND_NOTE_A4 },
				{ GAME_ENGINE_SOUND_FROG_JUMP_QUATER_NOTE_DURATION }
		},
		{
				{ SOUND_NOTE_F4 },
				{ GAME_ENGINE_SOUND_FROG_JUMP_QUATER_NOTE_DURATION }
		},
};

#define GAME_ENGINE_SOUND_FROG_ATE_STRAWBERRY_QUARTER_NOTE_DURATION				125
static const sound_t game_engine_sound_frog_ate_strawberry[] =
{
		{
				{ SOUND_NOTE_A4 },
				{ GAME_ENGINE_SOUND_FROG_ATE_STRAWBERRY_QUARTER_NOTE_DURATION * 2 }
		},
		{
				{ SOUND_NOTE_B4 },
				{ GAME_ENGINE_SOUND_FROG_ATE_STRAWBERRY_QUARTER_NOTE_DURATION }
		},
		{
				{ SOUND_NOTE_C5 },
				{ GAME_ENGINE_SOUND_FROG_ATE_STRAWBERRY_QUARTER_NOTE_DURATION }
		}

};

#define GAME_ENGINE_SOUND_FROG_CROSSED_ROAD_TOTAL_NOTES		(sizeof(game_engine_sound_frog_crossed_road)/sizeof(sound_t))
#define GAME_ENGINE_SOUND_GAME_OVER_TOTAL_NOTES				(sizeof(game_engine_sound_game_over)/sizeof(sound_t))
#define GAME_ENGINE_SOUND_CAR_RUN_OVER_FROG_TOTAL_NOTES		(sizeof(game_engine_sound_car_run_over_frog)/sizeof(sound_t))
#define GAME_ENGINE_SOUND_FROG_JUMP_TOTAL_NOTES				(sizeof(game_engine_sound_frog_jump)/sizeof(sound_t))
#define GAME_ENGINE_SOUND_FROG_ATE_STRAWBERRY_TOTAL_NOTES	(sizeof(game_engine_sound_frog_ate_strawberry)/sizeof(sound_t))

void game_engine_sound_muted_reset()
{
	game_engine_muted = 0;
}

void game_engine_play_sound_game_over()
{
	sound_play(game_engine_sound_game_over,GAME_ENGINE_SOUND_GAME_OVER_TOTAL_NOTES);
}

void game_engine_play_sound_frog_crossed_road()
{
	sound_play(game_engine_sound_frog_crossed_road,GAME_ENGINE_SOUND_FROG_CROSSED_ROAD_TOTAL_NOTES);
}

void game_engine_play_sound_car_run_over_frog()
{
	sound_play(game_engine_sound_car_run_over_frog, GAME_ENGINE_SOUND_CAR_RUN_OVER_FROG_TOTAL_NOTES);
}

void game_engine_play_sound_frog_jump()
{
	if(!sound_playing())
	{
		sound_play(game_engine_sound_frog_jump, GAME_ENGINE_SOUND_FROG_JUMP_TOTAL_NOTES);
	}
}

void game_engine_play_sound_frog_ate_strawberry()
{
	sound_play(game_engine_sound_frog_ate_strawberry, GAME_ENGINE_SOUND_FROG_ATE_STRAWBERRY_TOTAL_NOTES);
}

void game_engine_periodic_interrupt_start()
{
	timer_a1_init_20Hz();
}

void game_engine_periodic_interrupt_stop()
{
	timer_a1_stop();
}

void game_engine_entity_move(game_engine_entity_t *entity, game_engine_mv_dir_t dir, uint8_t cels)
{
	switch(dir)
	{
		case GAME_ENGINE_MV_DIR_UP:
			entity->y -= cels;
			break;
		case GAME_ENGINE_MV_DIR_DOWN:
			entity->y += cels;
			break;
		case GAME_ENGINE_MV_DIR_LEFT:
			entity->x -= cels;
			break;
		case GAME_ENGINE_MV_DIR_RIGHT:
			entity->x += cels;
			break;
		default:
			break;
	}
}

// Check if the entity is inside the game plane
// If so, return 0 indicating that none of the limits on each direction
// was exceeded. If not, return a value indicating which limits were exceeded
// bit equals 1 if limit was exceeded, 0 if it was not
// Up		Down	Left	Right
// BIT3		BIT2	BIT1	BIT0
uint8_t game_engine_entity_validate_coordinates(game_engine_entity_t *entity)
{
	uint8_t value = 0;
	int16_t entity_limit_up = entity->y;
	int16_t entity_limit_down = entity->y + entity->image->n_rows;
	int16_t entity_limit_left = entity->x;
	int16_t entity_limit_right = entity->x + entity->image->n_cols;
	value |= entity_limit_up < 0 ? BIT3 : 0;
	value |= entity_limit_down > (LCD_MAX_ROWS-1) ? BIT2 : 0;
	value |= entity_limit_left < 0 ? BIT1 : 0;
	value |= entity_limit_right > (LCD_MAX_COLS-1) ? BIT0 : 0;
	return value;
}

void game_engine_entity_force_position(game_engine_entity_t *entity, int16_t x, int16_t y)
{
	entity->x = x;
	entity->y = y;
}

void game_engine_input_read()
{
	joystick_read();
	buttons_read();
}

static uint16_t game_engine_car_got_off_screen(game_engine_car_entity_t * car)
{
	return (car->entity.x < (0 - car->entity.image->n_cols)) ||
		   (car->entity.x > (LCD_MAX_COLS-1));
}

void game_engine_move_all_cars()
{
	static uint8_t speed_odd_factor = 1;
	uint8_t i = 0;
	for(i = 0; i < GAME_ENGINE_CARS_MAX; i++)
	{
		uint8_t speed_odd_value = game_engine_cars[i].speed & BIT0;
		game_engine_entity_move(&game_engine_cars[i].entity,GAME_ENGINE_MV_DIR_RIGHT,(game_engine_cars[i].speed >> 1) + (speed_odd_value && speed_odd_factor ? 1 : 0));
		if(game_engine_car_got_off_screen(&game_engine_cars[i]))
		{
			game_engine_entity_force_position(&game_engine_cars[i].entity, 0 - game_engine_cars[i].entity.image->n_cols,game_engine_cars[i].entity.y);
		}
	}
	speed_odd_factor ^= 1;
}

void game_engine_screen_draw_scenario()
{
	lcd_print_on_buffer(0,0,&sprite_scenario,0);
}


static uint8_t frog_animation_in_progress = 0;
static uint8_t frog_animation_direction = 0;		// 0 -> up, 1 -> down, 2 -> left, 3 -> right
void game_engine_move_frog()
{
	if(frog_animation_in_progress)
	{
		frog_animation_in_progress = 0;
		switch(frog_animation_direction)
		{
			case 0:	// up
				game_engine_frog.entity.image = game_engine_frog.animation_up[1];
				game_engine_entity_move(&game_engine_frog.entity,GAME_ENGINE_MV_DIR_UP,3);
				break;
			case 1:	// down
				game_engine_frog.entity.image = game_engine_frog.animation_down[1];
				game_engine_entity_move(&game_engine_frog.entity,GAME_ENGINE_MV_DIR_DOWN,3);
				break;
			case 2:	// left
				game_engine_frog.entity.image = game_engine_frog.animation_left[1];
				game_engine_entity_move(&game_engine_frog.entity,GAME_ENGINE_MV_DIR_LEFT,3);
				break;
			case 3:	// right
				game_engine_frog.entity.image = game_engine_frog.animation_right[1];
				game_engine_entity_move(&game_engine_frog.entity,GAME_ENGINE_MV_DIR_RIGHT,3);
				break;
		}
	} else
	{
		if(button_up)
		{
			frog_animation_in_progress = 1;
			frog_animation_direction = 0;
			game_engine_frog.entity.image = game_engine_frog.animation_up[0];
			game_engine_entity_move(&game_engine_frog.entity,GAME_ENGINE_MV_DIR_UP,4);
			game_engine_play_sound_frog_jump();
		} else if(button_down)
		{
			frog_animation_in_progress = 1;
			frog_animation_direction = 1;
			game_engine_frog.entity.image = game_engine_frog.animation_down[0];
			game_engine_entity_move(&game_engine_frog.entity,GAME_ENGINE_MV_DIR_DOWN,4);
			game_engine_play_sound_frog_jump();
		} else if(joystick_direction == JOYSTICK_DIR_LEFT)
		{
			frog_animation_in_progress = 1;
			frog_animation_direction = 2;
			game_engine_frog.entity.image = game_engine_frog.animation_left[0];
			game_engine_entity_move(&game_engine_frog.entity,GAME_ENGINE_MV_DIR_LEFT,4);
			game_engine_play_sound_frog_jump();
		} else if(joystick_direction == JOYSTICK_DIR_RIGHT)
		{
			frog_animation_in_progress = 1;
			frog_animation_direction = 3;
			game_engine_frog.entity.image = game_engine_frog.animation_right[0];
			game_engine_entity_move(&game_engine_frog.entity,GAME_ENGINE_MV_DIR_RIGHT,4);
			game_engine_play_sound_frog_jump();
		}
	}
	uint8_t bounds_exceeded = game_engine_entity_validate_coordinates(&game_engine_frog.entity);
	if(bounds_exceeded)
	{
		int16_t frog_x = game_engine_frog.entity.x;
		int16_t frog_y = game_engine_frog.entity.y;
		if(bounds_exceeded & BIT3)
		{
			frog_y = 0;
		} else if(bounds_exceeded & BIT2)
		{
			frog_y = LCD_MAX_ROWS - game_engine_frog.entity.image->n_rows;
		}
		if(bounds_exceeded & BIT1)
		{
			frog_x = 0;
		} else if(bounds_exceeded & BIT0)
		{
			frog_x = LCD_MAX_COLS - game_engine_frog.entity.image->n_cols;
		}
		game_engine_entity_force_position(&game_engine_frog.entity,frog_x,frog_y);
	}
}

void game_engine_screen_draw_all_entities()
{
	uint8_t i = 0;
	if(game_engine_strawberry.visible)
	{
		lcd_print_on_buffer(game_engine_strawberry.entity.x, game_engine_strawberry.entity.y, game_engine_strawberry.entity.image,1);
	}
	for(i = 0; i < GAME_ENGINE_CARS_MAX; i++)
	{
		lcd_print_on_buffer(game_engine_cars[i].entity.x, game_engine_cars[i].entity.y, game_engine_cars[i].entity.image,1);
	}
	lcd_print_on_buffer(game_engine_frog.entity.x, game_engine_frog.entity.y, game_engine_frog.entity.image,1);
}

#pragma vector=TIMER1_A0_VECTOR
__interrupt void timer_a1_ccr0_isr()
{
	__low_power_mode_off_on_exit();
}

void game_engine_frog_action()
{
	if(button_up && button_down)
	{
		button_up = button_down = 0;
	}
	if(frog_animation_in_progress || button_up || button_down || joystick_direction != JOYSTICK_DIR_MIDDLE)
	{
		game_engine_move_frog();
	}
}

void game_engine_cars_init()
{
	uint16_t i;
	for (i = 0; i < GAME_ENGINE_CARS_MAX; ++i) {
		game_engine_cars[i].speed = 1 + (random() >> 14) + (game_engine_score/5);
	}
}

uint8_t game_engine_frog_ate_strawberry()
{
	if(game_engine_strawberry.visible)
	{
		uint8_t frog_did_not_eat_strawberry =
				((game_engine_frog.entity.y + game_engine_frog.entity.image->n_rows) < (game_engine_strawberry.entity.y)) 		||
				((game_engine_frog.entity.y) > (game_engine_strawberry.entity.y + game_engine_strawberry.entity.image->n_rows)) ||
				((game_engine_frog.entity.x) > (game_engine_strawberry.entity.x + game_engine_strawberry.entity.image->n_cols)) ||
				((game_engine_frog.entity.x + game_engine_frog.entity.image->n_cols) < (game_engine_strawberry.entity.x) );
		return !frog_did_not_eat_strawberry;
	} else
	{
		return 0;
	}
}

static uint8_t game_engine_car_run_over_frog(game_engine_car_entity_t* car)
{
	uint8_t car_not_run_over_frog =
			((car->entity.y + car->entity.image->n_rows) < (game_engine_frog.entity.y)) 			|| 		// car's bottom edge is HIGHER THAN frog's top edge
			((car->entity.y) > (game_engine_frog.entity.y + game_engine_frog.entity.image->n_rows)) ||		// car's top edge is LOWER THAN frog's bottom edge
			((car->entity.x) > (game_engine_frog.entity.x + game_engine_frog.entity.image->n_cols)) ||		// car's left edge is to THE RIGHT OF frog's right edge
			((car->entity.x + car->entity.image->n_cols) < (game_engine_frog.entity.x) );					// car's right edge is to THE LEFT OF frog's left edge
	return !car_not_run_over_frog;
}

uint8_t game_engine_any_car_run_over_frog()
{
	uint16_t i;
	for(i = 0; i < GAME_ENGINE_CARS_MAX; ++i)
	{
		if(game_engine_car_run_over_frog(&game_engine_cars[i]))
		{
			return 1;
		}
	}
	return 0;
}

uint8_t game_engine_frog_crossed_the_road()
{
	return (game_engine_frog.entity.y + game_engine_frog.entity.image->n_rows) <= 8;
}

void game_engine_reset_entities()
{
	uint16_t i;
	uint8_t y_cars[] = {10, 20, 30, 40};
	for(i = 0; i < GAME_ENGINE_CARS_MAX; ++i)
	{
		uint8_t t = random() >> 14;
		uint8_t m = y_cars[t];
		uint8_t n = y_cars[i];
		y_cars[i] = m;
		y_cars[t] = n;
	}
	for(i = 0; i < GAME_ENGINE_CARS_MAX; ++i)
	{
		uint8_t car_sprite = random() >> 14;
		switch(car_sprite)
		{
			case 0x0:
				game_engine_cars[i].entity.image = &sprite_car0;
				break;
			case 0x1:
				game_engine_cars[i].entity.image = &sprite_car1;
				break;
			case 0x2:
				game_engine_cars[i].entity.image = &sprite_truck0;
				break;
			case 0x3:
				game_engine_cars[i].entity.image = &sprite_trator0;
				break;
		}

		int16_t x_car = ((random() >> 9) % (LCD_MAX_COLS + game_engine_cars[i].entity.image->n_cols)) - game_engine_cars[i].entity.image->n_cols;
		game_engine_entity_force_position(&game_engine_cars[i].entity,x_car,y_cars[i]);
	}
	game_engine_entity_force_position(&game_engine_frog.entity,
			(LCD_MAX_COLS >> 1) - (game_engine_frog.entity.image->n_cols >> 1),
			LCD_MAX_ROWS - game_engine_frog.entity.image->n_rows);
	game_engine_cars_init();
}


void game_engine_screen_draw_score()
{
	lcd_print_udec_on_buffer(1,56,game_engine_score);
}

void game_engine_reset_score()
{
	game_engine_score = GAME_ENGINE_INITIAL_SCORE;
}

void game_engine_inc_score()
{
	game_engine_score++;
}

void game_engine_screen_draw_lives()
{
	uint16_t i;
	for (i = 0; i < game_engine_lives; ++i) {
		lcd_print_on_buffer(1 + (i * (sprite_life.n_cols + 1)), 1, &sprite_life, 1);
	}
}

void game_engine_screen_draw_muted()
{
	if(game_engine_muted)
	{
		lcd_print_on_buffer(LCD_MAX_COLS - sprite_muted.n_cols, 0, &sprite_muted, 1);
	}
}

void game_engine_reset_lives()
{
	game_engine_lives = GAME_ENGINE_INITIAL_LIVES;
}

void game_engine_dec_lives()
{
	game_engine_lives--;
}

void game_engine_inc_lives()
{
	game_engine_lives++;
}

uint8_t game_engine_no_more_lives()
{
	return game_engine_lives == 0;
}

void game_engine_wait_on_any_button_press()
{
	buttons_read();
	uint16_t previous_buttons_state;
	uint16_t buttons_state = button_up || button_down;
	do
	{
		previous_buttons_state = buttons_state;
		buttons_read();
		buttons_state = button_up || button_down;
	} while(previous_buttons_state != 0 || buttons_state == 0);
}

void game_engine_screen_game_over()
{
	lcd_clear_buffer();
	lcd_print_string_on_buffer(2,20,"GAME OVER!");
	lcd_print_string_on_buffer(16,30,"Score");
	if(game_engine_score < 10)
	{
		lcd_print_udec_on_buffer(28,40,game_engine_score);
	} else if(game_engine_score < 100)
	{
		lcd_print_udec_on_buffer(26,40,game_engine_score);
	} else if(game_engine_score < 1000)
	{
		lcd_print_udec_on_buffer(22,40,game_engine_score);
	} else if(game_engine_score < 10000)
	{
		lcd_print_udec_on_buffer(20,40,game_engine_score);
	} else
	{
		lcd_print_udec_on_buffer(16,40,game_engine_score);
	}
}

void game_engine_strawberry_disable()
{
	game_engine_strawberry.visible = 0;
}

void game_engine_strawberry_random_enable()
{
	if(game_engine_lives < GAME_ENGINE_INITIAL_LIVES)
	{
		uint16_t k = random() % (GAME_ENGINE_INITIAL_LIVES * 2);
		if(k < (GAME_ENGINE_INITIAL_LIVES - game_engine_lives))
		{
			game_engine_strawberry.visible = 1;
			game_engine_entity_force_position(&game_engine_strawberry.entity, random() % (LCD_MAX_COLS - game_engine_strawberry.entity.image->n_cols), (random() % 4) * 10 + 11);
		} else
		{
			game_engine_strawberry.visible = 0;
		}
	} else
	{
		game_engine_strawberry.visible = 0;
	}
}

void game_engine_reset_time()
{
	game_engine_time = GAME_ENGINE_INITIAL_TIME;
}


void game_engine_dec_time()
{
	game_engine_time--;
}


uint8_t game_engine_no_more_time()
{
	return game_engine_time == 0;
}

void game_engine_screen_draw_time()
{
	uint16_t i, j;
	lcd_print_on_buffer(46, 55, &sprite_time_margin, 1);
	for (i = 0, j = 0; i < game_engine_time; i += GAME_ENGINE_TIME_STEP, j++)
	{
		if(game_engine_time >= i + GAME_ENGINE_TIME_STEP_THRESHOLD)
		{
			lcd_print_on_buffer(47 + (j * sprite_time.n_cols), 56, &sprite_time, 1);
		}
	}
}

void game_engine_sound_mute()
{
	if(button_mute_pressed())
	{
		game_engine_muted ^= 1;
		sound_switch();
	}
}
