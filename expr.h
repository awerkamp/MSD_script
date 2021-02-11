//
// Created by Awerkamp on 1/22/21.
//

#ifndef MSD_SCRIPT_EXPR_H
#define MSD_SCRIPT_EXPR_H

#include<string>
#include<iostream>
using namespace std;

class Expr {
public:
    virtual bool equals(Expr *e) = 0;
    virtual int interp() = 0;
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
    std::string to_string_pretty(std::ostream &out);
};

class Num : public Expr {
public:
    int val;
    explicit Num(int val);
    bool equals(Expr *e) override;
    int interp() override;
    bool has_variable() override;
    Expr* subst(std::string s, Expr* e) override;
    void print(std::ostream &out) override;
    void pretty_print_at(std::ostream &out, enum printStatus status) override;
};

class Add : public Expr {
public:
    Expr *lhs;
    Expr *rhs;
    Add(Expr *lhs, Expr *rhs);
    bool equals(Expr *e) override;
    int interp() override;
    bool has_variable() override;
    Expr* subst(std::string s, Expr* e) override;
    void print(std::ostream &out) override;
    void pretty_print_at(std::ostream &out, enum printStatus status) override;
};

class Mult : public Expr {
public:
    Expr *lhs;
    Expr *rhs;
    Mult(Expr *lhs, Expr *rhs);
    bool equals(Expr *e) override;
    int interp() override;
    bool has_variable() override;
    Expr* subst(std::string s, Expr* e) override;
    void print(std::ostream &out) override;
    void pretty_print_at(std::ostream &out, enum printStatus status) override;
};

class _let : public Expr {
public:
    std::string lhs;
    Expr *rhs;
    Expr *body;
    _let(std::string lhs, Expr *rhs, Expr *body);
    bool equals(Expr *e) override;
    int interp() override;
    bool has_variable() override;
    Expr* subst(std::string s, Expr* e) override;
    void print(std::ostream &out) override;
    void pretty_print_at(std::ostream &out, enum printStatus status) override;
};

class Var : public Expr {
public:
    std::string name;
    explicit Var(std::string name);
    bool equals(Expr *e) override;
    int interp() override;
    bool has_variable() override;
    Expr* subst(std::string s, Expr* e) override;
    void print(std::ostream &out) override;
    void pretty_print_at(std::ostream &out, enum printStatus status) override;
};



#endif //MSD_SCRIPT_EXPR_H
