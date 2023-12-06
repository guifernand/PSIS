
typedef enum direction_t {UP, DOWN, LEFT, RIGHT} direction_t;

typedef struct message_t{   
    int msg_type; /*0:newclient  1:lizard connect   -1:roach connect  2:Lizardmove  -2: Roach move  3:Disconect lizard -3:Disconect roach */
    char ch; 
    int secret;
    direction_t direction ;
}message_t;

typedef struct reply_message_t{   
    int score; /* 1:lizard connect   -1:roach connect  2:Lizardmove  -2: Roach move  3:Disconect lizard -3:Disconect roach */
    char ch; 
    int secret;
}reply_message_t;




#define ADDR_MOVES "tcp://127.0.0.1:5555" //Deve estar na consola do client

#define ADDR_DISP "tcp://127.0.0.1:5556"