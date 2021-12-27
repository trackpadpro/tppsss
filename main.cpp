#include <iostream>
#include <iomanip>
#include <cstring>
#include <time.h>
#include "controlTheory/controlTheory.h"
#include "webScraper/webScraper.h"

char input[6];
time_t tmr;
bool updatingCSV = true;

int main(){
    long int unruh = time(&tmr);
    webScraper* scraper;
    scraper = new webScraper(&tmr);

    std::cout<<"Online."<<std::endl;

    //Go offline if data cannot regularly be pulled
    while(std::cin.good() && updatingCSV){
        std::cin>>std::setw(6)>>input;

        if(strcmp(input,"break")==0){
            break;
        }

        //Collect market data when non-break terminal command is given
        updatingCSV = scraper->updateCSV();
        
        if(strcmp(input,"fetch")==0){
            //[call buy/sell calculation]
            //buy/sell calculation will occur within 
            //cout<<"buy:"<<endl;
            //cout<<"sell:"<<endl;
        }

        //Prevent extremely long commands from causing spam
        else if(time(&tmr)>unruh+1){
            std::cout<<"Unrecognized Command."<<std::endl;
            unruh = time(&tmr);
        }
    }

    delete scraper;

    std::cout<<"Offline."<<std::endl;

    return 0;
}