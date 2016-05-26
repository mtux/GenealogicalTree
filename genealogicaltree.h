
#ifndef GENEALOGICALTREE_H
#define GENEALOGICALTREE_H

#include <string>
#include <ctime>
#include <map>
#include <vector>
#include <memory>
#include <experimental/optional>

using String = std::string;

struct Person
{
    Person(const String& name, const String& l_name, const String& location, time_t b_day)
    :Name(name), LastName(l_name), Location(location), BirthDate(b_day)
    {}
    
    String  Name;
    String  LastName;
    String  Location;
    time_t  BirthDate;
};
using Persons = std::vector<Person>;
using OptionalPerson = std::experimental::optional<Person>;

class GenealogicalTree
{
    struct PersonNode;
    using PersonPtr = std::shared_ptr<PersonNode>;
    
public:
    GenealogicalTree();
    ~GenealogicalTree();
    
    bool AddPerson( Person person, OptionalPerson parent1 = OptionalPerson(), OptionalPerson parent2 = OptionalPerson() );
    
    Persons FindPersonByName( const String& name );
    Persons FindPersonByLastName( const String& last_name );
    Persons FindPersonByLocation( const String& location );
    Persons FindPersonByBirthDate( time_t birth_date );

    GenealogicalTree(const GenealogicalTree& other) = delete;
    GenealogicalTree& operator=(const GenealogicalTree& other) = delete;
    bool operator==(const GenealogicalTree& other) = delete;
    
private:
    void SetParent( PersonPtr person, Person parent );

private:
    struct PersonNode
    {
        PersonNode(const Person& p_info) : Info(p_info) {}
        
        Person      Info;
        
        PersonPtr   Parent1 = nullptr;
        PersonPtr   Parent2 = nullptr;
    };
    
    //TODO: Replace "std map" with a homemade "balanced binary search tree" if required.    
    std::multimap<String, PersonPtr> NameMap;
    std::multimap<String, PersonPtr> LastNameMap;
    std::multimap<String, PersonPtr> LocationMap;
    std::multimap<time_t, PersonPtr> BirthDateMap;
};

#endif // GENEALOGICALTREE_H
