//
// Created by Awerkamp on 3/30/21.
//

#include "env.h"

PTR(Env) Env::empty = NEW(EmptyEnv)();

//EmptyEnv::EmptyEnv() {
//}

ExtendedEnv::ExtendedEnv(std::string name, PTR(Val) val, PTR(Env) rest) {
    this->name = name;
    this->val = val;
    this->rest = rest;
}

EmptyEnv::EmptyEnv() {

}
