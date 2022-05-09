#include <curl/curl.h>
#include <time.h>
#include <cstring>
#include <string>
#include <iostream>
#include <fstream>
#include "SCM.h"

size_t writeFunction(void* contents,size_t size,size_t nmemb,std::string* data){
    data->append((char*) contents,size*nmemb);

    return size*nmemb;
}

bool authSteamCheck(const char tempToken[STEAMCOOKIESIZE]){
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

    size_t found = str.find("true");
    
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

        //Append UTC with the format used for price history
        char bfr[23];
        strftime(bfr,23,"%b %d %Y %X\n",gmtime(&tmr));
        writeCSV<<str<<","<<bfr;

        return true;
    }
}

bool rebaseSCM(const char steamLoginSecure[STEAMCOOKIESIZE]){
    //Do not fetch more than 20 times per minute (cap at 15)
    //Price history uses UTC, which is [EDT+4]/[EST+5]
    //Price history uses median price

    //Convert price to USD if price_prefix!=$
    //Add repetition guard for multiple rebase results

    CURL* curl;
    CURLcode res;
    curl = curl_easy_init();
    std::ofstream writeCSV("./data/SCMData.csv",std::ios::app);
    std::string str;
    char pre[18+STEAMCOOKIESIZE] = "steamLoginSecure=";

    //Fetch JSON as string
    if(curl){
        curl_easy_setopt(curl,CURLOPT_FOLLOWLOCATION,1L);
        curl_easy_setopt(curl,CURLOPT_HTTPPROXYTUNNEL,1L);
        curl_easy_setopt(curl,CURLOPT_COOKIE,strcat(pre,steamLoginSecure));
        curl_easy_setopt(curl,CURLOPT_URL,"https://steamcommunity.com/market/pricehistory/?appid=730&market_hash_name=AK-47%20%7C%20Redline%20%28Field-Tested%29");
        curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,writeFunction);
        curl_easy_setopt(curl,CURLOPT_WRITEDATA,&str);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }

    size_t found = str.find("true");
    
    if(found==std::string::npos){
        return false;
    }

    else{
        //Insert currency guard

        found = str.find("[\"");

        while(found!=std::string::npos){
            str.erase(0,found+2);
            found = str.find(",\"")+2;
            size_t digits = found-str.find("\",")-1;

            std::cout<<","<<str.substr(found,str.find("\"]")-found)<<str.substr(found-digits,digits-1)<<str.substr(0,18)<<std::endl;
            writeCSV<<","<<str.substr(found,str.find("\"]")-found)<<str.substr(found-digits,digits-1)<<str.substr(0,18)<<std::endl;

            found = str.find("[\"");
        }

        return true;
    }
}
