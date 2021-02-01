//
// Created by Awerkamp on 1/22/21.
//

#ifndef MSD_SCRIPT_EXPR_H
#define MSD_SCRIPT_EXPR_H

#include<string>
#include<iostream>
using namespace std;

//int accumulator;



class Expr {
public:
    virtual bool equals(Expr *e) = 0;
    virtual int interp() = 0;
    virtual bool has_variable() = 0;
    virtual Expr* subst(std::string s, Expr* e) = 0;
    virtual void print(std::ostream &out) = 0;
    virtual void pretty_print(std::ostream &out) = 0;

    std::string to_string(std::ostream &out);
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
    void pretty_print(std::ostream &out) override;


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
    void pretty_print(std::ostream &out) override;
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
    void pretty_print(std::ostream &out) override;

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
    void pretty_print(std::ostream &out) override;
};


#endif //MSD_SCRIPT_EXPR_H
