///
// Created by Awerkamp on 1/22/21.
//

#ifndef MSD_SCRIPT_EXPR_H
#define MSD_SCRIPT_EXPR_H

#include<string>
#include<iostream>
// Do not #include val.h
using namespace std;

class Val;

class Expr {
public:

    virtual bool equals(Expr *e) = 0;
    virtual Val* interp() = 0;
    virtual bool has_variable() = 0;
    virtual Expr* subst(std::string s, Expr* e) = 0;
    virtual void print(std::ostream &out) = 0;
        enum printStatus {
        print_group_none,
        print_group_add,
        print_group_add_or_mult,
        print_group_let
    } printStatus;
    void pretty_print(std::ostream &out);
    virtual void pretty_print_at(std::ostream &out, enum printStatus status) = 0;
    std::string to_string(std::ostream &out);
    std::string to_string_pretty();
    static Expr* parse_num(std::istream &in);
    static void consume(std::istream &in, int expect);
    static void skip_whitespace(std::istream &in);
    static Expr* parse_multicand(std::istream &in);
    static Expr* parse_addend(std::istream &in);
    static Expr* parse_expr(std::istream &in);
    static Expr* parse_comparg(std::istream &in);

    static Expr *parse_str(std::string s);
    static Expr* parse_let(std::istream &in);
    static Expr* parse_var(std::istream &in);
};


class NumExpr : public Expr {
public:
    int rep;
    explicit NumExpr(int val);
    bool equals(Expr *e) override;
    Val* interp() override;
    bool has_variable() override;
    Expr* subst(std::string s, Expr* e) override;
    void print(std::ostream &out) override;
    void pretty_print_at(std::ostream &out, enum printStatus status) override;
};

class BoolExpr : public Expr {
public:
    std::string rep;
    explicit BoolExpr(string val);
    bool equals(Expr *e) override;
    Val* interp() override;
    bool has_variable() override;
    Expr* subst(std::string s, Expr* e) override;
    void print(std::ostream &out) override;
    void pretty_print_at(std::ostream &out, enum printStatus status) override;
};

class EqExpr : public Expr {
public:
    Expr *lhs;
    Expr *rhs;
    EqExpr(Expr *lhs, Expr *rhs);
    bool equals(Expr *e) override;
    Val* interp() override;
    bool has_variable() override;
    Expr* subst(std::string s, Expr* e) override;
    void print(std::ostream &out) override;
    void pretty_print_at(std::ostream &out, enum printStatus status) override;
};

class AddExpr : public Expr {
public:
    Expr *lhs;
    Expr *rhs;
    AddExpr(Expr *lhs, Expr *rhs);
    bool equals(Expr *e) override;
    Val* interp() override;
    bool has_variable() override;
    Expr* subst(std::string s, Expr* e) override;
    void print(std::ostream &out) override;
    void pretty_print_at(std::ostream &out, enum printStatus status) override;
};

class MultExpr : public Expr {
public:
    Expr *lhs;
    Expr *rhs;
    MultExpr(Expr *lhs, Expr *rhs);
    bool equals(Expr *e) override;
    Val* interp() override;
    bool has_variable() override;
    Expr* subst(std::string s, Expr* e) override;
    void print(std::ostream &out) override;
    void pretty_print_at(std::ostream &out, enum printStatus status) override;
};

class VarExpr : public Expr {
public:
    std::string name;
    explicit VarExpr(std::string name);
    bool equals(Expr *e) override;
    Val* interp() override;
    bool has_variable() override;
    Expr* subst(std::string s, Expr* e) override;
    void print(std::ostream &out) override;
    void pretty_print_at(std::ostream &out, enum printStatus status) override;
};

class LetExpr : public Expr {
public:
    std::string lhs;
    Expr *rhs;
    Expr *body;
    LetExpr(std::string lhs, Expr *rhs, Expr *body);
    bool equals(Expr *e) override;
    Val* interp() override;
    bool has_variable() override;
    Expr* subst(std::string s, Expr* e) override;
    void print(std::ostream &out) override;
    void pretty_print_at(std::ostream &out, enum printStatus status) override;
};

class IfExpr : public Expr {
public:
    Expr *condition;
    Expr *statement1;
    Expr *statement2;
    IfExpr(Expr *condition, Expr *statement1, Expr *statement2);
    bool equals(Expr *e) override;
    Val* interp() override;
    bool has_variable() override;
    Expr* subst(std::string s, Expr* e) override;
    void print(std::ostream &out) override;
    void pretty_print_at(std::ostream &out, enum printStatus status) override;
};



#endif //MSD_SCRIPT_EXPR_H
