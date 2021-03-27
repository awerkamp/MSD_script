//
// Created by Awerkamp on 2/24/21.
//

#include "val.h"
#include "expr.h"

Expr* NumVal::to_expr() {

    return new NumExpr(this->val);
}

Val *NumVal::add_to(Val* other_val) {
    auto* other_num = dynamic_cast<NumVal*>(other_val);
    if (other_num == NULL) throw std::runtime_error("add of non-number");
    return new NumVal(val + other_num->val); // todo: not sure if this is right https://www.youtube.com/watch?v=Go5LJu-X_F0&list=PLbdXd8eufjyUWQw3Mqb3SNQEkdjd_S_rB&index=6
}

Val *NumVal::mult_by(Val* other_val) {
    auto* other_num = dynamic_cast<NumVal*>(other_val);
    if (other_num == NULL) throw std::runtime_error("multiply of non-number");
    return new NumVal(val * other_num->val);
}

bool NumVal::equals(Val *e) {
    auto *t = dynamic_cast<NumVal*>(e);
    if (t == nullptr) {
        return false;
    } else {
        return (this->val == t->val);
    }
}

Val* NumVal::call(Val* actual_arg) {
    throw std::runtime_error("Cannot call a number");
}



Expr* BoolVal::to_expr() {

    return new BoolExpr(this->val);
}

Val *BoolVal::add_to(Val* other_val) {
    throw runtime_error("Cannot add to a boolean value");
}

Val *BoolVal::mult_by(Val *other_val) {
    throw runtime_error("Cannot multiply by a boolean value");
}

bool BoolVal::equals(Val *e) {
    auto * t = dynamic_cast<BoolVal*>(e);
    if (t == nullptr) {
        return false;
    } else {
        return this->val == t->val;
    }
}

Val* BoolVal::call(Val *actual_arg) {
    throw std::runtime_error("Cannot call a bool");
}

FunVal::FunVal(std::string formal_arg, Expr * body) {
    this->formal_arg = formal_arg;
    this->body = body;
}

Val *FunVal::call(Val *actual_arg) {
    return this->body->subst(formal_arg, actual_arg->to_expr())->interp();
}

bool FunVal::equals(Val* other) {
    FunVal *c = dynamic_cast<FunVal*>(other);
    if (c == nullptr) {
        return false;
    } else {
        return this->val == c->val;
    }
}

Expr* FunVal::to_expr() {
    FunExpr* funExpr = new FunExpr(this->formal_arg, this->body);

    return funExpr;
}

Val* FunVal::add_to(Val *other) {
    throw std::runtime_error("unable to add two functions");
}

Val* FunVal::mult_by(Val *other) {
    throw std::runtime_error("unable to multiply two functions");
}