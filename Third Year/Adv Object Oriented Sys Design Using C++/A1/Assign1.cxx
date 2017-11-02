#include <iostream>
#include <iomanip>
#include <map>
#include <string>
#include <algorithm>

using namespace std;

int main(){

map<string, unsigned> hist;
string s;

while (cin >> s)    //while no error in input stream or EOF
    ++hist[s];      //increment the count in the row with string s as a key

cout << "\n\nAn unsorted frequency histogram of the input is:\n";

auto const longest_entry_in_map = max_element(begin(hist),end(hist),//retrun Iterator pointer to the longest entry in map hist
    [](auto elem1, auto elem2){
        bool result = (elem1.first.size() <= elem2.first.size())? true : false;
        return result;//return true if elem1 is greater than elem2, flase otherwise
      });

auto greatest_length = (longest_entry_in_map == end(hist)) ? 0 : longest_entry_in_map->first.size(); //Check if longest entery exist, if so, greatest_length = longest_entry_in_map's key's size, else greatest_length = 0

for_each(begin(hist), end(hist),[&greatest_length] (auto const &current){
            cout
                << left
                << setfill(' ')             //set fill symbol
                << setw(greatest_length)    //set width of area to fill
                << current.first
                << " |"
                << setfill('*')             //set new fill symbol
                << setw(current.second)     //set width of area to fill
                << ""
                << "\n";
         });

return 0;
}
