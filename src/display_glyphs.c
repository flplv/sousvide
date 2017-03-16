#include <stdint.h>
#include "display_glyphs.h"

#define XXX 0b11100000
#define XX_ 0b11000000
#define X_X 0b10100000
#define X__ 0b10000000
#define _XX 0b01100000
#define _X_ 0b01000000
#define __X 0b00100000
#define ___ 0b00000000

#define XXXXXX 0b11111100
#define XXXXX_ 0b11111000
#define XXXX_X 0b11110100
#define XXXX__ 0b11110000
#define XXX_XX 0b11101100
#define XXX_X_ 0b11101000
#define XXX__X 0b11100100
#define XXX___ 0b11100000
#define XX_XXX 0b11011100
#define XX_XX_ 0b11011000
#define XX_X_X 0b11010100
#define XX_X__ 0b11010000
#define XX__XX 0b11001100
#define XX__X_ 0b11001000
#define XX___X 0b11000100
#define XX____ 0b11000000
#define X_XXXX 0b10111100
#define X_XXX_ 0b10111000
#define X_XX_X 0b10110100
#define X_XX__ 0b10110000
#define X_X_XX 0b10101100
#define X_X_X_ 0b10101000
#define X_X__X 0b10100100
#define X_X___ 0b10100000
#define X__XXX 0b10011100
#define X__XX_ 0b10011000
#define X__X_X 0b10010100
#define X__X__ 0b10010000
#define X___XX 0b10001100
#define X___X_ 0b10001000
#define X____X 0b10000100
#define X_____ 0b10000000
#define _XXXXX 0b01111100
#define _XXXX_ 0b01111000
#define _XXX_X 0b01110100
#define _XXX__ 0b01110000
#define _XX_XX 0b01101100
#define _XX_X_ 0b01101000
#define _XX__X 0b01100100
#define _XX___ 0b01100000
#define _X_XXX 0b01011100
#define _X_XX_ 0b01011000
#define _X_X_X 0b01010100
#define _X_X__ 0b01010000
#define _X__XX 0b01001100
#define _X__X_ 0b01001000
#define _X___X 0b01000100
#define _X____ 0b01000000
#define __XXXX 0b00111100
#define __XXX_ 0b00111000
#define __XX_X 0b00110100
#define __XX__ 0b00110000
#define __X_XX 0b00101100
#define __X_X_ 0b00101000
#define __X__X 0b00100100
#define __X___ 0b00100000
#define ___XXX 0b00011100
#define ___XX_ 0b00011000
#define ___X_X 0b00010100
#define ___X__ 0b00010000
#define ____XX 0b00001100
#define ____X_ 0b00001000
#define _____X 0b00000100
#define ______ 0b00000000

const uint8_t bitmap_number_zero [GLYPH_DATA_LEN] = {
    XXX,
    X_X,
    X_X,
    X_X,
    XXX
};

const uint8_t bitmap_number_one [GLYPH_DATA_LEN] = {
    __X,
    __X,
    __X,
    __X,
    __X
};

const uint8_t bitmap_number_two [GLYPH_DATA_LEN] = {
    XXX,
    __X,
    XXX,
    X__,
    XXX
};

const uint8_t bitmap_number_three [GLYPH_DATA_LEN] = {
    XXX,
    __X,
    XXX,
    __X,
    XXX
};

const uint8_t bitmap_number_four [GLYPH_DATA_LEN] = {
    X_X,
    X_X,
    XXX,
    __X,
    __X
};

const uint8_t bitmap_number_five [GLYPH_DATA_LEN] = {
    XXX,
    X__,
    XXX,
    __X,
    XXX
};

const uint8_t bitmap_number_six [GLYPH_DATA_LEN] = {
    XXX,
    X__,
    XXX,
    X_X,
    XXX
};

const uint8_t bitmap_number_seven [GLYPH_DATA_LEN] = {
    XXX,
    __X,
    __X,
    __X,
    __X
};

const uint8_t bitmap_number_eight [GLYPH_DATA_LEN] = {
    XXX,
    X_X,
    XXX,
    X_X,
    XXX,
};

const uint8_t bitmap_number_nine [GLYPH_DATA_LEN] = {
    XXX,
    X_X,
    XXX,
    __X,
    XXX,
};

const uint8_t bitmap_symbol_temperature [GLYPH_DATA_LEN] = {
    XXXXXX,
    X_XX__,
    XXXX__,
    ___X__,
    ___XXX
};

const uint8_t bitmap_letter_time [GLYPH_DATA_LEN] = {
    ____X_,
    X_____,
    XXX_X_,
    X___X_,
    XX__X_
};
