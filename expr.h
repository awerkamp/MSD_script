//
// Created by Awerkamp on 1/22/21.
//

#ifndef MSD_SCRIPT_EXPR_H
#define MSD_SCRIPT_EXPR_H

class Expr {
public:
    virtual bool equals(Expr *e) = 0;
};

class Num : public Expr {
public:
    int val;
    bool equals(Expr *e) override;
    explicit Num(int val);
};

class Var : public Expr {
public:
    std::string name;
    bool equals(Expr *e) override;
    explicit Var(std::string name);
};

class Add : public Expr {
public:
    bool equals(Expr *e) override;
    Expr *lhs;
    Expr *rhs;
    Add(Expr *lhs, Expr *rhs);
};

class Mult : public Expr {
public:
    bool equals(Expr *e) override;
    Expr *lhs;
    Expr *rhs;
    Mult(Expr *lhs, Expr *rhs);
};

#endif //MSD_SCRIPT_EXPR_H
