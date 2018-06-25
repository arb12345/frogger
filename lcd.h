#ifndef LCD_H_
#define LCD_H_

#include <msp430.h>
#include <stdint.h>
#include "timer.h"

// P3.2 -> LCD SPI CLK
// P7.4 -> LCD RST
// P3.0 -> LCD SPI MOSI
// P2.6 -> LCD CS
// P8.2 -> LCD DC (RS)

// LCD Emulated Screen Dimensions
#define LCD_MAX_ROWS	64
#define LCD_MAX_COLS	64

typedef struct
{
	const uint8_t n_rows;
	const uint8_t n_cols;
	const uint8_t *image;
} sprite_t;

// Initialize SPI and configure LCD
void lcd_init();

// Clear LCD buffer in RAM
void lcd_clear_buffer();

// Send through SPI all the pixels colors stored in LCD buffer in RAM
void lcd_display_buffer();

// Writes on the LCD buffer in RAM the given sprite given the top-left position by coordinates x and y.
// It can also interpret the color LCD_4BIT_RGBI_BLACK as transparent, and ingore all the pixels with that color
void lcd_print_on_buffer(int16_t x, int16_t y, const sprite_t* sprite, uint8_t black_as_transparent);

// Writes on the LCD buffer in RAM a given character given the top-left position by coordinates x and y.
void lcd_print_char_on_buffer(int16_t x, int16_t y, uint8_t data);

// Writes on the LCD buffer in RAM a given string of characters given the top-left position by coordinates x and y.
void lcd_print_string_on_buffer(int16_t x, int16_t y, char* data);

// Writes on the LCD buffer in RAM the string of digits which represents an unsigned decimal number, given the top-left position by coordinates x and y.
void lcd_print_udec_on_buffer(int16_t x, int16_t y, uint16_t n);

// LCD Ascii [0x20 ... 0x7E] 6 pixels wide, 7 pixels high
#define LCD_ASCII_CHAR_HEIGHT			7
#define LCD_ASCII_CHAR_WIDTH			6

// RGBI 4bit Color Palette					   R G B I
#define LCD_4BIT_RGBI_BLACK			0x0		// 0 0 0 0
#define LCD_4BIT_RGBI_DARK_GREY		0x1		// 0 0 0 1
#define LCD_4BIT_RGBI_DARK_BLUE		0x2		// 0 0 1 0
#define LCD_4BIT_RGBI_BLUE			0x3		// 0 0 1 1
#define LCD_4BIT_RGBI_DARK_GREEN	0x4		// 0 1 0 0
#define LCD_4BIT_RGBI_GREEN			0x5		// 0 1 0 1
#define LCD_4BIT_RGBI_DARK_CYAN		0x6		// 0 1 1 0
#define LCD_4BIT_RGBI_CYAN			0x7		// 0 1 1 1
#define LCD_4BIT_RGBI_DARK_RED		0x8		// 1 0 0 0
#define LCD_4BIT_RGBI_RED			0x9		// 1 0 0 1
#define LCD_4BIT_RGBI_DARK_PURPLE	0xA		// 1 0 1 0
#define LCD_4BIT_RGBI_PURPLE		0xB		// 1 0 1 1
#define LCD_4BIT_RGBI_BROWN			0xC		// 1 1 0 0
#define LCD_4BIT_RGBI_YELLOW		0xD		// 1 1 0 1
#define LCD_4BIT_RGBI_GREY			0xE		// 1 1 1 0
#define LCD_4BIT_RGBI_WHITE			0xF		// 1 1 1 1

#endif /* LCD_H_ */
