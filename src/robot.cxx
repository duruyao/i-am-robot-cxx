/**
 * @file robot.cxx 
 * defination of 'Robot' class.
 *
 * @author  duruyao
 * @version 1.0  20-03-25
 * @update  [id] [yy-mm-dd] [author] [description] 
 */

#include "robot.h"

uint8_t Robot::id = 0;

static sem_t mutex_1;
static sem_t mutex_2;
static sem_t mutex_3;

static pthread_t super_tid;
static pthread_t sub_tid;

Robot::Robot(Window *win, char *dir) : win(win), act_dir(dir), jump(0) {
/// printf("Robot::Robot()\n"); ///
    ++this->id;
    
    char filename[127];
    char file[127] = "attribute.txt";
    memset(filename, '\0', 127);
    strcpy(filename, act_dir);
    strcat(filename, file);
    
    FILE *fd = NULL;
    char  tmp[63];
    if ((fd = fopen(filename, "r")) == NULL) {
        fprintf(stderr, "Cannot open %s\n", filename);
        exit(1);
    }
    fscanf(fd, "%s%" SCNu8 "", tmp, &(this->width));
    fscanf(fd, "%s%" SCNu8 "", tmp, &(this->height));
    fscanf(fd, "%s%" SCNu8 "", tmp, &(this->gap_x));
    fscanf(fd, "%s%" SCNu8 "", tmp, &(this->gap_y));
    fscanf(fd, "%s%" SCNu8 "", tmp, &(this->act_nb));
    fscanf(fd, "%s%d", tmp, &(this->pos.x));
    fscanf(fd, "%s%d", tmp, &(this->pos.y));
    this->init_x = this->pos.x;
    this->init_y = this->pos.y;

    this->act = new Action[act_nb];
    strcpy(file, "act_0/");
    for (int i = 0; i < act_nb; i++) {
        strcpy(filename, act_dir);
        file[4] = '0' + i;
        strcat(filename, file);
        act[i].load_step(filename, height, width);
    }

    fclose(fd);
}

Robot::~Robot() {
    --this->id;
    delete[] act;
/// printf("Robot::~Robot()\n"); ///
}

int Robot::control() {
    struct termios old_tio;
    struct termios new_tio;
    
    /* get the terminal settings for stdin */
    tcgetattr(STDIN_FILENO, &old_tio);

    /* we want to keep the old setting to restore them at the end */
    new_tio = old_tio;

    /* disable canonical mode (buffered i/o) and local echo */
    new_tio.c_lflag &= (~ICANON & ~ECHO);

    /* set the new settings immediately */
    tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);

    /* default act is 'stand' */
    stand(stdout, 0);
    
    sem_init(&mutex_1, 0, 1);
    sem_init(&mutex_2, 0, 0);
    sem_init(&mutex_3, 0, 1);

    pthread_create(&super_tid, NULL, &super_thread, (void *)this);
    pthread_create(&sub_tid, NULL, &sub_thread, (void *)this);

    pthread_join(super_tid, NULL);
    pthread_join(sub_tid,  NULL);

    sem_destroy(&mutex_1);
    sem_destroy(&mutex_2);
    sem_destroy(&mutex_3);

    /* restore the former settings */
    tcsetattr(STDIN_FILENO, TCSANOW, &old_tio);
    
    return 0;
}

void * super_thread(void *vargp) {
    char   c;
    int    cnt = 0;
    Robot *bot = (Robot *)vargp;
    do {
        sem_wait(&mutex_1); /* mutex_1: */
        if (cnt == 0) {     /* mutex_2: */
            sem_post(&mutex_2);
            cnt++;
        }
        c = getchar();
        sem_post(&mutex_1); /* mutex_1 */
        switch (c) {
            case 'w':
                bot->stand_jump(stdout, 0);
                break;
            case 'a':
                bot->walk_left(stdout, 1);
                break;
            case 's':
                break;
            case 'd':
                bot->walk_right(stdout, 2);
                break;
            default:
                break;
        }

    } while (c != 27);
    pthread_cancel(sub_tid); 
    pthread_exit(NULL);
}

void * sub_thread(void *vargp) {
    char c;
    Robot *bot = (Robot *)vargp;
    sem_wait(&mutex_2);     /* mutex_2 */
    while (1) {
        sem_wait(&mutex_1); /* mutex_1 */
        if (my_kbhit()) {
            c = getchar();
            if (c == 'w') {
                sem_wait(&mutex_3); /* mutex_3 */
                bot->jump = 1;
                sem_post(&mutex_3); /* mutex_3 */
            }
        }
        sem_post(&mutex_1); /* mutex_1 */
        usleep(500);
    }
    pthread_exit(NULL);
}

int Robot::stand(FILE *out, int idx) {
    if (this->act == NULL)
        return -1;
    return (act[idx].print_step(stdout, 0, pos, height));
}

int Robot::stand_jump(FILE *out, int idx) {
    if (this->act == NULL)
        return -1;
    sem_wait(&mutex_3); /* mutex_3 */
    pos.y = init_y;
    EARSE_N_LINE(stdout, height + pos.y);
    pos.y -= gap_y;
    act[idx].print_step(stdout, 0, pos, height);
    usleep(220000);
    EARSE_N_LINE(stdout, height + pos.y);
    pos.y = init_y;
    act[idx].print_step(stdout, 0, pos, height);
    sem_post(&mutex_3); /* mutex_3 */
    
    return 0;
}

int Robot::walk_left(FILE *out, int idx) {
    if (this->act == NULL)
        return -1;
    for (int i = 0; i < act[idx].get_st_nb(); i++) {
        sem_wait(&mutex_3); /* mutex_3 */
        EARSE_N_LINE(stdout, height + pos.y);
        if (i == 0)
            pos.y = init_y;
        else if (i == 1) {
            if (jump) pos.y = init_y - gap_y / 2;
            pos.x -= (pos.x >= 0 + gap_x / 4) * gap_x;
        }
        else if (i == 2) {
            if (jump) pos.y = init_y - gap_y;
            pos.x -= (pos.x >= 0 + gap_x / 4) * gap_x;
        }
        else if (i == 3) {
            if (jump) pos.y = init_y - gap_y;
            pos.x -= (pos.x >= 0 + gap_x / 4) * gap_x;
        }
        else if (i == 4) {
            if (jump) pos.y = init_y - gap_y / 2;
            pos.x -= (pos.x >= 0 + gap_x / 4) * gap_x;
        }
        else if (i == 5) {
            pos.y = init_y;
            jump = 0;
        }
        act[idx].print_step(stdout, i, pos, height);
        sem_post(&mutex_3); /* mutex_3 */
        if (i != act[idx].get_st_nb() - 1)
            usleep(150000);
    }

    return 0;
}

int Robot::walk_right(FILE *out, int idx) {
    if (this->act == NULL)
        return -1;
    for (int i = 0; i < act[idx].get_st_nb(); i++) {
        sem_wait(&mutex_3); /* mutex_3 */
        EARSE_N_LINE(stdout, height + pos.y);
        if (i == 0)
            pos.y = init_y;
        else if (i == 1) {
            if (jump) pos.y = init_y - gap_y / 2;
            pos.x += (pos.x + gap_x / 4 < win->col - 1 - width) * gap_x;
        }
        else if (i == 2) {
            if (jump) pos.y = init_y - gap_y;
            pos.x += (pos.x + gap_x / 4 < win->col - 1 - width) * gap_x;
        }
        else if (i == 3) {
            if (jump) pos.y = init_y - gap_y;
            pos.x += (pos.x + gap_x / 4 < win->col - 1 - width) * gap_x;
        }
        else if (i == 4) {
            if (jump) pos.y = init_y - gap_y / 2;
            pos.x += (pos.x + gap_x / 4 < win->col - 1 - width) * gap_x;
        }
        else if (i == 5) {
            pos.y = init_y;
            jump = 0;
        }
        act[idx].print_step(stdout, i, pos, height);
        sem_post(&mutex_3); /* mutex_3 */
        if (i != act[idx].get_st_nb() - 1)
            usleep(150000);
    }
    
    return 0; 
}

