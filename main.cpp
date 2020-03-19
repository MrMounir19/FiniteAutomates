#include "DFA/DFA.h"
#include "NFA/NFA.h"


int main() {

    /*
    NFA* Test = new NFA("test2.json");
    Test->createDot();
    DFA Test2 = Test->toDFA();
    Test2.createJson();
    DFA* Test3 = new DFA("jsonDFA.json");
    Test3->createDot();
    */

    NFA N("../test3.json");
    N.createDot();
    DFA D = N.toDFA();
    D.createDot();


    return 0;
}
