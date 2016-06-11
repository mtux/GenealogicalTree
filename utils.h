#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <sstream>
#include <algorithm>
#include <vector>

const int DATE_START_YEAR      = 1600;
const int DATE_END_YEAR        = 2016;
const int NUM_OF_DAYS_PER_YEAR = 365;

typedef unsigned int uint32_t;

namespace Utils
{
    namespace //Anonymouse, because it's internal stuff
    {
        const int MonthLength[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    }

    class Date
    {
    public:
		Date(): NumOfDaysSinceEpoch(0) {}
		Date(int year, int month, int day)
        {
            if( year > DATE_END_YEAR     || year < DATE_START_YEAR ||
                month > 12               || month < 1              ||
                day > MonthLength[month] || day < 1  
                ) {
                throw std::invalid_argument("Invalid Date!");
            }
            NumOfDaysSinceEpoch = (year - DATE_START_YEAR) * NUM_OF_DAYS_PER_YEAR;
            for(int i = 1; i < month; ++i)
                NumOfDaysSinceEpoch += MonthLength[i];
            NumOfDaysSinceEpoch += day;
        }

        bool IsValid() const   { return NumOfDaysSinceEpoch > 0; }

        int  GetYear() const   { return  DATE_START_YEAR + (int)(NumOfDaysSinceEpoch / NUM_OF_DAYS_PER_YEAR); }

        int  GetMonth() const
        {
            auto year_day_count = (NumOfDaysSinceEpoch % NUM_OF_DAYS_PER_YEAR);
            int mon = 0;
            while(year_day_count > 0){
                ++mon;
                year_day_count -= MonthLength[mon];
            }
            return mon;
        }

        int  GetDay()  const
        {
            auto day_count = (NumOfDaysSinceEpoch % NUM_OF_DAYS_PER_YEAR);
            int mon = 0;
            while(day_count > 0){
                ++mon;
                day_count -= MonthLength[mon];
            }
            return (int)day_count + MonthLength[mon];
        }

        friend bool operator<(Date lhs, Date rhs) { return lhs.NumOfDaysSinceEpoch < rhs.NumOfDaysSinceEpoch;  }
        friend bool operator<=(Date lhs, Date rhs){ return lhs.NumOfDaysSinceEpoch <= rhs.NumOfDaysSinceEpoch; }
        friend bool operator==(Date lhs, Date rhs){ return lhs.NumOfDaysSinceEpoch == rhs.NumOfDaysSinceEpoch; }
        friend bool operator>(Date lhs, Date rhs) { return lhs.NumOfDaysSinceEpoch > rhs.NumOfDaysSinceEpoch;  }
        friend bool operator>=(Date lhs, Date rhs){ return lhs.NumOfDaysSinceEpoch >= rhs.NumOfDaysSinceEpoch; }

    private:        
        long long NumOfDaysSinceEpoch;
    };
        
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
