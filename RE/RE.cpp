#include "RE.h"

struct Regex {
    int firststate;
    int laststate;
};







eNFA createRE(string input, char epsilon) {
    vector<string> states = {"s0"};
    vector<char> alphabet;
    string startstate = "s0";
    vector<string> finalstates;
    map<string, map<char, set<string>>> transition;
    int count = 1;
    //Create alphabet

    for (char symbol = 32; symbol < 127; ++symbol) {
        if (symbol != '(' and symbol != ')' and symbol != '+' and symbol != '*') {
            alphabet.push_back(symbol);
        }
    }

    //Create transitions and states
    transition.insert(make_pair("s0", createEmptyMap(alphabet)));
    transition.insert(make_pair("final", createEmptyMap(alphabet)));
    vector<string> allUnions = splitUnion(input);
    vector<Regex> allRegex;
    for (auto x:allUnions) {
        vector<pair<string, bool>> allSubRe = splitConcatenation(x);
        states.push_back(createState(count, false));
        transition.insert(make_pair(createState(count), createEmptyMap(alphabet)));
        bool begin = true;
        Regex newRegex = createSubRE(allSubRe[0].first, transition, count, epsilon, alphabet, states, allSubRe[0].second);
        for (auto x:allSubRe) {
            if (!begin) {
                Regex temp2 = createSubRE(x.first, transition, count, epsilon, alphabet, states, x.second);
                transition.at(createState(newRegex.laststate, false)).at(epsilon).insert(createState(temp2.firststate, false));
                newRegex.laststate = temp2.laststate;
            }
            begin = false;
        }
        allRegex.push_back(newRegex);
    }
    for (auto x:allRegex) {
        transition.at("s0").at(epsilon).insert(createState(x.firststate, false));
        transition.at(createState(x.laststate, false)).at(epsilon).insert("final");
    }
    finalstates.push_back("final");
    states.push_back("final");

    // REMOVE EPSILON FROM ALPHABET
    vector<char> newAlphabet;
    for (auto x:alphabet) {
        if (x != epsilon) {
            newAlphabet.push_back(x);
        }
    }

    return eNFA(states, newAlphabet, startstate, finalstates, transition, epsilon);
}

Regex createSubRE(string input, map<string, map<char, set<string>>>& transition, int& count, char epsilon, vector<char>& alphabet, vector<string>& states, bool repeat) {
    if (!containsPlus(input)) {
        Regex newRegex;
        if (input.empty()) {
            newRegex.firststate = count;
            newRegex.laststate = count;
            states.push_back(createState(count, false));
            transition.insert(make_pair(createState(count), createEmptyMap(alphabet)));
            if (repeat) {
                transition.at(createState(newRegex.firststate, false)).at(epsilon).insert(createState(newRegex.laststate, false));
            }
        }
        else {
            newRegex.firststate = count;
            states.push_back(createState(count, false));
            transition.insert(make_pair(createState(count, false), createEmptyMap(alphabet)));
            for (unsigned int i = 0; i < input.size(); i++) {
                transition.at(createState(count)).at(input[i]).insert(createState(count, false));
                states.push_back(createState(count, false));
                transition.insert(make_pair(createState(count, false), createEmptyMap(alphabet)));
            }
            newRegex.laststate = count;
            if (repeat) {
                transition.at(createState(newRegex.firststate, false)).at(epsilon).insert(createState(newRegex.laststate, false));
                transition.at(createState(newRegex.laststate, false)).at(epsilon).insert(createState(newRegex.firststate, false));
            }
            count++;
        }
        return newRegex;
    }
    else {
        vector<string> allUnions = splitUnion(input);
        if (allUnions.size() == 1) {
            vector<pair<string, bool>> allSubRe = splitConcatenation(allUnions[0]);
            bool begin = true;
            Regex newRegex = createSubRE(allSubRe[0].first, transition, count, epsilon, alphabet, states, allSubRe[0].second);
            for (auto x:allSubRe) {
                if (!begin) {
                    Regex temp = createSubRE(x.first, transition, count, epsilon, alphabet, states, x.second);
                    transition.at(createState(newRegex.laststate, false)).at(epsilon).insert(createState(temp.firststate, false));
                    newRegex.laststate = temp.laststate;
                }
                begin = false;
            }
            if (repeat) {
                transition.at(createState(newRegex.firststate, false)).at(epsilon).insert(createState(newRegex.laststate, false));
                if (newRegex.firststate != newRegex.laststate) {
                    transition.at(createState(newRegex.laststate, false)).at(epsilon).insert(createState(newRegex.firststate, false));
                }
            }

            return newRegex;
        }
        else {
            Regex newRegex;
            newRegex.firststate = count;
            states.push_back(createState(count, false));
            transition.insert(make_pair(createState(count), createEmptyMap(alphabet)));
            vector<Regex> allRegex;
            for (auto x:allUnions) {
                allRegex.push_back(createSubRE(x, transition, count, epsilon, alphabet, states, false));
            }
            newRegex.laststate = count;
            states.push_back(createState(count, false));
            transition.insert(make_pair(createState(count), createEmptyMap(alphabet)));
            for (auto regex:allRegex) {
                transition.at(createState(newRegex.firststate, false)).at(epsilon).insert(createState(regex.firststate, false));
                transition.at(createState(regex.laststate, false)).at(epsilon).insert(createState(newRegex.laststate, false));
            }
            if (repeat) {
                transition.at(createState(newRegex.firststate, false)).at(epsilon).insert(createState(newRegex.laststate, false));
                if (newRegex.firststate != newRegex.laststate) {
                    transition.at(createState(newRegex.laststate, false)).at(epsilon).insert(createState(newRegex.firststate, false));
                }
            }
            return newRegex;
        }
    }
}

vector<string> splitUnion(string input) {
    int leftbrackets = 0;
    int rightbrackets = 0;
    vector<string> allRegex;
    string currentRegex;
    for (auto x:input) {
        if (x == '(') {
            leftbrackets++;
        }
        else if (x == ')') {
            rightbrackets++;
        }
        if (x == '+' and leftbrackets == rightbrackets) {
            allRegex.push_back(currentRegex);
            currentRegex = "";
        }
        else {
            currentRegex += x;
        }
    }
    allRegex.push_back(currentRegex);
    return allRegex;
}

vector<pair<string, bool>> splitConcatenation(string input) {
    int leftbrackets = 0;
    int rightbrackets = 0;
    vector<pair<string, bool>> allRegex;
    string currentRegex;
    for (auto x:input) {
        if (x == '(') {
            if (leftbrackets == rightbrackets and !currentRegex.empty()) {
                allRegex.push_back(make_pair(currentRegex, false));
                currentRegex = "";
            }
            leftbrackets++;
        }
        else if (x == ')') {
            rightbrackets++;
            if (leftbrackets == rightbrackets) {
                allRegex.push_back(make_pair(currentRegex, false));
                currentRegex = "";
                leftbrackets = 0;
                rightbrackets = 0;
            }
        }
        else if (x == '*' and currentRegex.empty()) {
            allRegex[allRegex.size()-1].second = true;
            currentRegex = "";
        }
        else if (x == '*' and leftbrackets == 0) {
            if (currentRegex.size() == 1) {
                allRegex.push_back(make_pair(currentRegex, true));
                currentRegex = "";
            }
            else {
                string temp;
                temp.push_back(currentRegex[currentRegex.size()-1]);
                allRegex.push_back(make_pair(temp, true));
                currentRegex.pop_back();
                allRegex.push_back(make_pair(currentRegex, false));
            }
        }
        else {
            currentRegex += x;
        }
    }
    if (!currentRegex.empty()) {
        allRegex.push_back(make_pair(currentRegex, false));
    }

    return allRegex;
}

bool containsPlus(string input) {
    for (auto x:input) {
        if (x == '+') {
            return true;
        }
    }
    return false;
}

string createState(int& count, bool increaseCount) {
    string output = "s" + to_string(count);
    if (increaseCount) {
        count++;
    }
    return output;
}

map<char, set<string>> createEmptyMap(vector<char>& alphabet) {
    map<char, set<string>> empty_map;
    set<string> leeg;
    for (auto symbol:alphabet) {
        empty_map.insert(make_pair(symbol, leeg));
    }
    return empty_map;
}