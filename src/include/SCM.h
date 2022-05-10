#ifndef SCM_H
#define STEAMCOOKIESIZE 63
#define SCM_H

bool authSteamCheck(const char tempToken[STEAMCOOKIESIZE]);
bool updateSCM(const time_t &tmr);
bool rebaseSCM(const time_t &tmr,const char steamLoginSecure[STEAMCOOKIESIZE]);

#endif
