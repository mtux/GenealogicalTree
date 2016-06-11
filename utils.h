#ifndef UTILS_H
#define UTILS_H

#include <ctime>
#include <string>
#include <sstream>
#include <algorithm>
#include <vector>

#define DATE_START_YEAR 1900

typedef unsigned int uint32_t;

namespace Utils
{
    struct Date
    {
		Date(): Year(-1), Month(-1), Day(-1) {}
		Date(int year, int month, int day): Year(year), Month(month), Day(day) {}
        int Year;
        int Month;
        int Day;
    };
    
    inline time_t ConvertDateToCTime( Date date )
    {
        tm t;
        t.tm_hour = t.tm_min = t.tm_sec = t.tm_isdst = 0;
        t.tm_year = date.Year - DATE_START_YEAR;
        t.tm_mon  = date.Month - 1;
        t.tm_mday = date.Day;
        return std::mktime( &t );
    }
    
    inline Date ConvertCTimeToDate( time_t time )
    {
        Date date;
        tm t;
        localtime_s(&t, &time);
        date.Year = t.tm_year + DATE_START_YEAR;
        date.Month = t.tm_mon + 1;
        date.Day = t.tm_mday;
        return date;
    }
    
    inline void Trim(std::string &s) {
        const char* space_chars = " \t\n\r";
        size_t first_not_space = s.find_first_not_of( space_chars );
        s.erase( 0, first_not_space );
        s.erase( s.find_last_not_of(space_chars) + 1 );
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
