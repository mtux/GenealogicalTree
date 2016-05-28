#ifndef TREEFILEGENERATOR_H
#define TREEFILEGENERATOR_H
#include <string>

class GenealogicalTree;

class TreeFileGenerator
{
public:
    TreeFileGenerator();

    void Generate( const std::string& path, int num_of_people );
};

#endif // TREEFILEGENERATOR_H
