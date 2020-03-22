/**
 * @file robot.h 
 * declaration of 'robot'.
 *
 * @author  duruyao
 * @version 1.0  20-03-21
 * @update  [id] [yy-mm-dd] [author] [description] 
 */

#ifndef __ROBOTH__
#define __ROBOTH__

#ifdef  __cplusplus
extern "C" {
#endif /* __plusplus */

#include <time.h>
#include <math.h>
#include <stdio.h>
#include <float.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <unistd.h>
#include <termios.h>
#include <pthread.h>
#include <inttypes.h>
#include <semaphore.h>
#include <sys/ioctl.h>


/**********************************************************/
/*                                                        */
/* macro defination                                       */
/*                                                        */
/**********************************************************/

#define UP_N_LINE(out, n) ({                    \
    typeof(n) n_ = (n);                         \
    fprintf(out, "\033[%dA", n_);               \
})

#define EARSE_N_LINE(out, n) ({                 \
    typeof(n) n_ = (n);                         \
    while (n_--) {                              \
        fprintf(out, "\033[1A");                \
        fprintf(out, "%c[2K", 27);              \
    }                                           \
})

#define CLR_OUT(out)                            \
    fprintf(out, "\033[2J")

#define ALLOC(n, type) ({                       \
    typeof(n) n_ = (n);                         \
    (type *)(malloc(sizeof(type) * n_));        \
})

#define FREE(n_, pp_) ({                \
    if (pp_ != NULL) {                  \
        while (n_--) {                  \
            if (pp_[n_])                \
                free(pp_[n_]);          \
        }                               \
        free(pp_);                      \
    }                                   \
})


/**********************************************************/
/*                                                        */
/* struct declaration                                     */
/*                                                        */
/**********************************************************/


/*****************************************************************************/
/* struct of STEP                                                            */
/*****************************************************************************/

struct STEP;
typedef struct STEP Step;

/******************************************************************************/
/* struct of ACTION                                                           */
/******************************************************************************/

struct ACTION;
typedef struct ACTION Act;

/******************************************************************************/
/* struct of POSITION                                                         */
/******************************************************************************/

struct POSITION;
typedef struct POSITION Pos;

/******************************************************************************/
/* struct of ROBOT                                                            */
/******************************************************************************/

struct ROBOT;
typedef struct ROBOT Bot;


/**********************************************************/
/*                                                        */
/* struct defination                                      */
/*                                                        */
/**********************************************************/


/******************************************************************************/
/* struct of STEP                                                             */
/******************************************************************************/

struct STEP {
    uint8_t s_nb;
    char  **str;
};

/******************************************************************************/
/* struct of ACTION                                                           */
/******************************************************************************/

struct ACTION {
    char        cmd;
    uint8_t     st_nb;
    Step       *step;
    const char *folder;
};

/******************************************************************************/
/* struct of POSITION                                                         */
/******************************************************************************/

struct POSITION {
    int x;
    int y;
};

/******************************************************************************/
/* struct of ROBOT                                                            */
/******************************************************************************/

struct ROBOT {
    uint8_t id;
    uint8_t width;
    uint8_t height;
    uint8_t act_nb;
    Act *action;
    Pos  init_pos;
    Pos  pos;

    int (*init)(Bot *, uint8_t, uint8_t, uint8_t, uint8_t, Act *, Pos);
    int (*ctrl)(Bot *);
};


/**********************************************************/
/*                                                        */
/* func declaration                                       */
/*                                                        */
/**********************************************************/


/******************************************************************************/
/* func of ROBOT                                                              */
/******************************************************************************/

extern  int     create_bot      (Bot **);

extern  int     delete_bot      (Bot **);

extern  int     bot_init        (Bot *, uint8_t, uint8_t,
                                        uint8_t, uint8_t, Act *, Pos);

extern  int     bot_ctrl        (Bot *);

/******************************************************************************/
/* func of ACTION                                                             */
/******************************************************************************/

extern  int     load_act        (Act *, uint8_t, uint8_t, uint8_t,
                                        char *, uint8_t *, const char **);

extern  int     unload_act      (Act *, uint8_t);

/******************************************************************************/
/* func of STEP                                                               */
/******************************************************************************/

extern  int     print_step      (FILE *, Pos, char **, uint8_t);

/******************************************************************************/
/* func of thread                                                             */
/******************************************************************************/

extern  void *  main_thread     (void *);

extern  void *  sub_thread      (void *);

/******************************************************************************/
/* func of listening keyboard                                                 */
/******************************************************************************/

extern  int     kbhit           (void);

#ifdef __cplusplus
}
#endif /* __plusplus */

#endif /* !__ROBOTH__ */

