
#include "genealogicaltree.h"
#include "treefilegenerator.h"
#include <ctime>
#include <iostream>

void DisplayCommandOptions();
void DoFileGeneration(int num_of_people, const std::string& path);

int main(int argc, char **argv)
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
        } else if ( *it == "-g" ) {
            //TODO: Handle user input error
            int num_of_people = std::stoi( *++it );
            std::string path = *++it;
            DoFileGeneration(num_of_people, path);
        } else {
            DisplayCommandOptions();
            break;
        }
    }
    
    return 0;
}

void DisplayCommandOptions()
{
    std::cout << "Available Options:\n\t-g <num-of_people> <path-to-generate>\tGenerates a randomely populated text file with a genealogical tree data." << std::endl;
}

void DoFileGeneration(int num_of_people, const std::string& path)
{
    if ( ! path.empty() && num_of_people > 10 && num_of_people < 1000000 ) {
        TreeFileGenerator gen;
        gen.Generate( path, num_of_people );
    }
}
