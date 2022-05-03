#include <curl/curl.h>
#include <time.h>
#include <cstring>
#include <iostream>
#include <fstream>
#include "SCM.h"

size_t writeFunction(void* contents,size_t size,size_t nmemb,std::string* data){
    data->append((char*) contents,size*nmemb);

    return size*nmemb;
}

bool authSteamCheck(char tempToken[STEAMCOOKIESIZE]){
    if(strlen(tempToken)==STEAMCOOKIESIZE&&tempToken[17]=='%'&&tempToken[20]=='%'){
        //Do not use ::find_first_not_of to avoid converting to std::string
        short int index = strspn(tempToken,"0123456789");

        if(index==17){
            index = strspn(tempToken+18,"ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");

            if(index==2){
                index = strspn(tempToken+21,"ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");

                return index==STEAMCOOKIESIZE-21;
            }
        }
    }

    return false;
}

bool updateSCM(const time_t &tmr){
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
