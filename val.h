//
// Created by Awerkamp on 2/24/21.
//
#include <string>

#ifndef MSD_SCRIPT_VAL_H
#define MSD_SCRIPT_VAL_H

class Expr;

class Val {
    public:
        virtual Expr* to_expr() = 0;  // should be virtual for sure (not sure about others)
        virtual Val* add_to(Val* other_val) = 0;
        virtual Val* mult_by(Val* other_val) = 0;
        virtual bool equals(Val *e) = 0;
        std::string val;
        virtual Val* call(Val * actual_arg) = 0;
};

class NumVal: public Val {
public:
    int val;
    explicit NumVal(int val) {
        this->val =  val;
    }
    Expr* to_expr() override;
    Val* add_to(Val* other_val) override;
    Val* mult_by(Val* other_val) override;
    bool equals(Val *e) override;
    Val *call(Val *actual_arg) override;
};

class BoolVal: public Val {
public:
    bool val;
    explicit BoolVal(bool val) {
        this->val = val;
    }
    Expr* to_expr() override;
    Val* add_to(Val* other_val) override;
    Val* mult_by(Val* other_val) override;
    bool equals(Val *e) override;
    Val *call(Val *actual_arg) override;
};

class FunVal: public Val {
public:
    std::string formal_arg;
    Expr* body;
    FunVal(std::string formal_arg, Expr *body);

    bool equals(Val* other) override;
    Expr* to_expr() override;
    Val* add_to(Val *other) override;
    Val* mult_by(Val *other) override;
    Val *call(Val *actual_arg) override;
};

#endif //MSD_SCRIPT_VAL_H
