#include <iostream>
#include <fstream> //I/O Files Library
#include <string>
#include <sstream> //Library helps to convert string into int
#include<stdlib.h> //For Random number generating function
#include<time.h> //For Time
#define cards_number 60
#define special_cards_number 10
using namespace std;
struct CardsSet{
        char Type; // T -> turtle Card // R -> Rabbit card // F -> Fast "golden card" // S -> Slow card //  0 -> No action // * -> already withdrawed
        string Question;
        char Answer; // 1 or 2 or 3 or 4
        string Choice1;
        string Choice2;
        string Choice3;
        string Choice4;
        };

class Turtle_Rabbit_Run{
public:
    CardsSet GameCards[cards_number];
//------------------------------------------------------------------------------------------------------------------------------------------------//

    void FillCards (string cards_type_file,string cards_questions_file,string cards_choices_file,string cards_answer_file) //Fill Cards
{
    ifstream QF_Type (cards_type_file);
    for(int i=0 ; i<cards_number ; i++) //Input Type File
    {
        string line;
        getline (QF_Type,line);
        stringstream geek(line);
        geek >> GameCards[i].Type;
    }
    QF_Type.close();
    ifstream QF_Questions (cards_questions_file);
    for(int i=0 ; i<cards_number-special_cards_number ; i++) //Input Questions File
        getline (QF_Questions,GameCards[i].Question);
    QF_Questions.close();
    ifstream QF_Choices (cards_choices_file);
    for(int i=0 ; i<cards_number-special_cards_number ; i++) //Input Choices File
    {
        QF_Choices >> GameCards[i].Choice1;
        QF_Choices >> GameCards[i].Choice2;
        QF_Choices >> GameCards[i].Choice3;
        QF_Choices >> GameCards[i].Choice4;
    }
    QF_Choices.close();
    ifstream QF_Answers (cards_answer_file);
    for(int i=0 ; i<cards_number-special_cards_number ; i++) //Input Answers File
    {
        QF_Answers >> GameCards[i].Answer;
    }
    QF_Answers.close();
}
   //------------------------------------------------------------------------------------------------------------------------------------------------//
    CardsSet *Withdraw_A_Random_Card()
{
    srand( time( NULL ) );
    int Index = rand()%cards_number;
    if(GameCards[Index].Type != '*')
    {
        GameCards[Index].Type = '*'; //Set Card as deleted
        return &(GameCards[Index]);
    }
    int linear_Prob_Index = Index;
    while(GameCards[Index].Type == '*'){
            linear_Prob_Index=(linear_Prob_Index+1)% cards_number;
            if(linear_Prob_Index==Index)  // no more cards to be withdrawed
                return NULL;
    }
    return &(GameCards[linear_Prob_Index]);
     }
};
int main ()
{


    Turtle_Rabbit_Run game;
    game.FillCards("QF_Type.txt","QF_Questions.txt","QF_Choices.txt","QF_Answers.txt");
    CardsSet *CurrentCard;
    CurrentCard = game.Withdraw_A_Random_Card();
    //cout<<CurrentCard.Type<<endl<<CurrentCard.Question<<endl<<CurrentCard.Answer<<endl<<;

    cout<<game.GameCards[0].Type<<endl<<game.GameCards[0].Question<<endl<<game.GameCards[0].Answer<<endl;
    cout<<game.GameCards[0].Choice1<<endl<<game.GameCards[0].Choice2<<endl<<game.GameCards[0].Choice3<<endl<<game.GameCards[0].Choice4<<endl;
    cout<<game.GameCards[1].Type<<endl<<game.GameCards[1].Question<<endl<<game.GameCards[1].Answer<<endl;
    cout<<game.GameCards[1].Choice1<<endl<<game.GameCards[1].Choice2<<endl<<game.GameCards[1].Choice3<<endl<<game.GameCards[1].Choice4<<endl;




    /*for(int i=0 ; i<100 ; i++)
    {
        cout<<GameCards[i].Type<<" ";
    }*/
}
