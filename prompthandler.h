#ifndef PROMPTHANDLER_H
#define PROMPTHANDLER_H

#include "genealogicaltree.h"

#include <string>

class PromptHandler
{
public:
    PromptHandler();
    ~PromptHandler();
    
    int EventLoop();
    
private:
    void DisplayMessage( const std::string& message);
    void DisplayErrorMessage( const std::string& error_message );
    
    void GenerateFile();
    void LoadFile();
    void FindPerson();
    void FindAllDescendantsForAllAscendants();
    
    void DisplayEventOptions();
    void DisplayPersons( const Persons& persons );
    void DisplayDescendantInfo(const DescendantInfo& info);
    void DisplayPersonColumnTitles(const std::string& leading_text = std::string(),
                                   const std::string& trailing_text = std::string());
    void DisplayOnePersonInColumns(int row_num, const Person& person,
                                   const std::string& leading_text = std::string(),
                                   const std::string& trailing_text = std::string());
    
    GenealogicalTree Tree;
};

#endif // PROMPTHANDLER_H
