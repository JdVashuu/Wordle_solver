#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

vector<string> loadWords(){
    vector<string> words;
    string filename = "DATA/Word_list.txt";
    ifstream file(filename);

    if(!file.is_open()){
        cerr << "the file couldnt be opened :  " << filename << endl;
        abort();
    }

    string line;

    while(getline(file,line)){
        //capitalising letters
        for_each(line.begin(), line.end(), [](char& c){
            c = :: toupper(c) ;
        });

        if(find(words.begin(),words.end(), line) == words.end()){
            words.push_back(line);
        }
    }

    return words;
}

int main(){
    vector<string> words = loadWords();
    for(int i = 0; i < 15; i++){
        words[i];
    }


}
