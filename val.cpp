//
// Created by Awerkamp on 2/24/21.
//

#include "val.h"
#include "expr.h"
#include "env.h"
#include <string>
#include "step.hpp"

//PTR(Expr) NumVal::to_expr() {
//
//    return NEW(NumExpr)(this->val);
//}

PTR(Val)  NumVal::add_to(PTR(Val) other_val) {
    PTR(NumVal)other_num = CAST(NumVal)(other_val);
    if (other_num == NULL) throw std::runtime_error("add of non-number");
    unsigned long long n = (unsigned long long) val + (unsigned long long) other_num->val;
    if (n > 2147483647) { // largest int without overflow
        throw runtime_error("Too large of numbers being added together");
    }
    return NEW(NumVal)(val + other_num->val); // todo: not sure if this is right https://www.youtube.com/watch?v=Go5LJu-X_F0&list=PLbdXd8eufjyUWQw3Mqb3SNQEkdjd_S_rB&index=6
}

PTR(Val)  NumVal::mult_by(PTR(Val) other_val) {
    PTR(NumVal) other_num = CAST(NumVal)(other_val);
    if (other_num == NULL) throw std::runtime_error("multiply of non-number");
    unsigned long long n = (unsigned long long) val * (unsigned long long) other_num->val;
    if (n > 2147483647) { // largest int without overflow
        throw runtime_error("Too large of numbers being multiplied together");
    }
    return NEW (NumVal)(val * other_num->val);
}

bool NumVal::equals(PTR(Val)  e) {
    PTR(NumVal) t = CAST(NumVal)(e);
    if (t == nullptr) {
        return false;
    } else {
        return (this->val == t->val);
    }
}

PTR(Val) NumVal::call(PTR(Val) actual_arg) {
    throw std::runtime_error("Cannot call a number");
}

std::string NumVal::to_string() {

    std::string str = std::to_string(val);

    return str;
}

bool NumVal::is_true() {
    throw std::runtime_error("Not a bool");
}

void NumVal::call_step(PTR(Val) actual_arg_val, PTR(Cont) rest) {
    throw std::runtime_error("Can't call number");
}



//
//PTR(Expr) BoolVal::to_expr() {
//
//    return NEW (BoolExpr)(this->val);
//}

PTR(Val)  BoolVal::add_to(PTR(Val) other_val) {
    throw runtime_error("Cannot add to a boolean value");
}

PTR(Val)  BoolVal::mult_by(PTR(Val)  other_val) {
    throw runtime_error("Cannot multiply by a boolean value");
}

bool BoolVal::equals(PTR(Val)  e) {
    PTR(BoolVal) t = CAST(BoolVal)(e);
    if (t == nullptr) {
        return false;
    } else {
        return this->val == t->val;
    }
}

PTR(Val) BoolVal::call(PTR(Val)  actual_arg) {
    throw std::runtime_error("Cannot call bool");
}

std::string BoolVal::to_string() {
    if (val) {
        return "_true";
    } else {
        return "_false";
    }
}

bool BoolVal::is_true() {
    return val;
}

void BoolVal::call_step(PTR(Val) actual_arg, PTR(Cont) rest) {
    throw std::runtime_error("Can't call bool");
}




FunVal::FunVal(std::string formal_arg, PTR(Expr) body, PTR(Env) env) {
    this->formal_arg = formal_arg;
    this->body = body;
    this->env = env;
}

PTR(Val) FunVal::call(PTR(Val) actual_arg) {
//    return this->body->subst(formal_arg, actual_arg->to_expr())->interp();
    // return body->interp(NEW(ExtendedEnv)(formal_arg, actual_arg, env));
    return body->interp(NEW(ExtendedEnv) (formal_arg, actual_arg, env));
}

bool FunVal::equals(PTR(Val) other) {
    PTR(FunVal) c = CAST(FunVal)(other);
    if (c == nullptr) {
        return false;
    } else {
        return this->val == c->val;
    }
}

//PTR(Expr) FunVal::to_expr() {
//    PTR(FunExpr)funExpr = NEW(FunExpr)(this->formal_arg, this->body);
//
//    return funExpr;
//}

PTR(Val) FunVal::add_to(PTR(Val) other) {
    throw std::runtime_error("unable to add two functions");
}

PTR(Val) FunVal::mult_by(PTR(Val) other) {
    throw std::runtime_error("unable to multiply two functions");
}

std::string FunVal::to_string() {
    std::string formal_arg;
    PTR(Expr) body;
    PTR(Env) env;

    return "[function]";
}

bool FunVal::is_true() {
    throw std::runtime_error("Not a value");
}

void FunVal::call_step(PTR(Val) actual_arg_val, PTR(Cont) rest) {
    Step::mode = Step::interp_mode;
    Step::expr = body;
    Step::env = NEW(ExtendedEnv)(formal_arg, actual_arg_val, env);
    Step::cont = rest;
}