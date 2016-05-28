#ifndef PROMPTHANDLER_H
#define PROMPTHANDLER_H

#include <string>
#include "genealogicaltree.h"

class PromptHandler
{
public:
    PromptHandler(  );
    ~PromptHandler();
    
    int EventLoop();
    
private:
    void GenerateFile();
    void LoadFile();
    void DisplayEventOptions();
    void FindPerson();
    void DisplayPersons( const Persons& persons );
    
    GenealogicalTree Tree;
};

#endif // PROMPTHANDLER_H
