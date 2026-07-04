/*
 * File name: vt100.h
 * Description: ANSI VT100 standard escape sequence support
 *
 * Copyright (C) 2026 Lee Geon-goo <github.com/DS1TPT>
 *
 * This work is provided "AS IS", WITHOUT WARRANTY OF ANY KIND. You can 
 * redistribute it and/or modify it under the terms of the Do What The Fuck
 * You Want To Greater Public License, as published by Lee Geon-goo 
 * <github.com/DS1TPT>. You should have received a copy of the Do What The
 * Fuck You Want To Greater Public License. If not, see
 * <https://github.com/DS1TPT/wtfgpl> for more details.
 */

#ifndef VT100_H
#define VT100_H

/* Includes */
#include <stdio.h>
#include <stdint.h>

/* Defines */
#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE 1
#endif

#define VT_KEY_ESC 27
#define VT_KEY_BS 8
#define VT_KEY_DEL 127
#define VT_KEY_CR 13
#define VT_KEY_LF 10
/* Keycodes representing keys that generated escape sequences will have
 * 0x100 added to their last char's ASCII value.
 * This rule also apply to the keypad numeric mode, to tell between normal
 * numeric input and keypad input.
 */
#define VT_KEY_UP ((int)(0x100 + 'A'))
#define VT_KEY_DOWN ((int)(0x100 + 'B'))
#define VT_KEY_RIGHT ((int)(0x100 + 'C'))
#define VT_KEY_LEFT ((int)(0x100 + 'D'))
#define VT_KEY_PF1 ((int)(0x100 + 'P'))
#define VT_KEY_PF2 ((int)(0x100 + 'Q'))
#define VT_KEY_PF3 ((int)(0x100 + 'R'))
#define VT_KEY_PF4 ((int)(0x100 + 'S'))
#define VT_KEY_KP0 ((int)(0x100 + 'p'))
#define VT_KEY_KP1 ((int)(0x100 + 'q'))
#define VT_KEY_KP2 ((int)(0x100 + 'r'))
#define VT_KEY_KP3 ((int)(0x100 + 's'))
#define VT_KEY_KP4 ((int)(0x100 + 't'))
#define VT_KEY_KP5 ((int)(0x100 + 'u'))
#define VT_KEY_KP6 ((int)(0x100 + 'v'))
#define VT_KEY_KP7 ((int)(0x100 + 'w'))
#define VT_KEY_KP8 ((int)(0x100 + 'x'))
#define VT_KEY_KP9 ((int)(0x100 + 'y'))
#define VT_KEY_KP_DASH ((int)(0x100 + 'm'))
#define VT_KEY_KP_COMMA ((int)(0x100 + 'l'))
#define VT_KEY_KP_PERIOD ((int)(0x100 + 'n'))
#define VT_KEY_KP_ENTER ((int)(0x100 + 'M'))
#define VT_KEY_UNDEFINED ((int)0x200)

#define VT_FORMAT_MASK_BOLD       ((vt_formatBitmaskTypedef) 0x20)
#define VT_FORMAT_MASK_DIM        ((vt_formatBitmaskTypedef) 0x10)
#define VT_FORMAT_MASK_UNDERLINED ((vt_formatBitmaskTypedef) 0x8)
#define VT_FORMAT_MASK_BLINK      ((vt_formatBitmaskTypedef) 0x4)
#define VT_FORMAT_MASK_REVERSE    ((vt_formatBitmaskTypedef) 0x2)
#define VT_FORMAT_MASK_HIDDEN     ((vt_formatBitmaskTypedef) 0x1)

/* Platform-specific defines */
#ifdef _WIN32
/* Win32 system */
#include <windows.h>
#include <conio.h>
#include <fcntl.h>

#define PATH_MAX_LEN MAX_PATH

#else
/* POSIX compliant system */
#include <unistd.h>
#include <termios.h>
#include <dirent.h>
#include <limits.h>
#include <sys/select.h>
#include <time.h>

#define PATH_MAX_LEN PATH_MAX

#endif

/* Type definitions */
/* bitmasking order(7-0): X X bold dim underlined blink reverse hidden (X: not used) */
typedef uint8_t vt_formatBitmaskTypedef;

typedef int BOOL;

typedef enum {
    VT_FORMAT_RESET_ALL = 0,
    VT_FORMAT_BOLD_SET = 1,
    /* 21 is double underlined(ECMA-48) or not bold.
     * function vt_format() will print 22(Normal intensity) instead of 21. */
    VT_FORMAT_BOLD_RESET = 21,
    VT_FORMAT_DIM_SET = 2,
    VT_FORMAT_DIM_RESET = 22,
    VT_FORMAT_UNDERLINED_SET = 4,
    VT_FORMAT_UNDERLINED_RESET = 24,
    VT_FORMAT_BLINK_SET = 5,
    VT_FORMAT_BLINK_RESET = 25,
    VT_FORMAT_REVERSE_SET = 7,
    VT_FORMAT_REVERSE_RESET = 27,
    VT_FORMAT_HIDDEN_SET = 8,
    VT_FORMAT_HIDDEN_RESET = 28
} vt_formatTypedef;

typedef enum {
    VT_FG_SKIP = 0,
    VT_FG_BLACK = 30,
    VT_FG_RED = 31,
    VT_FG_GREEN = 32,
    VT_FG_YELLOW = 33,
    VT_FG_BLUE = 34,
    VT_FG_MAGENTA = 35,
    VT_FG_CYAN = 36,
    VT_FG_LIGHT_GRAY = 37,
    VT_FG_DARK_GRAY = 90,
    VT_FG_LIGHT_RED = 91,
    VT_FG_LIGHT_GREEN = 92,
    VT_FG_LIGHT_YELLOW = 93,
    VT_FG_LIGHT_BLUE = 94,
    VT_FG_LIGHT_MAGENTA = 95,
    VT_FG_LIGHT_CYAN = 96,
    VT_FG_WHITE = 97,
    VT_FG_DEFAULT = 39
} vt_fgcolorTypedef;

typedef enum {
    VT_BG_SKIP = 0,
    VT_BG_BLACK = 40,
    VT_BG_RED = 41,
    VT_BG_GREEN = 42,
    VT_BG_YELLOW = 43,
    VT_BG_BLUE = 44,
    VT_BG_MAGENTA = 45,
    VT_BG_CYAN = 46,
    VT_BG_LIGHT_GRAY = 47,
    VT_BG_DARK_GRAY = 100,
    VT_BG_LIGHT_RED = 101,
    VT_BG_LIGHT_GREEN = 102,
    VT_BG_LIGHT_YELLOW = 103,
    VT_BG_LIGHT_BLUE = 104,
    VT_BG_LIGHT_MAGENTA = 105,
    VT_BG_LIGHT_CYAN = 106,
    VT_BG_WHITE = 107,
    VT_BG_DEFAULT = 49
} vt_bgcolorTypedef;


/* Function declarations */

/* Get a key input from stdin. Returns keycode of the key pressed.
 * This function uses sleep to check if there is any remaining bytes unread
 * by getchar() to determine if input is 3B escape sequence or lone escape key input.
 */
int vt_getKeypress();

/* Reset terminal to initial state */
void vt_reset();

/* Set cursor position. X: col, Y: Row */
void vt_cursorPos(unsigned x, unsigned y);

/* Move cursor up n lines */
void vt_cursorUp(unsigned n);

/* Move cursor down n lines */
void vt_cursorDn(unsigned n);

/* Move cursor right n columns */
void vt_cursorRt(unsigned n);

/* Move cursor left n columns */
void vt_cursorLf(unsigned n);

/* Move cursor to upper left corner */
void vt_cursorHome();

/* Save cursor position and attributes */
void vt_saveCursor();

/* Restore cursor position and attributes */
void vt_restoreCursor();

/* Get cursor Position. V: bits 31-16, uint16. H: bits 15-0, uint16. */
int32_t vt_getCursor(); 

/* Move to next line */
void vt_nextLine();

/* Move/scroll window up one line */
void vt_index();

/* Move/scroll window down one line */
void vt_revIndex();

/* Clear line from cursor right */
void vt_clearEol();

/* Clear line from cursor left */
void vt_clearBol();

/* Clear a line from cursor position */
void vt_clearLine();

/* Clear screen from cursor down */
void vt_clearEos();

/* Clear screen from cursor up */
void vt_clearBos();

/* Clear entire screen */
void vt_clearScreen();

/* Set single format attribute. Use _SET to enable and _RESET to disable */
void vt_format(vt_formatTypedef attr);

/* Set multiple format attributes at once. */
void vt_formatMulti(vt_formatBitmaskTypedef attrBitmasked);

/* Get current format attribute. Same bitmask as the arg. of vt_formatMulti function */
vt_formatBitmaskTypedef vt_getFormat();

/* Set foreground and background color. Pass VT_FG_SKIP to skip setting foreground color
 * and/or VT_BG_SKIP to skip setting background color.
 */
void vt_color(vt_fgcolorTypedef fg, vt_bgcolorTypedef bg);

/* Set new line mode */
void vt_setNl();

/* Set line feed mode */
void vt_setLf();

/* Set cursor key to application */
void vt_setAppl();

/* Set cursor key to cursor */
void vt_setCursor();

/* Set number of columns to 132 */
void vt_setCol();

/* Set number of columns to 80 */
void vt_resetCol();

/* Set smooth scrolling */
void vt_setSmooth();

/* Set jump scrolling */
void vt_setJump();

/* Set reverse video on screen */
void vt_setRevScrn();

/* Set normal video on screen */
void vt_setNormScrn();

/* Set origin to relative */
void vt_setOrgRel();

/* Set origin to absolute */
void vt_setOrgAbs();

/* Set auto-wrap mode */
void vt_setWrap();

/* Reset auto-wrap mode */
void vt_resetWrap();

/* Set auto-repeat mode */
void vt_setRep();

/* Reset auto-repeat mode */
void vt_resetRep();

/* Set interlacing mode */
void vt_setInter();

/* Reset interlacing mode */
void vt_resetInter();

/* Set a tab at the current column */
void vt_tabSet();

/* Clear a tab at the current column */
void vt_tabClr();

/* Clear all tabs */
void vt_tabClrAll();

/* Enable/Disable ANSI VT support on Windows. *nix systems do not need this function */
void vt_enableAnsi(BOOL state);

/* Enable/Disable echo and canonical input mode. */
void vt_setEchoCanonical(BOOL state); 

#endif /* VT100_H */
