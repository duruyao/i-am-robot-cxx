/**
 * @file main.c 
 * main controler of 'robot'.
 *
 * @author  duruyao
 * @version 1.0  20-03-21
 * @update  [id] [yy-mm-dd] [author] [description] 
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

#include "robot.h"

#define ACT_NB   3
#define WIDTH   27
#define HEIGHT  16

Act         act[ACT_NB];
char        cmd[ACT_NB]     = {'\0', 'a', 'd'};
uint8_t     step_nb[ACT_NB] = {1, 6, 6};
const char *folder[ACT_NB];

int main(int argc, char **argv) {
    int  ret;
    Pos  pos = {8, 4};
    Bot *bot = NULL;

    folder[0] = "./act/stand/";
    folder[1] = "./act/walk_left/";
    folder[2] = "./act/walk_right/";
    
    ret = load_act(act, ACT_NB, WIDTH, HEIGHT, cmd, step_nb, folder);
    if (ret < 0)
        return -1;
    
    create_bot(&bot);
    bot->init(bot, 1, WIDTH, HEIGHT, 3, act, pos);
    bot->ctrl(bot);
    delete_bot(&bot);

    unload_act(act, ACT_NB);
    return 0;
}

