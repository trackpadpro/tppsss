#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>
#include <time.h>
#include "controlTheory.h"
#include <curl/curl.h>

char input[6];
time_t tmr;
bool updatingCSV = true, steamCommunity = true, stockMarket = true;

int main(){
    long int unruh = time(&tmr);
    long int xtal = time(&tmr)-300;

    std::cout<<"Online."<<std::endl;

    //Go offline if data cannot regularly be pulled
    while(std::cin.good() && updatingCSV){
        std::cin>>std::setw(6)>>input;

        if(strcmp(input,"break")==0){
            break;
        }

        //Collect market data when non-break terminal command is given
        std::ofstream writeCSV("./data/marketData.csv");

        writeCSV<<"time"<<std::endl<<ctime(&tmr);

        if(time(&tmr)>xtal+300){
            if(steamCommunity){
                //steamCommunity = ;
            }

            if(stockMarket){
                //stockMarket = ;
            }

            xtal = time(&tmr);
        }

        if(steamCommunity && stockMarket){
            updatingCSV = true;
        }

        else{
            std::cout<<"Markets Unavailable "<<ctime(&tmr);
            updatingCSV = false;
        }
        
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

    std::cout<<"Offline."<<std::endl;

    return 0;
}