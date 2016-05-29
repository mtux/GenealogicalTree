#ifndef TREEFILEGENERATOR_H
#define TREEFILEGENERATOR_H
#include <string>

const int GEN_MIN_PEOPLE_NUM = 10;
const int GEN_MAX_PEOPLE_NUM = 1000000;

class GenealogicalTree;

class TreeFileGenerator
{
public:
    TreeFileGenerator();

    bool Generate( const std::string& path, int num_of_people );
    
    std::string GetLastError();
    
private:
    std::string LastError;
};

#endif // TREEFILEGENERATOR_H
