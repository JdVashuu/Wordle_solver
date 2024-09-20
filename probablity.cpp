#include <algorithm>
#include <cstddef>
#include<iostream>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;
using PatternTable = unordered_map<string, unordered_map<string, string> >;


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

int main(){
    vector<std::string> wordList = {"apple", "grape", "spoon", "horse"};
    PatternTable pattern_table = precomputePattern(wordList);
    double entropy_grape = calculateEntropy(wordList[1], wordList, pattern_table);
    cout << "Entropy for 'grape': " << entropy_grape << endl;

   return 0;

}
