#include "../Structures.h"
#include "ServerVars.h"

char convertPos(int index){
    return index + 'a';
}

int convertChar(char c){
    return c -'0';
}

int makeSecret(int clientId,int pos){
    return clientId<<10 + pos; 
}

void decodeSecret(int secret,int decoded[2]){
    decoded[0]= secret & 0x3FF;
    decoded[1]= secret>>10;
}

int getClientIdFromSecret(int secret){
    return secret>>10;
}

int getRoachPosFromSecret(int secret){
    return secret & 0x3FF;
}


void initializeLizards(liz_info_t* lizards){
    for(int i=0;i<MAX_NUM_PLAYERS;i++){
        lizards[i].score=-1;
        lizards[i].pos_x=-1;
        lizards[i].pos_y=-1;
        lizards[i].secret=-1;
    }
}

void resetLizard(liz_info_t* lizards,int index){
    lizards[index].score=-1;
    lizards[index].pos_x=-1;
    lizards[index].pos_y=-1;
    lizards[index].secret=-1;
}

void initializeRoaches(roach_info_t* roaches){
    for(int i=0;i< MAX_NUM_ROACHES;i++){
        roaches[i].pos_x=-1;
        roaches[i].pos_y=-1;
        roaches[i].secret=-1;
        roaches[i].id=-1;
    }
}

int RoachIdx(int x,int y,liz_info_t *lizards,roach_info_t *roaches){
    for (int i = 0; i < MAX_NUM_ROACHES; i++){
         if (roaches[i].pos_x==x && roaches[i].pos_y==y){
            return i;
        }
    }
    return -1; 
}

int LizIdx(int x,int y,liz_info_t *lizards,roach_info_t *roaches){
    //check for lizards
    for (int i = 0; i < MAX_NUM_PLAYERS; i++){
        if (lizards[i].pos_x==x && lizards[i].pos_y==y){
            return i;
        }
    }
    return -1; 
}

void find_empty_pos(liz_info_t *lizards,roach_info_t *roaches,int* finalx,int*finaly){
    int RoachPos = -1;
    int LizPos = -1;
    while (LizPos==-1||RoachPos==-1){
        *finalx=rand() % (WINDOW_SIZE+1);
        *finaly=rand() % (WINDOW_SIZE+1);
        RoachPos=LizIdx(*finalx,*finaly,lizards,roaches);
        if(RoachPos==-1)return;
        LizPos=RoachIdx(*finalx,*finaly,lizards,roaches);
    }
}

int create_lizard(liz_info_t *lizards,roach_info_t *roaches,List* deadRoaches){
    for (int i = 0; i < MAX_NUM_PLAYERS; i++){
        if(lizards[i].secret==-1){
            find_empty_pos(lizards,roaches,&(lizards[i].pos_x),&(lizards[i].pos_y));
            lizards[i].score = 0;
            lizards[i].secret= rand();
            return i;
        }
    }
    return -1;
}

char create_roach(liz_info_t *lizards,roach_info_t *roaches,char ch,List* deadRoaches,int clientId){
    int client_roaches = 0;
    
    for (int i = 0; i < MAX_NUM_ROACHES; i++){
        if (getClientIdFromSecret(roaches[i].secret) == clientId){
            client_roaches++;
        }
        
        if(client_roaches == MAX_NUM_ROACHES){
            return -1;
        }
            
        if(roaches[i].secret==-1){
            find_empty_pos(lizards,roaches,roaches[i].pos_x,roaches[i].pos_y);
            roaches[i].id = ch - '0';
            roaches[i].secret=makeSecret(clientId,i);
            return i;
        }
    }
    return -1;
}

void setReplyMessage(reply_message_t* replyMessage,int score,char ch, int secret){
    replyMessage->ch=ch;
    replyMessage->score=score;
    replyMessage->secret=secret;
}

void treatRoachClientConect(message_t incomingMessage,reply_message_t* replyMessage){
    replyMessage->secret = (rand()<<10)>>10;
}

void treatConnectMessage(liz_info_t* lizards,roach_info_t* roaches,message_t incomingMessage, List* deadRoaches,reply_message_t* replyMessage){
    int newPos=-1;
    if (incomingMessage.msg_type==1){
        //lizard connect message
        newPos= create_lizard(lizards,roaches,deadRoaches);
        if (newPos==-1){
            setReplyMessage(replyMessage,-1,-1,-1);
        }else{
            setReplyMessage(replyMessage,lizards[newPos].score,convertPos(newPos),lizards[newPos].secret);
        }
    }else{
        //roach connect message
        newPos= create_roach(lizards,roaches,incomingMessage.ch,deadRoaches,incomingMessage.secret);
        if (newPos==-1){
            setReplyMessage(replyMessage,-1,-1,-1);
        }else{
            setReplyMessage(replyMessage,roaches[newPos].id,'0',roaches[newPos].secret);
        }
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

int SecretMatchesLiz(int secret, char ch,liz_info_t* lizards){
    if(secret==-1)return -1;
    int ich= convertChar(ch);
    for (int i = 0; i < MAX_NUM_PLAYERS; i++){
        if (lizards[i].secret==secret && i==ich){
            return i;
        } 
    }
    return -1;
}

bool isRoachDead(int RoachPos,List* deadRoaches){
    Node* aux = deadRoaches->head;
    while (aux!=NULL){
        if (RoachPos == aux->id){
            return true;  
        }
        aux=aux->next;
    }
    return false;
}

int SecretMatchesRoach(int secret,roach_info_t* roaches){
    int clientID=getClientIdFromSecret(secret);
    int RoachPos=getRoachPosFromSecret(secret);
    if(roaches[RoachPos].secret==clientID)return RoachPos;
    return -1;
}

void lizzardsBumping(int lizzard1, int lizzard2,liz_info_t* lizards){
    int newScore = (lizards[lizzard1].score+ lizards[lizzard2].score)/2;
    lizards[lizzard1].score=newScore;
    lizards[lizzard2].score=newScore;
}

void EatRoaches(int x,int y,roach_info_t* roaches,liz_info_t* lizzardEating,List* deadRoaches){

    for(int i=0;i<MAX_NUM_ROACHES;i++){
        if(roaches[i].pos_x==x&&roaches[i].pos_y==y){
            lizzardEating->score+= roaches[i].id;
            addToEnd(deadRoaches,time(NULL),i);
            //Make Roach Disappear in Visual thingie magingie
        }
    }
}

//SE CALHAR METER UMA LISTA DE ALTERAÇOES?? PARA O SUBSCRIBE? OU IR MANDANDO?

//-1 nao mexeu, 1 mexeu, 2 chocou com lizzard, 3 chocou com roaches
int lizardMovement(liz_info_t* lizards, message_t m, WINDOW* my_win,roach_info_t* roaches,List* deadRoaches){
    int pos_x = -1;
    int pos_y = -1;
    int Idx= 0;
    int ch_pos = SecretMatchesLiz(m.secret,m.ch,lizards);
    if(ch_pos != -1){
        pos_x = lizards[ch_pos].pos_x;
        pos_y = lizards[ch_pos].pos_y;

        /* calculates new mark position */
        new_position(&pos_x, &pos_y,  m.direction);
        
        //Lizzard
        Idx=LizIdx(pos_x,pos_y,lizards,roaches);
        if(Idx!=-1){
            lizzardsBumping(ch_pos,Idx,lizards);
            return -1;
        }else{
            EatRoaches(pos_x,pos_y,Idx,&lizards[ch_pos],deadRoaches);
        }
        
        lizards[ch_pos].pos_x = pos_x;
        lizards[ch_pos].pos_y = pos_y;

        /*deletes old place */
        wmove(my_win, pos_x, pos_y);
        waddch(my_win,' ');

        /* draw mark on new position */
        wmove(my_win, pos_x, pos_y);
        waddch(my_win,m.ch| A_BOLD);
        wrefresh(my_win);	

        //Send to subscribers
        return 1; 
    }     
    return -1;  
}

int roachMovement(roach_info_t* roaches, List* deadRoaches, message_t m, liz_info_t *, WINDOW* my_win){
    int pos_x = -1;
    int pos_y = -1;
    
    int ch_pos = SecretMatchesRoach(m.secret,roaches);
    if(ch_pos != -1){
        if(!isRoachDead(ch_pos, deadRoaches)){
            pos_x = roaches[ch_pos].pos_x;
            pos_y = roaches[ch_pos].pos_y;

            new_position(&pos_x, &pos_y, m.direction);

            if(RoachIdx(pos_x,pos_y,lizards,roaches)>=0){
                /*deletes old place */
                wmove(my_win, pos_x, pos_y);
                waddch(my_win,' ');

                /* claculates new mark position */
                roaches[ch_pos].pos_x = pos_x;
                roaches[ch_pos].pos_y = pos_y;

                /* draw mark on new position */
                wmove(my_win, pos_x, pos_y);
                waddch(my_win,m.ch| A_BOLD);
                wrefresh(my_win);
            }
            return 1;
        }	
    }     
    return -1;
}

int treatMoveMessage(liz_info_t* lizards,roach_info_t* roaches,message_t incomingMessage,List* deadRoaches,reply_message_t* replyMessage,WINDOW* my_win){
    int ret = -1;
    
    if (incomingMessage.msg_type==1){
        ret = lizardMovement(lizards,incomingMessage,my_win,roaches,deadRoaches);
    }else{
        ret = roachMovement(roaches,deadRoaches,incomingMessage,lizards,my_win);
    }   
    
    setReplyMessage(replyMessage,ret,ret,ret);
}

int treatDisconnectMessage(liz_info_t* lizards,roach_info_t* roaches,message_t incomingMessage,reply_message_t* replyMessage){
    int pos=-1;
    if (incomingMessage.msg_type==1){
        pos=SecretMatchesLiz(replyMessage->secret,replyMessage->ch,lizards);
        if(pos==-1){
            //Unauthorized
            setReplyMessage(replyMessage,-1,-1,-1);
        }else{
            resetLizard(lizards,pos);
            setReplyMessage(replyMessage,1,1,1);
        }
    }else{
        pos=SecretMatchesrRoach(replyMessage->secret,replyMessage->ch,roaches);
        if(pos==-1){
            //Unauthorized
            setReplyMessage(replyMessage,-1,-1,-1);
        }else{
            resetRoach(roaches,pos);
            setReplyMessage(replyMessage,1,1,1);
        }
    }    
}

void sendReply(void *responder,reply_message_t *replyMessage){
    int bytes=0;
    bytes=zmq_send (responder, replyMessage, sizeof(replyMessage), 0);
    if(bytes==-1){
        printf("Error Sending reply To client");
        exit(1);
    }
}

int main(){	
    srand(time(NULL));

    List* deadRoaches=NULL;
    //Create and initialize vector that will store lizzards
    liz_info_t lizards[MAX_NUM_PLAYERS];
    initializeLizards(lizards);

    //Create and initialize vector that will store roaches
    roach_info_t roaches[MAX_NUM_PLAYERS];
    initializeRoaches(roaches);

    message_t incomingMessage;
    reply_message_t replyMessage; //struct diferente?
    
    void *context = zmq_ctx_new ();
    void *responder = zmq_socket (context, ZMQ_REP);
    int rc = zmq_bind (responder, ADDR_MOVES);
    assert (rc == 0);

    //int n;
    //zmq_recv (responder, &n, sizeof(n), 0);
    //zmq_send (responder, &n, sizeof(n), 0);

    /* creates a window and draws a border */
    WINDOW * my_win = newwin(WINDOW_SIZE, WINDOW_SIZE, 0, 0);
    box(my_win, 0 , 0);	
	wrefresh(my_win);
    
    int bytes=0;
    while (1){
        bytes=zmq_recv (responder, &incomingMessage, sizeof(message_t), 0);
        if (bytes==-1){
           printf("Error: %d",errno);
           break;
        }else if (bytes=0){
           printf("No message Received");
           continue;
        }
        switch (abs(incomingMessage.msg_type)){
            case 0:
                treatRoachClientConect(incomingMessage,&replyMessage);
                break;
            case 1:
                treatConnectMessage(lizards,roaches,incomingMessage,deadRoaches,&replyMessage);
                break;
            case 2:
                treatMoveMessage(lizards,roaches,incomingMessage,deadRoaches,&replyMessage,my_win);
                break;
            case 3:
                treatDisconnectMessage(lizards,roaches,incomingMessage,&replyMessage);
                break;
            default:
                continue;
        }
        sendReply(responder,&replyMessage);
    }

	return 0;
}