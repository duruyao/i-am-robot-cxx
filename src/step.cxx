/**
 * @file step.cxx
 * defination of 'Step' class.
 *
 * @author  duruyao
 * @version 1.0  20-03-30
 * @update  [id] [yy/mm/dd] [name] [description] 
 */

#include "step.h"

Step::Step() :
    dir("\0"), str(NULL), row(0), col(0) {
    memset(dir, '\0', 255);
}

Step::Step(const char *dir) :
    dir("\0"), str(NULL), row(0), col(0) {
    int ret;

    memset(this->dir, '\0', 255);
    memcpy(this->dir, dir, 255);
    ret = load();
    if (ret < 0) exit(1);
}

Step::Step(const Step &that) {
    memset(dir, '\0', 255);
    memcpy(dir, that.dir, 255);
    row = that.row;
    col = that.col;
    
    str = ALLOC(row, char *);
    if (str == NULL) {
        fprintf(stderr, "Allocate memory error\n");
        exit(1);
    }
    for (int i = 0; i < row; i++) {
        str[i] = ALLOC(col + 1, char);
        memset(str[i], '\0', col + 1);
    }

    for (int i = 0; i < row; i++)
        memcpy(str[i], that.str[i], col + 1);
}

Step & Step::operator=(const Step &that) {
    if (this == &that) return *this;
    FREE(row, str);
    
    memset(dir, '\0', 255);
    memcpy(dir, that.dir, 255);
    row = that.row;
    col = that.col;

    str = ALLOC(row, char *);
    if (str == NULL) {
        fprintf(stderr, "Allocate memory error\n");
        exit(1);
    }
    for (int i = 0; i < row; i++) {
        str[i] = ALLOC(col + 1, char);
        memset(str[i], '\0', col + 1);
    }

    for (int i = 0; i < row; i++)
        memcpy(str[i], that.str[i], col + 1);
}

Step::~Step() {
    FREE(row, str);
}

int Step::load() {
    FILE *fd = NULL;
    char  tmp[63];

    if (dir == NULL) return -1;
    FREE(row, str);

    fd = fopen(dir, "r");
    if (fd == NULL) {
        fprintf(stderr, "Cannot open %s\n", dir);
        return -1;
    }
    fscanf(fd, "%s%" SCNu8 "", tmp, &row);
    fscanf(fd, "%s%" SCNu8 "", tmp, &col);
    fgets(tmp, 63, fd); /* get a '\n' from stdin */

    str = ALLOC(row, char *);
    if (str == NULL) {
        fprintf(stderr, "Allocate memory error\n");
        return -1;
    }
    for (int i = 0; i < row; i++) {
        str[i] = ALLOC(col + 1, char);
        memset(str[i], '\0', col + 1);
    }

    for (int i = 0; i < row; i++)
        fgets(str[i], col + 1, fd);
    fclose(fd);

    return 0;
}

int Step::print(FILE *out, Position pos) const {
    uint8_t x = pos.x;
    uint8_t y = pos.y;
    
    if (str == NULL) return -1;
    while (y--) fprintf(out, "\n");

    for (int i = 0; i < row; i++) {
        for (int j = 0; j < x; fprintf(out, " "), j++);
        if (str[i] == NULL) return -1;
        fprintf(out, "%s", str[i]);
    }

    return 0;
}

