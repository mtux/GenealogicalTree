#include "prompthandler.h"

#include <iostream>

#include "genealogicaltree.h"
#include "treefilegenerator.h"
#include "treefileloader.h"

PromptHandler::PromptHandler( int argc, char **argv )
{
    std::vector<std::string> args(argv + 1, argv + argc);
    if( argc == 1 )
        DisplayCommandOptions();
    
    // Loop over command-line args
    // Better solution would be to use something like Boost.Program_options, but I'm not sure if it's allowed here or not.
    for (auto it = args.cbegin(); it != args.cend(); ++it) {
        if ( *it == "-h" ) {
            DisplayCommandOptions();
            break;
        } else if ( *it == "-l" ) {
            std::string path = *++it;
            LoadFile( path );            
        } else if ( *it == "-g" ) {
            //TODO: Handle user input error
            int num_of_people = std::stoi( *++it );
            std::string path = *++it;
            GenerateFile(num_of_people, path);
        } else {
            DisplayCommandOptions();
            break;
        }
    }
}

PromptHandler::~PromptHandler()
{

}

int PromptHandler::EventLoop()
{
    //TODO
    return 0;
}

void PromptHandler::DisplayCommandOptions()
{
    std::cout << "Available Options:" << std::endl;
    std::cout << "\t-g <num-of-people> <path-to-generate>\n\t\tGenerates a randomely populated text file with a genealogical tree data to be loaded later." << std::endl;
    std::cout << "\t-l <path-to-load>\n\t\tLoad a text file containing tree data." << std::endl;
    std::cout << "\t-h\n\t\tShow this message." << std::endl;
}

void PromptHandler::GenerateFile(int num_of_people, const std::string& path)
{
    if ( ! path.empty() && num_of_people > 10 && num_of_people < 1000000 ) {
        TreeFileGenerator gen;
        gen.Generate( path, num_of_people );
    }
}

void PromptHandler::LoadFile(const std::string& path)
{
    //TODO: Use std::filesystem::exists() to make sure the path is valid.
    if ( ! path.empty() ) {
        TreeFileLoader loader( &Tree );
        auto loaded = loader.LoadFile( path );
        if( loaded )
            std::cout << "File "<< path << " loaded properly." << std::endl;
        else
            std::cout << "Loading file " << path << " failed!" << std::endl;
    }
}
