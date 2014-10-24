#include <stdio.h>


#define PAK 52
#define BOX 1
#define TOTAL_CARD PAK*BOX



const char * cards[] = { "K" , "A" , "2" , "3" , "4" , "5" , "6" , "7" , "8" , "9" , "10" ,
                         "J" , "Q" };
const char * color[] = { "S" , "H" , "D" , "C" , "C"};
/*
 * S - spade
 * H - heart
 * D - diamond
 * C - club
 */
int box[] = {0};
int remain = TOTAL_CARD; 
int user_point = 0;
int makers_point = 0;
int ** player = NULL;

int card_check(int num)
{
    int i;

    for (i = 0; i < TOTAL_CARD;i++)
    {
        if(box[i] == num )
            return 0;
        else if (box[i] == 0)
            return 1;
    }
    return -1;
}

void pak_init(void)
{
    int i , a;

    for(i = 0; i < TOTAL_CARD ; i++)
    {   
        do{
            a = (rand()%TOTAL_CARD + 1);
        }while(!card_check( a ));
        box[i] = a;
    }
}

int assign_card(void)
{
    int card;

    card = box[TOTAL_CARD - remain];
    if(card < 1)
        return -1;
    else{    
        remain--;
        return card;
    }
}

void blackjack_start(int *send_card)
{
    int y;

    for(y = 0 ; y < 2; y++)
    {
        send_card[y] = assign_card()%13;
        user_point = 2;
        makers_point = 2;
    }
}

int blackjack_judge(int *count,int point)
{
    int i,result = 0,tmp;
    for(i = 0; i < point;i++ )
    {   
        if (count[i] > 10 || count[i] == 0)
            tmp = 10;
        else
            tmp = count[i];
        result += tmp;
    }
    return result;
}

int blackjack_check(int *count,int point,int other)    
{
    int result = blackjack_judge(count,point); 

    printf("Count %d ",result);

    if(result < 21){
        printf("\n");
        return 0;
    }else if (result == 21 ){
        printf("[ Black Jack !! ]\n");
        if(other)
            printf("\t\t You Win !!\t\n");
        else
            printf("\t\t You lose !!\t\n");
        return 2;
    }else{
        printf("[ Burst !!]\n");
        if(other)
            printf("\t\t You lose !!\t\n");
        else
            printf("\t\t You Win !!\t\n");
        return 1;
    }
}

void blackjack_spelling(char * spelling,int *tmp_card,int point)
{
    int i;
    char tmp[128];
    for(i = 0; i < point;i++ )
    {
        sprintf(tmp,"[%s] ",cards[tmp_card[i]]);
        strcat(spelling,tmp);
    }
}

int blackjack_result(int *makers , int *user)
{
    int i,result;
    char makers_card[128] = {0};
    char user_card[128] = {0};
   
    blackjack_spelling(makers_card,makers,makers_point);
    blackjack_spelling(user_card,user,user_point);

    printf("Makers card  %s ",makers_card);
    result = (blackjack_check(makers,makers_point,0) > 0) ? 1 : 0;
    printf("Your card  %s ",user_card);
    result = (result) ? 1 : (blackjack_check(user,user_point,1) > 0 ) ? 1 : 0;
    return result;
}

int makers_play(int *user,int *makers)
{
    int result,count = 2;
    while(1)
    {
        printf("\n");
        result = blackjack_judge(makers,count);
        if (result > 17)
        {
            if(blackjack_judge(makers,count) > blackjack_judge(user,user_point))
                return 3;
            else if(blackjack_judge(makers,count) < blackjack_judge(user,user_point))
                return 4;

            return 1;
        }
        makers_point = count + 1;
        makers[count] = assign_card()%13;
        result = blackjack_result(makers,user);
        count++;
        if(result)return result;
        else if (count > 5){
            printf("\t\t You Lose !! \t\n");
            return result;
        }
    }
}
    
int user_play(int *user,int *makers)
{
    int count = 2,result = 0;
    char o;
    while(1)
    {   
        printf("Choose :  [A] Again [B] Pass \n");
        o = getchar();
        switch(o)
        {
            case 'a':
            case 'A':
                user_point = count + 1;
                user[count] = assign_card()%13;
                //printf("\n %d =%d=  \n",count,user[count]);
                result = blackjack_result(makers,user);
                count++;
                if(result)return result;
                else if (count > 5){
                    printf("\t\t You Win !! \t\n");
                    return result;
                }
                    
            break;

            case 'b':
            case 'B':
                return result;
            break;    
        }   
    }   
    return result;
}

void blackjack (void)
{
    int user[]={0};
    int makers[]={0};
    int result;
    char o;
    int count = 0;

    //sleep(2);
    while(1)
    {  
        printf("\n-------------------------------------\n");
        printf("Game Start !!\n");
        blackjack_start(makers); 
        blackjack_start(user);
        result = blackjack_result(makers,user);
        if(result)continue;

        result = user_play(user,makers);
        if(result)
            continue;

        result = makers_play(user,makers);
        if(result == 3)
            printf("\t\t You Lose !! \t\n");
        else if(result == 4)
            printf("\t\t You Win !! \t\n");
        continue;


        printf("-------------------------------------\n\n");
        break;
        /*
           for(i = 0; i < 5; i++)
           {
           user[i] = assign_card()%13;
           makers[i] = assign_card()%13;
           switch(i)
           {
           case 1:
           printf("Makers card [%d] / [%d]  Count %d\n",makers[i - 1 ],makers[i],(makers[i - 1 ] + makers[i]));
           printf("Your card [%d] / [%d]  Count %d\n",user[i - 1 ],user[i],(user[i - 1 ] + user[i]));
           break;
           }
           }*/
    }
}

void main (void)
{
    int i;
    char o;

    srand(time(NULL));

    pak_init();

    while(1)
    {
        printf("Option : \n");
        printf("1) Blackjack\n");
        o = getchar();
        switch(o)
        {
            case '1':
                blackjack();
            break;

            default:
                sleep(1);
            break;
        }
        break;
    }/*
    i = assign_card();
    printf("No. %d  Get %s%s\n",count,color[i/13],cards[i%13]);
    printf("The card box is empty!!\n");*/
}
