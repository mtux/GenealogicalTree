
#ifndef GENEALOGICALTREE_H
#define GENEALOGICALTREE_H

#include "utils.h"

#include <string>
#include <ostream>
#include <ctime>
#include <map>
#include <vector>
#include <memory>

struct Person
{
    Person() : BirthDate(-1) {}
    Person(const std::string& name, const std::string& l_name, const std::string& location, time_t b_day)
    :Name(name), LastName(l_name), Location(location), BirthDate(b_day)
    {}
    
    std::string  Name;
    std::string  LastName;
    std::string  Location;
    time_t       BirthDate;
    
    void SetBirthDate( Utils::Date date )
    {
        BirthDate = Utils::ConvertDateToCTime( date );
    }
    Utils::Date GetBirthDate() const
    {
        return Utils::ConvertCTimeToDate( BirthDate );
    }
};

using Persons = std::vector<Person>;

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

/*
 * This class manages actual graph of data. It is a Directed Acyclic Graph, which consists of PersonNode objects as nodes of the graph,
 * and the parent1 and parent2 provide connection between people.
 * There's no direct way to find children of a person, due to the fact that there's no need as of now.
 * it would be possible to have it by adding a list of pointers to children in each node though.
 * 4 balanced binary trees (using std::multimap now) contain indexes of names to people in the graph to make finding anyone a fast thing to do.
 */
class GenealogicalTree
{
    
public:
    GenealogicalTree();
    ~GenealogicalTree();
    
    bool IsEmpty();
    
    bool AddPerson( const Person& person );
    bool AddPerson( const Person& person, const Person& parent1 );
    bool AddPerson( const Person& person, const Person& parent1, const Person& parent2 );
    
    Persons FindPersonByName( const std::string& name );
    Persons FindPersonByLastName( const std::string& last_name );
    Persons FindPersonByLocation( const std::string& location );
    Persons FindPersonByBirthDate( int year, int month, int day );
    
    DescendantInfos FindAllDescendantsForAllAscendants( const std::string& descendants_name, const std::string& ascendants_name );    
    
    GenealogicalTree(const GenealogicalTree& other) = delete;
    GenealogicalTree& operator=(const GenealogicalTree& other) = delete;
    bool operator==(const GenealogicalTree& other) = delete;
    
private:
    //Internal definitions:
    struct PersonNode;
    using PersonPtr = std::shared_ptr<PersonNode>;
    using PersonPtrs = std::vector<PersonPtr>;
    using VisitedFlags = uint64_t;
    
    //TODO: Performance tip: It might worth having a pool and a factory to create and keep PersonNodes together.
    struct PersonNode
    {
        PersonNode(const Person& p_info) : Info(p_info), Visited(0) {}
        
        Person      Info;
        
        //This will be used in BFS.
        VisitedFlags Visited;// Why do we use a 64bit integer where a boolean should suffic? because clearing this bit in all the graph is expensive during search for all descendants of all ascendants. This way we will be using all the 64 bits before a need to clear arise. Thus, only clear after 64 searches.
        
        PersonPtr   Parent1 = nullptr;
        PersonPtr   Parent2 = nullptr;
    };
    
    //TODO: Replace "std map" with a homemade "balanced binary search tree" if required.    
    std::multimap<std::string, PersonPtr> NameMap;
    std::multimap<std::string, PersonPtr> LastNameMap;
    std::multimap<std::string, PersonPtr> LocationMap;
    std::multimap<time_t, PersonPtr>      BirthDateMap;
    
    PersonPtr AddPersonImpl( const Person& person );
    PersonPtr AddPersonImpl( const Person& person, const Person& parent1 );
    PersonPtr AddPersonImpl( const Person& person, const Person& parent1, const Person& parent2 );
    
    void SetParent( GenealogicalTree::PersonPtr person, const Person& parent );
    
    PersonPtr FindPerson( const std::string& name, const std::string& last_name,
                          const std::string& location, time_t birth_date = -1 );
    
    PersonPtrs FindPersonPtrByName( const std::string& name );
    PersonPtrs FindPersonPtrByLastName( const std::string& last_name );
    PersonPtrs FindPersonPtrByLocation( const std::string& location );
    PersonPtrs FindPersonPtrByBirthDate( int year, int month, int day );
    
    Persons GetPersonsFromPersonPtrs( PersonPtrs ptrs );
    
    void ClearVisitedMasks();
    
    using Distance = uint32_t;
    using AscendantPtrs = std::vector< std::pair<PersonPtr, Distance> >;
    AscendantPtrs FindAllAscendants(const std::string& ascendants_name, PersonPtr descendent, VisitedFlags visited_mask);
    
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
