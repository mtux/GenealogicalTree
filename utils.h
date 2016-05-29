#ifndef UTILS_H
#define UTILS_H

#include <ctime>
#include <string>
#include <sstream>
#include <algorithm>

namespace Utils
{
    struct Date
    {
        int Year  = -1;
        int Month = -1;
        int Day   = -1;
    };
    
    inline time_t ConvertDateToCTime( Date date )
    {
        tm t;
        t.tm_hour = t.tm_min = t.tm_sec = 0;
        t.tm_year = date.Year - 1900;
        t.tm_mon  = date.Month;
        t.tm_mday = date.Day;
        return std::mktime( &t );
    }
    
    inline Date ConvertCTimeToDate( time_t time )
    {
        Date date;
        auto t = std::localtime(&time);
        if( t ) {
            date.Year = t->tm_year + 1900;
            date.Month = t->tm_mon;
            date.Day = t->tm_mday;
        }
        return date;
    }
    
    inline void Trim(std::string &s) {
        s.erase(s.begin(), std::find_if_not(s.begin(), s.end(), std::ptr_fun<int, int>(std::isspace)));
        s.erase(std::find_if_not(s.rbegin(), s.rend(), std::ptr_fun<int, int>(std::isspace)).base(), s.end());
    }
    
    inline std::vector<std::string> Split(const std::string &s, char delim) {
        std::stringstream ss(s);
        std::string item;
        std::vector<std::string> elems;
        while ( std::getline(ss, item, delim) ) {
            elems.push_back( std::move(item) );
        }
        return elems;
    }
    
}

#endif
