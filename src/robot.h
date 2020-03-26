/**
 * @file robot.h 
 * declaration of 'Robot' class.
 *
 * @author  duruyao
 * @version 1.0  20-03-25
 * @update  [id] [yy-mm-dd] [author] [description] 
 */

#ifndef __ROBOTH__
#define __ROBOTH__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>

#include <unistd.h>
#include <termios.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/ioctl.h>

#include "window.h"
#include "action.h"
#include "step.h"
#include "func.h"

using namespace std;

class Robot {
private:
    static uint8_t id;
    char  *act_dir;
    
    int jump;
    int init_x;
    int init_y;

    uint8_t width;
    uint8_t height;
    uint8_t gap_x;
    uint8_t gap_y;
    uint8_t act_nb;

    Position pos;
    Action  *act;
    Window  *win;
    
    int stand(FILE *, int);
    int stand_jump(FILE *, int);
    int walk_left(FILE *, int);
    int walk_right(FILE *, int);

public:
    Robot(Window *, char *);
    ~Robot();
    int control();
    friend void *super_thread(void *);
    friend void *sub_thread(void *);
};

void *super_thread(void *);

void *sub_thread(void *);

#endif /* !__ROBOTH__ */

