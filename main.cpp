#include <iostream>
#include <iomanip>
#include <cstring>
#include <time.h>
using namespace std;

//[buy/sell declaration]

char input[6];
time_t tmr;
long int unruhe;

int main(){
    unruhe = time(&tmr);
    cout<<"online"<<endl;

    while(cin.good()){
        cin>>setw(6)>>input;
        if(strcmp(input,"break")==0){
            break;
        }
        else if(strcmp(input,"fetch")==0){
            //[call buy/sell calculation]
            //cout<<"buy:"<<endl;
            //cout<<"sell:"<<endl;
        }
        else if(time(&tmr)>unruhe+1){
            cout<<"unrecognized command"<<endl;
            unruhe = time(&tmr);
        }
    }

    cout<<"offline"<<endl;
    return 0;
}

//[implement buy/sell calculation]