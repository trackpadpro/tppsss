#include <iostream>
#include <iomanip>
#include <cstring>
#include <time.h>

//[buy/sell declaration]

char input[6];
time_t tmr;
long int unruhe;

int main(){
    unruhe = time(&tmr);
    std::cout<<"online"<<std::endl;

    while(std::cin.good()){
        std::cin>>std::setw(6)>>input;
        if(strcmp(input,"break")==0){
            break;
        }
        else if(strcmp(input,"fetch")==0){
            //[call buy/sell calculation]
            //cout<<"buy:"<<endl;
            //cout<<"sell:"<<endl;
        }
        else if(time(&tmr)>unruhe+1){
            std::cout<<"unrecognized command"<<std::endl;
            unruhe = time(&tmr);
        }
    }

    std::cout<<"offline"<<std::endl;
    return 0;
}

//[implement buy/sell calculation]