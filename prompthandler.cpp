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

const char* WRONG_INPUT_MESSAGE = "Wrong character! Please choose from menu.";

inline void PromptHandler::DisplayMessage(const string& message)
{
    cout << "#### " << message << "\nPress enter to continue." << endl;
    std::string tmp;
    getline(cin, tmp);
}

inline void PromptHandler::DisplayErrorMessage(const string& error_message)
{
    cout << "**** ERROR => " << error_message << "\nPress enter to continue." << endl;
    std::string tmp;
    getline(cin, tmp);
}

int PromptHandler::EventLoop()
{
    char cmd = 0;
    string line;
    const std::string EMPTY_TREE_MSG = "The tree is empty. Try loading some data file first.";
    while ( cmd != 'q' ) {
        DisplayEventOptions();
        getline(cin, line);
        cmd = line[0];
        switch( cmd ) {
            case 'f':
                if( ! Tree.IsEmpty() )
                    FindPerson();
                else
                    DisplayErrorMessage( EMPTY_TREE_MSG );
                break;
            case 'a':
                if( ! Tree.IsEmpty() )
                    FindAllDescendantsForAllAscendants();
                else
                    DisplayErrorMessage( EMPTY_TREE_MSG );
                break;
            case 'l':
                LoadFile();
                break;
            case 'g':
                GenerateFile();
                break;
            case 'q':
                break;
            default:
                DisplayErrorMessage( WRONG_INPUT_MESSAGE );
                break;
        }
    }
    
    return 0;
}

void PromptHandler::DisplayEventOptions()
{
    cout << "\n\n======================================================================" << endl;
    cout << "What do you want to do?" << endl;
    cout << "\t[g] Create a genealogical tree file with randomly generated data" << endl;
    cout << "\t[l] Load a genealogical tree file" << endl;
    cout << "\t[f] Find a person" << endl;
    cout << "\t[a] Find all descendants with name Bob for all ascendants with name Will" << endl;
    cout << "\t[q] Quit" << endl;
    cout << "> ";
}

void PromptHandler::GenerateFile()
{
    int num_of_people = 0;
    string num_str;
    string path;
    while( num_of_people < GEN_MIN_PEOPLE_NUM || num_of_people > GEN_MAX_PEOPLE_NUM ) {
        cout << "Number of People("<< GEN_MIN_PEOPLE_NUM << '-' << GEN_MAX_PEOPLE_NUM << ")? ";
        getline(cin, num_str);
        try{
            num_of_people = stoi( num_str );
        } catch( const invalid_argument& ) {
            if( num_str[0] == 'c' )
                return;
            DisplayErrorMessage( "Not a number. Please try again or enter 'c' to Cancel" );
        }        
    }
    cout << "Path to create into? ";
    getline( cin, path );
    Utils::Trim( path );
    if ( ! path.empty() ) {
        cout << "Creating ... ";
        cout.flush();
        TreeFileGenerator gen;
        const bool res = gen.Generate( path, num_of_people );
        cout << endl;
        if( res ) {
            const std::string msg = "File \"" + path + "\" generated successfully.";
            DisplayMessage( msg );
        } else {
            const std::string err_msg = "File generation failed with the following error:\n\t" + gen.GetLastError();
            DisplayErrorMessage( err_msg );
        }
    } else {
        DisplayMessage( "Path cannot be empty. Please try again later." );
    }
}

void PromptHandler::LoadFile()
{
    string path;
    cout << "Path to load? ";
    getline(cin, path);
    Utils::Trim(path);
    if ( ! path.empty() ) {
        cout << "Loading ... ";
        cout.flush();
        TreeFileLoader loader( &Tree );
        const auto count = loader.LoadFile( path );
        cout << endl;
        if( count >= 0 ) {
            const std::string msg = to_string( count ) + " people loaded successfully.";
            DisplayMessage( msg );
        } else {
            const std::string err_msg = "Loading file \"" + path + "\" failed!";
            DisplayErrorMessage( err_msg );
        }
    } else {
        DisplayErrorMessage( "Path cannot be empty. Please try again later." );
    }
}

bool PromptHandler::FindPersonByName()
{
    cout << "Name? ";
    string name;
    getline(cin, name);
    if( !name.empty() ) {
        auto res = Tree.FindPersonByName( name );
        DisplayPersons( res );
        return true;
    } else {
        return false;
    }
}

bool PromptHandler::FindPersonByLastName()
{
    cout << "Last Name? ";
    string lname;
    getline(cin, lname);
    if( !lname.empty() ) {
        auto res = Tree.FindPersonByLastName( lname );
        DisplayPersons( res );
        return true;
    } else {
        return false;
    }
}

bool PromptHandler::FindPersonByLocation()
{
    cout << "Location? ";
    string location;
    getline(cin, location);
    if( !location.empty() ) {
        auto res = Tree.FindPersonByLocation( location );
        DisplayPersons( res );
        return true;
    } else {
        return false;
    }
}

bool PromptHandler::FindPersonByBirthDate()
{
    cout << "Birthdate(yyyy mm dd)? ";
    int year, month, day;
    string date_str;
    getline(cin, date_str);
    auto strings = Utils::Split(date_str, ' ');
    if( strings.size() >= 3 ) {
        try {
            year =  stoi( strings[0] );
            month = stoi( strings[1] );
            day   = stoi( strings[2] );
            --month;
            auto res = Tree.FindPersonByBirthDate( year, month, day );
            DisplayPersons( res );
            return true;
        } catch( const invalid_argument& ) {
            DisplayErrorMessage( "Invalid date format! Please try again." );
            return false;
        }
    } else {
        DisplayErrorMessage( "Not enough numbers. Please try again." );
        return false;
    }
}

void PromptHandler::FindPerson()
{
    bool ok = false;
    while( ! ok ) {
        cout << "------------------------------------------------" << endl;
        cout << "By what criteria?" << endl;
        cout << "\t[1] Name"        << endl;
        cout << "\t[2] Last Name"   << endl;
        cout << "\t[3] Location"    << endl;
        cout << "\t[4] Birth Date"  << endl;
        cout << "\t[c] *** Cancel"  << endl;
        cout << "> ";
        string line;
        getline(cin, line);
        switch( line[0] ) {
            case '1':
                ok = FindPersonByName();
                break;
            case '2':
                ok = FindPersonByLastName();
                break;
            case '3':
                ok = FindPersonByLocation();
                break;
            case '4':
                ok = FindPersonByBirthDate();
                break;
            case 'c':
                ok = true;
                break;
            default:
                DisplayErrorMessage( WRONG_INPUT_MESSAGE );
                ok = false;
                break;
        }
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
    if( persons.empty() ) {
        DisplayMessage( "No one found." );
    } else {
        DisplayPersonColumnTitles();
        for( unsigned i = 0; i < persons.size(); ++i ) {
            DisplayOnePersonInColumns( i, persons[i] );
        }
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

    std::string date_str = "NULL";
    if( date.Day != -1 && date.Month != -1 )
        date_str = to_string((long long)date.Year) + '-' + to_string((long long)date.Month) + '-' + to_string((long long)date.Day);
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
    cout << "Descendants' Name ? (press enter to use \"Bob\") ";
    getline(cin, descendant_name);
    cout << "Ascendants' Name ? (press enter to use \"Will\") ";
    getline(cin, ascendant_name);
    Utils::Trim( descendant_name );
    Utils::Trim( ascendant_name  );
    if( descendant_name.empty() )
        descendant_name = "Bob";
    if( ascendant_name.empty() )
        ascendant_name = "Will";
    
    DescendantInfos all = Tree.FindAllDescendantsForAllAscendants( descendant_name, ascendant_name );
    
    if( !all.empty() ) {
        for_each( all.cbegin(), all.cend(), [&](const DescendantInfo& d) { DisplayDescendantInfo( d ); } );
    } else {
        DisplayMessage( "No one with the specified names found." );
    }
}

void PromptHandler::DisplayDescendantInfo(const DescendantInfo& info)
{
    cout << "\n+++++++++++++++++++++++++++++++++++++" << endl;
    DisplayOnePersonInColumns( -1, info.Descendant );
    if( info.Ascendants.empty() ) {
        cout << "\tNo ascendant with the requested name!" << endl;
    } else {
        cout << "\t------ Ascendants -------------------" << endl;
        DisplayPersonColumnTitles( "\t", "Distance" );
        for(unsigned i = 0; i < info.Ascendants.size(); ++i){
            DisplayOnePersonInColumns( i, info.Ascendants[i].Ascendant,
                                       "\t", std::to_string((long long)info.Ascendants[i].Distance) );
        }
    }
}
