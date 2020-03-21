/**
 * @file robot.c
 * defination of 'robot'.
 *
 * @author  duruyao
 * @version 1.0  20-03-21
 * @update  [id] [yy-mm-dd] [author] [description] 
 */

#include "robot.h"

#define HEIGHT  16
#define WIDTH   27


struct winsize w;
sem_t mutex_1;
sem_t mutex_2;
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
 *
 */

int load_act(Act *act, uint8_t act_nb,
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
            if ((act[i].step[j].str = ALLOC(HEIGHT, char*)) == NULL) {
                fprintf(stderr, "Allocate memory error\n");
                return -1;
            }
            act[i].step[j].s_nb = HEIGHT;

            for (int k = 0; k < HEIGHT; k++) {
                if ((act[i].step[j].str[k] = ALLOC(WIDTH + 1, char)) == NULL) {
                    fprintf(stderr, "Cannot open %s\n", filename);
                    return -1;
                }
                fgets(act[i].step[j].str[k], WIDTH + 1, fd);
                // fprintf(stdout, "%2d %s", k + 1, act[i].step[j].str[k]);
            }
            if (fd)
                fclose(fd);
        }
    }

    return 0;
}

/*
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
 *
 */

int bot_init(Bot *bot, uint8_t id, uint8_t wdt,
             uint8_t hgt, uint8_t act_nb, Act *act, Pos pos) {
    if (bot == NULL)
        return -1;
    bot->id     = id;
    bot->width  = wdt;
    bot->height = hgt;
    bot->act_nb = act_nb;
    bot->action = act;
    bot->pos    = pos;

    return 0;
}

/*
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

    print_step(stdout, bot->pos, bot->action[0].step[0].str, HEIGHT);
    
    sem_init(&mutex_1, 0, 1);
    pthread_create(&main_tid, NULL, &main_thread, (void *)(bot));
    usleep(500);
    pthread_create(&sub_tid,  NULL, &sub_thread,  NULL);

    pthread_join(main_tid, NULL);
    pthread_join(sub_tid,  NULL);
    sem_destroy(&mutex_1);

    /* restore the former settings */
    tcsetattr(STDIN_FILENO, TCSANOW, &old_tio);

    return 0;
}

/******************************************************************************/
/* func of thread                                                             */
/******************************************************************************/

/*
 *
 */

void *main_thread(void *vargp) {
    Bot *bot = (Bot *)vargp;
    char c;
    do {
        sem_wait(&mutex_1);
        c = getchar();
        sem_post(&mutex_1);
        switch (c) {
            case 'w':
                // EARSE_N_LINE(stdout, 1);
                // fprintf(stdout, "up\n");
                break;
            case 'a':
                for (int i = 0; i < bot->action[1].st_nb; i++) {
                    if (i == 2 && bot->pos.x >= 0 + 2)
                        bot->pos.x -= 2;
                    else if (i == 4 && bot->pos.x >= 0 + 2)
                        bot->pos.x -= 2;
                    EARSE_N_LINE(stdout, HEIGHT + bot->pos.y);
                    print_step(stdout, bot->pos,
                               bot->action[1].step[i].str, HEIGHT);
                    if (i != bot->action[1].st_nb - 1)
                        usleep(75000);
                }
                break;
            case 's':
                // EARSE_N_LINE(stdout, 1);
                // fprintf(stdout, "down\n");
                break;
            case 'd':
                for (int i = 0; i < bot->action[1].st_nb; i++) {
                    if (i == 2 && bot->pos.x < w.ws_col - bot->width + 1 - 2)
                        bot->pos.x += 2;
                    else if (i == 4 && bot->pos.x < w.ws_col -
                                                    bot->width + 1 - 2)
                        bot->pos.x += 2;
                    EARSE_N_LINE(stdout, HEIGHT + bot->pos.y);
                    print_step(stdout, bot->pos,
                               bot->action[2].step[i].str, HEIGHT);
                    if (i != bot->action[1].st_nb - 1)
                        usleep(75000);
                }
                break;
            default:
                break;
        }

    } while (c != 27);
    pthread_cancel(sub_tid); 
    pthread_exit(NULL);
}

void *sub_thread(void *vargp) {
    while (1) {
        sem_wait(&mutex_1);
        if (kbhit())
            getchar();
        sem_post(&mutex_1);
        // usleep(500);

    }
    pthread_exit(NULL);
}

/******************************************************************************/
/* func of listening keyboard                                                 */
/******************************************************************************/

/*
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

