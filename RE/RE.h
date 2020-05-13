//
// Created by mouni on 12-May-20.
//

#include "../eNFA/eNFA.h"


#ifndef FINITEAUTOMATES_RE_H
#define FINITEAUTOMATES_RE_H

struct Regex;

eNFA createRE(string input, char epsilon);

Regex createSubRE(string input, map<string, map<char, set<string>>>& transition, int& count, char epsilon, vector<char>& alphabet, vector<string>& states, bool repeat);

vector<string> splitUnion(string input);

vector<pair<string, bool>> splitConcatenation(string input);

bool containsPlus(string input);

string createState(int& count, bool increaseCount = true);

map<char, set<string>> createEmptyMap(vector<char>& alphabet);


//vector<char> defaultAlphabet;


#endif //FINITEAUTOMATES_RE_H
