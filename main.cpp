#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>
#include <string>
#include <time.h>
#include "controlTheory.h"
#include <curl/curl.h>

char input[6];
time_t tmr;
bool updatingCSV = true, steamCommunity = true, stockMarket = true;

size_t writeFunction(void* contents, size_t size, size_t nmemb, std::string* data);
bool updateSCM();

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
        steamCommunity = updateSCM();

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

size_t writeFunction(void* contents, size_t size, size_t nmemb, std::string* data) {
    data->append((char*) contents, size * nmemb);

    return size * nmemb;
}

bool updateSCM(){
    CURL* curl;
    CURLcode res;
    curl = curl_easy_init();
    std::ofstream writeCSV("./data/marketData.csv");
    std::string readString;

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "http://steamcommunity.com/market/priceoverview/?market_hash_name=AK-47%20%7C%20Redline%20%28Field-Tested%29&appid=730&currency=1");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFunction);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readString);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }

    writeCSV<<readString<<"time"<<std::endl<<ctime(&tmr);

    return true;
}