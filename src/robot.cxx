/**
 * @file robot.cxx 
 * defination of 'Robot' class.
 *
 * @author  duruyao
 * @version 1.0  20-03-25
 * @update  [id] [yy-mm-dd] [author] [description] 
 */

#include "robot.h"

uint8_t Robot::cnt = 0;

static sem_t mutex_1;
static sem_t mutex_2;
static sem_t mutex_3;

static pthread_t super_tid;
static pthread_t sub_tid;

Robot::Robot() :
    jump(0), init_x(0), init_y(0), dir("\0"), width(0), height(0),
    gap_x(0), gap_y(0), act_nb(0), pos({0, 0}), act(NULL), win(NULL) {
    ++cnt;
}

Robot::Robot(Window *win, const char *dir) :
    jump(0), init_x(0), init_y(0), dir("\0"), width(0), height(0),
    gap_x(0), gap_y(0), act_nb(0), pos({0, 0}), act(NULL), win(win) {
    int ret;

    ++cnt;
    memset(this->dir, '\0', 255);
    memcpy(this->dir,  dir, 255);
    ret = load();
    if (ret < 0) exit(1);
}

Robot::Robot(const Robot &that) {
    char  filename[255];
    char  file[255];
    memset(filename, '\0', 255);
    memset(file, '\0', 255);

    ++cnt;
    jump   = that.jump;
    init_x = that.init_x;
    init_y = that.init_y;
    memset(dir, '\0', 255);
    memcpy(dir, that.dir, 255);
    width  = that.width;
    height = that.height;
    gap_x  = that.gap_x;
    gap_y  = that.gap_y;
    act_nb = that.act_nb;
    pos    = that.pos;
    win    = that.win;

    act = new Action[act_nb];
    if (act = NULL) {
        fprintf(stderr, "Allocate memory error\n");
        exit(1);
    }
    for (int i = 0; i < act_nb; i++) {
        strcpy(filename, dir);
        snprintf(file, 255, "act_%d/", i);
        strcat(filename, file);
        act[i] = Action(filename);
    }
}

Robot & Robot::operator=(const Robot &that) {
    char  filename[255];
    char  file[255];
    memset(filename, '\0', 255);
    memset(file, '\0', 255);

    if (this == &that) return *this;
    if (act) delete[] act;

    jump   = that.jump;
    init_x = that.init_x;
    init_y = that.init_y;
    memset(dir, '\0', 255);
    memcpy(dir, that.dir, 255);
    width  = that.width;
    height = that.height;
    gap_x  = that.gap_x;
    gap_y  = that.gap_y;
    act_nb = that.act_nb;
    pos    = that.pos;
    win    = that.win;

    act = new Action[act_nb];
    if (act == NULL) {
        fprintf(stderr, "Allocate memory error\n");
        exit(1);
    }
    for (int i = 0; i < act_nb; i++) {
        strcpy(filename, dir);
        snprintf(file, 255, "act_%d/", i);
        strcat(filename, file);
        act[i] = Action(filename);
    }

    return *this;
}

Robot::~Robot() {
    --cnt;
    if (act) delete[] act;
}

int Robot::load() {
    FILE *fd = NULL;
    char  tmp[63];
    char  filename[255];
    char  file[255] = "attribute.txt";
    memset(filename, '\0', 255);
    strcpy(filename, dir);
    strcat(filename, file);
    
    fd = fopen(filename, "r");
    if (fd == NULL) {
        fprintf(stderr, "Cannot open %s\n", filename);
        return -1;
    }
    fscanf(fd, "%s%" SCNu8 "", tmp, &width);
    fscanf(fd, "%s%" SCNu8 "", tmp, &height);
    fscanf(fd, "%s%" SCNu8 "", tmp, &gap_x);
    fscanf(fd, "%s%" SCNu8 "", tmp, &gap_y);
    fscanf(fd, "%s%" SCNu8 "", tmp, &act_nb);
    fscanf(fd, "%s%d", tmp, &pos.x);
    fscanf(fd, "%s%d", tmp, &pos.y);
    fclose(fd);
    init_x = pos.x;
    init_y = pos.y;

    act = new Action[act_nb];
    if (act == NULL) {
        fprintf(stderr, "Allocate memory error\n");
        return -1;
    }
    for (int i = 0; i < act_nb; i++) {
        strcpy(filename, dir);
        snprintf(file, 255, "act_%d/", i);
        strcat(filename, file);
        act[i] = Action(filename);
    }
    
    return 0;
}

int Robot::control() {
    struct termios old_tio;
    struct termios new_tio;
    
    /* get the terminal settings for stdin */
    tcgetattr(STDIN_FILENO, &old_tio);
    /* we want to keep the old setting to restore them at the end */
    new_tio = old_tio;
    /* disable canonical mode (buffered io) and local echo */
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
    if (act == NULL) return -1;
    return (act[idx].print_step(stdout, 0, pos));
}

int Robot::stand_jump(FILE *out, int idx) {
    if (act == NULL) return -1;
    sem_wait(&mutex_3); /* mutex_3 */
    pos.y = init_y;
    EARSE_N_LINE(stdout, height + pos.y);
    pos.y -= gap_y;
    act[idx].print_step(stdout, 0, pos);
    usleep(220000);
    EARSE_N_LINE(stdout, height + pos.y);
    pos.y = init_y;
    act[idx].print_step(stdout, 0, pos);
    sem_post(&mutex_3); /* mutex_3 */
    
    return 0;
}

int Robot::walk_left(FILE *out, int idx) {
    if (act == NULL) return -1;
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
        act[idx].print_step(stdout, i, pos);
        sem_post(&mutex_3); /* mutex_3 */
        if (i != act[idx].get_st_nb() - 1)
            usleep(150000);
    }

    return 0;
}

int Robot::walk_right(FILE *out, int idx) {
    if (act == NULL) return -1;
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
        act[idx].print_step(stdout, i, pos);
        sem_post(&mutex_3); /* mutex_3 */
        if (i != act[idx].get_st_nb() - 1)
            usleep(150000);
    }
    
    return 0; 
}

