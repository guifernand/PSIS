#include "../Structures.h"
#include "ServerVars.h"

void initializeLizards(liz_info_t* lizards){
    for(int i=0;i<MAX_NUM_PLAYERS;i++){
        lizards[i].score=-1;
        lizards[i].pos_x=-1;
        lizards[i].pos_y=-1;
        lizards[i].secret=-1;
    }
}

void initializeRoaches(roach_info_t* roaches){
    for(int i=0;i< MAX_NUM_ROACHES;i++){
        roaches[i].pos_x=-1;
        roaches[i].pos_y=-1;
        roaches[i].secret=-1;
        roaches[i].id=-1;

    }
}


int isEmpty(int x,int y,liz_info_t *lizards,roach_info_t *roaches,int *score,Node* deadRoaches){
    //check for lizards
    for (int i = 0; i < MAX_NUM_PLAYERS; i++){
        if (lizards[i].pos_x==x && lizards[i].pos_y==y){
            *score = lizards[i].score;
            return 1;
        }
    }
    //check for roaches --------------------Make it propper
    for (int i = 0; i < MAX_NUM_ROACHES; i++){
         if (roaches[i].pos_x==x && roaches[i].pos_y==y){
            *score += roaches[i].id;
            addToEnd(deadRoaches,time(NULL),i);
        }
    }
    return 0; 
}


int find_empty_pos(liz_info_t *lizards,roach_info_t *roaches,int* finalx,int*finaly,Node* deadRoaches){
    int not_found = 1;
    int score=0;
    while (not_found==1){
        *finalx=random() % (WINDOW_SIZE+1);
        *finaly=random()% (WINDOW_SIZE+1);
        not_found=isEmpty(*finalx,*finaly,lizards,roaches,&score,deadRoaches);
    }
    return score;
}

int create_lizard (liz_info_t *lizards,roach_info_t *roaches,Node* deadRoaches){
    for (int i = 0; i < MAX_NUM_PLAYERS; i++){
        if(lizards[i].secret==-1){
            lizards[i].score = find_empty_pos(lizards,roaches,&lizards[i].pos_x,&lizards[i].pos_y,deadRoaches);
            lizards[i].secret= random();
            return i;
        }
    }
    return -1;
}

char create_roach (liz_info_t *lizards,roach_info_t *roaches,char ch,Node* deadRoaches){
    for (int i = 0; i < MAX_NUM_ROACHES; i++){
        if(roaches[i].secret==-1){
            find_empty_pos(lizards,roaches,roaches[i].pos_x,roaches[i].pos_y,deadRoaches);
            roaches[i].id = ch - '0';
            roaches[i].secret=random();
            return i;
        }
    }
    return -1;
}


int treatConnectMessage(liz_info_t* lizards,roach_info_t* roaches,message_t incomingMessage, Node* deadRoaches){
    if (incomingMessage.msg_type==1){
        //lizard connect message
        create_lizard(lizards,roaches,deadRoaches);
        //verificacao de numero de jogadores

    }else{
        //roach connect message
        return create_roach(lizards,roaches,incomingMessage.ch,deadRoaches);
    }
}

void new_position(int* x, int *y, direction_t direction){
    switch (direction){
        case UP:
            (*x) --;
            if(*x ==0)
                *x = 2;
            break;
        case DOWN:
            (*x) ++;
            if(*x ==WINDOW_SIZE-1)
                *x = WINDOW_SIZE-3;
            break;
        case LEFT:
            (*y) --;
            if(*y ==0)
                *y = 2;
            break;
        case RIGHT:
            (*y) ++;
            if(*y ==WINDOW_SIZE-1)
                *y = WINDOW_SIZE-3;
            break;
        default:
            break;
    }
}

int treatMoveMessage(liz_info_t* lizards,roach_info_t* roaches,message_t incomingMessage,Node* deadRoaches){
   
    int ch_pos = find_ch_info(char_data, n_chars, m.ch);
    if(ch_pos != -1){
        pos_x = char_data[ch_pos].pos_x;
        pos_y = char_data[ch_pos].pos_y;
        ch = char_data[ch_pos].ch;
        /*deletes old place */
        wmove(my_win, pos_x, pos_y);
        waddch(my_win,' ');

        /* claculates new direction */
        direction = m.direction;

        /* claculates new mark position */
        new_position(&pos_x, &pos_y, direction);
        char_data[ch_pos].pos_x = pos_x;
        char_data[ch_pos].pos_y = pos_y;

    }        
    
}

int treatDisconnectMessage(liz_info_t* lizards,roach_info_t* roaches,message_t incomingMessage){

}


int main()
{	
    srand(time(NULL));

    Node* deadRoaches;
    //Create and initialize vector that will store lizzards
    liz_info_t lizards[MAX_NUM_PLAYERS];
    initializeLizards(lizards);

    //Create and initialize vector that will store roaches
    roach_info_t roaches[MAX_NUM_PLAYERS];
    initializeRoaches(roaches);

    message_t incomingMessage;
    message_t outgoingMessage; //struct diferente?
    
    void *context = zmq_ctx_new ();
    void *responder = zmq_socket (context, ZMQ_REP);
    int rc = zmq_bind (responder, ADDR_MOVES);
    assert (rc == 0);

    //int n;
    //zmq_recv (responder, &n, sizeof(n), 0);
    //zmq_send (responder, &n, sizeof(n), 0);
    
    direction_t  direction;

    int bytes=0;
    while (1)
    {
        bytes=zmq_recv (responder, &incomingMessage, sizeof(message_t), 0);
        if (bytes==-1){
           printf("Error: %d",errno);
           break;
        }else if (bytes=0){
           printf("No message Received");
           continue;
        }

        switch (abs(incomingMessage.msg_type)){
        case 1:
            treatConnectMessage(lizards,roaches, incomingMessage,deadRoaches);
            break;
        case 2:
            treatMoveMessage(lizards,roaches, incomingMessage,deadRoaches);
            break;
        case 3:
            treatDisconnectMessage(lizards,roaches, incomingMessage);
            break;
        default:
            break;
        }
    }

	return 0;
}