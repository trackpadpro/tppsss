#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>
#include <string>
#include <time.h>
#include "controlTheory.h"
#include <curl/curl.h>

#define FETCHDELAY 120

char input[6];
time_t tmr;
bool updatingCSV = true, steamCommunity = true, stockMarket = true;

size_t writeFunction(void* contents,size_t size,size_t nmemb,std::string* data);
bool updateSCM();
bool updateStonks();

int main(){
    long int unruh = time(&tmr);
    long int xtal = time(&tmr)-FETCHDELAY;

    std::cout<<"Online"<<std::endl;

    //Go offline if data cannot regularly be pulled
    while(std::cin.good()&&updatingCSV){
        std::cin>>std::setw(6)>>input;

        if(strcmp(input,"break")==0){
            break;
        }

        //Collect market data when non-break terminal command is given
        if(time(&tmr)>xtal+FETCHDELAY){
            if(steamCommunity){
                steamCommunity = updateSCM();
            }

            if(stockMarket){
                stockMarket = updateStonks();
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
            //buy/sell calculation will occur within 
            //cout<<"buy:"<<endl;
            //cout<<"sell:"<<endl;
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

size_t writeFunction(void* contents,size_t size,size_t nmemb,std::string* data){
    data->append((char*) contents,size*nmemb);

    return size*nmemb;
}

bool updateSCM(){
    CURL* curl;
    CURLcode res;
    curl = curl_easy_init();
    std::ofstream writeCSV("./data/SCMData.csv",std::ios::app);
    std::string str;

    //Fetch JSON as string
    if(curl){
	    curl_easy_setopt(curl,CURLOPT_FOLLOWLOCATION,1L);
	    curl_easy_setopt(curl,CURLOPT_HTTPPROXYTUNNEL,1L);
        curl_easy_setopt(curl,CURLOPT_URL,"http://steamcommunity.com/market/priceoverview/?market_hash_name=AK-47%20%7C%20Redline%20%28Field-Tested%29&appid=730&currency=1");
        curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,writeFunction);
        curl_easy_setopt(curl,CURLOPT_WRITEDATA,&str);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }

    std::size_t found = str.find("true");
    
    if(found==std::string::npos){
        std::cout<<"Steam Community Market unavailable"<<std::endl;

        return false;
    }

    else{
        str.erase(0,33);
        found = str.find("volume");
        str.erase(found-3,1);
        str.erase(found-2,10);
        found = str.find("median");
        str.erase(found-3,1);
        str.erase(found-2,17);
        str.pop_back();
        str.pop_back();

        writeCSV<<str<<","<<ctime(&tmr);

        return true;
    }
}

bool updateStonks(){
    std::cout<<"Stock market unavailable"<<std::endl;

    return false;
}
