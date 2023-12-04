#define MAX_NUM_PLAYERS 26
#define MAX_NUM_ROACHES 300
#define WINDOW_SIZE 30

#include <ncurses.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>  
#include <stdlib.h>
#include <zmq.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include "LinkedListFun.h"
#include <time.h>

typedef struct liz_info_t{
    int secret;
    int pos_x, pos_y;
    int score;
}liz_info_t;

typedef struct roach_info_t
{   
    int id;
    int secret;
    int pos_x, pos_y;
    long death_time;
}roach_info_t;
