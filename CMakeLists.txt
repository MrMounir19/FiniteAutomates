cmake_minimum_required(VERSION 3.10.2)
project(FINITEAUTOMATES)

set(CMAKE_CXX_STANDARD 14)

add_executable(indiv
        main.cpp
        DFA/DFA.h DFA/DFA.cpp
        json-develop/single_include/nlohmann/json.hpp
        NFA/NFA.cpp NFA/NFA.h
        eNFA/eNFA.cpp eNFA/eNFA.h
        TFA/TFA.cpp TFA/TFA.h
        RE/RE.cpp RE/RE.h)