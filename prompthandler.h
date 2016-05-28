#ifndef PROMPTHANDLER_H
#define PROMPTHANDLER_H

#include <string>
#include "genealogicaltree.h"

class PromptHandler
{
public:
    PromptHandler( int argc, char **argv );
    ~PromptHandler();
    
    int EventLoop();
    
private:
    void DisplayCommandOptions();
    void GenerateFile(int num_of_people, const std::string& path);
    void LoadFile(const std::string& path);
    
    GenealogicalTree Tree;
};

#endif // PROMPTHANDLER_H
