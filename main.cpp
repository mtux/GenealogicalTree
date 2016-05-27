
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
//     tree.AddPerson(p1);
//     tree.AddPerson(p2);
    tree.AddPerson(p3, p1, p2);
    auto res = tree.FindPersonPtrByLastName("Will");
    for(auto p: res){
        std::cout << p->Info.Name << ':' << p->Info.LastName << std::endl;
        std::cout << "Parent: " << p->Parent1->Info.Name << ':' << p->Parent1->Info.LastName << std::endl;
        std::cout << "Parent: " << p->Parent2->Info.Name << ':' << p->Parent2->Info.LastName << std::endl;
        std::cout << "=====================" ;
    }
}
