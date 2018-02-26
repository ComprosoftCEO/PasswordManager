#ifndef ESCAPE_CODES_HEADER
#define ESCAPE_CODES_HEADER

#include <Platform.h>

#define ESC_TERMINATOR (-1)


//Escape codes for Linux vs. Windows platforms
#if defined(PLAT_LINUX)

    #define ESC_UP_ARROW      {27,91,65,  ESC_TERMINATOR}
    #define ESC_DOWN_ARROW    {27,91,66,  ESC_TERMINATOR}
    #define ESC_LEFT_ARROW    {27,91,68,  ESC_TERMINATOR}
    #define ESC_RIGHT_ARROW   {27,91,67,  ESC_TERMINATOR}
    #define ESC_ENTER         {10,  ESC_TERMINATOR}
    #define ESC_BACKSPACE     {127, ESC_TERMINATOR}
    #define ESC_DELETE        {27,91,51,126, ESC_TERMINATOR}
    #define ESC_TAB           {9,   ESC_TERMINATOR}
    #define ESC_ESCAPE        {27,  ESC_TERMINATOR}
    #define ESC_HOME          {27,91,72,  ESC_TERMINATOR}
    #define ESC_END           {27,91,70,  ESC_TERMINATOR}
    #define ESC_PAGE_UP       {27,91,53,126,  ESC_TERMINATOR}
    #define ESC_PAGE_DOWN     {27,91,54,126,  ESC_TERMINATOR}
    #define ESC_INSERT        {27,91,50,126,  ESC_TERMINATOR}



#elif defined(PLAT_WINDOWS)

    #define ESC_UP_ARROW      {224,72,  ESC_TERMINATOR}
    #define ESC_DOWN_ARROW    {224,80,  ESC_TERMINATOR}
    #define ESC_LEFT_ARROW    {224,75,  ESC_TERMINATOR}
    #define ESC_RIGHT_ARROW   {224,77,  ESC_TERMINATOR}
    #define ESC_ENTER         {13,  ESC_TERMINATOR}
    #define ESC_BACKSPACE     {8,   ESC_TERMINATOR}
    #define ESC_DELETE        {224,83,  ESC_TERMINATOR}
    #define ESC_TAB           {9,   ESC_TERMINATOR}
    #define ESC_ESCAPE        {27,  ESC_TERMINATOR}
    #define ESC_HOME          {224,71,  ESC_TERMINATOR}
    #define ESC_END           {224,79,  ESC_TERMINATOR}
    #define ESC_PAGE_UP       {224,73,  ESC_TERMINATOR}
    #define ESC_PAGE_DOWN     {224,81,  ESC_TERMINATOR}
    #define ESC_INSERT        {224,82,  ESC_TERMINATOR}

#endif

#endif // ESCAPE_CODES_HEADER Included
