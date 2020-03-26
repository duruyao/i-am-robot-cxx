/**
 * @file step.cxx
 *
 * [description of the content]
 *
 * @author  duruyao
 * @version 1.0  [yy/mm/dd]
 * @update  [id] [yy/mm/dd] [name] [description] 
 */

#include "step.h"

Step::Step() : str(NULL) {
/// printf("Step::Step()\n"); ///
}

Step::~Step() {
    FREE((this->row), (this->str));
/// printf("Step::~Step()\n"); ///
}

int Step::load_str(char *dir, uint8_t row, uint8_t col) {
    this->row = row;
    this->col = col;
    if ((this->str = ALLOC(this->row, char *)) == NULL) {
        fprintf(stderr, "Allocate memory error\n");
        exit(1);
    }
    for (int i = 0; i < this->row; i++) {
        if ((this->str[i] = ALLOC(this->col + 1, char)) == NULL) {
            fprintf(stderr, "Allocate memory error\n");
            exit(1);
        }
        memset(str[i], '\0', this->col + 1);
    }

    FILE *fd = NULL;
    if ((fd = fopen(dir, "r")) == NULL) {
        fprintf(stderr, "Cannot open %s\n", dir);
        exit(1);
    }
    for (int i = 0; i < this->row; i++)
        fgets(str[i], this->col + 1, fd);
    
    fclose(fd);
}

int Step::print_str(FILE *out, Position pos, uint8_t height) {
    uint8_t x = pos.x;
    uint8_t y = pos.y;
    
    if (this->str == NULL)
        return -1;
    
    while (y--)
        fprintf(out, "\n");
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < x; fprintf(out, " "), j++);
        if (str[i] == NULL)
            return -1;
        fprintf(out, "%s", str[i]);
    }
    return 0;
}

