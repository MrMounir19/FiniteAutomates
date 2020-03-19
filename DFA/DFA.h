#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>
#include <map>
#include "../json-develop/single_include/nlohmann/json.hpp"

using namespace std;

#ifndef NFA_H
#define NFA_H


class DFA {
private:
    vector<string> states;
    vector<char> alphabet;
    string startstate;
    string currentstate;
    vector<string> finalstates;
    map<string, map<char, string>> transition;
public:
    DFA(vector<string> States, vector<char> Alphabet, map<string, map<char,string>> Transition, string Startstate, vector<string> Finalstates);

    DFA(string input);

    bool checkInput(string& input);

    bool checkFinal(string final);

    bool execute(string input);

    void createDot();

    void createJson();

};


#endif //NFA_H
