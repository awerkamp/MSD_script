///
// Created by Awerkamp on 1/22/21.
//

#ifndef MSD_SCRIPT_EXPR_H
#define MSD_SCRIPT_EXPR_H

#include<string>
#include<iostream>
#include <cstdio>
#include "pointer.h"
#include "env.h"
// Do not #include val.h
using namespace std;

class Val;
class Env;
//class Expr;

CLASS(Expr) {
public:

    virtual bool equals(PTR(Expr) e) = 0;
    virtual PTR(Val) interp(PTR(Env) env) = 0;
    virtual bool has_variable() = 0;
//    virtual PTR(Expr) subst(std::string s, PTR(Expr) e) = 0;
    virtual void print(std::ostream &out) = 0;
    virtual void step_interp() = 0;

    enum printStatus {
        print_group_none,
        print_group_add,
        print_group_add_or_mult,
        print_group_let
    } printStatus;
//    void pretty_print(std::ostream &out);
//    virtual void pretty_print_at(std::ostream &out, enum printStatus status) = 0;
    std::string to_string();
//    std::string to_string_pretty();
    static PTR(Expr) parse_num(std::istream &in);
    static void consume(std::istream &in, int expect);
    static void skip_whitespace(std::istream &in);
    static PTR(Expr) parse_multicand(std::istream &in);
    static PTR(Expr) parse_addend(std::istream &in);
    static PTR(Expr) parse_comparg(std::istream &in);
    static PTR(Expr) parse_expr(std::istream &in);
    static PTR(Expr) parse_inner(std::istream &in);

    static PTR(Expr) parse_str(std::string s);
    static PTR(Expr) parse_let(std::istream &in);
    static PTR(Expr) parse_true(std::istream &in);
    static PTR(Expr) parse_false(std::istream &in);
    static PTR(Expr) parse_var(std::istream &in);
    static PTR(Expr) parse_if(std::istream &in);
    static PTR(Expr) parse_fun(std::istream &in);
};


class NumExpr : public Expr {
public:
    int rep;
    explicit NumExpr(int val);
    bool equals(PTR(Expr) e) override;
    PTR(Val) interp(PTR(Env) env) override;
    bool has_variable() override;
//    PTR(Expr) subst(std::string s, PTR(Expr) e) override;
    void print(std::ostream &out) override;
//    void pretty_print_at(std::ostream &out, enum printStatus status);
    void step_interp() override;
};

class BoolExpr : public Expr {
public:
    bool rep;
    explicit BoolExpr(bool rep);
    bool equals(PTR(Expr) e) override;
    PTR(Val) interp(PTR(Env) env) override;
    bool has_variable() override;
//    PTR(Expr) subst(std::string s, PTR(Expr) e) override;
    void print(std::ostream &out) override;
//    void pretty_print_at(std::ostream &out, enum printStatus status);
    void step_interp() override;
};

class EqExpr : public Expr {
public:
    PTR(Expr) lhs;
    PTR(Expr) rhs;
    EqExpr(PTR(Expr) lhs, PTR(Expr) rhs);
    bool equals(PTR(Expr) e) override;
    PTR(Val) interp(PTR(Env) env) override;
    bool has_variable() override;
//    PTR(Expr) subst(std::string s, PTR(Expr) e) override;
    void print(std::ostream &out) override;
//    void pretty_print_at(std::ostream &out, enum printStatus status);
    void step_interp() override;
};

class AddExpr : public Expr {
public:
    PTR(Expr) lhs;
    PTR(Expr) rhs;
    AddExpr(PTR(Expr) lhs, PTR(Expr) rhs);
    bool equals(PTR(Expr) e) override;
    PTR(Val) interp(PTR(Env) env) override;
    bool has_variable() override;
//    PTR(Expr) subst(std::string s, PTR(Expr) e) override;
    void print(std::ostream &out) override;
//    void pretty_print_at(std::ostream &out, enum printStatus status);
    void step_interp() override;
};

class MultExpr : public Expr {
public:
    PTR(Expr) lhs;
    PTR(Expr) rhs;
    MultExpr(PTR(Expr) lhs, PTR(Expr) rhs);
    bool equals(PTR(Expr) e) override;
    PTR(Val) interp(PTR(Env) env) override;
    bool has_variable() override;
//    PTR(Expr) subst(std::string s, PTR(Expr) e) override;
    void print(std::ostream &out) override;
//    void pretty_print_at(std::ostream &out, enum printStatus status);
    void step_interp() override;
};

class VarExpr : public Expr {
public:
    std::string val;
    explicit VarExpr(std::string name);
    bool equals(PTR(Expr) e) override;
    PTR(Val) interp(PTR(Env) env) override;
    bool has_variable() override;
//    PTR(Expr) subst(std::string s, PTR(Expr) e) override;
    void print(std::ostream &out) override;
//    void pretty_print_at(std::ostream &out, enum printStatus status);
    void step_interp() override;
};

class LetExpr : public Expr {
public:
    std::string lhs;
    PTR(Expr) rhs;
    PTR(Expr) body;
    LetExpr(std::string lhs, PTR(Expr) rhs, PTR(Expr) body);
    bool equals(PTR(Expr) e) override;
    PTR(Val) interp(PTR(Env) env) override;
    bool has_variable() override;
//    PTR(Expr) subst(std::string s, PTR(Expr) e) override;
    void print(std::ostream &out) override;
//    void pretty_print_at(std::ostream &out, enum printStatus status);
    void step_interp() override;
};

class IfExpr : public Expr {
public:
    PTR(Expr) condition;
    PTR(Expr) statement1;
    PTR(Expr) statement2;
    IfExpr(PTR(Expr) condition, PTR(Expr) statement1, PTR(Expr) statement2);
    bool equals(PTR(Expr) e) override;
    PTR(Val) interp(PTR(Env) env) override;
    bool has_variable() override;
//    PTR(Expr) subst(std::string s, PTR(Expr) e) override;
    void print(std::ostream &out) override;
//    void pretty_print_at(std::ostream &out, enum printStatus status);
    void step_interp() override;
};

class FunExpr : public Expr {
public:
    std::string formal_arg;
    PTR(Expr) body;
    FunExpr(std::string formal_arg, PTR(Expr) body);
    bool equals(PTR(Expr) e) override;
    PTR(Val) interp(PTR(Env) env) override;
    bool has_variable() override;
//    PTR(Expr) subst(std::string s, PTR(Expr) e) override;
    void print(std::ostream &out) override;
//    void pretty_print_at(std::ostream &out, enum printStatus status) override;
    void step_interp() override;
};

class CallExpr : public Expr {
public:
    PTR(Expr) to_be_called;
    PTR(Expr) actual_arg;
    CallExpr(PTR(Expr) to_be_called, PTR(Expr) actual_arg);
    bool equals(PTR(Expr) e) override;
    PTR(Val) interp(PTR(Env) env) override;
    bool has_variable() override;
//    PTR(Expr) subst(std::string s, PTR(Expr) e) override;
    void print(std::ostream &out) override;
//    void pretty_print_at(std::ostream &out, enum printStatus status) override;
    void step_interp() override;
};


#endif //MSD_SCRIPT_EXPR_H
