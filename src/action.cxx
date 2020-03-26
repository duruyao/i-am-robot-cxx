/**
 * @file action.cxx 
 * defination of 'Action' class.
 *
 * @author  duruyao
 * @version 1.0  20-03-25
 * @update  [id] [yy/mm/dd] [name] [description] 
 */

#include "action.h"

Action::Action() : step_dir(NULL) {
/// printf("Action::Action()\n"); ///
    memset(name, '\0', 127);
}

Action::~Action() {
    delete[] this->step;
/// printf("Action::~Action()\n"); ///
}

uint8_t Action::get_st_nb() {
    return this->st_nb;
}

int Action::load_step(char *dir, uint8_t row, uint8_t col) {
    this->step_dir = dir;

    char filename[127];
    char file[127] = "attribute.txt";
    memset(filename, '\0', 127);
    strcpy(filename, step_dir);
    strcat(filename, file);
    
    FILE *fd = NULL;
    char  tmp[63];
    if ((fd = fopen(filename, "r")) == NULL) {
        fprintf(stderr, "Cannot open %s\n", filename);
        exit(1);
    }
    fscanf(fd, "%s%s", tmp, this->name);
    fscanf(fd, "%s%" SCNu8 "", tmp, &(this->key));
    fscanf(fd, "%s%" SCNu8 "", tmp, &(this->st_nb));
    
    this->step = new Step[st_nb];
    strcpy(file, "step_0.txt");
    for (int i = 0; i < st_nb; i++) {
        strcpy(filename, step_dir);
        file[5] = '0' + i;
        strcat(filename, file);
        step[i].load_str(filename, row, col);
    }

    fclose(fd);
}

int Action::print_step(FILE *out, int st_idx, Position pos, uint8_t height) {
    if (this->step == NULL)
        return -1;
    return (this->step[st_idx].print_str(out, pos, height));
}

