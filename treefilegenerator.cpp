#include "treefilegenerator.h"
#include "genealogicaltree.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <random>

TreeFileGenerator::TreeFileGenerator()
{

}

std::string TreeFileGenerator::GetLastError()
{
    std::string err_msg = std::move( LastError );
    LastError.clear();
    return err_msg;
}

const int NUM_OF_NAMES = 100;
const std::string Names[NUM_OF_NAMES] =
{
    "James",
    "John",
    "Robert",
    "Michael",
    "Mary",
    "William",
    "David",
    "Richard",
    "Charles",
    "Joseph",
    "Thomas",
    "Patricia",
    "Christopher",
    "Linda",
    "Barbara",
    "Daniel",
    "Paul",
    "Mark",
    "Elizabeth",
    "Donald",
    "Jennifer",
    "George",
    "Maria",
    "Kenneth",
    "Susan",
    "Steven",
    "Edward",
    "Margaret",
    "Brian",
    "Ronald",
    "Dorothy",
    "Anthony",
    "Lisa",
    "Kevin",
    "Nancy",
    "Karen",
    "Betty",
    "Helen",
    "Jason",
    "Matthew",
    "Gary",
    "Timothy",
    "Sandra",
    "Jose",
    "Larry",
    "Jeffrey",
    "Frank",
    "Donna",
    "Carol",
    "Ruth",
    "Scott",
    "Eric",
    "Stephen",
    "Andrew",
    "Sharon",
    "Michelle",
    "Laura",
    "Sarah",
    "Kimberly",
    "Deborah",
    "Jessica",
    "Raymond",
    "Shirley",
    "Cynthia",
    "Angela",
    "Melissa",
    "Brenda",
    "Amy",
    "Jerry",
    "Gregory",
    "Anna",
    "Joshua",
    "Virginia",
    "Rebecca",
    "Kathleen",
    "Dennis",
    "Pamela",
    "Martha",
    "Debra",
    "Amanda",
    "Walter",
    "Stephanie",
    "Willie",
    "Patrick",
    "Terry",
    "Carolyn",
    "Peter",
    "Christine",
    "Marie",
    "Janet",
    "Frances",
    "Catherine",
    "Harold",
    "Henry",
    "Douglas",
    "Joyce",
    "Ann",
    "Diane",
    "Alice",
    "Jean",
};

const std::string LastNames[NUM_OF_NAMES] =
{
    "Smith",
    "Johnson",
    "Williams",
    "Jones",
    "Brown",
    "Davis",
    "Miller",
    "Wilson",
    "Moore",
    "Taylor",
    "Anderson",
    "Thomas",
    "Jackson",
    "White",
    "Harris",
    "Martin",
    "Thompson",
    "Garcia",
    "Martinez",
    "Robinson",
    "Clark",
    "Rodriguez",
    "Lewis",
    "Lee",
    "Walker",
    "Hall",
    "Allen",
    "Young",
    "Hernandez",
    "King",
    "Wright",
    "Lopez",
    "Hill",
    "Scott",
    "Green",
    "Adams",
    "Baker",
    "Gonzalez",
    "Nelson",
    "Carter",
    "Mitchell",
    "Perez",
    "Roberts",
    "Turner",
    "Phillips",
    "Campbell",
    "Parker",
    "Evans",
    "Edwards",
    "Collins",
    "Stewart",
    "Sanchez",
    "Morris",
    "Rogers",
    "Reed",
    "Cook",
    "Morgan",
    "Bell",
    "Murphy",
    "Bailey",
    "Rivera",
    "Cooper",
    "Richardson",
    "Cox",
    "Howard",
    "Ward",
    "Torres",
    "Peterson",
    "Gray",
    "Ramirez",
    "James",
    "Watson",
    "Brooks",
    "Kelly",
    "Sanders",
    "Price",
    "Bennett",
    "Wood",
    "Barnes",
    "Ross",
    "Henderson",
    "Coleman",
    "Jenkins",
    "Perry",
    "Powell",
    "Long",
    "Patterson",
    "Hughes",
    "Flores",
    "Washington",
    "Butler",
    "Simmons",
    "Foster",
    "Gonzales",
    "Bryant",
    "Alexander",
    "Russell",
    "Griffin",
    "Diaz",
    "Hayes",
};

const std::string Countries[NUM_OF_NAMES] =
{
    "Germany",
    "Ghana",
    "Gibraltar",
    "Greece",
    "Greenland",
    "Grenada",
    "Guam",
    "Guatemala",
    "Honduras",
    "Hong Kong",
    "Hungary",
    "Iceland",
    "India",
    "Indonesia",
    "Iran",
    "Iraq",
    "Ireland",
    "Isle of Man",
    "Israel",
    "Italy",
    "Jamaica",
    "Japan",
    "Jersey",
    "Jordan",
    "Kazakhstan",
    "Kenya",
    "Kiribati",
    "Korea, N",
    "Korea, S",
    "Liberia",
    "Libya",
    "Liechtenstein",
    "Lithuania",
    "Luxembourg",
    "Macao",
    "Macedonia",
    "Madagascar",
    "Malawi",
    "Malaysia",
    "Maldives",
    "Mali",
    "Malta",
    "Marshall Islands",
    "Micronesia",
    "Moldova",
    "Monaco",
    "Mongolia",
    "Montenegro",
    "Netherlands",
    "Netherlands Antilles",
    "New Caledonia",
    "New Zealand",
    "Nicaragua",
    "Niger",
    "Nigeria",
    "Niue",
    "Norfolk Island",
    "Northern Cyprus",
    "Northern Mariana Islands",
    "Norway",
    "Oman",
    "Pakistan",
    "Palau",
    "Palestine",
    "Philippines",
    "Pitcaim Islands",
    "Poland",
    "Portugal",
    "Puerto Rico",
    "Qatar",
    "Romania",
    "Russia",
    "Rwanda",
    "Senegal",
    "Serbia",
    "Seychelles",
    "Sierra Leone",
    "Singapore",
    "South Africa",
    "South Ossetia",
    "Spain",
    "Sri Lanka",
    "Sudan",
    "Suriname",
    "Svalbard",
    "Swaziland",
    "Sweden",
    "Switzerland",
    "Transnistria",
    "Trinidad and Tobago",
    "Tristan da Cunha",
    "Tunisia",
    "Turkey",
    "Turkmenistan",
    "Turks and Caicos Islands",
    "Tuvalu",
    "Uganda",
    "Ukraine",
    "United Kingdom",
    "United States",
};

inline std::ostream& operator<< (std::ostream& os, const Person& p)
{
    auto date = p.GetBirthDate();
    os << p.Name << ':' << p.LastName << ':' << p.Location << ':' << date.Year << ':' << date.Month << ':' << date.Day;
    return os;
}

bool TreeFileGenerator::Generate( const std::string& path, int num_of_people )
{
    LastError.clear();
    
    if( num_of_people < GEN_MIN_PEOPLE_NUM ) {
        LastError = "Too few people. Try more!";
        return false;
    }

    std::ofstream out;
    out.open( path );
    if( out.good() ) {
        out << "# Randomly generated data" << std::endl;
        std::vector<Person> people;
        //Enhancement tip: It might worth using two much smaller vectors to store two group of people.
        //                 and switch between them while choosing parents.
        try {
            people.reserve( num_of_people );
        } catch(const std::bad_alloc& e) {
            LastError = "Too many people. try less!";
            return false;
        }
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, NUM_OF_NAMES-1);
        std::uniform_int_distribution<> dis_day(1, 28);
        std::uniform_int_distribution<> dis_mon(0, 11);
        std::uniform_int_distribution<> dis_year(1900, 2016);
        std::uniform_int_distribution<> dis_p(0, num_of_people);

        auto populate_one_random_person = [&]( Person& p ){
            p.Name = Names[dis(gen)];
            p.LastName = LastNames[dis(gen)];
            p.Location = Countries[dis(gen)];
            p.SetBirthDate( Utils::Date {dis_year(gen), dis_mon(gen), dis_day(gen)} );
        };

        for(int i = 0; i < 10; ++i){
            populate_one_random_person( people[i] );
            out << people[i] << std::endl;
        }

        for(int i = 10; i < num_of_people; ++i){
            populate_one_random_person( people[i] );
            int p1 = dis_p(gen) % i;
            int p2 = dis_p(gen) % i;
            if( p1 == p2 )
                ++p2;
            out << people[i] << ';' << people[p1] << ';' << people[p2] << std::endl;
        }
        out.close();
        return true;
    } else {
        LastError = "File IO error!";
        return false;
    }
}
