
#ifndef GENEALOGICALTREE_H
#define GENEALOGICALTREE_H

#include "utils.h"

#include <string>
#include <ostream>
#include <map>
#include <vector>
#include <memory>

struct Person
{
    Person() {}
    Person(const std::string& name, const std::string& l_name, const std::string& location, Utils::Date b_day)
    :Name(name), LastName(l_name), Location(location), BirthDate(b_day)
    {}
    
    std::string  Name;
    std::string  LastName;
    std::string  Location;
    Utils::Date  BirthDate;
};

typedef std::vector<Person> Persons;

struct AscendantInfo
{
    AscendantInfo(Person asc, uint32_t dis): Ascendant(asc), Distance(dis){}
    Person      Ascendant;
    uint32_t    Distance;
};
typedef std::vector<AscendantInfo> AscendantInfos;

struct DescendantInfo
{
    DescendantInfo(Person desc, AscendantInfos ascendants): Descendant(desc), Ascendants(ascendants){}
    Person          Descendant;
    AscendantInfos  Ascendants;
};
typedef std::vector<DescendantInfo> DescendantInfos;

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
        
private:
    //Internal definitions:
    struct PersonNode;
    typedef std::shared_ptr<PersonNode> PersonPtr;
    typedef std::vector<PersonPtr> PersonPtrs;
    typedef unsigned long long VisitedFlags;
    
    //TODO: Performance tip: It might worth having a pool and a factory to create and keep PersonNodes together.
    struct PersonNode
    {
        PersonNode(const Person& p_info) : Info(p_info), Visited(0), Parent1(nullptr), Parent2(nullptr) {}
        
        Person      Info;
        
        //This will be used in BFS.
        VisitedFlags Visited;// Why do we use a 64bit integer where a boolean should suffic? because clearing this bit in all the graph is expensive during search for all descendants of all ascendants. This way we will be using all the 64 bits before a need to clear arise. Thus, only clear after 64 searches.
        
        PersonPtr   Parent1;
        PersonPtr   Parent2;
    };
    
    //TODO: Replace "std map" with a homemade "balanced binary search tree" if required.    
    std::multimap<std::string, PersonPtr> NameMap;
    std::multimap<std::string, PersonPtr> LastNameMap;
    std::multimap<std::string, PersonPtr> LocationMap;
    std::multimap<Utils::Date, PersonPtr> BirthDateMap;
    
    PersonPtr AddPersonImpl( const Person& person );
    PersonPtr AddPersonImpl( const Person& person, const Person& parent1 );
    PersonPtr AddPersonImpl( const Person& person, const Person& parent1, const Person& parent2 );
    
    void SetParent( GenealogicalTree::PersonPtr person, const Person& parent );
    
    PersonPtr FindPerson( const std::string& name, const std::string& last_name,
                          const std::string& location, Utils::Date birth_date = Utils::Date() );
    
    PersonPtrs FindPersonPtrByName( const std::string& name );
    PersonPtrs FindPersonPtrByLastName( const std::string& last_name );
    PersonPtrs FindPersonPtrByLocation( const std::string& location );
    PersonPtrs FindPersonPtrByBirthDate( int year, int month, int day );
    
    Persons GetPersonsFromPersonPtrs( PersonPtrs ptrs );
    
    void ClearVisitedMasks();
    
    typedef uint32_t Distance;
    typedef std::vector< std::pair<PersonPtr, Distance> > AscendantPtrs;
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
