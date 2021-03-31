//
// Created by Awerkamp on 3/30/21.
//

#ifndef MSD_SCRIPT_ENV_H
#define MSD_SCRIPT_ENV_H

#include <cstdio>
#include "pointer.h"
//#include "val.cpp"
#include "val.h"
#include <string>
#include <sstream>

class Env {

public:
    virtual PTR(Val) lookup(std::string find_name) = 0;
    static PTR(Env) empty;
};


class EmptyEnv: public Env {
public:
    EmptyEnv();
    PTR(Val) lookup(std::string find_name) {
        throw std::runtime_error("free variable: " + find_name);
    }
};

class ExtendedEnv: public Env {
public:
    std::string name;
    PTR(Val) val;
    PTR(Env) rest;

    ExtendedEnv(std::string name, PTR(Val) val, PTR(Env) rest);

    PTR(Val) lookup(std::string find_name) {
        if (find_name == name) {
            return val;
        } else {
            return rest->lookup(find_name);
        }
    }
};

#endif //MSD_SCRIPT_ENV_H
