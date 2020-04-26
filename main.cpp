#include "DFA/DFA.h"
#include "NFA/NFA.h"
#include "eNFA/eNFA.h"
#include "TFA/TFA.h"


int main(int argc, char** argv) {
    if (string(argv[1]) == "ssc") {
        NFA A(argv[2]);
        DFA B = A.toDFA();
        B.createDot(argv[3]);
    }
    else if (string(argv[1]) == "mssc") {
        eNFA A(argv[2]);
        DFA B = A.toDFA();
        B.createDot(argv[3]);
    }
    else if (string(argv[1]) == "productand") {
        DFA A(argv[2]);
        DFA B(argv[3]);
        DFA C = createProductDFA(A, B, doorsnede);
        C.createDot(argv[4]);
    }
    else if (string(argv[1]) == "productor") {
        DFA A(argv[2]);
        DFA B(argv[3]);
        DFA C = createProductDFA(A, B, unie);
        C.createDot(argv[4]);
    }
    else if (string(argv[1]) == "tfa") {
        DFA A(argv[2]);
        DFA B = minimalizedDFA(A, argv[4]);
        B.createDot(argv[3]);
    }
    else if (string(argv[1]) == "tfaeq") {
        DFA A(argv[2]);
        DFA B(argv[3]);
        equalDFAs(A, B, argv[4]);
    }

    return 0;
}
