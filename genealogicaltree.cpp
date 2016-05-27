
#include "genealogicaltree.h"
#include "utils.h"
#include <utility>

GenealogicalTree::GenealogicalTree()
{

}

GenealogicalTree::~GenealogicalTree()
{

}

GenealogicalTree::PersonPtr GenealogicalTree::AddPerson( Person person, OptionalPerson parent1, OptionalPerson parent2 )
{
    auto person_node = std::make_shared<PersonNode>( person );

    if( person_node ) {
        NameMap.insert(      std::make_pair(person.Name,      person_node) );
        LastNameMap.insert(  std::make_pair(person.LastName,  person_node) );
        LocationMap.insert(  std::make_pair(person.Location,  person_node) );
        BirthDateMap.insert( std::make_pair(person.BirthDate, person_node) );
        
        if( parent1 )
            SetParent( person_node, parent1.value() );
        if( parent2 )
            SetParent( person_node, parent2.value() );
        
        return person_node;
    } else {
        return nullptr;        
    }
    
}

void GenealogicalTree::SetParent(GenealogicalTree::PersonPtr person, Person parent)
{
    PersonPtr parent_ptr = FindPerson( parent.Name, parent.LastName, parent.Location, parent.BirthDate );
    
    if ( ! parent_ptr )
        parent_ptr = AddPerson( parent );
    
    if ( ! person->Parent1 )
        person->Parent1 = parent_ptr;
    else if ( ! person->Parent2 )
        person->Parent2 = parent_ptr;
}

Persons GenealogicalTree::FindPersonByName( const String& name )
{
    return GetPersonsFromPersonPtrs( FindPersonPtrByName(name) );
}

Persons GenealogicalTree::FindPersonByLastName( const String& last_name )
{
    return GetPersonsFromPersonPtrs( FindPersonPtrByLastName(last_name) );
}

Persons GenealogicalTree::FindPersonByLocation( const String& location )
{
    return GetPersonsFromPersonPtrs( FindPersonPtrByLocation(location) );
}

Persons GenealogicalTree::FindPersonByBirthDate( int year, int month, int day )
{
    return GetPersonsFromPersonPtrs( FindPersonPtrByBirthDate(year, month, day) );
}

GenealogicalTree::PersonPtr GenealogicalTree::FindPerson( const String& name, const String& last_name,
                                                          const String& location, time_t birth_date )
{
    auto matched_people = FindPersonPtrByName( name );
    for( const PersonPtr& person: matched_people ) {
        if( person->Info.LastName == last_name &&
            person->Info.Location == location &&
            (birth_date == -1 || person->Info.BirthDate == birth_date)
          ) {
                return person;
            }
    }
    return nullptr;
}

GenealogicalTree::PersonPtrs GenealogicalTree::FindPersonPtrByName(const String& name)
{
    return FindPersonPtrByKey( NameMap, name );
}

GenealogicalTree::PersonPtrs GenealogicalTree::FindPersonPtrByLastName(const String& last_name)
{
    return FindPersonPtrByKey( LastNameMap, last_name );
}

GenealogicalTree::PersonPtrs GenealogicalTree::FindPersonPtrByLocation(const String& location)
{
    return FindPersonPtrByKey( LocationMap, location );
}

GenealogicalTree::PersonPtrs GenealogicalTree::FindPersonPtrByBirthDate(int year, int month, int day)
{
    return FindPersonPtrByKey( BirthDateMap, Utils::ConvertDateToCTime( Utils::Date{year, month, day} ) );
}
