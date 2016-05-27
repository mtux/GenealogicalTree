#ifndef UTILS_H
#define UTILS_H
#include <ctime>

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
}

#endif
