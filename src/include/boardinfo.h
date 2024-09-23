#ifndef __BOARDINFO_H__
#define __BOARDINFO_H__

#define MAX_LEN 256

typedef struct {
    char board_serial[MAX_LEN];
    char board_model[MAX_LEN];
    char vendor[MAX_LEN];
    char board_revision[MAX_LEN];
} board_info_t;

board_info_t* get_board_info( );

#endif /* __BOARDINFO_H__ */
