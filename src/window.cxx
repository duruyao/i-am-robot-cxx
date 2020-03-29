/**
 * @file window.cxx 
 * defination of 'Window' class.
 *
 * @author  duruyao
 * @version 1.0  20-03-25
 * @update  [id] [yy/mm/dd] [name] [description] 
 */

#include "window.h"

Window::Window() : row(0), col(0) {
    struct winsize term;
    /* get size of current terminal */
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &term);
    row = term.ws_row;
    col = term.ws_col;
}

Window::Window(const Window &that) {
    row = that.row;
    col = that.col;
}

Window & Window::operator=(const Window &that) {
    if (this == &that) return *this;
    row = that.row;
    col = that.col;
    return *this;
}

Window::~Window() {

}

