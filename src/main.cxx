/**
 * @file [filename] 
 * [description of the content]
 *
 * @author  duruyao
 * @version 1.0  [yy/mm/dd]
 * @update  [id] [yy/mm/dd] [name] [description] 
 */

#include <stdlib.h>

#include "robot.h"
#include "window.h"

using namespace std;

int main(int argc, char **argv) {
    char dir[127] = "act/";
    Window win;
    Robot bot(&win, dir);
    bot.control();

    return 0;
}

