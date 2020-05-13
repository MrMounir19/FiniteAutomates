#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>
#include <map>
#include <set>
#include "../json-develop/single_include/nlohmann/json.hpp"
#include "../DFA/DFA.h"
#include "../NFA/NFA.h"

#ifndef ENFA_H
#define ENFA_H


class eNFA {
private:
    vector<string> states;
    vector<char> alphabet;
    string startstate;
    char epsilon;
    set<string> currentstates;
    vector<string> finalstates;
    map<string, map<char, set<string>>> transition;
public:

    eNFA(string input);

    bool checkInput(string& input);

    bool checkFinal(string final);

    bool execute(string input);

    void createDot();

    DFA toDFA();

    set<string> ECLOSE(set<string>& nstates);

    eNFA(vector<string> States, vector<char> Alphabet, string Startstate,
         vector<string> Finalstates, map<string, map<char, set<string>>> Transition, char Epsilon);
};


#endif //ENFA_H
