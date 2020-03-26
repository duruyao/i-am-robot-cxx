/**
 * @file window.h
 * defination of 'Window' class. 
 *
 * @author  duruyao
 * @version 1.0  20-03-25
 * @update  [id] [yy-mm-dd] [author] [description] 
 */

#ifndef __WINDOWH__
#define __WINDOWH__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>

using namespace std;

class Window {
public:
    uint8_t row;
    uint8_t col;

    Window();
    ~Window();
};

#endif /* !__WINDOWH__ */

