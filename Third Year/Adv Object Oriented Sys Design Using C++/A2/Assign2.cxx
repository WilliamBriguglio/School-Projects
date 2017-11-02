#include <string>
#include <locale>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;


template <typename TrimPredicate>
auto string_range_trim(std::string& str, TrimPredicate p) ->    //returns a pair of iterators pointing to the first non-whitespace and the last non-whitespace character
  std::pair<
    std::string::iterator,  // typedef to string iterator type
    std::string::iterator   // typedef to string iterator type
  >
{
    auto last_non_ws_iter = find_if_not(str.rbegin(), str.rend(), p).base();
    auto first_non_ws_iter = find_if_not(begin(str), last_non_ws_iter, p);
    return std::make_pair(first_non_ws_iter, last_non_ws_iter);
}

auto read_next_termcap_entry(istream& is) ->
  std::pair<bool, string>
{
  stringstream buffer;

  auto loc = is.getloc();

  buffer.imbue(loc);

  bool data = false; // set to true when there is data in buffer
  bool continued_line = true; // set to true if line is continued

  for (
    string line;
    (!data || continued_line) && getline(is,line);
  )
  {
    continued_line = false;

    if (line.empty()) //check if line is empty
        continue;

    auto trimmed_range = string_range_trim( //obtaint trimed line
      line,
      [loc](auto const &current){   //lamba function which returns true if the current character is a space
            return isspace(current,loc);
      }
    );

    if (trimmed_range.first == trimmed_range.second) //check if line is all whitespaces
        continue;

    if (*trimmed_range.first == '#')  //check if line is a comment
        continue;

    auto last_char_iter = trimmed_range.second;
    last_char_iter--;

    if (*last_char_iter == '\\'){   //check if line continuation
            continued_line = true;
            if(trimmed_range.first == last_char_iter)
                continue;
            trimmed_range.second = last_char_iter;
    }

    bool const header_line =    //check if header line
      isalpha(*trimmed_range.first, loc)
    ;

    bool const capability_line = find(begin(line), trimmed_range.first, '\t') != trimmed_range.first; // check if capability line

    if (header_line || capability_line) //if head or capability line, at trimmed line to buffer
    {
      data = true;
      copy(
        trimmed_range.first,
        trimmed_range.second,
        ostreambuf_iterator<char>(buffer));
    }
  }
  return { data, buffer.str() };    //return boolean which determines if data was read into buffeer and string from buffer
}

int main(int argc, char *argv[]){

    if (argc != 2){ //check argument count
        cerr << "Usage: " << argv[0] << " <termcap_db_file>\n";
        return 1;
    }

    ifstream in(argv[1]); //open filename from arugment one

    if (!in){ //check for ifstream errors
        cerr << "Unable to open file. Aborting.\n";
        return 2;
    }

    for (
        auto result = read_next_termcap_entry(in); //read in first line from termcap
        result.first; //as long as there was data read in last iteration
        result = read_next_termcap_entry(in) //read in next line in termcap
    )
        cout << result.second << '\n'; //output current line of termcap

    return 0;
}



