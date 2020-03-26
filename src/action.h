/**
 * @file action.h 
 * declaration of 'Action' class.
 *
 * @author  duruyao
 * @version 1.0  20-03-25
 * @update  [id] [yy-mm-dd] [author] [description] 
 */

#ifndef __ACTIONH__
#define __ACTIONH__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>

#include "step.h"
#include "func.h"

using namespace std;

class Robot;

class Action {
private:
    char   *step_dir;
    char    name[127];
    char    key;
    uint8_t st_nb;
    Step   *step;

public:
    Action();
    ~Action();
    uint8_t get_st_nb();
    int load_step(char *, uint8_t, uint8_t);
    int print_step(FILE *, int, Position, uint8_t);
};

#endif /* !__ACTIONH__ */

