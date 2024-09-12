#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include <unordered_map>
using namespace std;
using PatternTable = unordered_map<string, unordered_map<string, string> >;

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

void checks(vector<char>& nonolist, vector<char>& yeslist,vector<char> correct, WordInfo& word){
    for(int i = 0; i < 5; i++){
        if(word.color[i] == 0){
            nonolist.push_back(word.s[i]);
        }
    }

    for(int i = 0;i < 5; i++){
        if(word.color[i] == 1){
            yeslist.push_back(word.s[i]);
        }
    }

    for(int i = 0;i < 5; i++){
        if(word.color[i] == 2){
            correct[i] = word.s[i];
        }
    }

}

bool isPossible(string& word, vector<char> nonolist, vector<char> yesyeslist, vector<char> correct){

    //eleminating words with letters in nonolist
    for(char letter : nonolist){
        if(word.find(letter) != string::npos){
            return false;
        }
    }

    //to check if the word matches the known correct position in 'correct'
    for(size_t i = 0; i < correct.size(); i++){
        if(correct[i] != '_' & word[i] != correct[i]){
            return false;
        }
    }

    //to check if yesyeslist are present but not in place of 'correct'
    for( size_t i = 0; i < word.size(); ++i){
        if(find(yesyeslist.begin(), yesyeslist.end(), word[i]) != yesyeslist.end()){
            if(correct[i] == word[i]){
                return false;
            }
        }
    }

    //to ensure all letters of yesyeslist are present
    for(char letter : yesyeslist){
        if(word.find(letter) == string::npos){
            return false;
        }
    }
    return true;
}

vector<string> possibilities(vector<string> all_word,vector<string>& guessWords, vector<char> nonolist, vector<char> yesyeslist, vector<char> correct){

    //check if it appears
    for(string word : all_word){
        if(isPossible(word, nonolist, yesyeslist, correct)){
           guessWords.push_back(word);
        }
    }
    return guessWords;
}

//three-step process to calculate entropy
    //1. understanding feedback pattern
string generateFeedback(string guess, string target){
    string feedback  = "_____";
    //for green
    for(int i = 0; i < guess.length(); i++) {
        if(guess[i] == target[i]) {
            feedback[i] = 'G';
        }
    }
    //for yellow
    for(int i = 0; i < guess.length(); i++){
        if(feedback[i] != 'G'){
            if(find(target.begin(), target.end(), guess[i]) != target.end()){
                feedback[i] = 'Y';
            }
        }
    }
    return feedback;
}

    //2. precomputing and calculating the patterns
PatternTable precomputePattern(vector<string> wordList){
    PatternTable pattern_table;

    for(auto& guess : wordList){
        for(auto& target : wordList){
            string feedback = generateFeedback(guess, target);
            pattern_table[guess][target] = feedback;
        }
    }

    return pattern_table;
}

    //3. calculate frequency and entropy of target
double calculateEntropy(string& guess, vector<string>& wordList, PatternTable& pattern_table){
    unordered_map<string, int> patternCount;
    int totalWords = wordList.size();

    for(auto& target : wordList){
        string pattern = pattern_table.at(guess).at(target);
        patternCount[pattern]++;
    }

    double entropy = 0;
    for(auto& [pattern, count] : patternCount){
        double probablity = static_cast<double>(count)/ totalWords;
        entropy -= probablity * log2(probablity);
    }

    return entropy;
}

WordInfo getUserInput(){

}

string suggestBestWord(){

}


int main(){
    vector<string> all_words = loadWords();

    vector<char> nonolist;
    vector<char> yesyeslist;
    vector<char> correct(5, '_');

    vector<string> guessWords;
    int attempts = 0;
    const int MAX_ATTEMPTS = 6;

    while(attempts < MAX_ATTEMPTS){
        WordInfo currentGuess = getUserInput();
        checks(nonolist, yesyeslist, correct, currentGuess);

        guessWords = possibilities(all_words, guessWords, nonolist, yesyeslist, correct);
        vector<pair<string, double> > wordEntropies;
        for(const auto& word : guessWords) {
            double entropy = calculateEntropy(guessWords, all_words);
            wordEntropies.push_back({word, entropy});
        }

        string bestWord = suggestBestWord(wordEntropies);

        cout << "Suggested word: " << bestWord << endl;
        cout << "Remaining possibilities: " << guessWords.size() << endl;

        if (currentGuess.color[0] == 2 && currentGuess.color[1] == 2 &&
            currentGuess.color[2] == 2 && currentGuess.color[3] == 2 &&
            currentGuess.color[4] == 2) {
            cout << "Congratulations! You've guessed the word!" << endl;
            break;
        }

        attempts++;
    }
    if (attempts == MAX_ATTEMPTS) {
        cout << "Sorry, you've run out of attempts." << endl;
    }

    return 0;

    //int tempcolors[] = {1, 1, 0, 1, 1};
    //WordInfo word1;
    // word1.s = "chair";
    // for(int i = 0; i < 5; i++){
    //     word1.color[i] = tempcolors[i];
    // }
}
