#include <iostream> //for cout cin functions
#include <fstream> //I/O Files Library
# include <windows.h> //for clock timers
#include <string> //for strings
#include <sstream> //Library helps to convert string into int
#include<stdlib.h> //For Random number generating function
#include<time.h> //For Time
#include<cmath> //for math functions
#include<stdio.h>
#include <conio.h>
#define clrscr() system("cls"); // for clrscr(); function
#define getch() kbhit() //for pressing enter to start
using namespace std;
/*------------------------------------------------------------------------------------------------------------------------------------------*/
#define cards_number 60 //number of total cards
#define special_cards_number 10 //number of special cards
#define track_length 50 //track length
#define max_players 4 //max number of players to play
//#define Question_time 5 ////
/*-------------------------------------------------------------------------------------------------------------------------------------------*/
struct CardsSet
{
    char Type; // T -> turtle Card // R -> Rabbit card // F -> Fast "golden card" // S -> bonus card //  N -> No action
    bool state = 1; // 0 -> Withdrawed // 1 -> New
    string Question;
    short Answer; // 1 or 2 or 3 or 4
    string Choice1;
    string Choice2;
    string Choice3;
    string Choice4;
};
/*-----------------------------------------------------------------------------------------------------------------------------------------*/
struct player
{
    string playername;
    short correct_answers=0;
    short wrong_answers=0;
    int playerlocation=0 ;
};
/*------------------------------------------------------------------------------------------------------------------------------------------*/
class Turtle_Rabbit_Run{
public:
    CardsSet GameCards[cards_number];
    //int track[track_length];
    short number_of_players;
    player players[max_players];
    CardsSet *players_cards[max_players];
    Turtle_Rabbit_Run(){number_of_players=0;} //Constructor sets number of players = 0
//--------------------------------------------------------------------------------------------------------------------------------------------------------------//
void FillCards (string cards_type_file,string cards_questions_file,string cards_choices_file,string cards_answer_file) ////Revised ////"QF.TXT Need to be edited////
{
    ifstream QF_Type ("QF_Type.txt");
    for(int i=0 ; i<cards_number ; i++) //Input Type File
    {
        string line;
        getline(QF_Type,line);
       stringstream geek(line);
       geek>>GameCards[i].Type;
    }
    QF_Type.close();
    ifstream QF_Questions ("QF_Questions.txt");
    for(int i=0 ; i<cards_number-special_cards_number ; i++) //Input Questions File //Note: First 50 cards are the questions cards
        getline (QF_Questions,GameCards[i].Question);
    QF_Questions.close();
    ifstream QF_Choices ("QF_Choices.txt");
    for(int i=0 ; i<cards_number-special_cards_number ; i++) //Input Choices File //Note: First 50 cards are the questions cards
    {
        getline (QF_Choices,GameCards[i].Choice1); //If still needed
        getline (QF_Choices,GameCards[i].Choice2);
        getline (QF_Choices,GameCards[i].Choice3);
        getline (QF_Choices,GameCards[i].Choice4);
    }
    QF_Choices.close();
    ifstream QF_Answers ("QF_Answers.txt");
    for(int i=0 ; i<cards_number-special_cards_number ; i++) //Input Answers File //Note: First 50 cards are the questions cards
        QF_Answers >> GameCards[i].Answer;
    QF_Answers.close();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------*/
CardsSet *Withdraw_A_Random_Card() //Linear Probing is best option
{
    srand( time( NULL ) );
    int Index = rand()%cards_number;
    int linear_Prob_Index = Index;
    while(GameCards[linear_Prob_Index].state == 0)
    {
        linear_Prob_Index = (linear_Prob_Index+1) % cards_number;
        if(linear_Prob_Index==Index)  // no more cards to be withdrawed
            return NULL;
    }
    GameCards[linear_Prob_Index].state = 0;
    return &(GameCards[linear_Prob_Index]);
}
/*-------------------------------------------------------------------------------------------------------------------------------------------------------*/
int move_one_step(player *n_player) //Function to move 1 step ////Revised
{
    n_player->playerlocation += 1;
    return n_player->playerlocation;
}
/*-------------------------------------------------------------------------------------------------------------------------------------------------------*/
int move_two_steps( player *n_player ) //Function to move 2 steps ////Revised
{
    n_player->playerlocation += 2;
    return n_player->playerlocation;
}
/*-------------------------------------------------------------------------------------------------------------------------------------------------------*/
int move_four_steps(player *n_player) //Function to move 4 steps ////Revised
{
    n_player->playerlocation += 4;
    return n_player->playerlocation;
}
/*-------------------------------------------------------------------------------------------------------------------------------------------------------*/
int move_five_steps(player *n_player) //Function to move 5 steps ////Revised
{
    n_player->playerlocation += 5;
    return n_player->playerlocation;
}
/*------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void take_players_data() ////Revised
{
    string number;
    while(true)
    {
        cout << "please enter number of players:" << endl;
        cout << "( 2 - 3 - 4 )" <<endl;
        cin >> number;
        cout<<endl;
        if(number!="2" && number!="3" && number!="4")
            cout<<endl<<"invalid data, make sure you choose a number between 2 and 4"<<endl<<endl;
        else
            break;
    }
    stringstream geek(number);
    geek >> number_of_players; //convert string into int
    for(short i=0;i<number_of_players; i++)
    {
        cout << "please enter name of player " <<i+1<<": "<< endl;
        cin >> players[i].playername;
        cout<<endl;
    }
}
/*--------------------------------------------------------------------------------------------------------------------------------------------------------*/
void cards_under_flow() ////Revised ////Function to end the game and declare the winner in case of cards underflow
{
    cout<<"GAME OVER "<<endl;
    player *winner;
    winner = &(players[0]);
    for(short i=1;i<number_of_players;i++)
    {
        if(players[i].playerlocation>winner->playerlocation)
            winner = &(players[i]);
    }
    cout<<winner->playername<<" is the winner with total number of "<<winner->playerlocation<<" steps."<<endl;
}
/*-------------------------------------------------------------------------------------------------------------------------------------------------------*/
bool another_round() ////Revised
    {
        for(short i=0;i<number_of_players;i++)
        {
            players_cards[i] = Withdraw_A_Random_Card();
            if(players_cards[i]==NULL)
            {
                cards_under_flow(); //call a function to end the game due to cards underflow
                return false;
            }
        }
        return true;
    }
/*-------------------------------------------------------------------------------------------------------------------------------------------------------*/
 bool manage_card(CardsSet *card,player *player) ////Revised
 {
    cout<<"**** Good Luck,"<< player->playername<<"^_^ ****"<<endl<<endl;
    if(card->Type=='T' || card->Type=='R')
    {
        string answer;
        bool rabbit;
        if(card->Type=='T')
        {
            cout<<player->playername<<",You have got a turtle card, you can still be on track if you get the answer right!"<<endl;
            rabbit=0;
        }
        else
        {
            cout<<player->playername<<",You have got a rabbit card, keep it up and get the answer right!"<<endl;
            rabbit=1;
        }
        cout<<endl<<"Question is: ";
        cout<<card->Question<<endl;
        cout<<"1-"<<card->Choice1<<endl;
        cout<<"2-"<<card->Choice2<<endl;
        cout<<"3-"<<card->Choice3<<endl;
        cout<<"4-"<<card->Choice4<<endl;
        cout<<endl<<"Choose the number for the correct answer (1-2-3-4): ";
        while(true)
        {
            cin>>answer;
            if(answer!="1"&& answer!="2" && answer!="3" && answer!="4")
                cout<<endl<<"invalid data, make sure you choose a number representing your choice between 1 and 4"<<endl;
            else
                break;
        }
        short correct_choice;
        stringstream geek(answer);
        geek >> correct_choice;
        if(correct_choice == card->Answer)
        {
            player->correct_answers++;
            cout<<endl<<"congrats, "<<player->playername<<"! you got it right!"<<endl<<endl;
            if(rabbit)
            {
                if(move_four_steps(player)>=track_length)
                {
                    Winner(player);
                    return false;
                }
                cout << "You're great , you 're four step ahead " << endl;
            }
            else
            {
                if (move_two_steps(player)>=track_length)
                {
                    Winner(player);
                    return false;
                }
                cout << "You're great , you're two steps ahead " << endl;
            }
            cout<<endl<<"your new position is: "<<player->playerlocation;
            Sleep(3000);
            clrscr();
        }
        else
        {
            player->wrong_answers++;
            cout<<endl<<"Wrong Answer, hard luck next time!"<<endl<<endl;
            cout<<endl<<"your position is: "<<player->playerlocation;
            Sleep(3000);
            clrscr();
        }
    }
    else if(card->Type=='F')
    {
        cout<<endl<<player->playername<<", You have got the golden card!"<<endl<<endl;
        if (move_five_steps(player)>=track_length)
        {
            Winner(player);
            return false;
        }
        cout << "Great , you 're five step ahead " << endl;
        cout<<endl<<"your new position is: "<<player->playerlocation; //else is not necessary
        Sleep(3000);
        clrscr();
    }
    else if(card->Type=='S')
    {
        cout<<endl<<player->playername<<", You have got the bonus card!"<<endl<<endl;
        if (move_one_step(player)>=track_length)
        {
            Winner(player);
            return false;
        }
        cout << "Great , you're one step ahead " << endl;
        cout<<endl<<"your new position is: "<<player->playerlocation; //else is not necessary
        Sleep(3000);
        clrscr();
    }
    else if(card->Type=='N')
    {
        cout<<endl<<player->playername<<", You have got the stop card, you are not part of this round"<<endl<<endl;
        cout<<endl<<"your position is: "<<player->playerlocation;
        Sleep(3000);
        clrscr();
    }
    return true;
}
/*-------------------------------------------------------------------------------------------------------------------------------------------------*/
void Winner(player *winner) ////Revised
{
    cout<<"GAME OVER"<<endl;
    cout<<"Great job, "<<winner->playername<<", you are the first player to end the track!"<<endl;
    Sleep(3000);
    clrscr();
    cout<<winner->playername<<" is the winner"<<endl;
    Sleep(5000);
}
/*-------------------------------------------------------------------------------------------------------------------------------------------------*/
void display_achievements() ////Revised
{
    cout<<endl<<"players achievements: "<<endl<<endl;
    float full_mark;
    for(int i=0;i<number_of_players;i++)
    {
        full_mark=players[i].correct_answers + players[i].wrong_answers;
        cout<<"-Player "<<players[i].playername<<" got the right answer of "<<players[i].correct_answers<<" questions out of ";
        cout<<full_mark<<" questions"<<endl;
        cout<<"   with average score of "<<((players[i].correct_answers)/full_mark)*100<<"%"<<endl<<endl;
    }
}
/*---------------------------------------------------------------------------------------------------------------------------------------------*/
void display_instructions() ////Revised
{
    cout<<"Hello, this is Turtle-Rabbit-Run game, the instructions for the game is pretty simple:"<<endl<<endl;
    cout<<"-You can play it with one,two or even three of your friends."<<endl<<endl;
    cout<<"-Basically, it is something like a context between you and your friends testing your general knowledges"<<endl<<endl;
    cout<<"-You play with total number of "<<cards_number<<" cards"<<endl<<endl;
    cout<<"-"<<cards_number-special_cards_number<<" of them are question cards"<<" and "<<special_cards_number<<" are special cards"<<endl<<endl;
    //cout<<"-You play with total number of "<<cards_number<<" cards"<<endl<<endl;
    cout<<"-Question cards could be of type turtle or rabbit, special cards could be the golden card, "<<endl;
    cout<<"the bonus card or the stop card."<<endl<<endl;
    cout<<"-number of steps that each card allows you to move:"<<endl;
    cout<<"  1-the stop card: zero steps"<<endl;
    cout<<"  2-the bonus card: one step"<<endl;
    cout<<"  3-turtle card: two steps"<<endl;
    cout<<"  4-rabbit card: four steps"<<endl;
    cout<<"  5-the fast card: five steps"<<endl<<endl;
    cout<<"Let's get started ^_^"<<endl;
    cout<<"--------------------------------------------------------------------------------------------------------------------"<<endl;
    cout<<"Please press enter"<<endl;
    cin.ignore();
    getch();
    clrscr();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------*/
};
int main ()
{
    Turtle_Rabbit_Run game; //game object
    game.FillCards("QF_Type.txt","QF_Questions.txt","QF_Choices.txt","QF_Answers.txt"); //passing game data files
    game.display_instructions(); //Function to display game instructions
    game.take_players_data(); //Function to take number of players & their data
    Sleep(1000);
    clrscr();
    int round=1;
    while(true)
    {
        cout<<"---------------- Round#"<<round++<<" ----------------"<<endl;
        if(!(game.another_round()))
            break;
        //cout<<game.number_of_players<<endl;
        short i;
        for(i=0;i<game.number_of_players;i++)
        {
            if(!(game.manage_card(game.players_cards[i],&(game.players[i]))))
                break;
        }
        if(i!=game.number_of_players)
            break; ////what is the use of this statement?!
    }
    clrscr();
    game.display_achievements();
    getch();
    return 0;
}
