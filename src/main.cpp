#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstring>
#include <string>
#include <vector>
#include <time.h>
#include <curl/curl.h>
#include "SCM.h"
#include "stocks.h"
#include "controlTheory.h"

struct asset{
    std::string market; //Steam Community/Stock/?Crypto
    std::string div; //game/exchange
    std::string hash; /*market hash name/ticker
                        SCM uses %7C = '|', %26 = &, %28[exterior]%29*/
};

void setup();

char input[5], steamCookie[STEAMCOOKIESIZE];
time_t tmr;
size_t unruh = time(&tmr), xtal, rebaseCalls = 0;
bool online = true, steamCommunity = false, stockMarket = false, cookieSCM = false;
std::vector<asset> assets;

std::string dataPath = "./data/";
char uc[21] = "Unrecognized command";
char ucSteam[96] = "Unrecognized command. The steamLoginSecure cookie can be set later using the \"setup\" command.";

int main(){
    std::ifstream authSteam(dataPath+"auth/steamLoginSecure.txt");

    if(authSteam.good()){
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
        std::cin.sync();

        if(strcmp(input,"y")==0){
            setup();
        }

        else if(strcmp(input,"n")==0){
            std::cout<<"The steamLoginSecure cookie can be set later using the \"setup\" command."<<std::endl;
        }

        else{
            std::cout<<ucSteam<<std::endl;
        }
    }

    std::ifstream assetFile(dataPath+"trackList.txt");

    if(assetFile.is_open()){
        asset temp;

        while(assetFile.good()){
            //This loop will add an extra asset to the vector if the tracking list is '\n'-terminated

            assetFile>>temp.market>>temp.div>>temp.hash;

            assets.push_back(temp);

            if(temp.market=="steam"){
                steamCommunity = true;
            }

            else if(temp.market=="stock"){
                stockMarket = true;
            }
        }

        assetFile.close();
    }

    else{
        std::cout<<"\""+dataPath+"trackList.txt\" not found"<<std::endl;
    }

    std::cout<<"Online"<<std::endl;

    while(std::cin.good()&&online){
        std::cout<<"$$$> ";
        std::cin>>std::setw(6)>>input;
        std::cin.sync();

        if(strcmp(input,"break")==0){
            break;
        }
        
        if(strcmp(input,"fetch")==0){
            for(auto equity: assets){
                //[loading bar]

                if(equity.market=="steam"){
                    if(rebaseCalls==0){
                        xtal = time(&tmr)+60;
                    }

                    else if(time(&tmr)>xtal+rebaseCalls*5){
                        xtal = time(&tmr)+60;
                        rebaseCalls = 0;
                    }
                    
                    if(cookieSCM){
                        while(time(&tmr)<xtal+rebaseCalls*5-60){
                            //Delay SCM rebases that would otherwise cause the rate of cookie usage to exceed 12/min
                        }

                        cookieSCM = rebaseSCM(equity.div,equity.hash,tmr,dataPath,steamCookie);

                        ++rebaseCalls;

                        if(!cookieSCM){
                            std::cout<<"SCM rebase failed"<<std::endl;
                        }
                    }

                    if(steamCommunity){
                        steamCommunity = updateSCM(equity.div,equity.hash,tmr,dataPath);

                        if(!steamCommunity){
                            std::cout<<"Steam Community Market unavailable "<<ctime(&tmr);
                        }
                    }
                }

                else if(equity.market=="stock"&&stockMarket){
                    stockMarket = updateStonks();

                    if(!stockMarket){
                        std::cout<<"Stock market unavailable "<<ctime(&tmr);
                    }
                }
            }

            //[call buy/sell calculation]
            //Buy/sell calculation will utilize controlTheory.
        }

        else if(strcmp(input,"setup")==0){
            setup();

            unruh = time(&tmr);
        }

        //Prevent extremely long commands from causing spam
        else if(time(&tmr)>unruh+1){
            std::cout<<uc<<std::endl;

            unruh = time(&tmr);
        }
    }

    std::cout<<"Offline"<<std::endl;

    return 0;
}

void setup(){
    std::cout<<"Select a setting: [steam\n                   asset\n                   leave]"<<std::endl;

    while(std::cin.good()){
        std::cout<<"sss> ";
        std::cin>>std::setw(6)>>input;
        std::cin.sync();

        if(strcmp(input,"break")==0){
            online = false;

            return;
        }

        else if(strcmp(input,"leave")==0){
            break;
        }
        
        else if(strcmp(input,"steam")==0){
            std::cout<<"Would you like to set/reset your steamLoginSecure? [y/n] ";
            std::cin>>std::setw(2)>>input;
            std::cin.sync();

            if(strcmp(input,"y")==0){
                std::cout<<"Your steamLoginSecure cookie can be found in a browser that is logged into the Steam Community.\nPlease enter the content of the cookie: ";
                char temp[STEAMCOOKIESIZE];
                std::cin>>std::setw(STEAMCOOKIESIZE+1)>>temp;
                std::cin.sync();

                if(authSteamCheck(temp)){
                    std::copy(std::begin(temp),std::end(temp),std::begin(steamCookie));

                    std::ofstream authSteam(dataPath+"auth/steamLoginSecure.txt");

                    if(authSteam.is_open()){
                        authSteam<<temp;

                        std::cout<<"steamLoginSecure set"<<std::endl;
                    }
                }

                else{
                    std::cout<<"Invalid input. Try again later using the \"setup\" command."<<std::endl;
                }

                cookieSCM = true;
            }

            else if(strcmp(input,"n")!=0){
                if(time(&tmr)>unruh+1){
                    std::cout<<ucSteam<<std::endl;

                    unruh = time(&tmr);
                }    
            }
        }

        else if(strcmp(input,"asset")==0){
            std::cout<<"Would you like to set or remove a tracked asset? [y/n] ";
            std::cin>>std::setw(2)>>input;
            std::cin.sync();

            char ucAsset[91] = "Unrecognized command. The tracked assets can be changed later using the \"setup\" command.";

            if(strcmp(input,"y")==0){
                std::cout<<"Would you like to [set/clear/skip]? ";
                std::cin>>std::setw(6)>>input;
                std::cin.sync();

                if(strcmp(input,"set")==0||strcmp(input,"clear")==0){
                    std::fstream assetFile(dataPath+"trackList.txt",std::ios::app);

                    if(assetFile.is_open()){
                        while(assetFile.good()){
                            bool flag = false;

                            if(strcmp(input,"set")==0){
                                //Add URL parsing

                                asset temp;
                                char nums[11] = "0123456789";
                                size_t found;

                                std::cout<<"Is this asset part of the [Steam/stock/crypto] market? ";
                                std::cin>>temp.market;
                                std::cin.sync();

                                if(temp.market=="Steam"||temp.market=="scm"||temp.market=="SCM"){
                                    temp.market = "steam";
                                }

                                else if(temp.market=="Stock"){
                                    temp.market = "stock";
                                }

                                else if(temp.market=="Crypto"||temp.market=="crypto"){
                                    temp.market = "crypt";
                                }

                                if(!(temp.market=="steam"||temp.market=="stock"||temp.market=="crypt")){
                                    flag = true;

                                    std::cout<<uc<<std::endl;
                                }

                                if(temp.market=="steam"&&!flag){
                                    std::cout<<"Please provide the app ID: ";
                                    std::cin>>temp.div;
                                    std::cin.sync();

                                    found = temp.div.find_last_not_of(nums);

                                    if(found!=std::string::npos){
                                        if(temp.div=="CSGO"||temp.div=="csgo"||temp.div=="cs"){
                                            temp.div = "730";
                                        }

                                        else if(temp.div=="DOTA"||temp.div=="dota"||temp.div=="DOTA2"||temp.div=="dota2"){
                                            temp.div = "570";
                                        }

                                        else if(temp.div=="TF"||temp.div=="tf"||temp.div=="TF2"||temp.div=="tf2"){
                                            temp.div = "440";
                                        }

                                        else if(temp.div.at(found)=='='){
                                            temp.div = temp.div.substr(found+1,std::string::npos);
                                            
                                            if(temp.div.find_first_of(nums)==std::string::npos){
                                                flag = true;
                                            }
                                        }

                                        else{
                                            flag = true;

                                            std::cout<<uc<<std::endl;
                                        }
                                    }

                                    if(!flag){
                                        std::cout<<"Please provide the market hash name: ";
                                        std::cin>>temp.hash;
                                        std::cin.sync();

                                        found = temp.hash.find_last_of('=');

                                        if(found!=std::string::npos){
                                            temp.hash = temp.hash.substr(found+1,std::string::npos);
                                        }

                                        steamCommunity = true;
                                    }
                                }

                                else{
                                    std::cout<<"Currently, only Steam Community Market assets are supported."<<std::endl;
                                }

                                for(auto equity: assets){
                                    if(equity.market==temp.market&&equity.div==temp.div&&equity.hash==temp.hash){
                                        flag = true;
                                    }
                                }

                                if(!flag){
                                    assetFile<<"\n"<<temp.market<<" "<<temp.div<<" "<<temp.hash;
                                    assets.push_back(temp);
                                }
                            }

                            else{
                                std::cout<<"Please select an asset you would no longer like to track:"<<std::endl;

                                size_t i;
                                size_t page = 1;
                                size_t pages;
                                size_t excess;

                                while(std::cin.good()){
                                    pages = assets.size()/10+1;
                                    excess = assets.size()%10;

                                    std::cout<<"Page "<<page<<" [next/prev/close]\n";

                                    if(page<pages){
                                        for(i = 0+(page-1)*10;i<page*10;++i){
                                            std::cout<<'['<<i-(page-1)*10<<"] "<<assets[i].market<<' '<<assets[i].div<<' '<<assets[i].hash<<"\n";
                                        }
                                    }

                                    else{
                                        for(i = 0+(page-1)*10;i<(page-1)*10+excess;++i){
                                            std::cout<<'['<<i-(page-1)*10<<"] "<<assets[i].market<<' '<<assets[i].div<<' '<<assets[i].hash<<"\n";
                                        }
                                    }

                                    std::cin>>std::setw(6)>>input;
                                    std::cin.sync();

                                    if(strcmp(input,"close")==0){
                                        break;
                                    }

                                    else if(strcmp(input,"next")==0){
                                        //Nest to avoid "Unrecognized command" output
                                        if(page<pages-1||(excess>0&&page<pages)){
                                            ++page;
                                        }
                                    }

                                    else if(strcmp(input,"prev")==0){
                                        if(page>1){
                                            --page;
                                        }
                                    }
                                        
                                    else if(strlen(input)==1){
                                        std::istringstream toInt(input);
                                        toInt>>i;

                                        std::cout<<"Erase entry ["<<i<<"] "<<assets.at((page-1)*10+i).hash<<"? [y/n] "<<std::endl;
                                        std::cin>>std::setw(2)>>input;
                                        std::cin.sync();

                                        if(strcmp(input,"y")==0){
                                            assets.erase(assets.begin()+(page-1)*10+i);

                                            std::ofstream assetFile(dataPath+"trackList.txt");

                                            if(assetFile.is_open()){
                                                assetFile<<assets.at(0).market<<" "<<assets.at(0).div<<" "<<assets.at(0).hash;

                                                for(i = 1;i<assets.size();++i){
                                                    assetFile<<"\n"<<assets.at(i).market<<" "<<assets.at(i).div<<" "<<assets.at(i).hash;
                                                }
                                            }

                                            else{
                                                std::cout<<"\""+dataPath+"trackList.txt\" not found"<<std::endl;
                                            }
                                        }

                                        else if(strcmp(input,"n")!=0){
                                            std::cout<<uc<<std::endl;
                                        }
                                    }

                                    else{
                                        std::cout<<uc<<std::endl;
                                    }
                                }
                            }

                            std::cout<<"Set or remove another? [y/n] ";
                            std::cin>>std::setw(2)>>input;
                            std::cin.sync();

                            if(strcmp(input,"y")==0){
                                std::cout<<"Would you like to [set/clear/skip]? ";
                                std::cin>>std::setw(6)>>input;
                                std::cin.sync();
                            }

                            else if(strcmp(input,"n")!=0){
                                if(time(&tmr)>unruh+1){
                                    std::cout<<uc<<std::endl;

                                    unruh = time(&tmr);
                                }
                            }

                            else{
                                break;
                            }
                        }
                    }

                    else{
                        std::cout<<"\""+dataPath+"trackList.txt\" not found"<<std::endl;
                    }
                }

                else if(strcmp(input,"skip")!=0){
                    if(time(&tmr)>unruh+1){
                        std::cout<<ucAsset<<std::endl;

                        unruh = time(&tmr);
                    }
                }
            }

            else if(strcmp(input,"n")!=0){
                std::cout<<ucAsset<<std::endl;
            }
        }

        else{
            if(time(&tmr)>unruh+1){
                std::cout<<uc<<std::endl;

                unruh = time(&tmr);
            }
        }
    }

    std::cout<<"Settings closed"<<std::endl;
}
