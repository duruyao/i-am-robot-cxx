/**
 * @file step.h 
 * declaration of 'Step' class.
 *
 * @author  duruyao
 * @version 1.0  20-03-25
 * @update  [id] [yy-mm-dd] [author] [description] 
 */

#ifndef __STEPH__
#define __STEPH__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>

#include "func.h"

using namespace std;

class Step {
private:
    char    dir[255];
    char  **str;
    uint8_t row;
    uint8_t col;

public:
    Step();
    Step(const char *);
    Step(const Step &);
    Step & operator=(const Step &);
    virtual ~Step();

    int load();
    int print(FILE *, Position) const;
};

#endif /* !__STEPH__ */

