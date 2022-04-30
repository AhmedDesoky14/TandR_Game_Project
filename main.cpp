#include <iostream> //for cout cin functions
#include <fstream> //I/O Files Library
#include <windows.h> //for clock timers
#include <string> //for strings
#include <sstream> //Library helps to convert string into int
#include<stdlib.h> //For Random number generating function
#include<time.h> //For Time
#include<cmath> //for math functions
#include<stdio.h>
#include <conio.h>
#define clrscr() system("cls"); // for clrscr(); function
#define getch() kbhit() //for pressing enter to start
#ifdef _WIN32
#include <windows.h>
#include <chrono>
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#define DISABLE_NEWLINE_AUTO_RETURN  0x0008

void activateVirtualTerminal()
{
    HANDLE handleOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD consoleMode;
    GetConsoleMode( handleOut , &consoleMode);
    consoleMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    consoleMode |= DISABLE_NEWLINE_AUTO_RETURN;
    SetConsoleMode( handleOut , consoleMode );
}
#endif
/*------------------------------------------------------------------------------------------------------------------------------------------*/
#define cards_number 60 //number of total cards
#define special_cards_number 10 //number of special cards
#define track_length 5 //track length
#define max_players 4 //max number of players to play
//#define Question_time 5 ////
/*-------------------------------------------------------------------------------------------------------------------------------------------*/
using namespace std;
enum COLORS
{
    NC=-1,
    BLACK,
    RED,
    GREEN,
    YELLOW,
    BLUE,
    MAGENTA,
    CYAN,
    WHITE,
};
/*-------------------------------------------------------------------------------------------------------------------------------------------*/
const char *colorize(int font, int back = -1, int style = -1)
{
    static char code[20];
    if (font >= 0)
        font += 30;
    else
        font = 0;
    if (back >= 0)
        back += 40;
    else
        back = 0;
    if (back > 0 && style > 0)
        sprintf(code, "\033[%d;%d;%dm", font, back, style);
    else if (back > 0)
        sprintf(code, "\033[%d;%dm", font, back);
    else
        sprintf(code, "\033[%dm", font);
    return code;
    // font -> typing color
    // back -> background color
    // style: 1 -> bold , 4 -> underline, etc
}
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
    string SPACE="";
    string track="";
};
/*------------------------------------------------------------------------------------------------------------------------------------------*/
class Turtle_Rabbit_Run{
public:
    CardsSet GameCards[cards_number];
    //int track[track_length];
    short number_of_players;
    player players[max_players];
    CardsSet *players_cards[max_players];
    int PRIME;
    Turtle_Rabbit_Run(){
        number_of_players=0;
        } //Constructor sets number of players = 0
//--------------------------------------------------------------------------------------------------------------------------------------------------------------//
void FillCards () ////Revised
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
int inline Hash(int value){
return value%cards_number;
}

/*----------------------------------------------------------------------------------------------------------------------------------------------------*/
int linear_probing(int value){
int probe= Hash(value), initialPos = probe;
        while(GameCards[probe].state == 0)
    {
        probe = (probe + 1) % cards_number;
        if(probe==initialPos)  // no more cards to be withdrawed
            return -1;
    }
        return probe;
}
/*------------------------------------------------------------------------------------------------------------------------------------------*/
CardsSet *Withdraw_A_Random_Card() //Linear Probing is best option
{
    srand( time( NULL ) );
    int probe=linear_probing(rand());
    if(probe==-1) {
        return NULL;}
    GameCards[probe].state = 0;
    return &(GameCards[probe]);
}
/*-------------------------------------------------------------------------------------------------------------------------------------------------------*/
int move_one_step(player *n_player) //Function to move 1 step ////Revised
{
    n_player->playerlocation += 1;
    n_player->SPACE=n_player->SPACE+" ";
    n_player->track=n_player->track+"-";
    return n_player->playerlocation;
}
/*-------------------------------------------------------------------------------------------------------------------------------------------------------*/
int move_two_steps( player *n_player ) //Function to move 2 steps ////Revised
{
    n_player->playerlocation += 2;
    n_player->SPACE=n_player->SPACE+"  ";
    n_player->track=n_player->track+"--";
    return n_player->playerlocation;
}
/*-------------------------------------------------------------------------------------------------------------------------------------------------------*/
int move_four_steps(player *n_player) //Function to move 4 steps ////Revised
{
    n_player->playerlocation += 4;
    n_player->SPACE=n_player->SPACE+"    ";
    n_player->track=n_player->track+"----";
    return n_player->playerlocation;
}
/*-------------------------------------------------------------------------------------------------------------------------------------------------------*/
int move_five_steps(player *n_player) //Function to move 5 steps ////Revised
{
    n_player->playerlocation += 5;
    n_player->SPACE=n_player->SPACE+"     ";
    n_player->track=n_player->track+"-----";
    return n_player->playerlocation;
}
/*------------------------------------------------------------------------------------------------------------------------------------------------------------*/
short take_players_data() ////Revised
{
    string number;
    while(true)
    {
        cout << colorize(CYAN, BLACK,1)<< "please enter number of players:" << colorize(NC)<< endl;
        cout << colorize(CYAN, BLACK, 1)<< "( 2 - 3 - 4 )" << colorize(NC)<<endl;
        cin >> number;
        cout<<endl;
        if(number!="2" && number!="3" && number!="4")
            cout<<endl<< colorize(RED, BLACK)<<"invalid data, make sure you choose a number between 2 and 4"<< colorize(NC)<<endl<<endl;
        else
            break;
    }
    stringstream geek(number);
    geek >> number_of_players; //convert string into int
    int color[max_players]={YELLOW,BLUE,RED,GREEN};
    for(short i=0;i<number_of_players; i++)
    {
        cout << colorize(color[i], BLACK,1)<< "please enter name of player_" <<i+1<<": "<< colorize(NC)<< endl;
        cin >> players[i].playername;
        cout<<endl;
    }
    return number_of_players;
}
/*--------------------------------------------------------------------------------------------------------------------------------------------------------*/
void cards_under_flow() ////Revised ////Function to end the game and declare the winner in case of cards underflow
{

    cout<< colorize(GREEN, BLACK,1)<<"GAME OVER !! "<< colorize(NC)<<endl<<endl;
    Sleep(2000);
    player *winner;
    winner = &(players[0]);
    for(short i=1;i<number_of_players;i++)
    {
        if(players[i].playerlocation>winner->playerlocation)
            winner = &(players[i]);
    }
    cout<< colorize(GREEN, BLACK,1)<<winner->playername<<" is the winner with total number of "<<winner->playerlocation<<" steps."<< colorize(NC)<<endl;
    Sleep(2000);
    clrscr();
}
/*-------------------------------------------------------------------------------------------------------------------------------------------------------*/
void print_card(char type)
{
    auto startprint = chrono::steady_clock::now();
    string SPACE="    ";
    cout<< colorize(YELLOW, BLACK) <<SPACE<< "- - - - - - - - - - - - - - " << colorize(NC)<< "\n";
    for(char i=0; i<5;i++)
    cout << colorize(YELLOW, BLACK)<<SPACE<< "-                         - " << colorize(NC)<< "\n";
    cout << colorize(YELLOW, BLACK)<<SPACE<< "-"<< colorize(NC);
    if(type=='T')
    cout << colorize(YELLOW, BLACK)<<"       TURTLE CARD       "<< colorize(NC);
    else if(type=='R')
    cout << colorize(YELLOW, BLACK)<<"       RABBIT CARD       "<< colorize(NC);
    else if(type=='F')
    cout<< colorize(YELLOW, BLACK) <<"       GOLDEN CARD       "<< colorize(NC);
    else if(type=='S')
    cout << colorize(YELLOW, BLACK)<<"       BONUS  CARD       "<< colorize(NC);
    else if(type=='N')
    cout << colorize(YELLOW, BLACK)<<"       STOP!! CARD       "<< colorize(NC);
    cout << colorize(YELLOW, BLACK)<<"- " << colorize(NC)<< "\n";
    for(char i=0; i<5;i++)
    cout << colorize(YELLOW, BLACK)<<SPACE<< "-                         - " << colorize(NC)<< "\n";
    cout << colorize(YELLOW, BLACK)<<SPACE<< "- - - - - - - - - - - - - - " << colorize(NC)<< "\n";

    auto endprint = chrono::steady_clock::now();
    cout <<endl<<"time taken in print card in microseconds: "<< chrono::duration_cast<chrono::microseconds>(endprint - startprint).count()<< " us" << endl;
    cout << "time taken in print card in seconds: "<< chrono::duration_cast<chrono::seconds>(endprint - startprint).count()<< " sec"<<endl;

}
/*-------------------------------------------------------------------------------------------------------------------------------------------------------*/
bool another_round() ////Revised
    {  auto start = chrono::steady_clock::now();
        for(short i=0;i<number_of_players;i++)
        {
            players_cards[i] = Withdraw_A_Random_Card();
            if(players_cards[i]==NULL)
            {
                cards_under_flow(); //call a function to end the game due to cards underflow
                return false;
            }
        }
        auto end = chrono::steady_clock::now();
        cout << "time taken to draw cards of players in microseconds: "<< chrono::duration_cast<chrono::microseconds>(end - start).count()<< " us" << endl;
        cout << "time taken to draw cards of players in seconds: "<< chrono::duration_cast<chrono::seconds>(end - start).count()<< " sec"<<endl;
        cout<<"-------------------------------------------------------------------------------------------------------------------"<<endl;

        return true;
    }
/*-------------------------------------------------------------------------------------------------------------------------------------------------------*/
 bool manage_card(CardsSet *card,player *player) ////Revised
 {

    cout<<endl<< colorize(YELLOW, BLACK)<<"**** Good Luck "<< player->playername<<" ^_^ ****"<< colorize(NC)<<endl<<endl;
    print_card(card->Type);
    if(card->Type=='T' || card->Type=='R')
    {
        string answer;
        cout<< colorize(YELLOW, BLACK)<<endl<<"Question is: "<< colorize(NC)<<endl<<endl;
        cout<< colorize(CYAN, BLACK,1)<<card->Question<< colorize(NC)<<endl;
        cout<< colorize(YELLOW, BLACK,1)<<"1-"<<card->Choice1<< colorize(NC)<<endl;
        cout<< colorize(YELLOW, BLACK,1)<<"2-"<<card->Choice2<< colorize(NC)<<endl;
        cout<< colorize(YELLOW, BLACK,1)<<"3-"<<card->Choice3<< colorize(NC)<<endl;
        cout<< colorize(YELLOW, BLACK,1)<<"4-"<<card->Choice4<< colorize(NC)<<endl;

/*
 int counter = 11 ; //amount of seconds
 Sleep(1000);
 while (counter-- >= 1)
 {
 cout << colorize(RED, BLACK)<< "\rTime remaining: " << counter<<" ... Choose the number for the correct answer (1-2-3-4): " <<colorize(NC) ;
 Sleep(1000);
 if (kbhit())
 {
 cin>>answer;
 if(answer!="1"&& answer!="2" && answer!="3" && answer!="4")
                cout<< colorize(RED, BLACK)<<endl<<"\ninvalid data, make sure you choose a number representing your choice between 1 and 4\n"<< colorize(NC)<<endl;
            else
                break;;
 }
 if (counter == 0)
 {
 cout << colorize(RED, BLACK)<< "\nSorry, time is up. hard luck next round" <<colorize(NC)<< endl;
 }
 }    */

       cout<< colorize(YELLOW, BLACK)<<endl<<"Choose the number for the correct answer (1-2-3-4): "<< colorize(NC);
        while(true)
        {
            cin>>answer;
            if(answer!="1"&& answer!="2" && answer!="3" && answer!="4")
                cout<< colorize(RED, BLACK)<<endl<<"invalid data, make sure you choose a number representing your choice between 1 and 4"<< colorize(NC)<<endl;
            else
                break;
        }
        short correct_choice;
        stringstream geek(answer);
          geek >> correct_choice;
        if(correct_choice == card->Answer)
        {
            player->correct_answers++;
            cout<< colorize(RED, BLACK)<<endl<<"congrats, "<<player->playername<<"! you got it right!"<< colorize(NC)<<endl<<endl;
            if(card->Type=='R')
            {
                if(move_four_steps(player)>=track_length)
                {
                    Winner(player);
                    return false;
                }
                display_player_track(player);
                cout << colorize(RED, BLACK)<< "You're great , you're four steps ahead " << colorize(NC)<< endl;
            }
            else
            {
                if (move_two_steps(player)>=track_length)
                {
                    Winner(player);
                    return false;
                }
                display_player_track(player);
                cout << colorize(RED, BLACK)<< "You're great , you're two steps ahead " << colorize(NC)<< endl;
            }
            cout<< colorize(RED, BLACK)<<endl<<"your new position is: "<< colorize(NC)<<player->playerlocation;
            Sleep(3000);
            clrscr();
        }
        else
        {
            player->wrong_answers++;
            cout<<endl<< colorize(RED, BLACK)<<"Wrong Answer"<< colorize(NC);
            cout<< colorize(RED, BLACK)<<", hard luck next time!"<< colorize(NC)<<endl<<endl;
            cout<<endl<< colorize(RED, BLACK)<<"your position is: "<<player->playerlocation<< colorize(NC);
            Sleep(3000);
            clrscr();
        }
    }
    else if(card->Type=='F')
    {
     //   cout<<endl<< colorize(GREEN, BLACK,1)<<player->playername<<", You have got the golden card!"<< colorize(NC)<<endl<<endl;
        if (move_five_steps(player)>=track_length)
        {
            Winner(player);
            return false;
        }
        display_player_track(player);
        cout << colorize(RED, BLACK)<< "Great , yous're five steps ahead " << colorize(NC)<< endl;

        cout<<endl<< colorize(RED, BLACK)<<"your new position is: "<<player->playerlocation<< colorize(NC); //else is not necessary
        Sleep(3000);
        clrscr();
    }
    else if(card->Type=='S')
    {
      //  cout<<endl<< colorize(RED, BLACK,1)<<player->playername<<", You have got the bonus card!"<< colorize(NC)<<endl<<endl;
        if (move_one_step(player)>=track_length)
        {
            Winner(player);
            return false;
        }
        display_player_track(player);
        cout << colorize(RED, BLACK)<< "Great , you're one step ahead " << colorize(NC)<< endl;
        cout<<endl<< colorize(RED, BLACK)<<"your new position is: "<<player->playerlocation<< colorize(NC); //else is not necessary
        Sleep(3000);
        clrscr();
    }
    else if(card->Type=='N')
    {
        cout<<endl<< colorize(RED, BLACK)<<player->playername<<" ,Sorry you are not part of this round"<< colorize(NC)<<endl<<endl;
        cout<<endl<< colorize(RED, BLACK)<<"your position is: "<<player->playerlocation<< colorize(NC);
        Sleep(3000);
        clrscr();
    }
    return true;
}
/*-------------------------------------------------------------------------------------------------------------------------------------------------*/
void Winner(player *winner) ////Revised
{
    cout<< colorize(GREEN, BLACK,1)<<"GAME OVER"<< colorize(NC)<<endl;
    cout<< colorize(GREEN, BLACK,1)<<"Great job, "<<winner->playername<<", you are the first player to end the track!"<< colorize(NC)<<endl;
    Sleep(3000);
    clrscr();
    cout<< colorize(CYAN, BLACK,1)<<"  "<<winner->playername<<" is the winner"<< colorize(NC)<<endl<<endl;
    Sleep(5000);
}
/*-------------------------------------------------------------------------------------------------------------------------------------------------*/
void display_achievements() ////Revised
{
    cout<<endl<< colorize(MAGENTA, BLACK, 1)<<"players achievements: "<< colorize(NC)<<endl<<endl;
    float full_mark;
    int color[max_players]={YELLOW,BLUE,RED,GREEN};
    for(int i=0;i<number_of_players;i++)
    {
        full_mark=players[i].correct_answers + players[i].wrong_answers;
        cout<< colorize(color[i], BLACK, 1)<<"-Player "<<players[i].playername<<" got the right answer of "<<players[i].correct_answers<<" questions out of "<< colorize(NC);
        cout<< colorize(color[i], BLACK, 1)<<full_mark<<" questions"<< colorize(NC)<<endl;
        cout<< colorize(color[i], BLACK, 1)<<"   with average score of "<<((players[i].correct_answers)/full_mark)*100<<"%"<< colorize(NC)<<endl<<endl;
    }
}
/*---------------------------------------------------------------------------------------------------------------------------------------------*/

void display_track()
{
    Sleep(1000);
    auto start = chrono::steady_clock::now();
    int color[max_players]={YELLOW,BLUE,RED,GREEN};
    for(char i=0;i<number_of_players;i++)
    {
        cout<< colorize(color[i], BLACK, 1)<<players[i].playername<<": \n\n";
        cout<< colorize(color[i], BLACK, 1)<<players[i].SPACE<<"(^_^)"<< colorize(NC)<<endl;
        cout<< colorize(color[i], BLACK, 1)<<players[i].SPACE<<"  |  "<< colorize(NC)<<endl;
        cout<< colorize(color[i], BLACK, 1)<<players[i].track<<">|||<"<< colorize(NC)<<endl;
        cout<< colorize(color[i], BLACK, 1)<<players[i].SPACE<<"  |  "<< colorize(NC)<<endl;
        cout<< colorize(color[i], BLACK, 1)<<players[i].SPACE<<" / \\ "<<players[i].playerlocation<< colorize(NC)<<endl<<endl;
        Sleep(2000);
    }
    auto end = chrono::steady_clock::now();
    cout << "time taken to display track in microseconds: "<< (chrono::duration_cast<chrono::microseconds>(end - start).count()) -number_of_players*2000000<< " us" << endl;
    Sleep(3000);
    clrscr();
}
/*---------------------------------------------------------------------------------------------------------------------------------------------*/
void display_player_track(player* player)
{
        Sleep(1000);
        cout<<endl;
        cout<< colorize(YELLOW, BLACK)<<player->SPACE<<"(^_^)"<< colorize(NC)<<endl;
        cout<< colorize(YELLOW, BLACK)<<player->SPACE<<"  |  "<< colorize(NC)<<endl;
        cout<< colorize(YELLOW, BLACK)<<player->track<<">|||<"<< colorize(NC)<<endl;
        cout<< colorize(YELLOW, BLACK)<<player->SPACE<<"  |  "<< colorize(NC)<<endl;
        cout<< colorize(YELLOW, BLACK)<<player->SPACE<<" / \\ "<<player->playerlocation<< colorize(NC)<<endl<<endl;
        Sleep(1000);
}
/*---------------------------------------------------------------------------------------------------------------------------------------------*/
void display_instructions() ////Revised
{   auto start = chrono::steady_clock::now();
    cout<< colorize(YELLOW, BLACK, 1)<<"Hello, this is "<< colorize(NC);
    cout<< colorize(CYAN, BLACK, 1)<<"Turtle-Rabbit-Run"<< colorize(NC);
    cout<< colorize(YELLOW, BLACK, 1)<<" game, the instructions for the game is pretty simple:"<< colorize(NC)<<endl<<endl;
    cout<< colorize(YELLOW, BLACK, 1)<<"-You can play it with one,two or even three of your friends."<<endl<<endl;
    cout<< colorize(YELLOW, BLACK, 1)<<"-Basically, it is something like a context between you and your friends testing your general knowledges"<< colorize(NC)<<endl<<endl;
    cout<< colorize(YELLOW, BLACK, 1)<<"-You play with total number of "<<cards_number<<" cards"<<endl<<endl;
    cout<< colorize(YELLOW, BLACK, 1)<<"-"<<cards_number-special_cards_number<<" of them are question cards"<<" and "<<special_cards_number<<" are special cards"<< colorize(NC)<<endl<<endl;
    //cout<<"-You play with total number of "<<cards_number<<" cards"<<endl<<endl;
    cout<< colorize(YELLOW, BLACK, 1)<< "-Question cards could be of type turtle or rabbit, special cards could be the golden card, "<< colorize(NC)<<endl;
    cout<< colorize(YELLOW, BLACK, 1)<<"the bonus card or the stop card."<< colorize(NC)<<endl<<endl;
    cout<< colorize(YELLOW, BLACK, 1)<<"-number of steps that each card allows you to move:"<< colorize(NC)<<endl;
    cout<< colorize(CYAN, BLACK, 1)<<"  1-the stop card: zero steps"<< colorize(NC)<<endl;
    cout<< colorize(CYAN, BLACK, 1)<<"  2-the bonus card: one step"<< colorize(NC)<<endl;
    cout<< colorize(CYAN, BLACK, 1)<<"  3-turtle card: two steps"<< colorize(NC)<<endl;
    cout<< colorize(CYAN, BLACK, 1)<<"  4-rabbit card: four steps"<< colorize(NC)<<endl;
    cout<< colorize(CYAN, BLACK, 1)<<"  5-the golden card: five steps"<< colorize(NC)<<endl<<endl;
    cout<< colorize(YELLOW, BLACK, 1)<<"Let's get started ^_^"<< colorize(NC)<<endl;
    cout<< colorize(CYAN, BLACK, 1)<<"--------------------------------------------------------------------------------------------------------------------"<<endl;
    auto end = chrono::steady_clock::now();
    cout << "time taken to display instructions in microseconds: "<< chrono::duration_cast<chrono::microseconds>(end - start).count()<< " us" << endl;
    cout<<"--------------------------------------------------------------------------------------------------------------------"<<endl;
    cout<< colorize(YELLOW, BLACK, 1)<<"Please press Enter"<< colorize(NC);
    cin.ignore();
    getch();
    clrscr();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------*/
};
int main ()
{ auto startgame = chrono::steady_clock::now();
#ifdef _WIN32
    activateVirtualTerminal();
#endif
    Turtle_Rabbit_Run game; //game object
    {
        auto start = chrono::steady_clock::now();
        game.FillCards();
        auto end = chrono::steady_clock::now();
        cout <<colorize(YELLOW, BLACK)<<"time taken in fillcards in microseconds: "<< chrono::duration_cast<chrono::microseconds>(end - start).count()<< " us" <<colorize(NC)<<endl;
        cout <<colorize(YELLOW, BLACK)<<"time taken in fillcards in seconds: "<< chrono::duration_cast<chrono::seconds>(end - start).count()<< " sec"<<colorize(NC)<<endl;
        cout<<"--------------------------------------------------------------------------------------------------------------------"<<endl;
    } //Filling game card from data files

    game.display_instructions(); //Function to display game instructions
    game.take_players_data(); //Function to take number of players & their data
    Sleep(1000);
    clrscr();
    int round=1;
    while(true)
    {
        if(!(game.another_round()))
            break;
        cout<< colorize(YELLOW, BLACK)<<"---------------- Round#"<<round++<<" ----------------"<< colorize(NC)<<endl;
        short i;
        for(i=0;i<game.number_of_players;i++)
        {
            if(!(game.manage_card(game.players_cards[i],&(game.players[i]))))
                break;
        }
        game.display_track();
        if(i!=game.number_of_players)
            break; ////what is the use of this statement?!
    }
    clrscr();
    game.display_achievements();
    auto endgame = chrono::steady_clock::now();
    //cout << "time taken in all game in nanoseconds: "<< chrono::duration_cast<chrono::nanoseconds>(endgame - startgame).count()<< " ns" << endl;
    cout << "time taken in all game in seconds: "<< chrono::duration_cast<chrono::seconds>(endgame - startgame).count()<< " sec"<<endl;
    return 0;
}


