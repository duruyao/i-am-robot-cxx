/**
 * @file main.c 
 * main controler of 'robot'.
 *
 * @author  duruyao
 * @version 1.0  20-03-21
 * @update  [id] [yy-mm-dd] [author] [description] 
 */

#include <time.h>
#include <math.h>
#include <stdio.h>
#include <float.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>

#include "robot.h"

#define ACT_NB      3

Act         act[ACT_NB];
char        cmd[ACT_NB]     = {'\0', 'a', 'd'};
uint8_t     step_nb[ACT_NB] = {1, 3, 3};
const char *folder[ACT_NB];

int main(int argc, char **argv) {
    int  ret;
    Pos  pos = {0, 0};
    Bot *bot = NULL;

    folder[0] = "./act/stand/";
    folder[1] = "./act/walk_left/";
    folder[2] = "./act/walk_right/";
    
    ret = act_load(act, ACT_NB, cmd, step_nb, folder);
    if (ret < 0)
        return -1;
    
    create_bot(&bot);
    bot->init(bot, 1, 26, 16, 3, act, pos);
    bot->ctrl(bot);
    delete_bot(&bot);

    act_unload(act, ACT_NB);
    return 0;
}

