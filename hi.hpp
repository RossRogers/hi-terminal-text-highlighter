#pragma once

#include <boost/regex.hpp>
#include <boost/program_options.hpp>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

const int BEGINNING_PARSED_LINE_LENGTH = 200;


enum attribute_t {
    RESET, // Resets the console text settings
    BRIGHT,
    DIM,
    UNDERLINE,
    BLINK,
    RSVD1,
    RSVD2,
    REVERSE, // Reverses the text colors
    HIDDEN };
    
enum color_t {
    BLACK,
    RED,
    GREEN,
    BROWN,
    BLUE,
    PURPLE,
    CYAN,
    WHITE,
    NONE };
    
const int FG_OFFSET = 30;
const int BG_OFFSET = 40;
const int LNX_ESC_CHAR = 0x1b;



static void set_console_colors( color_t foreground, color_t background);


// creates the string necessary for altering the colors of the console
static void set_colors_str( color_t foreground, color_t background, char *ret_color_st);


// reset the console colors
static void reset_console_colors();

static const char* color_swatch();

static color_t interpret_color( const char *color_str );
static const char * color_t_2_str( color_t c );

struct Filter {
    boost::regex* regex;
    color_t foreground;
    color_t background;
};


