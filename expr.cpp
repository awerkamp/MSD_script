//
// Created by Awerkamp on 1/22/21.
//

#include "expr.h"

#include<string>
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;

enum printStatus {
    print_group_none,
    print_group_add,
    print_group_add_or_mult
} printStatus;

std::string Expr::to_string(std::ostream &out) {
    this->print(out);
    std::stringstream ss;
    ss << out.rdbuf();
    return ss.str();
}

Num::Num(int val) {
    this->val = val;
};

bool Num::equals(Expr *e) {
    Num *t = dynamic_cast<Num*>(e);
    if (t == nullptr) {
        return false;
    } else {
        return (this->val == t->val);
    }
}

 int Num::interp() {
    return this->val;
}

bool Num::has_variable() {
    return false;
}

Expr* Num::subst(std::string s, Expr *e) {
    return this;
}

void Num::print(std::ostream &out) {
    out << this->val;
}

void Num::pretty_print(std::ostream &out) {
    out << this->val;
}

Add::Add(Expr *lhs, Expr *rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}

bool Add::equals(Expr *e) {
    Add *t = dynamic_cast<Add*>(e);
    if (t == nullptr) {
        return false;
    } else {
        return (this->lhs->equals(t->lhs) && this->rhs->equals(t->rhs));
    }
}

int Add::interp() {
    return this->lhs->interp() + this->rhs->interp();
}

bool Add::has_variable() {
    return (this->lhs->has_variable() || this->rhs->has_variable());
}

Expr* Add::subst(std::string s, Expr *e) {
    return new Add(this->lhs->subst(s,e),this->rhs->subst(s,e));
}

void Add::print(std::ostream &out) {
    out << "(";
    this->lhs->print(out);
    out << "+";
    this->rhs->print(out);
    out << ")";
}

void Add::pretty_print(std::ostream &out) {

    if(printStatus == print_group_add || printStatus == print_group_add_or_mult) {
        out << "(";
        printStatus = print_group_none;
        this->lhs->pretty_print(out);
        out << " + ";
        this->rhs->pretty_print(out);
        out << ")";
    } else {
        printStatus = print_group_none;
        this->lhs->pretty_print(out);
        out << " + ";
        this->rhs->pretty_print(out);
    }
}

Mult::Mult(Expr *lhs, Expr *rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}

bool Mult::equals(Expr *e) {
    Mult *t = dynamic_cast<Mult*>(e);
    if (t == nullptr) {
        return false;
    } else {
        return (this->lhs->equals(t->lhs) && this->rhs->equals(t->rhs));
    }
}

int Mult::interp() {
    return this->lhs->interp() * this->rhs->interp();
}

bool Mult::has_variable() {
    return (this->lhs->has_variable() || this->rhs->has_variable());
}

Expr* Mult::subst(std::string s, Expr *e) {
    return new Mult(this->lhs->subst(s,e), this->rhs->subst(s,e));
}

void Mult::print(std::ostream &out) {
    out << "(";
    this->lhs->print(out);
    out << "*";
    this->rhs->print(out);
    out << ")";
}

void Mult::pretty_print(std::ostream &out){

    if(printStatus == print_group_add_or_mult) {
        out << "(";
        printStatus = print_group_add_or_mult;
        this->lhs->pretty_print(out);
        out << " * ";
        printStatus = print_group_add;
        this->rhs->pretty_print(out);
        out << ")";
    } else {
        printStatus = print_group_add_or_mult;
        this->lhs->pretty_print(out);
        out << " * ";
        printStatus = print_group_add;
        this->rhs->pretty_print(out);
    }
    printStatus = print_group_none;
}

Var::Var(std::string name) {
    this->name = name;
}

bool Var::equals(Expr *e) {
    Var *t = dynamic_cast<Var*>(e);
    if (t == nullptr) {
        return false;
    } else {
        return (this->name == t->name);
    }
}

int Var::interp() {
    throw std::runtime_error("There is no value for this expression");
}

bool Var::has_variable() {
    return true;
}


Expr* Var::subst(std::string s, Expr *e) {
    if (this->name == s) {
        return e;
    } else {
        return this;
    }
}

void Var::print(std::ostream &out) {
    out << this->name;
}

void Var::pretty_print(std::ostream &out){
    out << this->name;
}