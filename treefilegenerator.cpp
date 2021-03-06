#include "treefilegenerator.h"
#include "genealogicaltree.h"
#include "names.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <random>

TreeFileGenerator::TreeFileGenerator()
{

}

std::string TreeFileGenerator::GetLastError()
{
    std::string err_msg = std::move( LastError );
    LastError.clear();
    return err_msg;
}

inline std::ostream& operator<< (std::ostream& os, const Person& p)
{
    auto date = p.GetBirthDate();
    os << p.Name << ':' << p.LastName << ':' << p.Location << ':' << date.Year << ':' << date.Month << ':' << date.Day;
    return os;
}

bool TreeFileGenerator::Generate( const std::string& path, int num_of_people )
{
    LastError.clear();
    
    if( num_of_people < GEN_MIN_PEOPLE_NUM ) {
        LastError = "Too few people. Try more!";
        return false;
    }
    std::ofstream out;
    out.open( path );
    if( out.good() ) {
        out << "# Randomly generated data" << std::endl;
        std::vector<Person> people;
        //Enhancement tip: It might worth using two much smaller vectors to store two group of people.
        //                 and switch between them while choosing parents.
        try {
            people.reserve( num_of_people );
        } catch(const std::bad_alloc& e) {
            LastError = "Too many people. try less!";
            return false;
        }
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, NUM_OF_NAMES-1);
        std::uniform_int_distribution<> dis_day(1, 28);
        std::uniform_int_distribution<> dis_mon(0, 11);
        std::uniform_int_distribution<> dis_year(1900, 2016);
        std::uniform_int_distribution<> dis_p(0, num_of_people);

        auto populate_one_random_person = [&]( Person& p ){
            p.Name = Names[dis(gen)];
            p.LastName = LastNames[dis(gen)];
            p.Location = Countries[dis(gen)];
            p.SetBirthDate( Utils::Date {dis_year(gen), dis_mon(gen), dis_day(gen)} );
        };

        for(int i = 0; i < 10; ++i){
            populate_one_random_person( people[i] );
            out << people[i] << std::endl;
        }

        for(int i = 10; i < num_of_people; ++i){
            populate_one_random_person( people[i] );
            int p1 = dis_p(gen) % i;
            int p2 = dis_p(gen) % i;
            if( p1 == p2 )
                ++p2;
            out << people[i] << ';' << people[p1] << ';' << people[p2] << std::endl;
        }
        out.close();
        return true;
    } else {
        LastError = "File IO error!";
        return false;
    }
}
