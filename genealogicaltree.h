
#ifndef GENEALOGICALTREE_H
#define GENEALOGICALTREE_H

#include "utils.h"

#include <string>
#include <ostream>
#include <ctime>
#include <map>
#include <vector>
#include <memory>
#include <experimental/optional>

using String = std::string;

struct Person
{
    Person() : BirthDate(-1) {}
    Person(const String& name, const String& l_name, const String& location, time_t b_day)
    :Name(name), LastName(l_name), Location(location), BirthDate(b_day)
    {}
    
    String  Name;
    String  LastName;
    String  Location;
    time_t  BirthDate;
    
    void SetBirthDate( Utils::Date date )
    {
        BirthDate = Utils::ConvertDateToCTime( date );
    }
    Utils::Date GetBirthDate() const
    {
        return Utils::ConvertCTimeToDate( BirthDate );
    }
};

inline std::ostream& operator<< (std::ostream& os, const Person& p)
{
    auto date = p.GetBirthDate();
    os << p.Name << ':' << p.LastName << ':' << p.Location << ':' << date.Year << ':' << date.Month << ':' << date.Day;
    return os;
}

using Persons = std::vector<Person>;
using OptionalPerson = std::experimental::optional<Person>;

struct AscendantInfo
{
    AscendantInfo(Person asc, uint32_t dis): Ascendant(asc), Distance(dis){}
    Person      Ascendant;
    uint32_t    Distance;
};
using AscendantInfos = std::vector<AscendantInfo>;
struct DescendantInfo
{
    DescendantInfo(Person desc, AscendantInfos ascendants): Descendant(desc), Ascendants(ascendants){}
    Person          Descendant;
    AscendantInfos  Ascendants;
};
using DescendantInfos = std::vector<DescendantInfo>;

class GenealogicalTree
{
    struct PersonNode;
    using PersonPtr = std::shared_ptr<PersonNode>;
    using PersonPtrs = std::vector<PersonPtr>;
    
public:
    GenealogicalTree();
    ~GenealogicalTree();
    
    PersonPtr AddPerson( Person person, OptionalPerson parent1 = OptionalPerson(),
                                        OptionalPerson parent2 = OptionalPerson() );
    
    Persons FindPersonByName( const String& name );
    Persons FindPersonByLastName( const String& last_name );
    Persons FindPersonByLocation( const String& location );
    Persons FindPersonByBirthDate( int year, int month, int day );
    
    DescendantInfos FindAllDescendantsForAllAscendants( const String& descendants_name, const String& ascendants_name );
    
    GenealogicalTree(const GenealogicalTree& other) = delete;
    GenealogicalTree& operator=(const GenealogicalTree& other) = delete;
    bool operator==(const GenealogicalTree& other) = delete;
    
private:
    //TODO: Performance tip: It might worth having a pool and a factory to create and keep PersonNodes together.
    struct PersonNode
    {
        PersonNode(const Person& p_info) : Info(p_info), Visited(0) {}
        
        Person      Info;
        
        //This will be used in BFS.
        uint64_t    Visited;// Why do we use a 64bit integer where a boolean should suffic? because clearing this bit in all the graph is expensive during search for all descendants of all ascendants. This way we will be using all the 64 bits before a need to clear arise. Thus, only clear after 64 searches.
        
        PersonPtr   Parent1 = nullptr;
        PersonPtr   Parent2 = nullptr;
    };
    
    //TODO: Replace "std map" with a homemade "balanced binary search tree" if required.    
    std::multimap<String, PersonPtr> NameMap;
    std::multimap<String, PersonPtr> LastNameMap;
    std::multimap<String, PersonPtr> LocationMap;
    std::multimap<time_t, PersonPtr> BirthDateMap;
    
    void SetParent( GenealogicalTree::PersonPtr person, const Person& parent );
    
    PersonPtr FindPerson( const String& name, const String& last_name, const String& location, time_t birth_date = -1 );
    
    PersonPtrs FindPersonPtrByName( const String& name );
    PersonPtrs FindPersonPtrByLastName( const String& last_name );
    PersonPtrs FindPersonPtrByLocation( const String& location );
    PersonPtrs FindPersonPtrByBirthDate( int year, int month, int day );
    
    Persons GetPersonsFromPersonPtrs( PersonPtrs ptrs );
    
    void ClearVisitedMasks();
    
    struct AscendantPtr
    {
        AscendantPtr(PersonPtr asc, uint32_t dis): Ascendant(asc), Distance(dis){}
        PersonPtr   Ascendant;
        uint32_t    Distance;
    };
    using AscendantPtrs = std::vector<AscendantPtr>;
    AscendantPtrs FindAllAscendants(String ascendants_name, PersonPtr descendent, uint64_t visited_mask);
    
    template<typename MapType, typename KeyType>
    PersonPtrs FindPersonPtrByKey(const MapType& map, KeyType key)
    {
        PersonPtrs ptrs;
        auto it = map.lower_bound(key);
        while ( it != map.end() && (*it).first == key ) {
            ptrs.push_back( (*it).second );
            ++it;
        }
        return ptrs;
    }
    
};

#endif // GENEALOGICALTREE_H
