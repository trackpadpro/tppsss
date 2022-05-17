#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>
#include <string>
#include <time.h>
#include <curl/curl.h>
#include "SCM.h"
#include "stocks.h"
#include "controlTheory.h"

#define FETCHDELAY 15

char input[5], steamCookie[STEAMCOOKIESIZE];
time_t tmr;
size_t unruh = time(&tmr), xtal = time(&tmr)-FETCHDELAY;
bool updatingCSV = true, steamCommunity = true, stockMarket = true, cookieSCM = false;

void setup();

int main(){
    std::ifstream authSteam("./data/auth/steamLoginSecure.txt");

    if(authSteam.is_open()){
        char temp[STEAMCOOKIESIZE];
        authSteam.getline(temp,STEAMCOOKIESIZE+1);
        authSteam.close();

        if(authSteamCheck(temp)){
            std::copy(std::begin(temp),std::end(temp),std::begin(steamCookie));

            cookieSCM = true;
        }

        else{
            std::cout<<"steamLoginSecure corrupted"<<std::endl;
            setup();
        }
    }
    
    else{  
        std::cout<<"Unable to find steamLoginSecure. This cookie is used for fetching full item price history.\nWould you like to set it? [y/n] ";
        std::cin>>std::setw(2)>>input;

        if(strcmp(input,"y")==0){
            setup();
        }

        else if(strcmp(input,"n")==0){
            std::cout<<"The steamLoginSecure cookie can be set later using the \"setup\" command."<<std::endl;
        }

        else{
            std::cout<<"Unrecognized command. The steamLoginSecure cookie can be set later using the \"setup\" command."<<std::endl;
        }
    }

    if(cookieSCM){
        std::cout<<"Initializing SCM price history"<<std::endl;

        cookieSCM = rebaseSCM(tmr,steamCookie);

        if(!cookieSCM){
            std::cout<<"SCM rebase failed"<<std::endl;
        }
    }

    std::cout<<"Online"<<std::endl;

    //Go offline if data cannot regularly be pulled
    while(std::cin.good()&&updatingCSV){
        std::cin>>std::setw(6)>>input;

        if(strcmp(input,"break")==0){
            break;
        }

        //Collect market data when non-break terminal command is given
        if(time(&tmr)>xtal+FETCHDELAY){
            if(cookieSCM){
                cookieSCM = rebaseSCM(tmr,steamCookie);

                if(!cookieSCM){
                    std::cout<<"SCM rebase failed"<<std::endl;
                }
            }

            if(steamCommunity){
                steamCommunity = updateSCM(tmr);

                if(!steamCommunity){
                    std::cout<<"Steam Community Market unavailable "<<ctime(&tmr);
                }
            }

            if(stockMarket){
                stockMarket = updateStonks();

                if(!stockMarket){
                    std::cout<<"Stock market unavailable "<<ctime(&tmr);
                }
            }

            xtal = time(&tmr);
        }

        if(steamCommunity||stockMarket){
            updatingCSV = true;
        }

        else{
            std::cout<<"Markets unavailable "<<ctime(&tmr);
            updatingCSV = false;
        }
        
        if(strcmp(input,"fetch")==0){
            //[call buy/sell calculation]
            //Buy/sell calculation will utilize controlTheory.
        }

        else if(strcmp(input,"setup")==0){
            setup();
            unruh = time(&tmr);
        }

        //Prevent extremely long commands from causing spam
        else if(time(&tmr)>unruh+1){
            std::cout<<"Unrecognized command"<<std::endl;
            unruh = time(&tmr);
        }
    }

    std::cout<<"Offline"<<std::endl;

    return 0;
}

void setup(){
    std::cout<<"Would you like to set/reset your steamLoginSecure? [y/n] ";
    std::cin>>std::setw(2)>>input;

    if(strcmp(input,"y")==0){
        std::cout<<"Your steamLoginSecure cookie can be found in a browser that is logged into the Steam Community.\nPlease enter the content of the cookie: ";
        char temp[STEAMCOOKIESIZE];
        std::cin>>std::setw(STEAMCOOKIESIZE+1)>>temp;

        if(authSteamCheck(temp)){
            std::copy(std::begin(temp),std::end(temp),std::begin(steamCookie));

            std::ofstream authSteam("./data/auth/steamLoginSecure.txt");
            authSteam<<temp;

            std::cout<<"steamLoginSecure set"<<std::endl;
        }

        else{
            std::cout<<"Invalid input. Try again later using the \"setup\" command."<<std::endl;
        }

        cookieSCM = true;
    }

    else if(strcmp(input,"n")!=0){
        std::cout<<"Unrecognized command. The steamLoginSecure cookie can be set later using the \"setup\" command."<<std::endl;
    }
}
