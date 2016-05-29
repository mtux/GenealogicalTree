#include "treefileloader.h"
#include "utils.h"

#include <fstream>
#include <vector>
#include <ctime>

TreeFileLoader::TreeFileLoader( GenealogicalTree *tree )
    : Tree(tree)
{
}

std::string TreeFileLoader::GetLastError()
{
    std::string tmp = std::move( LastError );
    LastError.clear();
    return tmp;
}

int64_t TreeFileLoader::LoadFile( const std::string& path )
{
    std::ifstream input(path);
    if( input.good() ) {
        std::string line;
        int64_t count = 0;
        while( input.good() ){
            std::getline(input, line);
            if( line.size() > 1 && line[0] != '#' ) {
                count += LoadPerson(line) ? 1 : 0;
            }
        }
        return count;
    } else {
        LastError = "Cannot find or open the specified file.";
        return -1;
    }
}

bool TreeFileLoader::LoadPerson(const std::string& line)
{
    auto persons = Utils::Split(line, ';');
    int parent_count = persons.size() - 1;
    Person p;
    if( ReadOnePerson( persons[0], p ) ) {
        Person p1, p2;
        if( parent_count >= 1 )
            ReadOnePerson( persons[1], p1 );
        if( parent_count >= 2 )
            ReadOnePerson( persons[2], p2 );
        Tree->AddPerson(p, p1, p2);
        return true;
    } else {
        return false;
    }
}

bool TreeFileLoader::ReadOnePerson( const std::string& line, Person& p )
{
    auto elems = Utils::Split(line, ':');
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
