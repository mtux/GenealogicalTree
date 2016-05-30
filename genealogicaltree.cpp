
#include "genealogicaltree.h"
#include "utils.h"

#include <utility>
#include <algorithm>
#include <queue>

GenealogicalTree::GenealogicalTree()
{

}

GenealogicalTree::~GenealogicalTree()
{

}

bool GenealogicalTree::AddPerson(const Person& person)
{
    return AddPersonImpl( person ) != nullptr;
}

bool GenealogicalTree::AddPerson(const Person& person, const Person& parent1)
{
    return AddPersonImpl( person, parent1 ) != nullptr;
}

bool GenealogicalTree::AddPerson( const Person& person, const Person& parent1, const Person& parent2 )
{
    return AddPersonImpl( person, parent1, parent2 ) != nullptr;
}

template<typename MapType, typename KeyType, typename ValueType>
void RemoveItem(MapType& map, KeyType key, ValueType value)
{
    auto it = map.lower_bound(key);
    while ( it != map.end() && (*it).first == key ) {
        if ( (*it).second == value ) {
            map.erase(it);
            break;
        }
        ++it;
    }
}

GenealogicalTree::PersonPtr GenealogicalTree::AddPersonImpl(const Person& person)
{
    auto person_node = std::make_shared<PersonNode>( person );
    
    if( person_node ) {
        auto n_res  = NameMap.insert(      std::make_pair(person.Name,      person_node ) );
        auto ln_res = LastNameMap.insert(  std::make_pair(person.LastName,  person_node ) );
        auto l_res  = LocationMap.insert(  std::make_pair(person.Location,  person_node ) );
        auto bd_res = BirthDateMap.insert( std::make_pair(person.BirthDate, person_node ) );
        
        if ( n_res != NameMap.end()     && ln_res != LastNameMap.end() &&
             l_res != LocationMap.end() && bd_res != BirthDateMap.end() ) {            
            return person_node;
        } else {
            //There was an error, so roll back changes
            RemoveItem( NameMap,      person.Name,      person_node );
            RemoveItem( LastNameMap,  person.LastName,  person_node );
            RemoveItem( LocationMap,  person.Location,  person_node );
            RemoveItem( BirthDateMap, person.BirthDate, person_node );
        }
    }
    return nullptr;
}

GenealogicalTree::PersonPtr GenealogicalTree::AddPersonImpl(const Person& person, const Person& parent1)
{
    auto person_node = AddPersonImpl( person );
    if( person_node ) {
        SetParent( person_node, parent1 );
    }
    return person_node;
}

GenealogicalTree::PersonPtr GenealogicalTree::AddPersonImpl( const Person& person,
                                                             const Person& parent1,
                                                             const Person& parent2 )
{
    auto person_node = AddPersonImpl( person );
    if( person_node ) {
        SetParent( person_node, parent1 );
        SetParent( person_node, parent2 );
    }
    return person_node;
}

void GenealogicalTree::SetParent( GenealogicalTree::PersonPtr person, const Person& parent )
{
    PersonPtr parent_ptr = FindPerson( parent.Name, parent.LastName, parent.Location, parent.BirthDate );
    
    if ( ! parent_ptr )
        parent_ptr = AddPersonImpl( parent );
    
    if ( ! person->Parent1 )
        person->Parent1 = parent_ptr;
    else if ( ! person->Parent2 )
        person->Parent2 = parent_ptr;
}

Persons GenealogicalTree::FindPersonByName( const std::string& name )
{
    return GetPersonsFromPersonPtrs( FindPersonPtrByName(name) );
}

Persons GenealogicalTree::FindPersonByLastName( const std::string& last_name )
{
    return GetPersonsFromPersonPtrs( FindPersonPtrByLastName(last_name) );
}

Persons GenealogicalTree::FindPersonByLocation( const std::string& location )
{
    return GetPersonsFromPersonPtrs( FindPersonPtrByLocation(location) );
}

Persons GenealogicalTree::FindPersonByBirthDate( int year, int month, int day )
{
    return GetPersonsFromPersonPtrs( FindPersonPtrByBirthDate(year, month, day) );
}

Persons GenealogicalTree::GetPersonsFromPersonPtrs( GenealogicalTree::PersonPtrs ptrs )
{
    Persons persons;
    std::for_each(ptrs.cbegin(), ptrs.cend(), [&](PersonPtr p){ persons.push_back(p->Info); });
    return persons;
}

GenealogicalTree::PersonPtr GenealogicalTree::FindPerson( const std::string& name, const std::string& last_name,
                                                          const std::string& location, time_t birth_date )
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

GenealogicalTree::PersonPtrs GenealogicalTree::FindPersonPtrByName( const std::string& name )
{
    return FindPersonPtrByKey( NameMap, name );
}

GenealogicalTree::PersonPtrs GenealogicalTree::FindPersonPtrByLastName( const std::string& last_name )
{
    return FindPersonPtrByKey( LastNameMap, last_name );
}

GenealogicalTree::PersonPtrs GenealogicalTree::FindPersonPtrByLocation( const std::string& location )
{
    return FindPersonPtrByKey( LocationMap, location );
}

GenealogicalTree::PersonPtrs GenealogicalTree::FindPersonPtrByBirthDate( int year, int month, int day )
{
    return FindPersonPtrByKey( BirthDateMap, Utils::ConvertDateToCTime( Utils::Date{year, month, day} ) );
}

DescendantInfos GenealogicalTree::FindAllDescendantsForAllAscendants( const std::string& descendants_name,
                                                                      const std::string& ascendants_name )
{
    PersonPtrs descendants = FindPersonPtrByName( descendants_name );
    VisitedFlags visited_mask = 0;
    
    DescendantInfos result;
    if( !descendants.empty() ) {
        for( PersonPtr descendant: descendants ){
            if( visited_mask == 0 ) {
                ClearVisitedMasks();
                visited_mask = 1;
            }
            
            AscendantPtrs ascendants = FindAllAscendants( ascendants_name, descendant, visited_mask );
            
            AscendantInfos asc_infos;
            std::for_each( ascendants.cbegin(), ascendants.cend(), [&](const std::pair<PersonPtr, Distance>& asc){
                asc_infos.emplace_back(asc.first->Info, asc.second);
            } );
            result.emplace_back( descendant->Info, asc_infos );
            
            visited_mask <<= 1;
        }
    }
    return result;
}

//The BFS is being done here for each descendant to find all of his/her ascendants with the requested name.
GenealogicalTree::AscendantPtrs GenealogicalTree::FindAllAscendants( const std::string& ascendants_name,
                                                                     PersonPtr descendent,
                                                                     VisitedFlags visited_mask )
{
    std::queue<PersonPtr>   ascendant_q;
    std::queue<Distance>    distance_q;
    auto push = [&]( PersonPtr person, Distance distance )
    {
        if( person && !(person->Visited & visited_mask) ) {
            ascendant_q.push( person );
            distance_q.push( distance );
            person->Visited |= visited_mask;
        }
    };
    auto pop = [&]() -> std::pair<PersonPtr, Distance>
    {
        std::pair<PersonPtr, Distance> res;
        res.first = ascendant_q.front();    ascendant_q.pop();
        res.second = distance_q.front();    distance_q.pop();
        return res;
    };
    
    push( descendent->Parent1, 1 );
    push( descendent->Parent2, 1 );
    
    AscendantPtrs ascendants;
    std::pair<PersonPtr, Distance> current;
    while( !ascendant_q.empty() ) {
        current = pop();
        if( current.first->Info.Name == ascendants_name ) {
            ascendants.push_back( current );
        }
        push( current.first->Parent1, current.second + 1 );
        push( current.first->Parent2, current.second + 1 );
    }
    
    return ascendants;
}

inline void GenealogicalTree::ClearVisitedMasks()
{
    std::for_each( NameMap.begin(), NameMap.end(), [](auto& pair){pair.second->Visited = 0;} );
}
