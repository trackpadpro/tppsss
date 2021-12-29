#include "webScraper.h"
#include <iostream>
#include <fstream>
#include <time.h>

webScraper::webScraper(time_t* tmrPtr){
    steamCommunity = true;
    stockMarket = true;
    xtal = tmrPtr;
    unruh = time(xtal)-300;
}

webScraper::~webScraper(){
    //Depending on choice of CSV file locator, may need to manually implement
}

bool webScraper::updateCSV(){
    std::ofstream writeCSV("webScraper/marketData.csv");
    writeCSV<<"time\n"<<ctime(xtal); 
    if(time(xtal)>unruh+300){
        if(steamCommunity){
            //steamCommunity = ;
        }

        if(stockMarket){
            //stockMarket = ;
        }

        unruh = time(xtal);
    }

    if(steamCommunity && stockMarket){
        return true;
    }

    else{
        std::cout<<"Markets Unavailable "<<ctime(xtal);
        return false;
    }
}