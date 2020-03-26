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

#include "func.h"

using namespace std;

class Step {
private:
    char  **str;
    uint8_t row;
    uint8_t col;

public:
    Step();
    ~Step();
    int load_str(char *, uint8_t, uint8_t);
    int print_str(FILE *, Position, uint8_t);
};

#endif /* !__STEPH__ */

