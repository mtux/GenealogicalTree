
#include "genealogicaltree.h"
#include <utility>

GenealogicalTree::GenealogicalTree()
{

}

GenealogicalTree::~GenealogicalTree()
{

}

bool GenealogicalTree::AddPerson( Person person, OptionalPerson parent1, OptionalPerson parent2 )
{
    auto person_node = std::make_shared<PersonNode>( person );

    bool ok = false;
    if( person_node ) {
        NameMap.insert(      std::make_pair(person.Name,      person_node) );
        LastNameMap.insert(  std::make_pair(person.LastName,  person_node) );
        LocationMap.insert(  std::make_pair(person.Location,  person_node) );
        BirthDateMap.insert( std::make_pair(person.BirthDate, person_node) );
        ok = true;
        
        if( parent1 )
            SetParent( person_node, parent1.value() );
        if( parent2 )
            SetParent( person_node, parent2.value() );
    }
    
    return ok;
}

void GenealogicalTree::SetParent(GenealogicalTree::PersonPtr person, Person parent)
{

}

template<typename MapType, typename KeyType>
Persons FindPersons(const MapType& map, KeyType key)
{
    Persons ids;
    auto it = map.lower_bound(key);
    while ( it != map.end() && (*it).first == key ) {
        ids.push_back( (*it).second->Info );
        ++it;
    }
    return ids;
}

Persons GenealogicalTree::FindPersonByName( const String& name )
{
    return FindPersons(NameMap, name);
}

Persons GenealogicalTree::FindPersonByLastName( const String& last_name )
{
    return FindPersons(LastNameMap, last_name);
}

Persons GenealogicalTree::FindPersonByLocation( const String& location )
{
    return FindPersons(LocationMap, location);
}

Persons GenealogicalTree::FindPersonByBirthDate( time_t birth_date )
{
    return FindPersons(BirthDateMap, birth_date);
}
