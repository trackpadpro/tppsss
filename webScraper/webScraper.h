#include <time.h>

#ifndef WEBSCRAPER_H
#define WEBSCRAPER_H

//Do add variable for CSV file location to allow simple alterations to be made within main.cpp
//Private variable will require public function that changes it
//Class will have the unruh variable
class webScraper{
private:
    bool steamCommunity, stockMarket;
    long int unruh;
    time_t* xtal;
    //string? CSV path & name;

public:
    webScraper(time_t* tmrPtr);
    ~webScraper();
    bool updateCSV();
};

#endif