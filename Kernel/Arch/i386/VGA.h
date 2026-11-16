#pragma once

#include <stdint.h>

extern "C"
{

enum VGAColor
{
	VGAColor_Black = 0,
	VGAColor_Blue = 1,
	VGAColor_Green = 2,
	VGAColor_Cyan = 3,
	VGAColor_Red = 4,
	VGAColor_Magenta = 5,
	VGAColor_Brown = 6,
	VGAColor_LightGray = 7,
	VGAColor_DarkGray = 8,
	VGAColor_LightBlue = 9,
	VGAColor_LightGreen = 10,
	VGAColor_LightCyan = 11,
	VGAColor_LightRed = 12,
	VGAColor_LightMagenta = 13,
	VGAColor_LightBrown = 14,
	VGAColor_White = 15,
};

static uint8_t vgaEntryColor(enum VGAColor fg, enum VGAColor bg) {
	return fg | bg << 4;
}

static uint16_t vgaEntry(unsigned char uc, uint8_t color) {
	return (uint16_t) uc | (uint16_t) color << 8;
}

}