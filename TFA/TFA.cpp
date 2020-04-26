#include "TFA.h"

int findIndex(vector<string> states, string state) {
    for (int i=0; i < states.size(); i++) {
        if (states[i] == state) {
            return i;
        }
    }
    return -1;
}

set<set<string>> fixMixedStates(set<set<string>>& mixedStates) {
    bool check = false;
    while (!check) {
        check = true;
        set<set<string>> states;
        for (auto x:mixedStates) {
            for (auto y:mixedStates) {
                if (x != y) {
                    set<string> z = x;
                    z.insert(y.begin(), y.end());
                    if (z.size() != x.size() + y.size()) {
                        states.insert(z);
                    } else {
                        states.insert(x);
                    }
                }
            }
        }
        if (states.empty()) {
            return mixedStates;
        }
        if (states != mixedStates) {
            check = false;
            mixedStates = states;
        }
    }

    return mixedStates;
}

bool checkIfIn(string state, set<set<string>> mixedStates) {
    for (auto x:mixedStates) {
        for (auto y:x) {
            if (state == y) {
                return true;
            }

        }
    }
    return false;
}

string findState(string state, set<set<string>> mixedStates) {
    for (auto mixedState:mixedStates) {
        for (auto sta:mixedState) {
            if (sta == state) {
                return setToString(mixedState);
            }
        }
    }
    return state;
}

DFA minimalizedDFA(DFA dfa, string filename) {
    //----------------------------------------------------------------------------------------
    map<string, map<string, bool>> tabel;
    sort(dfa.states.begin(), dfa.states.end());
    for (int i=1; i < dfa.states.size(); i++) {
        map<string, bool> emptymap;
        tabel.insert(make_pair(dfa.states[i], emptymap));
        for (int x = i-1; x >= 0; x--) {
            tabel.at(dfa.states[i]).insert(make_pair(dfa.states[x], false));
        }
    }
    //----------------------------------------------------------------------------------------
    // X IF FINAL AND NOT FINAL
    for (auto &vertical:tabel) {
        for (auto &horizontal:vertical.second) {
            if (dfa.checkFinal(vertical.first) xor dfa.checkFinal(horizontal.first)) {
                horizontal.second = true;
            }
        }
    }
    //----------------------------------------------------------------------------------------
    // "RECURSIE"
    bool check = false;

    while (!check) {
        check = true;
        for (auto &vertical:tabel) {
            for (auto &horizontal:vertical.second) {
                if (!horizontal.second) {
                    for (auto symbol:dfa.alphabet) {
                        string transition1 = dfa.transition.at(vertical.first).at(symbol);
                        string transition2 = dfa.transition.at(horizontal.first).at(symbol);
                        if (findIndex(dfa.states, transition1) > findIndex(dfa.states, transition2)) {
                            if (tabel.at(transition1).at(transition2)) {
                                check = false;
                                horizontal.second = true;
                            }
                        }
                        else if (findIndex(dfa.states, transition2) > findIndex(dfa.states, transition1)) {
                            if (tabel.at(transition2).at(transition1)) {
                                check = false;
                                horizontal.second = true;
                            }
                        }
                    }
                }
            }
        }
    }

    //----------------------------------------------------------------------------------------
    // CREATION HTML
    vector<string> lastrow = dfa.states;
    lastrow.pop_back();
    createHTML(tabel, lastrow, false, false, filename);

    //----------------------------------------------------------------------------------------
    // FIX MIXED STATES
    set<set<string>> mixedStates;
    for (auto &vertical:tabel) {
        for (auto &horizontal:vertical.second) {
            if (!horizontal.second) {
                mixedStates.insert({vertical.first, horizontal.first});
            }
        }
    }
    mixedStates = fixMixedStates(mixedStates);
    //----------------------------------------------------------------------------------------
    // CREATION NEW DFA
    vector<string> states;
    map<string, map<char, string>> transition;
    vector<string> finalstates;
    string startstate;
    bool start = false;

    // NORMAL STATES
    for (auto state:dfa.states) {
        // IF STATE ZIT NIET IN EEN MIXEDSTATE
        if (!checkIfIn(state, mixedStates)) {
            states.push_back(state);
            transition.insert(make_pair(state, dfa.transition.at(state)));
            // NEW TRANSITION FOR NORMAL STATE
            for (auto symbol:dfa.alphabet) {
                transition.at(state).at(symbol) = findState(transition.at(state).at(symbol), mixedStates);
            }
            // FIND IF STATE = STARTSTATE
            if (!start) {
                if (state == dfa.startstate) {
                    startstate = state;
                    start = true;
                }
            }
            // FIND IF STATE = FINALSTATE
            if (dfa.checkFinal(state)) {
                finalstates.push_back(state);
            }
        }
    }

    // MIXED STATES
    for (auto state:mixedStates) {
        states.push_back(setToString(state));
        // FIND IF FINALSTATE = MIXEDSTATE
        for (auto sta:state) {
            if (dfa.checkFinal(sta)) {
                finalstates.push_back(setToString(state));
                break;
            }
        }
        // FIND IF STARTSTATE = MIXEDSTATE
        if (!start) {
            for (auto sta:state) {
                if (dfa.startstate == sta) {
                    startstate = setToString(state);
                    start = true;
                    break;
                }
            }
        }
        // TRANSITIONS FOR MIXEDSTATES
        map<char, string> emptymap;
        transition.insert(make_pair(setToString(state), emptymap));
        for (auto symbol:dfa.alphabet) {
            set<string> trans;
            transition.at(setToString(state)).insert(make_pair(symbol, findState(dfa.transition.at(*state.begin()).at(symbol), mixedStates)));
        }
    }
    //----------------------------------------------------------------------------------------
    return DFA(states, dfa.alphabet, transition, startstate, finalstates);
}

bool equalDFAs(DFA dfa1, DFA dfa2, string filename) {
    //----------------------------------------------------------------------------------------
    map<string, map<string, bool>> tabel;
    DFA dfa = dfa1;
    dfa.states.insert(dfa.states.end(),dfa2.states.begin(), dfa2.states.end());
    dfa.transition.insert(dfa2.transition.begin(), dfa2.transition.end());
    dfa.finalstates.insert(dfa.finalstates.end(), dfa2.finalstates.begin(), dfa2.finalstates.end());
    sort(dfa.states.begin(), dfa.states.end());
    //----------------------------------------------------------------------------------------
    // CREATE TABLE
    for (int i=1; i < dfa.states.size(); i++) {
        map<string, bool> emptymap;
        tabel.insert(make_pair(dfa.states[i], emptymap));
        for (int x = 0; x < i; x++) {
            tabel.at(dfa.states[i]).insert(make_pair(dfa.states[x], false));
        }
    }
    //----------------------------------------------------------------------------------------
    // X IF FINAL AND NOT FINAL
    for (auto &vertical:tabel) {
        for (auto &horizontal:vertical.second) {
            if (dfa.checkFinal(vertical.first) xor dfa.checkFinal(horizontal.first)) {
                horizontal.second = true;
            }
        }
    }

    //----------------------------------------------------------------------------------------
    // "RECURSIE"

    bool check = false;
    while (!check) {
        check = true;
        for (auto &vertical:tabel) {
            for (auto &horizontal:vertical.second) {
                if (!horizontal.second) {
                    for (auto symbol:dfa.alphabet) {
                        string transition1 = dfa.transition.at(vertical.first).at(symbol);
                        string transition2 = dfa.transition.at(horizontal.first).at(symbol);
                        if (findIndex(dfa.states, transition1) > findIndex(dfa.states, transition2)) {
                            if (tabel.at(transition1).at(transition2)) {
                                check = false;
                                horizontal.second = true;
                            }
                        }
                        else if (findIndex(dfa.states, transition2) > findIndex(dfa.states, transition1)) {
                            if (tabel.at(transition2).at(transition1)) {
                                check = false;
                                horizontal.second = true;
                            }
                        }
                    }
                }
            }
        }
    }

    //----------------------------------------------------------------------------------------
    vector<string> lastrow = dfa.states;
    lastrow.pop_back();
    bool equivalent = true;
    if (tabel.at(dfa2.startstate).at(dfa1.startstate)) {
        equivalent = false;
    }

    createHTML(tabel, lastrow, true, equivalent, filename);
    //----------------------------------------------------------------------------------------
    return equivalent;
}

void createHTML(map<string, map<string, bool>>& tabel, vector<string> lastrow, bool equi, bool equivalent, string filename) {
    ofstream out(filename);

    out << "<html>\n"
           "<head>\n"
           "\t<style>\n"
           "table, th, td {\n"
           "  border: 1px solid black;\n"
           "  font-family:\"Courier New\", Courier, monospace; \n"
           "  font-size:150%\n"
           "}\n"
           "table {\n"
           "  border-collapse: collapse;\n"
           "}\n"
           "td,th {\n"
           "  height: 50px;\n"
           "  width: 50px;\n"
           "  text-align: center;\n"
           "}\n"
           "tr:nth-child(even) {background-color: #f2f2f2;}\n"
           "th {\n"
           "  background-color: #4CAF50;\n"
           "  color: white;\n"
           "}\n"
           "</style>\n"
           "</head>\n"
           "<body>\n";
    if (equi) {
        if (equivalent) {
            out << "\tEQUIVALENT = TRUE\n";
        }
        else {
            out << "\tEQUIVALENT = FALSE\n";
        }
    }


    out << "<table>\n";
    for (auto& rij:tabel) {
        out << "\t<tr>\n";
        out << "\t\t<th>" << rij.first << "</th>\n";
        for (auto element:rij.second) {
            if (element.second) {
                out << "\t\t<td>X</td>\n";
            }
            else {
                out << "\t\t<td></td>\n";
            }
        }
        out << "\t</tr>\n";
    }
    out << "\t<tr>\n";
    out << "\t\t<th></th>\n";
    for (auto x:lastrow) {
        out << "\t\t<th>" << x <<"</th>\n";
    }
    out << "\t</tr>\n";
    out << "</body>\n"
           "</html>";

    out.close();
}