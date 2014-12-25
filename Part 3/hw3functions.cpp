


#include "hw3functions.h"


gram_pair::gram_pair()
{
    phrase = "";
    instances = 0;
}


void parseData( const char* argv , std::vector<std::string>& book )
{
    std::ifstream infile( argv );

    std::string buffer;
    std::string line;
    std::string current_line;
    std::stringstream ss;
    std::pair<std::string , std::string> temp_pair;
    std::map<std::string , std::string> common_words;
    std::vector< std::string > punctuationArray;

    const char* punctuationList[] = {
                                     "!" ,
                                     "!'" ,
                                     "!," ,
                                     ",'" ,
                                     "." ,
                                     ".\"" ,
                                     ".'" ,
                                     ".]" ,
                                     "1." ,
                                     "2" ,
                                     "3." ,
                                     "4." ,
                                     "5." ,
                                     "6." ,
                                     "7." ,
                                     "8." ,
                                     "9." ,
                                     ":" ,
                                     ":'",
                                     ";" ,
                                     ";'" ,
                                     "?" ,
                                     "?'" ,
                                     "?." ,
                                     "[" ,
                                     "]"
                                     };



    for( unsigned int i = 0 ; i < (sizeof(punctuationList)/sizeof(char*)) ; ++i )
    {
        punctuationArray.push_back( punctuationList[i] );
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
            if( checkForPunctuation( buffer , punctuationArray ) )
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

            //This block removes any punctuation and capital letters. It then
            //inserts the word into the current clause
            buffer = parseWord( buffer );
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


bool checkForPunctuation( std::string buffer , std::vector<std::string> punctuationArray )
{
    //checks the buffer against all elements in the punctuation array
    for( unsigned int i = 0 ; i < punctuationArray.size() ; ++i )
    {
        if( buffer == punctuationArray[i] )
        {
            return true;
        }
    }

    return false;
}


std::map<std::string , unsigned int> calculateN_Grams( std::vector<std::string>& book , const unsigned int gram_length , bool ignoreCommonWords )
{
    //function objects
    int line_count = 1;
    unsigned int token_count = 0;
    std::map< std::string , unsigned int > gram_map;
    std::map< std::string , std::string > common_words;
    std::pair< std::string , std::string > temp_pair;
    std::vector<std::string> parsed_phrase;
    std::string compiled_gram;
    std::vector<std::string>::iterator start = book.begin();
    std::vector<std::string>::iterator end = book.end();


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

    //This loop inserts all the common words into a map
    for( unsigned int i = 0 ; i < 130 ; ++i )
    {
        temp_pair.first = temp_array[i];
        temp_pair.second = temp_array[i];
        common_words.insert( temp_pair );
    }

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
            if( ignoreCommonWords == false )
            {
                insertGram( *start , gram_map );
                ++start;
                continue;
            }

            if( ignoreCommonWords == true && !containsCommonWord( *start , common_words ) )
            {
                insertGram( *start , gram_map );
                ++start;
                continue;
            }

            else
            {
                ++start;
                continue;
            }
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

                if( ignoreCommonWords == false )
                {
                    insertGram( compiled_gram , gram_map );
                }

                if( ignoreCommonWords == true && !containsCommonWord( compiled_gram , common_words ) )
                {
                    insertGram( compiled_gram , gram_map );
                }
            }

        }
        ++start;
    }

    std::cout << "\r";

    return gram_map;
}



bool containsCommonWord( std::string gram , std::map< std::string , std::string > common_words )
{
    std::stringstream ss( gram );
    std::string line;

    //compares each word in the gram against the common words map
    while( ss >> line )
    {
        if( common_words[line] == line )
        {
            //if a common word is found then true is returned
            return true;
        }

        line.clear();
    }

    //if no common words are found, then returns false
    return false;
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
    std::cout << "\n\nThere are " << gram_map.size() << " unique " << gram_number << "-grams.\n";
    std::cout << "The 10 most common ones are:\n";

    for( unsigned int i = 0 ; i < sorted_grams.size() ; ++i )
    {
        gram_pair temp = sorted_grams[i];
        std::cout << "  " << temp.phrase << " [" << temp.instances << "]\n";
    }
}



void exportTwoGrams( std::map< std::string , unsigned int >& gram_map )
{

    std::ofstream outfile( "two_grams_edit_2.txt" );
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

    outfile.close();
}


