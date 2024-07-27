#include <stdbool.h>

#include "stm32l0xx_hal.h"

#include "display.h"
#include "main.h"

struct digit_segs encode_digit(const unsigned digit, const bool encode_zero_as_blank) {
	switch (digit)
	{
	case 0:
		return encode_zero_as_blank
			? (struct digit_segs){}
			: (struct digit_segs){.a=1,.b=1,.c=1,.d=1,.e=1,.f=1};
	case 1:
		return (struct digit_segs){.b=1,.c=1};
	case 2:
		return (struct digit_segs){.a=1,.b=1,.d=1,.e=1,.g=1};
	case 3:
		return (struct digit_segs){.a=1,.b=1,.c=1,.d=1,.g=1};
	case 4:
		return (struct digit_segs){.b=1,.c=1,.f=1,.g=1};
	case 5:
		return (struct digit_segs){.a=1,.c=1,.d=1,.f=1,.g=1};
	case 6:
		return (struct digit_segs){.a=1,.c=1,.d=1,.e=1,.f=1,.g=1};
	case 7:
		return (struct digit_segs){.a=1,.b=1,.c=1};
	case 8:
		return (struct digit_segs){.a=1,.b=1,.c=1,.d=1,.e=1,.f=1,.g=1};
	case 9:
		return (struct digit_segs){.a=1,.b=1,.c=1,.d=1,.f=1,.g=1};
	default:
		return (struct digit_segs){};
	}
}

struct display_segs encode_number(const unsigned n) {
	const unsigned ones = n % 10;
	const unsigned tens = n % 100 / 10;
	const unsigned hundreds = n % 1000 / 100;

	const bool blank_tens_zero = !hundreds;

	return (struct display_segs){
		.ones=encode_digit(ones, 0),
		.tens=encode_digit(tens, blank_tens_zero),
		.hundreds=!!hundreds,
	};
}

void write_display_pins(const struct display_segs display_segs, const bool phase) {
	HAL_GPIO_WritePin(COM_GPIO_Port, COM_Pin, phase);

	// Hundreds digit:
	HAL_GPIO_WritePin(SEG_1BC_GPIO_Port, SEG_1BC_Pin, display_segs.hundreds ^ phase);

	// Tens digit:
	HAL_GPIO_WritePin(SEG_2A_GPIO_Port, SEG_2A_Pin, display_segs.tens.a ^ phase);
	HAL_GPIO_WritePin(SEG_2B_GPIO_Port, SEG_2B_Pin, display_segs.tens.b ^ phase);
	HAL_GPIO_WritePin(SEG_2C_GPIO_Port, SEG_2C_Pin, display_segs.tens.c ^ phase);
	HAL_GPIO_WritePin(SEG_2D_GPIO_Port, SEG_2D_Pin, display_segs.tens.d ^ phase);
	HAL_GPIO_WritePin(SEG_2E_GPIO_Port, SEG_2E_Pin, display_segs.tens.e ^ phase);
	HAL_GPIO_WritePin(SEG_2F_GPIO_Port, SEG_2F_Pin, display_segs.tens.f ^ phase);
	HAL_GPIO_WritePin(SEG_2G_GPIO_Port, SEG_2G_Pin, display_segs.tens.g ^ phase);

	// Ones digit:
	HAL_GPIO_WritePin(SEG_3A_GPIO_Port, SEG_3A_Pin, display_segs.ones.a ^ phase);
	HAL_GPIO_WritePin(SEG_3B_GPIO_Port, SEG_3B_Pin, display_segs.ones.b ^ phase);
	HAL_GPIO_WritePin(SEG_3C_GPIO_Port, SEG_3C_Pin, display_segs.ones.c ^ phase);
	HAL_GPIO_WritePin(SEG_3D_GPIO_Port, SEG_3D_Pin, display_segs.ones.d ^ phase);
	HAL_GPIO_WritePin(SEG_3E_GPIO_Port, SEG_3E_Pin, display_segs.ones.e ^ phase);
	HAL_GPIO_WritePin(SEG_3F_GPIO_Port, SEG_3F_Pin, display_segs.ones.f ^ phase);
	HAL_GPIO_WritePin(SEG_3G_GPIO_Port, SEG_3G_Pin, display_segs.ones.g ^ phase);
}
