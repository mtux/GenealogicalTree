#ifndef FILELOADER_H
#define FILELOADER_H

#include "genealogicaltree.h"

#include <string>

class TreeFileLoader
{
public:
    TreeFileLoader( GenealogicalTree* tree );

    long long LoadFile( const std::string& path );

    std::string GetLastError();
    
private:
    bool LoadPerson(const std::string& line);
    bool ReadOnePerson(const std::string& line , Person &p);

    GenealogicalTree* Tree;
    std::string LastError;
};

#endif // FILELOADER_H
