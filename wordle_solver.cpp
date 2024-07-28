#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

class WordInfo{
    public:
        string s;
        int color[5];
};

vector<string> loadWords(){
    vector<string> words;
    string filename = "DATA/Word_list.txt";
    ifstream file(filename);

    if(!file.is_open()){
        cerr << "the file couldnt be opened :  " << filename << endl;
        abort();
    }

    string line;
    int wordCount;

    while(getline(file,line)){

        // Trim whitespace from the beginning and end of the line
        line.erase(0, line.find_first_not_of(" \t\r\n"));
        line.erase(line.find_last_not_of(" \t\r\n") + 1);

        //capitalising letters
        transform(line.begin(), line.end(), line.begin(), ::toupper);

        if(find(words.begin(),words.end(), line) == words.end()){
            words.push_back(line);
            wordCount++;
        }
    }

    cout << "loaded " << wordCount << " words from the file. \n";
    return words;
}




void CASE0(WordInfo& word, int index, vector<int> Available_alphabets){
    //ascii of a is 65
    int letter = (int)word.s[index];
    auto it = find(Available_alphabets.begin(), Available_alphabets.end(), letter);

    if(it != Available_alphabets.end()){
        Available_alphabets.erase(it);
    }
}

void CASE1(WordInfo& word, int index, vector<int> mandatory_letters){

    int letter = (int)word.s[index];
    mandatory_letters.push_back(letter);
}

void CASE2(WordInfo& word,int index,vector<string> guessWords){

}

vector<string> AllPossibleWords(WordInfo& word, vector<int> Available_alphabets, vector<int> mandatory_letters,  vector<string> guessWords
    , vector<string> words){

    vector<string> possibleWords;

    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 5; j++){
            if(word.color[j] == 0){
                CASE0(word, j, Available_alphabets);
            }else if(word.color[j] == 1){
                CASE1(word, j, mandatory_letters);
            }else {
                CASE2(word, j, guessWords);

            }
        }
    }

    return possibleWords;
}


int main(){
    vector<string> words = loadWords();

    vector<int> Available_alphabets;
    for(int i = 0; i < 26; i++){
        Available_alphabets[i] = (int)'a' + i;
    }

    vector<int> mandatory_letters;
    vector<string> guessWords;


    cout << "\nPress Enter to exit...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');





    //int tempcolors[] = {1, 1, 0, 1, 1};
    //WordInfo word1;
    // word1.s = "chair";
    // for(int i = 0; i < 5; i++){
    //     word1.color[i] = tempcolors[i];
    // }
}
