//Author: Matthew Lamont
//File: hw3functions.cpp
//Last Modified: 4/21/2014
//Description: Function header for part3 in Algorithms PS#3.


#include "hw3functions.h"



void parseData( const char* argv , std::vector<std::string>& book , bool removeCommons )
{
    //Common English and Shakespearian words that don't add meaning to an n-gram
    const char* temp_array[] = {"a","able","about","across","after","all","almost","also",
                             "am","among","an","and","any","are","art","as","at","ay","be","because",
                             "been","but","by","can","cannot","could","dear","did","do","does",
                             "dost","doth","either","else","ever","every","for","from","get",
                             "got","had","has","hath","have","he","her","hers","him","his","how",
                             "however","i","if","in","into","is","it","its","just","least","let",
                             "like","likely","may","me","might","most","must","my","neither",
                             "no","nor","not","o","of","off","often","on","only","or","other","our",
                             "own","rather","said","say","says","she","should","since","so","some",
                             "than","that","the","thee","their","them","then","there","these","they",
                             "thine","this","thou","thy","tis","to","too","twas","upon","us","wants",
                             "was","we","were","what","when","where","which","while","who","whom",
                             "why","will","with","would","yet","you","your"};


    std::ifstream infile( argv );

    std::string buffer;
    std::string line;
    std::string current_line;
    std::stringstream ss;
    std::pair<std::string , std::string> temp_pair;
    std::map<std::string , std::string> common_words;

    //This loop inserts all the common words into a map
    for( unsigned int i = 0 ; i < 130 ; ++i )
    {
        temp_pair.first = temp_array[i];
        temp_pair.second = temp_array[i];
        common_words.insert( temp_pair );
    }

    while( !infile.eof() )
    {
        buffer.clear();
        line.clear();
        getline( infile , line );

        //if a blank line is found then I tag it be ignored later
        if( line == "" )
        {
            line = "!!BLANK_LINE!!";
        }
        std::stringstream ss( line );

        while( ss >> buffer )
        {
            //Since a blank line marks the end of a clause, I insert the current clause into the book
            if( buffer == "!!BLANK_LINE!!" )
            {
                if( current_line.size() > 0 )
                {
                    book.push_back( current_line );
                    current_line.clear();
                    buffer.clear();
                    continue;
                }
                continue;
            }

            //If I find a period, then the current clause is inserted into the book
            if( buffer == "." )
            {
                if( current_line.size() > 0 )
                {
                    book.push_back( current_line );
                    current_line.clear();
                    buffer.clear();
                    continue;
                }
                continue;
            }

            //This block removes any common words, punctuation, and capital letters. It then
            //inserts the word into the current clause
            buffer = parseWord( buffer );
            if( removeCommons == true )
            {
                buffer = removeCommonWords( buffer , common_words );
            }
            buffer += " ";
            current_line += buffer;
        }
    }

    infile.close();
}

std::string parseWord( std::string word )
{
    std::string::iterator first = word.begin();
    std::string::iterator last = word.end();

    //remove any punctuation found in the word e.g. don't becomes dont
    while( first != last )
    {
        if( !isalpha( *first ) )
        {
            first = word.erase( first );
            last = word.end();
            --first;
        }

        ++first;
    }

    //makes word lowercase
    std::transform( word.begin() , word.end() , word.begin() , ::tolower );

    return word;
}

std::string removeWhiteSpace( const std::string& unformatted)
{
    //remove extra white-spaces from the word
    std::stringstream ss(unformatted);
    std::string wordFmt, word;
    while(ss >> word)
    {
        wordFmt += word;
        wordFmt += " ";
    }

    return wordFmt;
}

std::map<std::string , unsigned int> calculateN_Grams( std::vector<std::string>& book , const unsigned int gram_length )
{
    //function objects
    int line_count = 1;
    unsigned int token_count = 0;
    std::map< std::string , unsigned int > gram_map;
    std::vector<std::string> parsed_phrase;
    std::string compiled_gram;
    std::vector<std::string>::iterator start = book.begin();
    std::vector<std::string>::iterator end = book.end();

    while( start != end )
    {
        std::cout << "\rComputing Line " << line_count << " of " << book.size();
        ++line_count;

        //get the number of words in the clause
        token_count = getTokenCount( *start );

        //if there are less words that the gram length then I just skip over this clause
        if( token_count < gram_length )
        {
            ++start;
            continue;
        }

        //if the clause is the gram length, then the clause is inserted into the map
        if( token_count == gram_length )
        {
            insertGram( *start , gram_map );
            ++start;
            continue;
        }

        //if the clause is longer than the gram length, then each unique gram is found and stored.
        if( token_count > gram_length )
        {
            parsed_phrase = parsePhrase( *start );

            for( unsigned int i = 0 ; i <= parsed_phrase.size() - gram_length ; ++i )
            {
                compiled_gram = parsed_phrase[i];

                for( unsigned int j = 1 ; j < gram_length ; ++j )
                {
                    compiled_gram += " ";
                    compiled_gram += parsed_phrase[ i + j ];
                }

                insertGram( compiled_gram , gram_map );
            }

        }
        ++start;
    }

    std::cout << "\r";
    return gram_map;
}

unsigned int getTokenCount( std::string& line )
{
    std::stringstream ss( line );
    std::string buffer;
    unsigned int token_count = 0;

    //counts the number of words in the clause
    while( ss >> buffer )
    {
        ++token_count;
    }

    return token_count;
}




void insertGram( std::string& gram , std::map< std::string , unsigned int >& gram_map )
{
    //Remove white-spaces of key-value
    std::string gramFmt = removeWhiteSpace(gram);

    //if the gram is already in the map, then its number of instances is incremented
    if( gram_map.count(gramFmt) > 0 )
    {
        ++gram_map[gramFmt];
    }

    //if the gram is not in the map, then it is inserted
    else if( gram_map.count(gramFmt) == 0 )
    {
        gram_map[gramFmt] = 1;
    }

/*    std::map< std::string , unsigned int >::iterator itr = gram_map.find( gram );

    //if the gram is already in the map, then its number of instances is incremented
    if( itr != gram_map.end() )
    {
        std::pair< std::string , unsigned int > mPair = *itr;
        gram_map.erase( itr );
        int instances = mPair.second;
        gram_map[gram] = ++instances;
    }

    //if the gram is not in the map, then it is inserted
    else if( itr == gram_map.end() )
    {
        gram_map[gram] = 1;
    }
*/
}

std::vector<std::string> parsePhrase( std::string& phrase )
{
    std::vector<std::string> parsed_phrase;
    std::stringstream ss( phrase );
    std::string buffer;

    //inserts each word as an element into the vector
    while( ss >> buffer )
    {
        parsed_phrase.push_back( buffer );
    }

    return parsed_phrase;
}


std::string removeCommonWords( std::string word , std::map<std::string , std::string>& common_words )
{
    std::map<std::string,std::string>::iterator itr;
    itr = common_words.find( word );

    if( itr == common_words.end() )
    {
        return word;
    }

    else
    {
        return "";
    }
}

std::vector< gram_pair > extractPopularGrams( std::map<std::string , unsigned int>& gram_map )
{
    std::vector< gram_pair > sorting_vector;
    gram_pair temp;
    unsigned int line_count = 0;
    mapCompare myObject;

    std::map<std::string , unsigned int>::iterator first = gram_map.begin();
    std::map<std::string , unsigned int>::iterator last = gram_map.end();

    while( first != last )
    {
        std::cout << "\rChecking Gram Number " << line_count << " of " << gram_map.size();
        std::pair< std::string , unsigned int > temp_pair = *first;

        //if the top ten vector already has 10 entries, then the gram with the fewest instances
        //is replaced by the current gram
        if( sorting_vector.size() == 10 && temp_pair.second > sorting_vector[9].instances )
        {
            temp.phrase = temp_pair.first;
            temp.instances = temp_pair.second;
            sorting_vector[9] = temp;
            std::sort( sorting_vector.begin() , sorting_vector.end() , myObject );
        }

        //if the top ten vector doesn't yet have 10 entries, then the current gram is inserted
        else if ( sorting_vector.size() < 10 )
        {
            temp.phrase = temp_pair.first;
            temp.instances = temp_pair.second;
            sorting_vector.push_back( temp );
            std::sort( sorting_vector.begin() , sorting_vector.end() , myObject );
        }

        else
        {

        }

        ++first;
        ++line_count;
    }

    std::cout << "\r                                                ";
    return sorting_vector;

}

void displayPopularGrams( std::map< std::string , unsigned int >& gram_map , std::vector< gram_pair >& sorted_grams , short gram_number )
{
    std::cout << "\n\n\nThere are " << gram_map.size() << " unique " << gram_number << "-grams.\n";
    std::cout << "The 10 most common ones are:\n";

    for( unsigned int i = 0 ; i < sorted_grams.size() ; ++i )
    {
        gram_pair temp = sorted_grams[i];
        std::cout << "  " << temp.phrase << " [" << temp.instances << "]\n";
    }
}



void exportTwoGrams( std::map< std::string , unsigned int >& gram_map )
{

    std::ofstream outfile( "two_grams.txt");
    std::pair< std::string , unsigned int> mPair;
    std::string buffer;
    std::string line;

    std::map< std::string , unsigned int >::iterator first = gram_map.begin();
    std::map< std::string , unsigned int >::iterator last = gram_map.end();

    while( first != last )
    {
        buffer.clear();
        line.clear();
        mPair = *first;
        std::stringstream ss( mPair.first );
        ss >> line;
        buffer = line;
        buffer += " ";
        ss >> line;
        buffer += line;
        buffer += " ";
        ss.str("");
        ss.clear();
        line.clear();
        ss << mPair.second;
        line = ss.str();
        buffer += line;
        buffer += "\n";
        outfile.write( buffer.c_str() , buffer.size() );

        ++first;
    }
/*
    std::map< std::string , unsigned int >::iterator start = gram_map.begin();
    std::map< std::string , unsigned int >::iterator stop = gram_map.end();

    while( start != stop )
    {
        std::pair< std::string , unsigned int> mPair;
        mPair = *start;
        outfile << mPair.first << '\n';
        /*
        buffer.clear();
        line.clear();
        mPair = *first;
        std::stringstream ss( mPair.first );
        ss >> line;
        buffer = line;
        buffer += " ";
        ss >> line;
        buffer += line;
        buffer += " ";
        ss.str("");
        ss.clear();
        line.clear();
        ss << mPair.second;
        line = ss.str();
        buffer += line;
        buffer += "\n";
        outfile.write( buffer.c_str() , buffer.size() );

        ++start;
    }
*/
    outfile.close();
}
