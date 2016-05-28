#include "prompthandler.h"

#include "genealogicaltree.h"
#include "treefilegenerator.h"
#include "treefileloader.h"

#include <iostream>
#include <iomanip>

using namespace std;

PromptHandler::PromptHandler()
{
}

PromptHandler::~PromptHandler()
{

}

int PromptHandler::EventLoop()
{
    char cmd = 0;
    while ( cmd != 'q' ) {
        DisplayEventOptions();        
        cin >> cmd;
        switch( cmd ) {
            case 'f':
                FindPerson();
                break;
            case 'l':
                LoadFile();
                break;
            case 'g':
                GenerateFile();
                break;
            default:
                break;
        }
    }
    
    return 0;
}

void PromptHandler::DisplayEventOptions()
{
    cout << "======================================================================" << endl;
    cout << "What do you want to do?"   << endl;
    cout << "\t[f] Find a person"       << endl;
    cout << "\t[l] Load a data file"    << endl;
    cout << "\t[g] Generate a random data file" << endl;
    cout << "\t[q] Quit"                << endl;
}

void PromptHandler::GenerateFile()
{
    int num_of_people;
    string path;
    cout << "Number of People(10-1000000)? ";
    cin >> num_of_people;
    cout << "Path to create into? ";
    ws(cin);
    getline(cin, path);    
    if ( ! path.empty() && num_of_people > 10 && num_of_people < 1000001 ) {
        TreeFileGenerator gen;
        gen.Generate( path, num_of_people );
    }
}

void PromptHandler::LoadFile()
{
    string path;
    cout << "Path to load? ";
    ws(cin);
    getline(cin, path);
    //TODO: Use filesystem::exists() to make sure the path is valid.
    if ( ! path.empty() ) {
        TreeFileLoader loader( &Tree );
        auto loaded = loader.LoadFile( path );
        if( loaded )
            cout << "File "<< path << " loaded properly." << endl;
        else
            cout << "Loading file " << path << " failed!" << endl;
    }
}

void PromptHandler::FindPerson()
{
    cout << "------------------------------------------------" << endl;
    cout << "By what criteria?" << endl;
    cout << "\t[1] Name"        << endl;
    cout << "\t[2] Last Name"   << endl;
    cout << "\t[3] Location"    << endl;
    cout << "\t[4] Birth Date"  << endl;
    cout << "\t[q] *** Return"  << endl;
    char ch;
    cin >> ch;
    switch( ch ) {
        case '1':
        {
            cout << "Name? ";
            string name;
            ws(cin);
            getline(cin, name);
            if( !name.empty() ) {
                auto res = Tree.FindPersonByName( name );
                DisplayPersons( res );
            }
        }
            break;
        case '2':
        {
            cout << "Last Name? ";
            string lname;
            ws(cin);
            getline(cin, lname);
            if( !lname.empty() ) {
                auto res = Tree.FindPersonByLastName( lname );
                DisplayPersons( res );
            }
        }
            break;
        case '3':
        {
            cout << "Location? ";
            string location;
            ws(cin);
            getline(cin, location);
            if( !location.empty() ) {
                auto res = Tree.FindPersonByLocation( location );
                DisplayPersons( res );
            }
        }
            break;
        case '4':
        {
            cout << "Birthdate(yyyy mm dd)? ";
            int year, month, day;
            cin >> year >> month >> day;
            auto res = Tree.FindPersonByBirthDate( year, month, day );
            DisplayPersons( res );
        }
            break;
        case 'q':
            break;
    }
}

void PromptHandler::DisplayPersons(const Persons& persons)
{
    const int num_width = 7;
    const int name_width = 12;
    const int lname_width = 20;
    const int location_width = 30;
    cout << left << setw(num_width)      << "#"
                 << setw(name_width)     << "Name"
                 << setw(lname_width)    << "LastName"
                 << setw(location_width) << "Location"
                                 << '\t' << "BirthDate" << endl;
    for( int i = 0; i < persons.size(); ++i ) {
        auto date = Utils::ConvertCTimeToDate( persons[i].BirthDate );
        cout << left << setw(num_width)      << i
                     << setw(name_width)     << persons[i].Name
                     << setw(lname_width)    << persons[i].LastName
                     << setw(location_width) << persons[i].Location 
                                     << '\t' << date.Year << '-' << date.Month << '-' << date.Day << endl;
    }
}
