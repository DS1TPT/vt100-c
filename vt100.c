/*
 * File name: vt100.c
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


#include "vt100.h"

/* Global variables */
vt_formatBitmaskTypedef g_lastFormatAttr = 0;


/* Function implementations */

static int sleepms(unsigned ms)
{
#ifdef _WIN32
    Sleep(ms);
#else
    struct timespec ts;
    ts.tv_sec = ms / 1000;
    ts.tv_nsec = (ms % 1000) * 1000000;
    nanosleep(&ts, NULL);
#endif
}

static int chkRemainingInput()
{
#ifdef _WIN32
    return _kbhit() != 0;
#else
    fd_set fds;
    struct timeval tv = { 0, 0 };
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    return select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv) > 0;
#endif
}

int vt_getKeypress() 
{
    int ch;
    ch = getchar();
    if (ch == '\033') { /* Escape: Sequence or Single keypress? */
        sleepms(50);
        if (chkRemainingInput()) {
            if (getchar() == '[') {
                return getchar();
            }
        } else {
            return VT_KEY_ESC;
        }
    } else {
        return ch;
    }
}

void vt_reset() { printf("\033c"); }

void vt_cursorPos(unsigned x, unsigned y) { printf("\033[%u;%uf", y, x); }

void vt_cursorUp(unsigned n) { printf("\033[%uA", n); }

void vt_cursorDn(unsigned n) { printf("\033[%uB", n); }

void vt_cursorRt(unsigned n) { printf("\033[%uC", n); }

void vt_cursorLf(unsigned n) { printf("\033[%uD", n); }

void vt_cursorHome() { printf("\033[H"); }

void vt_saveCursor() { printf("\0337"); }

void vt_restoreCursor() { printf("\0338"); }

int32_t vt_getCursor()
{
    uint16_t row, col, pos;
    printf("\033[6n");
    scanf("\033[%0u;%0uR", &row, &col);
    pos = ((int32_t)row << 16) + col;
    return pos;
}

void vt_nextLine() { printf("\033E"); }

void vt_index() { printf("\033D"); }

void vt_revIndex() { printf("\033M"); }

void vt_clearEol() { printf("\033[0K"); }

void vt_clearBol() { printf("\033[1K"); }

void vt_clearLine() { printf("\033[2K"); }

void vt_clearEos() { printf("\033[0J"); }

void vt_clearBos() { printf("\033[1J"); }

void vt_clearScreen() { printf("\033[2J"); }

void vt_format(vt_formatTypedef attr) 
{
    if (attr == VT_FORMAT_BOLD_RESET) {
        printf("\033[%um", VT_FORMAT_DIM_RESET);
    } else {
        printf("\033[%um", attr); 
    }

    switch (attr) {
    case VT_FORMAT_RESET_ALL:
        g_lastFormatAttr = 0;
        break;
    case VT_FORMAT_BOLD_SET:
        g_lastFormatAttr = g_lastFormatAttr | VT_FORMAT_MASK_BOLD;
        break;
    case VT_FORMAT_BOLD_RESET:
        g_lastFormatAttr = g_lastFormatAttr & ~VT_FORMAT_MASK_BOLD;
        break;
    case VT_FORMAT_DIM_SET:
        g_lastFormatAttr = g_lastFormatAttr | VT_FORMAT_MASK_DIM;
        break;
    case VT_FORMAT_DIM_RESET:
        g_lastFormatAttr = g_lastFormatAttr & ~VT_FORMAT_MASK_DIM;
        break;
    case VT_FORMAT_UNDERLINED_SET:
        g_lastFormatAttr = g_lastFormatAttr | VT_FORMAT_MASK_UNDERLINED;
        break;
    case VT_FORMAT_UNDERLINED_RESET:
        g_lastFormatAttr = g_lastFormatAttr & ~VT_FORMAT_MASK_UNDERLINED;
        break;
    case VT_FORMAT_BLINK_SET:
        g_lastFormatAttr = g_lastFormatAttr | VT_FORMAT_MASK_BLINK;
        break;
    case VT_FORMAT_BLINK_RESET:
        g_lastFormatAttr = g_lastFormatAttr & ~VT_FORMAT_MASK_BLINK;
        break;
    case VT_FORMAT_REVERSE_SET:
        g_lastFormatAttr = g_lastFormatAttr | VT_FORMAT_MASK_REVERSE;
        break;
    case VT_FORMAT_REVERSE_RESET:
        g_lastFormatAttr = g_lastFormatAttr & ~VT_FORMAT_MASK_REVERSE;
        break;
    case VT_FORMAT_HIDDEN_SET:
        g_lastFormatAttr = g_lastFormatAttr | VT_FORMAT_MASK_HIDDEN;
        break;
    case VT_FORMAT_HIDDEN_RESET:
        g_lastFormatAttr = g_lastFormatAttr & ~VT_FORMAT_MASK_HIDDEN;
    }
}

void vt_formatMulti(vt_formatBitmaskTypedef attrBitmasked)
{
    printf("\033[%u;%u;%u;%u;%u;%um", 
           ((attrBitmasked & VT_FORMAT_MASK_BOLD) ? VT_FORMAT_BOLD_SET : VT_FORMAT_BOLD_RESET),
           ((attrBitmasked & VT_FORMAT_MASK_DIM) ? VT_FORMAT_DIM_SET : VT_FORMAT_DIM_RESET),
           ((attrBitmasked & VT_FORMAT_MASK_UNDERLINED) ? VT_FORMAT_UNDERLINED_SET : VT_FORMAT_UNDERLINED_RESET),
           ((attrBitmasked & VT_FORMAT_MASK_BLINK) ? VT_FORMAT_BLINK_SET : VT_FORMAT_BLINK_RESET),
           ((attrBitmasked & VT_FORMAT_MASK_REVERSE) ? VT_FORMAT_REVERSE_SET : VT_FORMAT_REVERSE_RESET),
           ((attrBitmasked & VT_FORMAT_MASK_HIDDEN) ? VT_FORMAT_HIDDEN_SET : VT_FORMAT_HIDDEN_RESET) );
    g_lastFormatAttr = attrBitmasked;
}

vt_formatBitmaskTypedef vt_getformat() { return g_lastFormatAttr; }

void vt_color(vt_fgcolorTypedef fg, vt_bgcolorTypedef bg)
{
    if (fg != VT_FG_SKIP && bg != VT_BG_SKIP) {
        printf("\033[%u;%um", fg, bg);
    } else if (fg != VT_FG_SKIP && bg == VT_BG_SKIP) {
        printf("\033[%um", fg);
    } else if (fg == VT_FG_SKIP && bg != VT_BG_SKIP) {
        printf("\033[%um", bg);
    }
}

void vt_setNl() { printf("\033[20h"); }

void vt_setLf() { printf("\033[20l"); }

void vt_setAppl() { printf("\033[?1h"); }

void vt_setCursor() { printf("\033[?1l"); }

void vt_setCol() { printf("\033[?3h"); }

void vt_resetCol() { printf("\033[?3l"); }

void vt_setSmooth() { printf("\033[?4h"); }

void vt_setJump() { printf("\033[?4l"); }

void vt_setRevScrn() { printf("\033[?5h"); }

void vt_setNormScrn() { printf("\033[?5l"); }

void vt_setOrgRel() { printf("\033[?6h"); }

void vt_setOrgAbs() { printf("\033[?6l"); }

void vt_setWrap() { printf("\033[?7h"); }

void vt_resetWrap() { printf("\033[?7l"); }

void vt_setRep() { printf("\033[?8h"); }

void vt_resetRep() { printf("\033[?8l"); }

void vt_setInter() { printf("\033[?9h"); }

void vt_resetInter() { printf("\033[?9l"); }

void vt_tabSet() { printf("\033H"); }

void vt_tabClr() { printf("\033[0g"); }

void vt_tabClrAll() { printf("\033[3g"); }

#ifdef _WIN32
void vt_enableAnsi(BOOL state)
{
    SetConsoleOutputCP(CP_UTF8); /* not related to ansi but enabling utf-8 code page is necessary */
    _setmode(_fileno(stdin), _O_BINARY); /* also not related, but necessary to handle enter */

    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);

    DWORD dwOutMode = 0, dwInMode = 0;
    if (state) {
        GetConsoleMode(hOut, &dwOutMode);
        dwOutMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        SetConsoleMode(hOut, dwOutMode);

        GetConsoleMode(hIn, &dwInMode);
        dwInMode |= ENABLE_VIRTUAL_TERMINAL_INPUT;
        SetConsoleMode(hIn, dwInMode);
    } else {
        GetConsoleMode(hOut, &dwOutMode);
        dwOutMode &= ~ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        SetConsoleMode(hOut, dwOutMode);

        GetConsoleMode(hIn, &dwInMode);
        dwInMode &= ~ENABLE_VIRTUAL_TERMINAL_INPUT;
        SetConsoleMode(hIn, dwInMode);
    }
}

void vt_setEchoCanonical(BOOL state) /* enable/disable echo and canonical input mode */
{
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode;
    GetConsoleMode(hStdin, &mode);
    if (state) {
        mode |= (ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT);
        setvbuf(stdin, NULL, _IOLBF, 0);
    } else {
        mode &= ~(ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT);
        setvbuf(stdin, NULL, _IONBF, 0);
    }
    SetConsoleMode(hStdin, mode);
}

#else

void vt_enableAnsi(BOOL state) {}

void vt_setEchoCanonical(BOOL state)  /* enable/disable echo and canonical input mode */
{
    static struct termios Orig, New;
    static BOOL isFirstRun = TRUE;

    if (isFirstRun == TRUE) {
        tcgetattr(STDIN_FILENO, &Orig);
        New = Orig;
        New.c_lflag &= ~(ICANON | ECHO);
        isFirstRun = FALSE;
    }
    
    if (state) {
        tcsetattr(STDIN_FILENO, TCSANOW, &Orig);
        setvbuf(stdin, NULL, _IOLBF, 0);
    } else {
        tcsetattr(STDIN_FILENO, TCSANOW, &New);
        setvbuf(stdin, NULL, _IONBF, 0);
    }
}

#endif
