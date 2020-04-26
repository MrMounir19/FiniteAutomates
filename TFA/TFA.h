#include <vector>
#include <map>
#include <set>
#include <string>

#include "../NFA/NFA.h"

#ifndef FINITEAUTOMATES_TFA_H
#define FINITEAUTOMATES_TFA_H

//---------------------------------------------------------------
// TFA FUNCTIONS

DFA minimalizedDFA(DFA dfa, string filename);

bool equalDFAs(DFA dfa1, DFA dfa2, string filename);



//---------------------------------------------------------------

int findIndex(vector<string> states, string state);

set<set<string>> fixMixedStates(set<set<string>>& mixedStates);

string findState(string state, set<set<string>> mixedStates);

bool checkIfIn(string state, set<set<string>> mixedStates);

void createHTML(map<string, map<string, bool>>& tabel, vector<string> lastrow, bool equi, bool equivalent, string filename);

//---------------------------------------------------------------


#endif //FINITEAUTOMATES_TFA_H
