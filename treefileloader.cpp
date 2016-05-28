#include "treefileloader.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <ctime>

TreeFileLoader::TreeFileLoader( GenealogicalTree *tree )
    : Tree(tree)
{
}

inline std::vector<std::string> split(const std::string &s, char delim) {
    std::stringstream ss(s);
    std::string item;
    std::vector<std::string> elems;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

bool TreeFileLoader::LoadFile( const std::string& path )
{
    std::ifstream input(path);
    if( input.good() ) {
        String line;
        while( input.good() ){
            std::getline(input, line);
            if( line.size() > 0 && line[0] != '#' ) {
                LoadPerson(line);
            }
        }
        return true;
    } else {
        return false;
    }
}

void TreeFileLoader::LoadPerson(String line)
{
    auto persons = split(line, ';');
    int parent_count = persons.size() - 1;
    Person p;
    if( ReadOnePerson( persons[0], p ) ) {
        Person p1, p2;
        if( parent_count >= 1 )
            ReadOnePerson( persons[1], p1 );
        if( parent_count >= 2 )
            ReadOnePerson( persons[2], p2 );
        Tree->AddPerson(p, p1, p2);
    }
}

bool TreeFileLoader::ReadOnePerson( const std::string& line, Person& p )
{
    auto elems = split(line, ':');
    if( elems.size() >= 3 ) {
        p.Name = elems[0];
        p.LastName = elems[1];
        p.Location = elems[2];
        if( elems.size() >= 6 ) {
            const int year  = std::stoi( elems[3] );
            const int month = std::stoi( elems[4] );
            const int day   = std::stoi( elems[5] );
            p.SetBirthDate( Utils::Date{year, month, day} );
        }
        else {
            p.BirthDate = -1;
        }
        return true;
    } else {
        return false;
    }
}
