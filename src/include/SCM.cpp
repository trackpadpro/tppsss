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
    //Price overview uses 24hr volume with a comma if above 1000; thrown out. 

    CURL* curl;
    CURLcode res;
    curl = curl_easy_init();
    std::ofstream writeCSV("./data/SCMData.csv",std::ios::app);
    std::string str;

    //Fetch JSON as string
    if(curl){
        curl_easy_setopt(curl,CURLOPT_FOLLOWLOCATION,1L);
        curl_easy_setopt(curl,CURLOPT_HTTPPROXYTUNNEL,1L);
        curl_easy_setopt(curl,CURLOPT_URL,"https://steamcommunity.com/market/priceoverview/?market_hash_name=Dreams%20%26%20Nightmares%20Case&appid=730&currency=1");
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
        found = str.find("median");
        str.erase(0,found+16);
        str.pop_back();
        str.pop_back();

        //Append UTC with the format used for price history
        char bfr[23];
        strftime(bfr,23,"%b %d %Y %X\n",gmtime(&tmr));
        writeCSV<<","<<str<<","<<bfr;

        return true;
    }
}

bool rebaseSCM(const time_t &tmr,const char steamLoginSecure[STEAMCOOKIESIZE]){
    //Do not call function more than 20 times per minute using the same steamLoginSecure cookie (cap at 15)
    //Price history uses UTC, which is [EDT+4]/[EST+5]
    //Price history uses median price

    //Convert price to USD if price_prefix!=$

    //Check if enough time has elapsed since last rebase
    std::ifstream rCSV("./data/SCMData.csv");
    char line[63], lline[63] = "", year[5], month[3], day[3], hour[3], lyear[5] = "0000", lmonth[3] = "00", lday[3] = "00", lhour[3] = "00", clmonth[4];

    if(rCSV.good()){
        strftime(year,5,"%Y",gmtime(&tmr));
        strftime(month,3,"%m",gmtime(&tmr));
        strftime(day,3,"%d",gmtime(&tmr));
        strftime(hour,3,"%H",gmtime(&tmr));

        while(rCSV.getline(line,64)){
            if(strstr(line,"+0")!=NULL){
                strcpy(lline,line);
            }
        }

        rCSV.close();

        //Assign latest rebase to l time if there was a previous rebase
        if(lline[0]!='\0'){
            char* dateComma = strrchr(lline,',');
            strncpy(lyear,dateComma+8,4);
            lyear[4] = '\0';
                
            //Fail rebase if CSV has future price entries
            if(strcmp(year,lyear)<0){
                return false;
            }

            else if(strcmp(year,lyear)==0){
                strncpy(clmonth,dateComma+1,3);
                clmonth[3] = '\0';

                if(strcmp(clmonth,"Jan")==0){
                    strcpy(lmonth,"01");
                }

                else if(strcmp(clmonth,"Feb")==0){
                    strcpy(lmonth,"02");
                }

                else if(strcmp(clmonth,"Mar")==0){
                    strcpy(lmonth,"03");
                }

                else if(strcmp(clmonth,"Apr")==0){
                    strcpy(lmonth,"04");
                }

                else if(strcmp(clmonth,"May")==0){
                    strcpy(lmonth,"05");
                }

                else if(strcmp(clmonth,"Jun")==0){
                    strcpy(lmonth,"06");
                }

                else if(strcmp(clmonth,"Jul")==0){
                    strcpy(lmonth,"07");
                }

                else if(strcmp(clmonth,"Aug")==0){
                    strcpy(lmonth,"08");
                }

                else if(strcmp(clmonth,"Sep")==0){
                    strcpy(lmonth,"09");
                }

                else if(strcmp(clmonth,"Oct")==0){
                    strcpy(lmonth,"10");
                }

                else if(strcmp(clmonth,"Nov")==0){
                    strcpy(lmonth,"11");
                }

                else if(strcmp(clmonth,"Dec")==0){
                    strcpy(lmonth,"12");
                }

                //Fail rebase if l time is not properly formatted
                else{
                    return false;
                }

                //Fail rebase if CSV has future price entries
                if(strcmp(month,lmonth)<0){
                    return false;
                }
                
                if(strcmp(month,lmonth)==0){
                    strncpy(lday,dateComma+5,2);
                    lday[2] = '\0';
                    strncpy(lhour,dateComma+13,2);
                    lhour[2] = '\0';

                    //Fail rebase if CSV has future price entries
                    if(strcmp(day,lday)<0){
                        return false;
                    }

                    else if(strcmp(day,lday)==0&&strcmp(hour,lhour)<0){
                        return false;
                    }

                    //Skip rebase if CSV is up-to-date
                    else if(strcmp(hour,lhour)==0&&strcmp(day,lday)==0){
                        return true;
                    }
                }
            }
        }
    }
    
    CURL* curl;
    CURLcode res;
    curl = curl_easy_init();
    std::string str, mdate;
    char pre[18+STEAMCOOKIESIZE] = "steamLoginSecure=";

    //Fetch JSON as string
    if(curl){
        curl_easy_setopt(curl,CURLOPT_FOLLOWLOCATION,1L);
        curl_easy_setopt(curl,CURLOPT_HTTPPROXYTUNNEL,1L);
        curl_easy_setopt(curl,CURLOPT_COOKIE,strcat(pre,steamLoginSecure));
        curl_easy_setopt(curl,CURLOPT_URL,"https://steamcommunity.com/market/pricehistory/?appid=730&market_hash_name=Dreams%20%26%20Nightmares%20Case");
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
        std::ofstream wCSV("./data/SCMData.csv",std::ios::app);

        while(found!=std::string::npos){
            str.erase(0,found+2);
            found = str.find(",\"")+2;
            size_t digits = found-str.find("\",")-1;
            mdate = str.substr(0,18);
            const char* mdt = mdate.c_str();

            strncpy(year,mdt+7,4);
            year[4] = '\0';

            //Prevent repeat entries
            if(strcmp(lyear,year)<0){
                wCSV<<str.substr(found,str.find("\"]")-found)<<str.substr(found-digits,digits-1)<<mdate<<std::endl;
            }

            if(strcmp(lyear,year)==0){
                strncpy(clmonth,mdt,3);
                clmonth[3] = '\0';

                if(strcmp(clmonth,"Jan")==0){
                    strcpy(month,"01");
                }

                else if(strcmp(clmonth,"Feb")==0){
                    strcpy(month,"02");
                }

                else if(strcmp(clmonth,"Mar")==0){
                    strcpy(month,"03");
                }

                else if(strcmp(clmonth,"Apr")==0){
                    strcpy(month,"04");
                }

                else if(strcmp(clmonth,"May")==0){
                    strcpy(month,"05");
                }

                else if(strcmp(clmonth,"Jun")==0){
                    strcpy(month,"06");
                }

                else if(strcmp(clmonth,"Jul")==0){
                    strcpy(month,"07");
                }

                else if(strcmp(clmonth,"Aug")==0){
                    strcpy(month,"08");
                }

                else if(strcmp(clmonth,"Sep")==0){
                    strcpy(month,"09");
                }

                else if(strcmp(clmonth,"Oct")==0){
                    strcpy(month,"10");
                }

                else if(strcmp(clmonth,"Nov")==0){
                    strcpy(month,"11");
                }

                else if(strcmp(clmonth,"Dec")==0){
                    strcpy(month,"12");
                }

                else{
                    return false;
                }

                if(strcmp(lmonth,month)<0){
                    wCSV<<str.substr(found,str.find("\"]")-found)<<str.substr(found-digits,digits-1)<<mdate<<std::endl;
                }

                if(strcmp(lmonth,month)==0){
                    strncpy(day,mdt+4,2);
                    day[2] = '\0';
                    strncpy(hour,mdt+12,2);
                    hour[2] = '\0';

                    if(strcmp(lday,day)<0){
                        wCSV<<str.substr(found,str.find("\"]")-found)<<str.substr(found-digits,digits-1)<<mdate<<std::endl;
                    }

                    else if(strcmp(lday,day)==0&&strcmp(lhour,hour)<0){
                        wCSV<<str.substr(found,str.find("\"]")-found)<<str.substr(found-digits,digits-1)<<mdate<<std::endl;
                    }
                }
            }

            found = str.find("[\"");
        }

        return true;
    }
}
