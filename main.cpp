#include <iostream>
#include <fstream> //I/O Files Library
#include <string>
#include <sstream> //Library helps to convert string into int
#include<stdlib.h> //For Random number generating function
#include<time.h> //For Time


using namespace std;


 struct CardsSet
    {
        int Type; //0 -> Stale Card //1 -> Rabbit //2 -> Turtle
        int State; //0 -> Not Deleted //1 -> Deleted
        string Question;
        char Answer; // a or b or c or d //if f means that card's question is canceled
        string Choice1;
        string Choice2;
        string Choice3;
        string Choice4;
    };
CardsSet GameCards[100];
//------------------------------------------------------------------------------------------------------------------------------------------------//
void FillCards () //Fill Cards
{
    ifstream QF_Type ("QF_Type.txt");
    for(int i=0 ; i<100 ; i++) //Input Type File
    {
        string line;
        getline (QF_Type,line);
        stringstream geek(line);
        geek >> GameCards[i].Type;
    }
    QF_Type.close();
    for(int i=0 ; i<100 ; i++) //Fill State 0
        GameCards[i].State = 0;
    ifstream QF_Questions ("QF_Questions.txt");
    for(int i=0 ; i<100 ; i++) //Input Questions File
        getline (QF_Questions,GameCards[i].Question);
    QF_Questions.close();
    ifstream QF_Choices ("QF_Choices.txt");
    for(int i=0 ; i<100 ; i++) //Input Choices File
    {
        QF_Choices >> GameCards[i].Choice1;
        QF_Choices >> GameCards[i].Choice2;
        QF_Choices >> GameCards[i].Choice3;
        QF_Choices >> GameCards[i].Choice4;
    }
    QF_Choices.close();
    ifstream QF_Answers ("QF_Answers.txt");
    for(int i=0 ; i<100 ; i++) //Input Answers File
    {
        QF_Answers >> GameCards[i].Answer;
    }
    QF_Answers.close();
}
//------------------------------------------------------------------------------------------------------------------------------------------------//
CardsSet Withdraw_A_Random_Card()
{
    CardsSet WithdrawedCard;
    srand( time( NULL ) );
    int Index = rand()%100;
    int QuadProbIndex = Index;
    int QuadProb = 1;
    if(GameCards[Index].State == 0)
    {
        GameCards[Index].State = 1; //Set Card as deleted
        WithdrawedCard = GameCards[Index];
    }
    else if(GameCards[QuadProbIndex].State == 1)
    {
        while(GameCards[QuadProbIndex].State == 1)
        {
            QuadProbIndex = (Index + QuadProb*QuadProb) % 100; //Using Quadratic Probing to access the undeleted cards
            QuadProb++;
        }
        GameCards[QuadProbIndex].State = 1; //Set Card as deleted
        WithdrawedCard = GameCards[QuadProbIndex];
    }
    return WithdrawedCard;
}
//------------------------------------------------------------------------------------------------------------------------------------------------//

int main ()
{
    FillCards();
    CardsSet CurrentCard;
    CurrentCard = Withdraw_A_Random_Card();
    //cout<<CurrentCard.Type<<endl<<CurrentCard.Question<<endl<<CurrentCard.Answer<<endl<<;
    cout<<GameCards[0].Type<<endl<<GameCards[0].State<<endl<<GameCards[0].Question<<endl<<GameCards[0].Answer<<endl;
    cout<<GameCards[0].Choice1<<endl<<GameCards[0].Choice2<<endl<<GameCards[0].Choice3<<endl<<GameCards[0].Choice4<<endl;
    cout<<GameCards[1].Type<<endl<<GameCards[1].State<<endl<<GameCards[1].Question<<endl<<GameCards[1].Answer<<endl;
    cout<<GameCards[1].Choice1<<endl<<GameCards[1].Choice2<<endl<<GameCards[1].Choice3<<endl<<GameCards[1].Choice4<<endl;
    /*for(int i=0 ; i<100 ; i++)
    {
        cout<<GameCards[i].Type<<" ";
    }*/
}
