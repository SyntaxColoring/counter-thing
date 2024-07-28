#pragma once

#include <stdbool.h>

struct digit_segs
{
	bool a;
	bool b;
	bool c;
	bool d;
	bool e;
	bool f;
	bool g;
};

struct display_segs
{
	bool hundreds; // Our display can only show 1 or nothing for the hundreds digit.
	struct digit_segs tens;
	struct digit_segs ones;
};

struct digit_segs encode_digit(unsigned digit, bool encode_zero_as_blank);
struct display_segs encode_number(unsigned n);
void write_display_pins(struct display_segs display_segs, bool phase);
