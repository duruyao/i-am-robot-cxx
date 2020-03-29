/**
 * @file [filename] 
 * demo of controlling Robot.
 *
 * @author  duruyao
 * @version 1.0  20-03-30
 * @update  [id] [yy/mm/dd] [name] [description] 
 */

#include <stdlib.h>

#include "robot.h"
#include "window.h"

using namespace std;

int main(int argc, char **argv) {
    // char dir[127] = "act/";
    Window win;
    Robot bot(&win, "act/");
    bot.control();

    return 0;
}

