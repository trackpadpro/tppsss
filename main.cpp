#include <iostream>
#include <iomanip>
#include <cstring>
#include <time.h>
#include "controlTheory/controlTheory.h"
#include "webScraper/webScraper.h"

char input[6];
time_t tmr;
long int unruh;
bool updatingCSV = true;

int main(){
    unruh = time(&tmr);
    std::cout<<"Online."<<std::endl;

    //Go offline if data cannot regularly be pulled
    while(updatingCSV){
        std::cin>>std::setw(6)>>input;
        if(strcmp(input,"break")==0){
            break;
        }
        updatingCSV = updateCSV(ctime(&tmr));
        if(strcmp(input,"fetch")==0){
            //[call buy/sell calculation]
            //cout<<"buy:"<<endl;
            //cout<<"sell:"<<endl;
        }
        //Prevent extremely long commands from causing 
        else if(time(&tmr)>unruh+1){
            std::cout<<"Unrecognized Command."<<std::endl;
            unruh = time(&tmr);
        }
    }

    std::cout<<"Offline."<<std::endl;
    return 0;
}