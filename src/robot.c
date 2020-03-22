/**
 * @file robot.c
 * defination of 'robot'.
 *
 * @author  duruyao
 * @version 1.0  20-03-21
 * @update  [id] [yy-mm-dd] [author] [description] 
 */

#include "robot.h"

int jump = 0;
struct winsize w;

sem_t mutex_1;
sem_t mutex_2;
sem_t mutex_3;

pthread_t main_tid;
pthread_t sub_tid;

/**********************************************************/
/*                                                        */
/* func defination                                        */
/*                                                        */
/**********************************************************/


/******************************************************************************/
/* func of ACTION                                                             */
/******************************************************************************/

/*
 * load resource of STEP of ACTION.
 * 
 * @param   act     is an 1-D array of ACTION.
 * @param   act_nb  is number of ACTION in array.
 * @param   width   is width of STEP.
 * @param   height  is height of STEP.
 * @param   cmd     is an 1-D array store control key of STEP.
 * @param   step_nb is an 1-D array store number of STEP.
 * @param   folder  is an 1-D array store position of STEP folder of ACTION.
 *
 * @return  0 on success, otherwise -1;
 *
 */

int load_act(Act *act, uint8_t act_nb,
             uint8_t width, uint8_t height,
             char *cmd, uint8_t *step_nb, const char **folder) {
    if (act == NULL)
        return -1;
    for (uint8_t i = 0; i < act_nb; i++) {
        act[i].cmd    = cmd[i];
        act[i].st_nb  = step_nb[i];
        act[i].folder = folder[i];
        if ((act[i].step = ALLOC(step_nb[i], Step)) == NULL) {
            fprintf(stderr, "Allocate memory error\n");
            return -1;
        }
        for (int j = 0; j < step_nb[i]; j++) {
            char filename[255];
            char file[6] = "0.txt";
            FILE *fd = NULL;
            
            memset(filename, '\0', 255);
            file[0] = '0' + j;  /* number of step <= 10 */
            strcat(filename, folder[i]);
            strcat(filename, file);

            if ((fd = fopen(filename, "r")) == NULL) {
                fprintf(stderr, "Cannot open %s\n", filename);
                return -1;
            }
            if ((act[i].step[j].str = ALLOC(height, char*)) == NULL) {
                fprintf(stderr, "Allocate memory error\n");
                return -1;
            }
            act[i].step[j].s_nb = height;

            for (int k = 0; k < height; k++) {
                if ((act[i].step[j].str[k] = ALLOC(width + 1, char)) == NULL) {
                    fprintf(stderr, "Cannot open %s\n", filename);
                    return -1;
                }
                fgets(act[i].step[j].str[k], width + 1, fd);
                // fprintf(stdout, "%2d %s", k + 1, act[i].step[j].str[k]);
            }
            if (fd)
                fclose(fd);
        }
    }

    return 0;
}

/*
 * release resource of ACTION.
 *
 * @param   act     is 1-D array of ACTION.
 * @param   act_nb  is number of ACTION in array.
 *
 * @return  0 on success, otherwise -1;
 *
 */

int unload_act(Act *act, uint8_t act_nb) {
    if (act == NULL)
        return 0;
    while (act_nb--) {
        Act a = act[act_nb];
        while ((a.st_nb)--) {
            Step s = a.step[a.st_nb];
            FREE((s.s_nb), (s.str));
        }
        free(a.step);
    }
    
    return 0;
}

/******************************************************************************/
/* func of STEP                                                               */
/******************************************************************************/

/*
 * print STEP to standard output.
 *
 * @param   out     is a pointer to FILE as standrad output.
 * @param   pos     is the first character position.
 * @param   str     is a 2-D array of character.
 * @param   hgt     is height of STEP.
 *
 * @return  0 on success, otherwise -1;
 *
 */

int print_step(FILE *out, Pos pos, char **str, uint8_t hgt) {
    uint8_t x = pos.x;
    uint8_t y = pos.y;
    while (y--)
        fprintf(out, "\n");
    for (int i = 0; i < hgt; i++) {
        for (int j = 0; j < x; fprintf(out, " "), j++);
        if (str[i] == NULL)
            return -1;
        fprintf(out, "%s", str[i]);
    }
    return 0;
}

/******************************************************************************/
/* func of ROBOT                                                              */
/******************************************************************************/

/*
 * create a ROBOT.
 *
 * @param   bot_p   is a pointer to pointer to ROBOT.
 *
 * @return  0 on success, otherwise -1;
 *
 */

int create_bot(Bot **bot_p) {
    if ((*bot_p = ALLOC(1, Bot)) == NULL) {
        fprintf(stderr, "Allocate memory error\n");
        return -1;
    }
    (*bot_p)->init = &bot_init;
    (*bot_p)->ctrl = &bot_ctrl;

    return 0;
}

/*
 * delete a ROBOT.
 *
 * @param   bot_p   is a pointer to pointer to ROBOT.
 *
 * @return  0 on success, otherwise -1;
 *
 */

int delete_bot(Bot **bot_p) {
    if (bot_p == NULL || *bot_p == NULL)
        return -1;

    Bot *bot = *bot_p;
    if (bot->action == NULL)
        return 0;

    free(bot);
    *bot_p = NULL;

    return 0;
}

/*
 * initialize a ROBOT.
 *
 * @param   bot     is a pointer to ROBOT.
 * @param   id      is ID of ROBOT.
 * @param   wdt     is width of ROBOT.
 * @param   hgt     is height of ROBOT.
 * @param   act_nb  is number of ACTION.
 * @param   act     is an 1-D array of ACTION.
 * @param   pos     is initial position. 
 *
 * @return  0 on success, otherwise -1;
 *
 */

int bot_init(Bot *bot, uint8_t id, uint8_t wdt,
             uint8_t hgt, uint8_t act_nb, Act *act, Pos pos) {
    if (bot == NULL)
        return -1;
    bot->id       = id;
    bot->width    = wdt;
    bot->height   = hgt;
    bot->act_nb   = act_nb;
    bot->action   = act;
    bot->pos      = pos;
    bot->init_pos = pos;

    return 0;
}

/*
 * control ROBOT.
 *
 * @param   bot_p   is a pointer to pointer to ROBOT.
 *
 * @return  0 on success, otherwise -1;
 *
 */

int bot_ctrl(Bot *bot) {
    struct termios old_tio;
    struct termios new_tio;

    /* get size of current terminal */
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    /* get the terminal settings for stdin */
    tcgetattr(STDIN_FILENO, &old_tio);

    /* we want to keep the old setting to restore them at the end */
    new_tio = old_tio;

    /* disable canonical mode (buffered i/o) and local echo */
    new_tio.c_lflag &= (~ICANON & ~ECHO);

    /* set the new settings immediately */
    tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);
    
    /* default action is 'stand' */
    print_step(stdout, bot->pos, bot->action[0].step[0].str, bot->height);
    
    sem_init(&mutex_1, 0, 1);
    sem_init(&mutex_2, 0, 0);
    sem_init(&mutex_3, 0, 1);
    pthread_create(&main_tid, NULL, &main_thread, (void *)(bot));
    pthread_create(&sub_tid,  NULL, &sub_thread,  (void *)(bot));

    pthread_join(main_tid, NULL);
    pthread_join(sub_tid,  NULL);

    sem_destroy(&mutex_1);
    sem_destroy(&mutex_2);
    sem_destroy(&mutex_3);

    /* restore the former settings */
    tcsetattr(STDIN_FILENO, TCSANOW, &old_tio);

    return 0;
}

/******************************************************************************/
/* func of thread                                                             */
/******************************************************************************/

/*
 * main thread of controling ROBOT.
 *
 * @param   vargp   is a pointer to ROBOT.
 *
 * @return  0 on success, otherwise -1;
 *
 */

void * main_thread(void *vargp) {
    char c;
    int cnt = 0;
    Bot *bot = (Bot *)vargp;
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
                sem_wait(&mutex_3); /* mutex_3 */
                bot->pos.y = bot->init_pos.y;
                EARSE_N_LINE(stdout, bot->height + bot->pos.y);
                bot->pos.y -= 4;
                print_step(stdout, bot->pos,
                           bot->action[0].step[0].str, bot->height);
                usleep(250000);
                EARSE_N_LINE(stdout, bot->height + bot->pos.y);
                bot->pos.y = bot->init_pos.y;
                print_step(stdout, bot->pos,
                           bot->action[0].step[0].str, bot->height);
                sem_post(&mutex_3); /* mutex_3 */
                break;
            case 'a':
                for (int i = 0; i < bot->action[1].st_nb; i++) {
                    sem_wait(&mutex_3); /* mutex_3 */
                    EARSE_N_LINE(stdout, bot->height + bot->pos.y);
                    if (i == 0)
                        bot->pos.y = bot->init_pos.y;
                    else if (i == 1) {
                        if (jump) bot->pos.y = bot->init_pos.y - 2;
                        bot->pos.x -= (bot->pos.x >= 0 + 1);
                    }
                    else if (i == 2) {
                        if (jump) bot->pos.y = bot->init_pos.y - 4;
                        bot->pos.x -= (bot->pos.x >= 0 + 1);
                    }
                    else if (i == 3) {
                        if (jump) bot->pos.y = bot->init_pos.y - 4;
                        bot->pos.x -= (bot->pos.x >= 0 + 1);
                    }
                    else if (i == 4) {
                        if (jump) bot->pos.y = bot->init_pos.y - 2;
                        bot->pos.x -= (bot->pos.x >= 0 + 1);
                    }
                    else if (i == 5) {
                        bot->pos.y = bot->init_pos.y;
                        jump = 0;
                    }
                    print_step(stdout, bot->pos,
                               bot->action[1].step[i].str, bot->height);
                    sem_post(&mutex_3); /* mutex_3 */
                    if (i != bot->action[1].st_nb - 1)
                        usleep(150000);
                }
                break;
            case 's':
                break;
            case 'd':
                for (int i = 0; i < bot->action[2].st_nb; i++) {
                    sem_wait(&mutex_3); /* mutex_3 */
                    EARSE_N_LINE(stdout, bot->height + bot->pos.y);
                    if (i == 0)
                        bot->pos.y = bot->init_pos.y;
                    else if (i == 1) {
                        if (jump) bot->pos.y = bot->init_pos.y - 2;
                        bot->pos.x += (bot->pos.x < 
                                       w.ws_col - bot->width + 1 - 2);
                    }
                    else if (i == 2) {
                        if (jump) bot->pos.y = bot->init_pos.y - 4;
                        bot->pos.x += (bot->pos.x < 
                                       w.ws_col - bot->width + 1 - 2);
                    }
                    else if (i == 3) {
                        if (jump) bot->pos.y = bot->init_pos.y - 4;
                        bot->pos.x += (bot->pos.x < 
                                       w.ws_col - bot->width + 1 - 2);
                    }
                    else if (i == 4) {
                        if (jump) bot->pos.y = bot->init_pos.y - 2;
                        bot->pos.x += (bot->pos.x < 
                                       w.ws_col - bot->width + 1 - 2);
                    }
                    else if (i == 5) {
                        bot->pos.y = bot->init_pos.y;
                        jump = 0;
                    }
                    print_step(stdout, bot->pos,
                               bot->action[2].step[i].str, bot->height);
                    sem_post(&mutex_3); /* mutex_3 */
                    if (i != bot->action[2].st_nb - 1)
                        usleep(150000);
                }
                break;
            default:
                break;
        }

    } while (c != 27);
    pthread_cancel(sub_tid); 
    pthread_exit(NULL);
}

/*
 * extra thread of controling ROBOT.
 *
 * @param   vargp   is a pointer to ROBOT.
 *
 * @return  0 on success, otherwise -1;
 *
 */

void * sub_thread(void *vargp) {
    char c;
    Bot *bot = (Bot *)vargp;
    sem_wait(&mutex_2);     /* mutex_2 */
    while (1) {
        sem_wait(&mutex_1); /* mutex_1 */
        if (kbhit()) {
            c = getchar();
            if (c == 'w') {
                sem_wait(&mutex_3); /* mutex_3 */
                jump = 1;
                sem_post(&mutex_3); /* mutex_3 */
            }
        }
        sem_post(&mutex_1); /* mutex_1 */
        usleep(5000);

    }
    pthread_exit(NULL);
}

/******************************************************************************/
/* func of listening keyboard                                                 */
/******************************************************************************/

/*
 * unblock listening event of keyboard.
 *
 * @return  1 on pressing any key, otherwise 0;
 *
 */

int kbhit(void) {
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    if(ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }
    return 0;
}

