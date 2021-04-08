//
// Created by Awerkamp on 2/24/21.
//


//#include "env.h"

#ifndef MSD_SCRIPT_VAL_H
#define MSD_SCRIPT_VAL_H

#include <string>
#include "pointer.h"
#include "expr.h"
#include "cont.hpp"
#include <cstdio>

class Expr;
class Env;

CLASS (Val) {
    public:
//        virtual PTR(Expr) to_expr() = 0;  // should be virtual for sure (not sure about others)
        virtual std::string to_string() = 0;
        virtual PTR(Val) add_to(PTR(Val) other_val) = 0;
        virtual PTR(Val) mult_by(PTR(Val) other_val) = 0;
        virtual bool equals(PTR(Val) e) = 0;
        std::string val;
        virtual PTR(Val) call(PTR(Val)  actual_arg) = 0;
        virtual bool is_true() = 0;
        virtual void call_step(PTR(Val) actual_arg_val, PTR(Cont) rest) = 0;
};

class NumVal: public Val {
public:
    int val;
    explicit NumVal(int val) {
        this->val =  val;
    }
//    PTR(Expr) to_expr() override;
    std::string to_string() override;
    PTR(Val) add_to(PTR(Val) other_val) override;
    PTR(Val) mult_by(PTR(Val) other_val) override;
    bool equals(PTR(Val) e) override;
    PTR(Val) call(PTR(Val) actual_arg) override;
    bool is_true() override;
    void call_step(PTR(Val) actual_arg_val, PTR(Cont) rest) override;
};

class BoolVal: public Val {
public:
    bool val;
    explicit BoolVal(bool val) {
        this->val = val;
    }
//    PTR(Expr) to_expr() override;
    std::string to_string() override;
    PTR(Val) add_to(PTR(Val) other_val) override;
    PTR(Val) mult_by(PTR(Val) other_val) override;
    bool equals(PTR(Val) e) override;
    PTR(Val) call(PTR(Val) actual_arg) override;
    bool is_true() override;
    void call_step(PTR(Val) actual_arg_val, PTR(Cont) rest) override;
};

class FunVal: public Val {
public:
    std::string formal_arg;
    PTR(Expr) body;
    PTR(Env) env;

    FunVal(std::string formal_arg, PTR(Expr) body, PTR(Env) env);

    bool equals(PTR(Val) other) override;
//    PTR(Expr) to_expr() override;
    std::string to_string() override;
    PTR(Val) add_to(PTR(Val) other) override;
    PTR(Val) mult_by(PTR(Val) other) override;
    PTR(Val) call(PTR(Val) actual_arg) override;
    bool is_true() override;
    void call_step(PTR(Val) actual_arg_val, PTR(Cont) rest) override;
};

#endif //MSD_SCRIPT_VAL_H
