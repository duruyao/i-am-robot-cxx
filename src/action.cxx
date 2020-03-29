/**
 * @file action.cxx 
 * defination of 'Action' class.
 *
 * @author  duruyao
 * @version 1.0  20-03-25
 * @update  [id] [yy/mm/dd] [name] [description] 
 */

#include "action.h"

Action::Action() :
    dir("\0"), name("\0"), key(0), st_nb(0), step(NULL) {
    memset(dir,  '\0', 255);
    memset(name, '\0', 255);
}

Action::Action(const char *dir) :
    dir("\0"), name("\0"), key(0), st_nb(0), step(NULL) {
    int ret;

    memset(this->dir,  '\0', 255);
    memset(name, '\0', 255);
    memcpy(this->dir,  dir,  255);
    ret = load();
    if (ret < 0) exit(1);
}

Action::Action(const Action &that) {
    char filename[255];
    char file[255];
    memset(filename, '\0', 255);
    memset(file, '\0', 255);
     
    memset(dir,  '\0', 255);
    memset(name, '\0', 255);
    memcpy(dir,  that.dir,  255);
    memcpy(name, that.name, 255);
    key   = that.key;
    st_nb = that.st_nb;
    step  = NULL;

    step = new Step[st_nb];
    if (step == NULL) {
        fprintf(stderr, "Allocate memory error\n");
        exit(1);
    }
    for (int i = 0; i < st_nb; i++) {
        strcpy(filename, dir);
        snprintf(file, 255, "step_%d.txt", i);
        strcat(filename, file);
        step[i] = Step(filename);
    }
}

Action & Action::operator=(const Action &that) {
    char filename[255];
    char file[255];
    memset(filename, '\0', 255);
    memset(file, '\0', 255);
     
    if (this == &that) return *this;
    if (step) delete[] step;

    memset(dir,  '\0', 255);
    memset(name, '\0', 255);
    memcpy(dir,  that.dir,  255);
    memcpy(name, that.name, 255);
    key   = that.key;
    st_nb = that.st_nb;
    step  = NULL;

    step = new Step[st_nb];
    if (step == NULL) {
        fprintf(stderr, "Allocate memory error\n");
        exit(1);
    }
    for (int i = 0; i < st_nb; i++) {
        strcpy(filename, dir);
        snprintf(file, 255, "step_%d.txt", i);
        strcat(filename, file);
        step[i] = Step(filename);
    }

    return *this;
}

Action::~Action() {
    if (step) delete[] step;
}

int Action::load() {
    FILE *fd = NULL;
    char  tmp[63];
    char  filename[255];
    char  file[255] = "attribute.txt";
    memset(filename, '\0', 255);
    memcpy(filename, dir, 255);
    strcat(filename, file);
    
    fd = fopen(filename, "r");
    if (fd == NULL) {
        fprintf(stderr, "Cannot open %s\n", filename);
        return -1;
    }
    fscanf(fd, "%s%s", tmp, name);
    fscanf(fd, "%s%" SCNu8 "", tmp, &key);
    fscanf(fd, "%s%" SCNu8 "", tmp, &st_nb);
    fclose(fd);
    
    step = new Step[st_nb];
    if (step == NULL) {
        fprintf(stderr, "Allocate memory error\n");
        return -1;
    }
    for (int i = 0; i < st_nb; i++) {
        strcpy(filename, dir);
        snprintf(file, 255, "step_%d.txt", i);
        strcat(filename, file);
        step[i] = Step(filename);
    }
    return 0;
}

int Action::print_step(FILE *out, int st_idx, Position pos) const {
    if (step == NULL) return -1;
    return (step[st_idx].print(out, pos));
}

