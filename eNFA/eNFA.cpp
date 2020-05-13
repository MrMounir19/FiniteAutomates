#include "eNFA.h"


eNFA::eNFA(string input) {
    std::ifstream i(input);
    nlohmann::json j = nlohmann::json::parse(i);
    //read alphabet
    for (auto symbol:j["alphabet"]) {
        string Input = symbol;
        alphabet.push_back(Input[0]);
    }
    //read epsilon
    string eps = j["eps"];
    epsilon = eps[0];
    alphabet.push_back(epsilon);

    //lege map
    map<char, set<string>> empty_map;
    set<string> leeg;
    for (auto symbol:alphabet) {
        empty_map.insert(make_pair(symbol, leeg));
    }

    //read states
    for (auto state:j["states"]) {
        states.push_back(state["name"]);
        if (state["starting"]) {startstate = state["name"];}
        if (state["accepting"]) {finalstates.push_back(state["name"]);}
        transition.insert(make_pair(state["name"], empty_map));
    }
    //read transitions
    for (auto trans:j["transitions"]) {
        string Input = trans["input"];
        string To = trans["to"];
        transition.at(trans["from"]).at(Input[0]).insert(To);
    }
    currentstates.insert(startstate);
}

bool eNFA::checkInput(string& input) {
    for (auto x:input) {
        if (x == epsilon) {
            goto out;
        }
        for (auto y:alphabet) {
            if (x==y) {goto out;}
        }
        cout << "Not valid input" << endl;
        return false;
        out: continue;
    }
    return true;
}

bool eNFA::checkFinal(string final) {
    for (auto x:finalstates) {
        if (final==x) {return true;}
    }
    return false;
}

bool eNFA::execute(string input) {
    if (checkInput(input)) {
        currentstates = ECLOSE(currentstates);
        for (auto x:input) {
            set<string> newCurrentStates;
            for (auto state:currentstates) {
                set<string> transitions = transition.at(state).at(x);
                newCurrentStates.insert(transitions.begin(), transitions.end());
            }
            currentstates = ECLOSE(newCurrentStates);
        }
        for (auto state:currentstates) {
            if (checkFinal(state)) {
                currentstates = {startstate};
                cout << "Input aanvaard" << endl;
                return true;
            }
        }
        cout << "Input niet aanvaard" << endl;
        currentstates={startstate};
    }
    return false;
}

set<string> eNFA::ECLOSE(set<string>& nstates) {
    set<string> eclose = nstates;
    while (true) {
        int size = eclose.size();
        for (auto state:eclose) {
            eclose.insert(transition.at(state).at(epsilon).begin(), transition.at(state).at(epsilon).end());
        }
        if (size == eclose.size()) {
            break;
        }
    }
    return eclose;
}

void eNFA::createDot() {
    ofstream DotFile("eNFA.dot");
    DotFile << "Digraph {\nrankdir=LR\ninv[style=invisible]\ninv -> "<< startstate << " [label=start]" << endl;
    DotFile << "node [shape = circle]\n";
    for (auto state:states) {
        for (auto input:alphabet) {
            if (!transition.at(state).at(input).empty()) {
                for (auto trans:transition.at(state).at(input)) {
                    DotFile << state << " -> " << trans << " [label=" << input << "]\n";
                }
            }
        }
        if (!transition.at(state).at(epsilon).empty()) {
            for (auto trans:transition.at(state).at(epsilon)) {
                DotFile << state << " -> " << trans << " [label=" << epsilon << "]\n";
            }
        }
    }

    for (auto state:finalstates) {
        DotFile << state << " [shape = doublecircle]" << endl;
    }
    DotFile << "}";
    DotFile.close();
    //system("dot -Tpng DFA.dot -o DFA.png");

}

DFA eNFA::toDFA() {
    //States
    set<set<string>> setDFAstates = {ECLOSE(currentstates)};
    vector<set<string>> vectorDFAstates = {ECLOSE(currentstates)};
    int i=0;
    while (i < setDFAstates.size()) {
        if (*(vectorDFAstates[i].begin()) != "DEAD") {
            for (auto symbol:alphabet) {
                int size = setDFAstates.size();
                set<string> newState;
                for (auto state:vectorDFAstates[i]) {
                    set<string> trans = ECLOSE(transition.at(state).at(symbol));
                    newState.insert(trans.begin(), trans.end());
                }
                if (newState.empty()) {
                    newState = {"DEAD"};
                }
                setDFAstates.insert(newState);
                if (setDFAstates.size() > size) {
                    vectorDFAstates.push_back(newState);
                }
            }
        }
        i++;
    }

    map<string, map<char, string>> DFAtransition;
    map<char,string> empty;
    //Transitions
    for (auto state:setDFAstates) {
        DFAtransition.insert(make_pair(setToString(state), empty));
        for (char symbol:alphabet) {
            set<string> newState;
            for (auto st:state) {
                if (st == "DEAD") {
                    newState = {"DEAD"};
                } else {
                    set<string> trans = ECLOSE(transition.at(st).at(symbol));
                    newState.insert(trans.begin(), trans.end());
                }
            }
            if (newState.empty()) { newState = {"DEAD"}; }
            DFAtransition.at(setToString(state)).insert(make_pair(symbol, setToString(newState)));
        }
    }
    //finalStates;
    vector<string> finalStates;
    vector<string> DFAstates;
    for (auto state:setDFAstates) {
        DFAstates.push_back(setToString(state));
        for (auto st:state) {
            if (checkFinal(st)) {
                finalStates.push_back(setToString(state));
                goto end;
            }
        }
        end:;
    }
    set<string> startstates = ECLOSE(currentstates);



    //Creation DFA
    DFA D = DFA(DFAstates, alphabet, DFAtransition, setToString(startstates), finalStates);
    return D;
}

eNFA::eNFA(vector<string> States, vector<char> Alphabet, string Startstate, vector<string> Finalstates, map<string, map<char, set<string>>> Transition, char Epsilon) {
       states = States;
       alphabet = Alphabet;
       startstate = Startstate;
       finalstates = Finalstates;
       transition = Transition;
       epsilon = Epsilon;
       currentstates.insert(startstate);
}
