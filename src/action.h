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

class Action {
private:
    char    dir[255];
    char    name[255];
    char    key;
    uint8_t st_nb;
    Step   *step;

public:
    Action();
    Action(const char *);
    Action(const Action &);
    Action & operator=(const Action &);
    virtual ~Action();

    inline uint8_t get_st_nb() const;
    int load();
    int print_step(FILE *, int, Position) const;
};

inline uint8_t Action::get_st_nb() const {
    return st_nb;
}

#endif /* !__ACTIONH__ */

