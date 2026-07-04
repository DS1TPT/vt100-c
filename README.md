# VT100-C
ANSI VT100 Escape Sequences, C language  

Supported Environment
-----------------------
- POSIX compliant OS
- MS Windows 10 Version 1511 and later (Windows does not have full ANSI terminal support before ver. 1511)

Exported typedefs
------------------
- vt_formatBitmaskTypedef (uint8_t, X X Bold Dim Underlined Blink Reverse Hidden, where X: not used)
- BOOL (int)
- vt_formatTypedef (enum)
- vt_fgcolorTypedef (enum)
- vt_bgcolorTypedef (enum)
  
List of non-VT100 functions
----------------------------
- int *vt_getKeypress(): Use this function to get a keystroke from terminal. This funcion uses sleep to check if there is any remaining bytes unread by getchar() to check if lone escape key or multi-byte escape sequences. This function returns VT_KEY_UNDEFINED if an undefined escape sequence has been input.
- vt_enableAnsi(BOOL state): Enable/Disable ANSI VT support on Windows. POSIX systems do not need to run this funciton.
- vt_setEchoCanonical(BOOL state): Enable/Disable echo and canonical input mode. This function implies setvbuf(stdin, NULL, ?1, 0), where ?1 is _IOLBF if state is TRUE and _IONBF if state is FALSE.

Includes
---------
On POSIX compliant OS, header of vt100-c includes
- unistd.h
- termios.h
- dirent.h
- limits.h
- sys/select.h
- time.h

On Windows, header of vt100-c includes
- windows.h
- conio.h
- fcntl.h
  
