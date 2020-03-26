/**
 * @file func.c 
 * defination of some algorithm.
 *
 * @author  duruyao
 * @version 1.0  20-03-26
 * @update  [id] [yy-mm-dd] [author] [description] 
 */

#include "func.h"

/******************************************************************************/
/* keyboard                                                                   */
/******************************************************************************/

/*
 * unblock listening event of keyboard.
 *
 * @return  1 on pressing any key, otherwise 0;
 *
 */

int my_kbhit(void) {
    int ch;
    int oldf;
    struct termios oldt;
    struct termios newt;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    if(ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }
    return 0;
}

