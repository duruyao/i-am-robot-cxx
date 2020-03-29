/**
 * @file func.h 
 * defination of structure and macro.
 *
 * @author  duruyao
 * @version 1.0  20-03-26
 * @update  [id] [yy-mm-dd] [author] [description] 
 */

#ifndef __FUNCH__
#define __FUNCH__

#ifdef  __cplusplus
extern "C" {
#endif /* __plusplus */

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
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

#define FREE(n, pp) ({                          \
    typeof(n)  n_  = (n);                       \
    typeof(pp) pp_ = (pp);                      \
    if (pp_ != NULL) {                          \
        while (n_--) {                          \
            if (pp_[n_])                        \
                free(pp_[n_]);                  \
        }                                       \
        free(pp_);                              \
    }                                           \
})


/**********************************************************/
/*                                                        */
/* struct defination                                      */
/*                                                        */
/**********************************************************/


/******************************************************************************/
/* Position                                                                   */
/******************************************************************************/

struct POSITION;
typedef struct POSITION Position;

struct POSITION {
    int x;
    int y;
};

/**********************************************************/
/*                                                        */
/* function declaration                                   */
/*                                                        */
/**********************************************************/


/******************************************************************************/
/* keyboard                                                                   */
/******************************************************************************/

extern  int     my_kbhit        (void);

#ifdef __cplusplus
}
#endif /* __plusplus */

#endif /* !__FUNCH__ */

