#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>
#include <map>
#include <set>
#include <iomanip>
#include "../json-develop/single_include/nlohmann/json.hpp"


using namespace std;

#ifndef DFA_H
#define DFA_H


class DFA {
public:
    vector<string> states;
    vector<char> alphabet;
    string startstate;
    string currentstate;
    vector<string> finalstates;
    map<string, map<char, string>> transition;

    DFA(vector<string> States, vector<char> Alphabet, map<string, map<char,string>> Transition, string Startstate, vector<string> Finalstates);

    DFA(string input);

    bool checkInput(string& input);

    bool checkFinal(string final);

    bool execute(string input);

    void createDot(string filename);

    void createJson();

};

enum type{unie, doorsnede};
DFA createProductDFA(DFA dfa1, DFA dfa2, type Type);


#endif //DFA_H
