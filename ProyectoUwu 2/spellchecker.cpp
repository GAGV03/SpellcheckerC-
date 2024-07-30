/*----------------------------------------------------------
 * Project: Spell Checker
 *
 * Date: 30-Nov-2022
 * Authors:
 *           A01747433 Carlos Alberto Sánchez Calderón
 *           A01747869 Gustavo Alejandro Gutierrez Valdes
 *----------------------------------------------------------*/


#include <fstream>
#include <regex>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <cstring>
#include <cctype>
#include <string>
#include <algorithm>
#include <ctime>
#include <sstream>
#include <chrono>
using namespace std;




unordered_map <char,int> soundex={
    {'b',1}, {'f',1},{'p',1},{'v',1},
    {'c',2}, {'g',2},{'j',2},{'k',2},{'q',2},{'s',2},{'x',2},{'z',2},
    {'d',3},{'t',3},
    {'l',4},
    {'m',5},{'n',5},
    {'r',6}
    
};
    
    
    unordered_set <char> dropped_letters{'a','e','i','o','u','h','w','y'};
    


struct word {
    std::string text;
    int line;
    int column;
    string soundexCode;
};

bool read_words(
        const std::string input_file_name,
        std::vector<word>& words)
{
    std::ifstream input_file(input_file_name);
    if (input_file.fail()) {
        return false;
    }
    
    std::regex reg_exp("[a-zA-Z]+");
    std::smatch match;
    std::string text;
    int line = 0;
    int column = 0;
    while (std::getline(input_file, text)) {
        ++line;
        column = 1;
        while (std::regex_search(text, match, reg_exp)) {
            column += match.position();
            words.push_back({match.str(), line, column});
            column += match.length();
            text = match.suffix().str();
        }
    }
    return true;
}


    
string Soundex(string word){
    string resultado="";
    resultado+=word[0];
    for(int i=1;i<word.length();++i){
        if(dropped_letters.count(word.at(i))==0&&resultado.length()<7){
            resultado+=to_string(soundex[word.at(i)]);
        }
    }
    
    int resta=7-resultado.length();
    for(int i=0;i<resta;++i){
        resultado+="0";
    }
    
    return resultado;
    
}



string toLowerCase(string palabra){
    for(int i = 0; i < palabra.length(); ++i){
        palabra[i] = tolower(palabra[i]);
    }
    return palabra;
}

int main(int argc, char* argv[]){
    
    unsigned t0,t1;
    t0 = clock();
    
    
    string wordname = "words.txt";
    vector<word> wordslist; 
    read_words(wordname,wordslist);
    string storyname = argv[1];
    vector<word> storywords;
    read_words(storyname,storywords);
    
    ifstream file(storyname);
    
    //unordered_map <Soundex,<vector de strings con el mismo soundex>>
    unordered_map <string, vector <string>> suggs;
    unordered_set<string> dic_set;
    
    for(word w: wordslist){
        string sound = Soundex(w.text);
        dic_set.insert(w.text);
        if(suggs.count(sound)==0){
            suggs[sound] = vector<string>{};
        }
        
        suggs[sound].push_back(w.text);
    }
    
    unordered_set<string> setListWrong;
    vector<word> wordWrong;
    
    for(word w:storywords){
        string littleW = toLowerCase(w.text);
        if(dic_set.count(littleW)==0){
            if(setListWrong.count(littleW)==0){
                setListWrong.insert(littleW);
                wordWrong.push_back(w);
            }
        }
    }
   
   
    for(word w: wordWrong){
        cout<<"Unrecognized word: "<<'"'<<w.text<<'"';
        cout<<". First found at line "<<w.line<<", column "<<w.column<<"."<<endl;
        
        vector<string> soundexSimilar = suggs[Soundex(toLowerCase(w.text))];
        int countador=0;
        if(soundexSimilar.size()>0)
            cout<<"Suggestions: ";
        for(string s: soundexSimilar){
            cout<<s;
            
            if(countador<soundexSimilar.size()-1){
                cout<<", ";
            }
            countador+=1;
        }
        if(countador==0){
            cout<<"No suggestions.";
        }
        cout<<endl<<endl;
        
    }
    
    t1 = clock();
    double time = (double(t1-t0)/CLOCKS_PER_SEC);
    cout << "Execution time: " << time << " secs" << "\n";
return 0;
}