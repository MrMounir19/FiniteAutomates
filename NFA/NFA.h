#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>
#include <map>
#include <set>
#include "../json-develop/single_include/nlohmann/json.hpp"
#include "../DFA/DFA.h"

using namespace std;

#ifndef DFA_NFA_H
#define DFA_NFA_H


class NFA {
private:
    vector<string> states;
    vector<char> alphabet;
    string startstate;
    set<string> currentstates;
    vector<string> finalstates;
    map<string, map<char, set<string>>> transition;
public:

    NFA(string input);

    bool checkInput(string& input);

    bool checkFinal(string final);

    bool execute(string input);

    void createDot();

    DFA toDFA();

};


#endif //DFA_NFA_H
