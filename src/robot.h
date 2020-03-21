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


/******************************************************************************/
/*                                                                            */
/* macro defination                                                           */
/*                                                                            */
/******************************************************************************/


#define ALLOC(n, type)                  \
    ((type *)(malloc(sizeof(type) * n)))

#define FREE(n_, pp_) ({                \
    if (pp_ != NULL) {                  \
        while (n_--) {                  \
            if (pp_[n_])                \
                free(pp_[n_]);          \
        }                               \
        free(pp_);                      \
    }                                   \
})


/******************************************************************************/
/*                                                                            */
/* struct declaration                                                         */
/*                                                                            */
/******************************************************************************/


struct STEP;
typedef struct STEP Step;

struct ACTION;
typedef struct ACTION Act;

struct POSITION;
typedef struct POSITION Pos;

struct ROBOT;
typedef struct ROBOT Bot;


/******************************************************************************/
/*                                                                            */
/* struct defination                                                          */
/*                                                                            */
/******************************************************************************/


struct STEP {
    uint8_t s_nb;
    char  **str;
};

struct ACTION {
    char        cmd;
    uint8_t     st_nb;
    Step       *step;
    const char *folder;
};

struct POSITION {
    int x;
    int y;
};

struct ROBOT {
    uint8_t id;
    uint8_t width;
    uint8_t height;
    uint8_t act_nb;
    Act *action;
    Pos  pos;

    int (*init)(Bot *, uint8_t, uint8_t, uint8_t, uint8_t, Act *, Pos);
    int (*ctrl)(Bot *);
};


/******************************************************************************/
/*                                                                            */
/* func declaration                                                           */
/*                                                                            */
/******************************************************************************/


extern  int create_bot      (Bot **);

extern  int delete_bot      (Bot **);

extern  int bot_init        (Bot *, uint8_t, uint8_t,
                                    uint8_t, uint8_t, Act *, Pos);

extern  int bot_ctrl        (Bot *);

extern  int act_load        (Act *, uint8_t, char *, uint8_t *, const char **);

extern  int act_unload      (Act *, uint8_t);

#ifdef __cplusplus
}
#endif /* __plusplus */

#endif /* !__ROBOTH__ */

