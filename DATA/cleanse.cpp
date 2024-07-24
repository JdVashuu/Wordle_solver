#include<iostream>
#include<fstream>
#include<string.h>

using namespace std;

void removeFirst3char(const string& filename){
    ifstream inputFile(filename);
    if(!inputFile.is_open()){
        cerr << "Error opening file: " << filename << endl;
        return;
    }

    string line;
    string modifiedContent;

    while(getline(inputFile, line)){
        if(line.length() >= 3){
            modifiedContent += line.substr(3) + "\n";
        }else{
            modifiedContent += "\n";
        }
    }
    inputFile.close();

    ofstream outputFile(filename);
    if(!outputFile.is_open()){
        cerr << "error opening file for writing" << filename << endl;
        return;
    }

    outputFile << modifiedContent;
    outputFile.close();

    cout << "file editing done completely" << endl;
}

int main(){
    string filename = "Wordle list copy.txt";
    removeFirst3char(filename);
    return 0;
}
