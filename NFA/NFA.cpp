


#include "NFA.h"

string setToString(set<string> states) {
    if (*states.begin() == "DEAD") {
        return "DEAD";
    }
    else {
        string state = "{";
        for (auto st:states) {
            state += st + ",";
        }
        state.pop_back();
        state += "}";
        return state;
    }
}

NFA::NFA(string input) {
    std::ifstream i(input);
    nlohmann::json j = nlohmann::json::parse(i);
    //read alphabet
    for (auto symbol:j["alphabet"]) {
        string Input = symbol;
        alphabet.push_back(Input[0]);
    }
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

bool NFA::checkInput(string& input) {
    for (auto x:input) {
        for (auto y:alphabet) {
            if (x==y) {goto out;}
        }
        cout << "Not valid input" << endl;
        return false;
        out: continue;
    }
    return true;
}

bool NFA::checkFinal(string final) {
    for (auto x:finalstates) {
        if (final==x) {return true;}
    }
    return false;
}

bool NFA::execute(string input) {

    if (checkInput(input)) {
        for (auto x:input) {
            set<string> newCurrentStates;
            for (auto state:currentstates) {
                set<string> transitions = transition.at(state).at(x);
                newCurrentStates.insert(transitions.begin(), transitions.end());
            }
            currentstates = newCurrentStates;
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

void NFA::createDot() {
    ofstream DotFile("NFA.dot");
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
    }
    for (auto state:finalstates) {
        DotFile << state << " [shape = doublecircle]" << endl;
    }
    DotFile << "}";
    DotFile.close();
    //system("dot -Tpng DFA.dot -o DFA.png");

}

DFA NFA::toDFA() {
    //States
    set<set<string>> setDFAstates = {{startstate}};
    vector<set<string>> vectorDFAstates = {{startstate}};
    int i=0;
    while (i < setDFAstates.size()) {
        if (*(vectorDFAstates[i].begin()) != "DEAD") {
            for (auto symbol:alphabet) {
                int size = setDFAstates.size();
                set<string> newState;
                for (auto state:vectorDFAstates[i]) {
                    newState.insert(transition.at(state).at(symbol).begin(), transition.at(state).at(symbol).end());
                }
                if (newState.empty()) {
                    newState = {"DEAD"};
                }
                setDFAstates.insert(newState);

                if (setDFAstates.size()>size) {
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
                }
                else {
                    newState.insert(transition.at(st).at(symbol).begin(), transition.at(st).at(symbol).end());
                }
                if (newState.empty()) {newState={"DEAD"};}
            }
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
    //Creation DFA
    DFA D = DFA(DFAstates, alphabet, DFAtransition, "{"+startstate+"}", finalStates);

    return D;

}




