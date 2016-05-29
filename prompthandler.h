#ifndef PROMPTHANDLER_H
#define PROMPTHANDLER_H

#include <string>
#include "genealogicaltree.h"

class PromptHandler
{
public:
    PromptHandler();
    ~PromptHandler();
    
    int EventLoop();
    
private:
    void GenerateFile();
    void LoadFile();
    void FindPerson();
    void FindAllDescendantsForAllAscendants();
    
    void DisplayEventOptions();
    void DisplayPersons( const Persons& persons );
    void DisplayDescendantInfo(int num, const DescendantInfo& info);
    void DisplayPersonColumnTitles(const String& leading_text = String(), const String& trailing_text = String());
    void DisplayOnePersonInColumns(int row_num, const Person& person,
                                   const String& leading_text = String(), const String& trailing_text = String());
    
    GenealogicalTree Tree;
};

#endif // PROMPTHANDLER_H
