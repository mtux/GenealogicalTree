/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright (C) 2016  <copyright holder> <email>
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */

#ifndef GENEALOGICALTREE_H
#define GENEALOGICALTREE_H

#include <string>
#include <ctime>
#include <map>

using PersonId = int32_t;
const PersonId UNKNOWN = -1;

struct Person
{
    std::string Name;
    std::string LastName;
    time_t      BirthDate;
    std::string Location;

    PersonId    Parent1 = UNKNOWN;
    PersonId    Parent2 = UNKNOWN;
};

class GenealogicalTree
{
public:
    GenealogicalTree();
    ~GenealogicalTree();

    GenealogicalTree(const GenealogicalTree& other) = delete;
    GenealogicalTree& operator=(const GenealogicalTree& other) = delete;
    bool operator==(const GenealogicalTree& other) = delete;

private:    
    //TODO: Replace "std::map" with a homemade "balanced binary search tree" if required.
    std::map<PersonId, Person> People;
    PersonId NextPersonId = 0;
    
    std::multimap<std::string, PersonId> NameMap;
    std::multimap<std::string, PersonId> LastNameMap;
    std::multimap<std::string, PersonId> LocationMap;
    std::multimap<time_t, PersonId>      BirthDateMap;
};

#endif // GENEALOGICALTREE_H
