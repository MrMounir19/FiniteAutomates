
#include <iomanip>
#include "DFA.h"

DFA::DFA(vector<string> States, vector<char> Alphabet, map<string, map<char,string>> Transition, string Startstate, vector<string> Finalstates):
    states(States), alphabet(Alphabet), transition(Transition), startstate{Startstate}, finalstates(Finalstates), currentstate(startstate) {}



bool DFA::checkInput(string& input) {
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

bool DFA::checkFinal(string final) {
    for (auto x:finalstates) {
        if (final==x) {return true;}
    }
    return false;
}

bool DFA::execute(string input) {
    if (checkInput(input)) {
        for (auto x:input) {
            currentstate = transition.at(currentstate).at(x);
        }
        if (checkFinal(currentstate)) {currentstate=startstate; cout << "Input aanvaard" << endl; return true;}
        cout << "Input niet aanvaard" << endl;
        currentstate=startstate;
    }
    return false;
}

void DFA::createDot() {
    ofstream DotFile("DFA.dot");
    DotFile << "Digraph {\nrankdir=LR\ninv[style=invisible]\ninv -> " << '"' << startstate << '"' << " [label=start]" << endl;
    DotFile << "node [shape = circle]\n";
    for (auto state:states) {
        for (auto input:alphabet) {
            DotFile << '"' << state << '"' << " -> " << '"' << transition.at(state).at(input) << '"' << " [label=" << input << "]\n";
        }
    }
    for (auto state:finalstates) {
        DotFile << '"' << state << '"' << " [shape = doublecircle]" << endl;
    }
    DotFile << "}";
    DotFile.close();
    //system("dot -Tpng DFA.dot -o DFA.png");
}

DFA::DFA(string input) {
    std::ifstream i(input);
    nlohmann::json j = nlohmann::json::parse(i);
    //read alphabet
    for (auto symbol:j["alphabet"]) {
        string Input = symbol;
        alphabet.push_back(Input[0]);
    }
    //read states
    for (auto state:j["states"]) {
        states.push_back(state["name"]);
        if (state["starting"]) {startstate = state["name"];}
        if (state["accepting"]) {finalstates.push_back(state["name"]);}
        map<char, string> empty_map;
        transition.insert(make_pair(state["name"], empty_map));
    }
    //read transitions
    for (auto trans:j["transitions"]) {
        string Input = trans["input"];
        pair<char, string> new_transition = make_pair(Input[0], trans["to"]);
        transition.at(trans["from"]).insert(new_transition);
    }
    currentstate=startstate;
}

void DFA::createJson() {
    nlohmann::json j;
    j["type"] = "DFA";
    for (auto symbol:alphabet) {
        j["alphabet"].push_back(string(1,symbol));
    }
    vector<nlohmann::json> stat;
    for (auto state:states) {
        nlohmann::json s;
        s["name"] = state;
        s["starting"] = state == startstate;
        s["accepting"] = checkFinal(state);
        stat.push_back(s);
    }
    j["states"] = stat;

    vector<nlohmann::json> trans;
    for (auto state:states) {
        nlohmann::json t;
        t["from"] = state;
        for (auto symbol:alphabet) {
            t["to"] = transition.at(state).at(symbol);
            t["input"]= string(1, symbol);
            trans.push_back(t);
        }
    }
    j["transitions"] = trans;
    ofstream jsonFile("jsonDFA.json");
    jsonFile << std::setw(4) << j << std::endl;
    jsonFile.close();
}
