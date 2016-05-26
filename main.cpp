
#include "genealogicaltree.h"
#include <ctime>
#include <iostream>

void test();

int main(int argc, char **argv)
{
    test();
    
    return 0;
}

void test()
{
    GenealogicalTree tree;
    std::tm t;
    t.tm_year = 1985;
    t.tm_mon = 9;
    t.tm_mday = 16;
    Person p1 {"Bob", "Lee", "Malaysia", std::mktime(&t) };
    Person p2 {"Jake", "Lee", "Iran", std::mktime(&t) };
    Person p3 {"Bob", "Will", "Malaysia", std::mktime(&t)   };
    tree.AddPerson(p1);
    tree.AddPerson(p2);
    tree.AddPerson(p3);
    auto res = tree.FindPersonByName("Bob");
    for(auto person: res){
        std::cout << person.Name << std::endl;
    }
}
