#include <iostream>
#include <fstream> //I/O Files Library
#include <string>
#include <sstream> //Library helps to convert string into int
#include<stdlib.h> //For Random number generating function
#include<time.h> //For Time
#include<cmath>
#define cards_number 60
#define special_cards_number 10
#define track_length 50
#define max_players 4
using namespace std;

struct CardsSet{
        char Type; // T -> turtle Card // R -> Rabbit card // F -> Fast "golden card" // S -> Slow card //  0 -> No action
        bool state=1; // 0 -> old // 1 -> new
        string Question;
        short Answer; // 1 or 2 or 3 or 4
        string Choice1;
        string Choice2;
        string Choice3;
        string Choice4;
        };


struct player
{
    string playername;
    short correct_answers=0;
    short wrong_answers=0;
    int playerlocation=0 ;
};


class Turtle_Rabbit_Run{

public:
    CardsSet GameCards[cards_number];
    //int track[track_length];
    short number_of_players;
    player players[max_players];
    CardsSet *players_cards[max_players];

    Turtle_Rabbit_Run(){number_of_players=0;
    }


void FillCards (string cards_type_file,string cards_questions_file,string cards_choices_file,string cards_answer_file)
{
    ifstream QF_Type (cards_type_file);
    for(int i=0 ; i<cards_number ; i++) //Input Type File
    {
        string line;
        getline(QF_Type,line);

       stringstream geek(line);
       geek>>GameCards[i].Type;
    }
    QF_Type.close();

    ifstream QF_Questions (cards_questions_file);
    for(int i=0 ; i<cards_number-special_cards_number ; i++) //Input Questions File
        getline (QF_Questions,GameCards[i].Question);
    QF_Questions.close();

    ifstream QF_Choices (cards_choices_file);
    for(int i=0 ; i<cards_number-special_cards_number ; i++) //Input Choices File
    {
       /* QF_Choices >> GameCards[i].Choice1;
        QF_Choices >> GameCards[i].Choice2;
        QF_Choices >> GameCards[i].Choice3;
        QF_Choices >> GameCards[i].Choice4;
        */
        getline (QF_Choices,GameCards[i].Choice1);
        getline (QF_Choices,GameCards[i].Choice2);
        getline (QF_Choices,GameCards[i].Choice3);
        getline (QF_Choices,GameCards[i].Choice4);
    }
    QF_Choices.close();
    ifstream QF_Answers (cards_answer_file);
    for(int i=0 ; i<cards_number-special_cards_number ; i++) //Input Answers File
    {
        QF_Answers >> GameCards[i].Answer;
    }
    QF_Answers.close();
}


CardsSet *Withdraw_A_Random_Card_quad()
{
    srand( time( NULL ) );
    int Index = rand()%cards_number;
    int Quadratic_Prob_Index = Index;
    int counter=1;
    while(GameCards[Quadratic_Prob_Index].state == 0){
            Quadratic_Prob_Index=(Index+int(pow(counter++,2))) % cards_number;
            if(Quadratic_Prob_Index==Index)  // no more cards to be withdrawed
                return NULL;
    }
    GameCards[Quadratic_Prob_Index].state = 0;
    return &(GameCards[Quadratic_Prob_Index]);
     }




CardsSet *Withdraw_A_Random_Card()
{
    srand( time( NULL ) );
    int Index = rand()%cards_number;
    int linear_Prob_Index = Index;
    while(GameCards[linear_Prob_Index].state == 0){
            linear_Prob_Index=(linear_Prob_Index+1) % cards_number;
            if(linear_Prob_Index==Index)  // no more cards to be withdrawed
                return NULL;
    }
    GameCards[linear_Prob_Index].state =0;
    return &(GameCards[linear_Prob_Index]);
     }


int move_one_step(player *n_player)
{
    n_player->playerlocation += 1;
    if(n_player->playerlocation >= track_length){
        cout << "We got a winner ! " << endl;
    }
    else
        cout << "You're great , you 're one step ahead " << endl;
    return n_player->playerlocation;
}


int move_two_steps( player *n_player )
{
    n_player->playerlocation += 2;
    if(n_player->playerlocation >= track_length)
        cout << "We got a winner ! " << endl;
    else
        cout << "You're great , you 're two steps ahead " << endl;
    return n_player->playerlocation;
}


int move_four_steps(player *n_player)
{
    n_player->playerlocation += 4;
    if(n_player->playerlocation >= track_length)
        cout << "We got a winner ! " << endl;
    else
        cout << "You're great , you 're four step ahead " << endl;
    return n_player->playerlocation;
}


int move_five_steps(player *n_player)
{
    n_player->playerlocation += 5;
    if(n_player->playerlocation >= track_length)
        cout << "We got a winner ! " << endl;
    else
        cout << "You're great , you 're five step ahead " << endl;
    return n_player->playerlocation;
}


int take_players_data(){
    string number;
    while(true){
    cout << "please enter number of players:" << endl;
    cout << "( 2 - 3 - 4 )" <<endl;
    cin >> number;
    cout<<endl;
    if(number!="2" && number!="3" && number!="4"){
        cout<<endl<<"invalid data, make sure you choose a number between 2 and 4"<<endl<<endl;
    }
    else
        break;
}
    stringstream geek(number);
    geek >> number_of_players;
    for(int i=0;i<number_of_players; i++){
            cout << "please enter name of player_" <<i+1<<": "<< endl;
            cin >> players[i].playername;
            cout<<endl;
    }
    return number_of_players;
    }



void cards_under_flow(){
    cout<<"GAME OVER "<<endl;
    player *winner;
    winner=&(players[0]);
    for(short i=1;i<number_of_players;i++){
        if(players[i].playerlocation>winner->playerlocation){
            winner=&(players[i]);
        }
    }
    cout<<winner->playername<<" is the winner with total number of "<<winner->playerlocation<<" steps."<<endl;
}


bool another_round(){
    for(short i=0;i<number_of_players;i++){
        players_cards[i]=Withdraw_A_Random_Card();
        if(players_cards[i]==NULL){
            cards_under_flow();
            return false;}
    }
    return true;
    }


 bool manage_card(CardsSet *card,player *player){
    cout<<"****Good Luck,"<< player->playername<<"^_^ ****"<<endl<<endl;
    if(card->Type=='T' || card->Type=='R'){
        string answer; bool rabbit;
        if(card->Type=='T')
        {cout<<player->playername<<",You have got the turtle card, you can still be on track if you get the answer right!"<<endl; rabbit=0;}
        else
        {
             cout<<player->playername<<",You have got the rabbit card, keep it up and get the answer right!"<<endl; rabbit=1;
        }
        cout<<endl<<"Question is: ";
        cout<<card->Question<<endl;
        cout<<"1-"<<card->Choice1<<endl;
        cout<<"2-"<<card->Choice2<<endl;
        cout<<"3-"<<card->Choice3<<endl;
        cout<<"4-"<<card->Choice4<<endl;
        cout<<endl<<"Choose the number for the correct answer (1-2-3-4): ";

        while(true){
            cin>>answer;
            if(answer!="1"&& answer!="2" && answer!="3" && answer!="4"){
                cout<<endl<<"invalid data, make sure you choose a number representing your choice between 1 and 4"<<endl;
            }
            else
                break;
            }

            short choice;
            stringstream geek(answer);
            geek >> choice;
            if(choice==card->Answer){
                player->correct_answers++;
                cout<<endl<<"congrats, "<<player->playername<<"! you got it right!"<<endl<<endl;
                if(rabbit){
                if(move_four_steps(player)>=track_length){
                    Winner(player);
                    return false;
                }
                }
                else{ if (move_two_steps(player)>=track_length){
                    Winner(player);
                    return false;
                }
                }
                    cout<<endl<<"your new position is: "<<player->playerlocation<<endl<<endl;

            }
            else {
                player->wrong_answers++;
                cout<<endl<<"Wrong Answer, hard luck with other cards!"<<endl<<endl;
            }

    }
    else if(card->Type=='F'){
        cout<<endl<<player->playername<<", You have got the golden card!"<<endl<<endl;
        if (move_five_steps(player)>=track_length){
            Winner(player);
            return false;
        }
        else
            cout<<endl<<"your new position is: "<<player->playerlocation<<endl<<endl;
    }

    else if(card->Type=='S'){
        cout<<endl<<player->playername<<", You have got the bonus card!"<<endl<<endl;
        if (move_one_step(player)>=track_length){
            Winner(player);
            return false;
        }
        else
            cout<<endl<<"your new position is: "<<player->playerlocation<<endl<<endl;
    }

    else if(card->Type=='0'){
        cout<<endl<<player->playername<<", You have got the stop card, you are not part of this round"<<endl<<endl;
    }

    else if(card->Type=='t'){
        cout<<endl<<player->playername<<", You have got the turtle plus card!"<<endl<<endl;
        if (move_two_steps(player)>=track_length){
            Winner(player);
            return false;
        }
        else
            cout<<endl<<"your new position is: "<<player->playerlocation<<endl<<endl;
    }

    else if(card->Type=='r'){
        cout<<endl<<player->playername<<", You have got the rabbit plus card!"<<endl<<endl;
        if (move_four_steps(player)>=track_length){
            Winner(player);
            return false;
        }
        else
            cout<<endl<<"your new position is: "<<player->playerlocation<<endl<<endl;
    }
    return true;
}


void Winner(player *winner){
cout<<"GAME OVER"<<endl;
cout<<"Great job, "<<winner->playername<<", you are the first player to end the track!"<<endl;
}


void display_achievements(){
cout<<endl<<"players achievements: "<<endl<<endl;
float full_mark;
for(int i=0;i<number_of_players;i++){
    full_mark=players[i].correct_answers+players[i].wrong_answers;
    cout<<"-Player "<<players[i].playername<<" got the right answer of "<<players[i].correct_answers<<" questions out of ";
    cout<<players[i].correct_answers+players[i].wrong_answers<<" questions"<<endl;
    cout<<"   with average score of "<<((players[i].correct_answers)/full_mark)*100<<"%"<<endl<<endl;
}
}


void display_instructions(){
cout<<"Hello, this is Turtle-Rabbit-Run game, the instructions for the game is pretty simple:"<<endl<<endl;
cout<<"-You can play it with one,two or even three of your friends."<<endl<<endl;
cout<<"-Basically, it is something like a context between you and your friends testing your general knowledges"<<endl<<endl;
cout<<"-You play with total number of "<<cards_number<<"cards"<<endl<<endl;
cout<<"-"<<cards_number-special_cards_number<<" of them are question cards"<<" and "<<cards_number<<" are special cards"<<endl<<endl;
cout<<"-You play with total number of "<<cards_number<<"cards"<<endl<<endl;
cout<<"-Question cards could be of type turtle or rabbit.special cards are fast card'the golden one',"<<endl;
cout<<"slow card, turtle plus card, rabbit plus card and the no action card."<<endl<<endl;
cout<<"-number of steps that each card allows you to move:"<<endl;
cout<<"  1-no action card: zero steps"<<endl;
cout<<"  2-slow card: only one step"<<endl;
cout<<"  3-turtle card: two steps"<<endl;
cout<<"  4-rabbit card: four steps"<<endl;
cout<<"  5-fast card: five steps"<<endl<<endl;
cout<<"Let's get started ^_^ !"<<endl;
cout<<endl<<"-------------------------------------"<<endl<<endl;

}

};
int main ()
{


    Turtle_Rabbit_Run game;
    game.FillCards("QF_Type.txt","QF_Questions.txt","QF_Choices.txt","QF_Answers.txt");
    game.display_instructions();
    short players_number=game.take_players_data();
   int round=1;
    while(true){
        cout<<"----------- Round#"<<round++<<" -----------"<<endl;
        if(!(game.another_round())){
            break;
        }
        //cout<<game.number_of_players<<endl;
        short i;
        for(i=0;i<game.number_of_players;i++){
            if(!(game.manage_card(game.players_cards[i],&(game.players[i])))){
                break;
            }
        cout<<endl<<endl;
        }
         if(i!=game.number_of_players)
            break;

    }
    game.display_achievements();




/*

    CurrentCard = game.Withdraw_A_Random_Card();*/
    //cout<<CurrentCard.Type<<endl<<CurrentCard.Question<<endl<<CurrentCard.Answer<<endl<<;
/*
    cout<<game.GameCards[0].Type<<endl<<game.GameCards[0].Question<<endl<<game.GameCards[0].Answer<<endl;
    cout<<game.GameCards[0].Choice1<<endl<<game.GameCards[0].Choice2<<endl<<game.GameCards[0].Choice3<<endl<<game.GameCards[0].Choice4<<endl;
    cout<<game.GameCards[1].Type<<endl<<game.GameCards[1].Question<<endl<<game.GameCards[1].Answer<<endl;
    cout<<game.GameCards[1].Choice1<<endl<<game.GameCards[1].Choice2<<endl<<game.GameCards[1].Choice3<<endl<<game.GameCards[1].Choice4<<endl;

*/

}


