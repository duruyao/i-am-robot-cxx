/**
 * @file window.cxx 
 * defination of 'Window' class.
 *
 * @author  duruyao
 * @version 1.0  20-03-25
 * @update  [id] [yy/mm/dd] [name] [description] 
 */

#include "window.h"

Window::Window() : row(0U), col(0U) {
/// printf("Window::Window()\n"); ///
    struct winsize term;

    /* get size of current terminal */
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &term);
    this->col = term.ws_col;
}

Window::~Window() {
/// printf("Window::~Window()\n"); ///
}

