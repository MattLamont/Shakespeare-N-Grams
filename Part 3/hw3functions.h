#ifndef HW3FUNCTIONS_H_INCLUDED
#define HW3FUNCTIONS_H_INCLUDED

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <map>




//This is a struct I use to store a gram and how many times it occurs in the file
struct gram_pair
{
    gram_pair();
    std::string phrase;
    unsigned int instances;
};

//This struct is used as an argument to the std::sort algorithm
struct mapCompare
{
    bool operator () ( const gram_pair& lhs , const gram_pair& rhs ) {return lhs.instances > rhs.instances;}
};


//This function iterates through the file and stores the data into a vector that is provided as the first
//argument. If the second argument is true then all the common words are removed. Otherwise they are left
//in.
void parseData( const char* argv , std::vector<std::string>& book );



//This a helper function that removes punctuation in words and makes the word lowercase.
std::string parseWord( std::string word );

std::string removeWhiteSpace( const std::string& unformatted);

bool checkForPunctuation( std::string buffer , std::vector<std::string> punctuationArray );

//This function takes the vector filled with the parsed file and the length of gram to be calculated. It then iterates
//through each element of the vector and finds the grams within it. Each gram is stored in a map and this map
//is returned.
std::map<std::string , unsigned int> calculateN_Grams( std::vector<std::string>& book , const unsigned int gram_length , bool ignoreCommonWords );


bool containsCommonWord( std::string gram , std::map< std::string , std::string > common_words );



//This is just a helper function that returns the number of words in a given line.
unsigned int getTokenCount( std::string& line );



//This function takes a gram and the map of current grams. If the gram already exists in the map, then its
//count is incremented. Otherwise the gram is inserted into the map.
void insertGram( std::string& gram , std::map< std::string , unsigned int >& gram_map );



//This is just a helper function that breaks down an entire file line into the individual words that are
//stored in a vector
std::vector<std::string> parsePhrase( std::string& phrase );



//This function takes the map filled with the grams and then places the top ten grams into a vector
//which is returned.
std::vector< gram_pair > extractPopularGrams( std::map<std::string , unsigned int>& gram_map );



//This function takes the map filled with the grams and a vector that contains the top ten grams. It then
//outputs them to the screen.
void displayPopularGrams( std::map< std::string , unsigned int >& gram_map , std::vector< gram_pair >& sorted_grams , short gram_number );



//This function iterates through the map and places each gram onto a seperate line
//in a text file
void exportTwoGrams( std::map< std::string , unsigned int >& gram_map );





#endif // HW3FUNCTIONS_H_INCLUDED
