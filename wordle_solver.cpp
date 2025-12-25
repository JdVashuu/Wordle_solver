#include <algorithm>
#include <cctype>
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

// using PatternTable = unordered_map<string, unordered_map<string, string> >;
unordered_map<string, int> wordToIndex;
vector<unsigned char> PatternTable;



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
            wordToIndex[line] = words.size() - 1;
            wordCount++;
        }

    }

    cout << "loaded " << wordCount << " words from the file. \n";
    return words;
}

void checks(vector<char>& nonolist, vector<char>& yeslist,vector<char>& correct, WordInfo& word){
    for(int i = 0; i < 5; i++){
        if(word.color[i] == 0){
            nonolist.push_back(word.s[i]);
        }
    }

    for(int i = 0;i < 5; i++){
        if(word.color[i] == 1){
            if(find(yeslist.begin(), yeslist.end(), word.s[i]) == yeslist.end()){
                yeslist.push_back(word.s[i]);
            }
        }
    }

    for(int i = 0;i < 5; i++){
        if(word.color[i] == 2){
            correct[i] = word.s[i];
        }
    }
}

bool isPossible(string& word, vector<char> nonolist, vector<char> yesyeslist, vector<char> correct) {

    // Eliminate words with letters in nonolist
    for (char letter : nonolist) {
        if (word.find(letter) != string::npos) {
            return false;
        }
    }

    // Check if the word matches the known correct position in 'correct'
    for (size_t i = 0; i < correct.size(); ++i) {
        if (correct[i] != '_' && word[i] != correct[i]) {
            return false;
        }
    }

    // Ensure all letters in yesyeslist are present but not in the same position as 'correct'
    // potentially wrong
    for (char letter : yesyeslist) {
            bool found_in_word = false;

            for (size_t i = 0; i < word.size(); ++i) {
                if (word[i] == letter) {
                    found_in_word = true;
                    break;  // We only need to find the letter once
                }
            }

            if (!found_in_word) {
                return false;
            }
    }


    // Passed all checks
    return true;
}

vector<string> possibilities(vector<string> all_word,vector<string>& guessWords, vector<char> nonolist, vector<char> yesyeslist, vector<char> correct){

    guessWords.clear();
    int count;
    //check if it appears
    for(string word : all_word){
        if(isPossible(word, nonolist, yesyeslist, correct)){
           guessWords.push_back(word);
           count++;
        }
    }

    cout << "Debug: Found " << count << " possible words" << endl;
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

    //3. calculate frequency and entropy of target
double calculateEntropy(const string& guess, vector<string>& wordList, int totalWords){
    int counts[243] = {0};
    int guessIdx = wordToIndex[guess];

    for(const string target : wordList){
        int targetIdx = wordToIndex[target];
        unsigned char patterns = PatternTable[guessIdx * totalWords + targetIdx];
        counts[patterns]++;
    }

    double entropy = 0;
    for(int i = 0; i < 243; i++){
        if(counts[i] > 0){
            double probablity = (double)counts[i] / totalWords;
            entropy -= probablity * log2(probablity);
        }
    }
    return entropy;
}

WordInfo getUserInput(){
    WordInfo input;
    cout << "Enter your guess(5 letters) : ";
    cin >> input.s;
    transform(input.s.begin(), input.s.end(),input.s.begin(), ::toupper);

    cout << "Enter the color feedback (0 for gray, 1 for yellow, 2 for green): ";
    for (int i = 0; i < 5; i++) {
        cin >> input.color[i];
    }
    return input;
}

string suggestBestWord(vector<pair<string, double> >&  wordEntropies){
    if(wordEntropies.empty()){
        return "";
    }
    auto maxEntropy = max_element(wordEntropies.begin(), wordEntropies.end(),
        [](const auto& a, const auto& b) { return a.second < b.second; });
    return maxEntropy->first;
}

unsigned char packPattern(const string& p){
    int val = 0;
    for(int i = 0; i < 5; i++){
        int v = (p[i] == 'G' ? 3 : (p[i] == 'Y' ? 2 : 1));
        val = val * 3 + v;
    }

    return (unsigned char)val;
}

// works on formula mat[row][col] = mat[row + col * rowCount] when we store 2D matrix as 1D
void generateBinaryCache(const vector<string> &words){
    int N = words.size();
    vector<unsigned char> buffer(N * N);

    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            string feedback = generateFeedback(words[i], words[j]);
            buffer[j + i * N] = packPattern(feedback);
        }
    }

    ofstream outfile("patterns.bin", ios::binary);
    outfile.write(reinterpret_cast<char*>(buffer.data()), buffer.size());
    outfile.close();

    cout << "Generated the pattern.bin\n";
}

void loadPatternTable(int N){
    PatternTable.resize(N * N);
    ifstream infile("patterns.bin", ios::binary);

    if(!infile){
        cout << "The pattern file doesnt exist. Generating now...\n";
        vector<string> all_words = loadWords();
        generateBinaryCache(all_words);
        infile.open("patterns.bin", ios::binary);
    }

    infile.read(reinterpret_cast<char*>(PatternTable.data()), N*N);
    cout << "Loaded binary file into the memory\n";
}

int main(){
    vector<string> all_words = loadWords();
    int totalWords = all_words.size();

    loadPatternTable(2310);

    vector<char> nonolist;
    vector<char> yesyeslist;
    vector<char> correct(5, '_');

    vector<string> guessWords = all_words;
    int attempts = 0;
    const int MAX_ATTEMPTS = 6;

    while (attempts < MAX_ATTEMPTS) {
        WordInfo currentGuess = getUserInput();
        checks(nonolist, yesyeslist, correct, currentGuess);

        guessWords = possibilities(all_words, guessWords, nonolist, yesyeslist, correct);
        if(guessWords.empty()){
            cout << "No words found, check your input\n";
            break;
        }

        vector<pair<string, double> > wordEntropies;

        cout << "Current state:" << endl;
        cout << "Nonolist: ";
        for(char c : nonolist) cout << c << " ";
        cout << endl;
        cout << "Yesyeslist: ";
        for(char c : yesyeslist) cout << c << " ";
        cout << endl;
        cout << "Correct: ";
        for(char c : correct) cout << c << " ";
        cout << endl;


        for (const auto& word : guessWords) {
            double entropy = calculateEntropy(word, guessWords, totalWords);
            wordEntropies.push_back( {word, entropy});
            cout << "Debug: Word: " << word << ", Entropy: " << entropy << endl;
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
}
