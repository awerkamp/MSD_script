//
// Created by Awerkamp on 2/24/21.
//

#include "val.h"
#include "expr.h"
#include "env.h"
#include <string>

//PTR(Expr) NumVal::to_expr() {
//
//    return NEW(NumExpr)(this->val);
//}

PTR(Val)  NumVal::add_to(PTR(Val) other_val) {
    PTR(NumVal)other_num = CAST(NumVal)(other_val);
    if (other_num == NULL) throw std::runtime_error("add of non-number");
    return NEW(NumVal)(val + other_num->val); // todo: not sure if this is right https://www.youtube.com/watch?v=Go5LJu-X_F0&list=PLbdXd8eufjyUWQw3Mqb3SNQEkdjd_S_rB&index=6
}

PTR(Val)  NumVal::mult_by(PTR(Val) other_val) {
    PTR(NumVal) other_num = CAST(NumVal)(other_val);
    if (other_num == NULL) throw std::runtime_error("multiply of non-number");
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
    throw std::runtime_error("Cannot call a bool");
}

std::string BoolVal::to_string() {
    if (val) {
        return "_true";
    } else {
        return "_false";
    }
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
}