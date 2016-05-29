#ifndef FILELOADER_H
#define FILELOADER_H

#include "genealogicaltree.h"

#include <string>

class TreeFileLoader
{
public:
    TreeFileLoader( GenealogicalTree* tree );

    bool LoadFile( const std::string& path );

private:
    void LoadPerson(const std::string& line);
    bool ReadOnePerson(const std::string& line , Person &p);

    GenealogicalTree* Tree;
};

#endif // FILELOADER_H
