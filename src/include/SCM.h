#if !defined(SCM_H)
    #define STEAMCOOKIESIZE 63
    #define SCM_H

    bool authSteamCheck(const char tempToken[STEAMCOOKIESIZE]);
    bool updateSCM(const std::string appID,const std::string marketHashName,const time_t &tmr);
    bool rebaseSCM(const std::string appID,const std::string marketHashName,const time_t &tmr,const char steamLoginSecure[STEAMCOOKIESIZE]);
    bool numMonth(const char* clmonth,char* month);
#endif
