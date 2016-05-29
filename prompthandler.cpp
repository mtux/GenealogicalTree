#include "prompthandler.h"

#include "genealogicaltree.h"
#include "treefilegenerator.h"
#include "treefileloader.h"

#include <iostream>
#include <iomanip>
#include <algorithm>

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
    string tmp;
    while ( cmd != 'q' ) {
        DisplayEventOptions();
        cin >> cmd;
        getline(cin, tmp);
        switch( cmd ) {
            case 'f':
                FindPerson();
                break;
            case 'a':
                FindAllDescendantsForAllAscendants();
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
    cout << "\t[a] Find all descendants with name Bob for all ascendants with name Will" << endl;
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
        const bool res = gen.Generate( path, num_of_people );
        if( res ) {
            cout << "File \"" << path << "\" generated successfully." << endl;
        } else {
            cout << "ERROR: File generation failed!\n";
        }
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

const int NUM_WIDTH = 7;
const int NAME_WIDTH = 12;
const int LNAME_WIDTH = 20;
const int LOCATION_WIDTH = 30;
const int DATE_WIDTH = 15;
const int TRAILING_WIDTH = 10;
void PromptHandler::DisplayPersons(const Persons& persons)
{
    DisplayPersonColumnTitles();
    for( int i = 0; i < persons.size(); ++i ) {
        DisplayOnePersonInColumns( i, persons[i] );

    }
}

void PromptHandler::DisplayPersonColumnTitles(const std::string& leading_text, const std::string& trailing_text)
{
    cout << leading_text;
    cout << left << setw(NUM_WIDTH)      << "#"
                 << setw(NAME_WIDTH)     << "Name"
                 << setw(LNAME_WIDTH)    << "LastName"
                 << setw(LOCATION_WIDTH) << "Location"
                 << setw(DATE_WIDTH)     << "BirthDate"
                 << setw(TRAILING_WIDTH) << trailing_text << endl;
}

void PromptHandler::DisplayOnePersonInColumns(int row_num, const Person& person,
                                              const std::string& leading_text, const std::string& trailing_text)
{
    auto date = person.GetBirthDate();
    std::string date_str = to_string(date.Year) + '-' + to_string(date.Month) + '-' + to_string(date.Day);
    cout << leading_text;
    if( row_num != -1 ) {
        cout << left << setw(NUM_WIDTH)      << row_num;
    }
    cout << left << setw(NAME_WIDTH)     << person.Name
                 << setw(LNAME_WIDTH)    << person.LastName
                 << setw(LOCATION_WIDTH) << person.Location 
                 << setw(DATE_WIDTH)     << date_str
                 << setw(TRAILING_WIDTH) << trailing_text << endl;
}

void PromptHandler::FindAllDescendantsForAllAscendants()
{
    string descendant_name, ascendant_name;
    cout << "Descendants' Name(press enter to use name \"Bob\")? ";
    ws(cin);
    getline(cin, descendant_name);
    cout << "Ascendants' Name(press enter to use name \"Will\")? ";
    getline(cin, ascendant_name);
    //TODO: trim input strings
    if( descendant_name.empty() )
        descendant_name = "Bob";
    if( ascendant_name.empty() )
        ascendant_name = "Will";
    
    DescendantInfos all = Tree.FindAllDescendantsForAllAscendants( descendant_name, ascendant_name );
    
    for_each( all.cbegin(), all.cend(), [&](const DescendantInfo& d) { DisplayDescendantInfo( d ); } );
}

void PromptHandler::DisplayDescendantInfo(const DescendantInfo& info)
{
    cout << "+++++++++++++++++++++++++++++++++++++" << endl;
    DisplayOnePersonInColumns( -1, info.Descendant );
    if( info.Ascendants.empty() ) {
        cout << "\tNo ascendant with the requested name found!";
    } else {
        cout << "\t------ Ascendants -------------------" << endl;
        DisplayPersonColumnTitles( "\t", "Distance" );
        for(int i = 0; i < info.Ascendants.size(); ++i){
            DisplayOnePersonInColumns( i, info.Ascendants[i].Ascendant,
                                       "\t", std::to_string(info.Ascendants[i].Distance) );
        }
    }
}
