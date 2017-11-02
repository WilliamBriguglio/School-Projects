#include <string>
#include <locale>
#include <algorithm>
#include <iostream>

template <typename TrimPredicate>
auto string_range_trim(std::string& str, TrimPredicate p) ->
  std::pair<
    std::string::iterator,  // typedef to string iterator type
    std::string::iterator   // typedef to string iterator type
  >
{
    auto last_non_ws_iter = find_if_not(str.rbegin(), str.rend(), p).base();
    auto first_non_ws_iter = find_if_not(begin(str), last_non_ws_iter, p);
    return std::make_pair(first_non_ws_iter, last_non_ws_iter);
}

int main(int argc, char *argv[]){

    using namespace std;

    string testString = "     A simple beautiful test string.    ";

    auto trimIterator = string_range_trim( testString, [](auto ch) {return isspace(ch, cin.getloc()); } );

    for_each(trimIterator.first, trimIterator.second ,[](auto const &current){
                cout << current;
            });

    cout << "|<------------\n";


    return 0;
}



