//
// Created by Awerkamp on 1/22/21.
//

#include "expr.h"
#include "catch.hpp"
#include<string>


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

Add::Add(Expr *lhs, Expr *rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}

bool Add::equals(Expr *e) {
    Add *t = dynamic_cast<Add*>(e);
    if (t == nullptr) {
        return false;
    } else {
        return (this->lhs == t->lhs && this->rhs == t->rhs);
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
        return (this->lhs == t->lhs && this->rhs == t->rhs);
    }
}

int num1 = 4;
int num2 = 5;
int num5 = 10;
int num6 = 11;

TEST_CASE("equals") {
    int a[] = { 1, 2, 3, 4};
//    Expr* expr = new Expr();
//    Expr* lhs = new Expr(3);
//    Add* add = new Add(new Expr*(), new Expr*());

    Expr* num1 = new Num(1);
    Expr* num2 = new Num(2);
    Expr* num3 = new Num(3);
    Expr* num4 = new Num(4);
    Expr* num5 = new Num(5);
    Expr* num6 = new Num(6);
    Expr* var1 = new Var("var1");
    Expr* var2 = new Var("var2");


    CHECK((new Add(num1, num2))->equals(new Add(num1, num2)) == true);
    CHECK((new Add(num1, num5))->equals(new Add(num3, num3)) == false);
    CHECK((num1)->equals(num1) == true);
    CHECK((num1)->equals(num2) == false);
    CHECK((var1)->equals(var1) == true);
    CHECK((var1)->equals(var2) == false);
    CHECK((new Mult(num1, num2))->equals(new Mult(num1, num2)) == true);
    CHECK((new Mult(num1, num2))->equals(new Mult(num1, num3)) == false);

}

