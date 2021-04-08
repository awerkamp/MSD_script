#ifndef cont_hpp
#define cont_hpp

#include "pointer.h"
#include <string>

class Expr;
class Env;
class Val;
class Step;

CLASS (Cont) {
public:
  /* To take one step in the computation starting
     with this continuation, reading from the registers
     in `Step` and updating them to indicate the next
     step. The `Step::cont` register will contain
     this continuation (so it's uninteresting), and
     the `Step::val` register will contain the value
     that this continuaion was waiting form.
     The `Step::expr` register is unspecified
     (i.e., must not be used by this method). */




  virtual void step_continue() = 0;
  static PTR(Cont) done;
};

//class DoneCont : public Cont {
//public:
//  DoneCont();
//  void step_continue() override;
//};
//
//class NegateCont : public Cont {
//public:
//  PTR(Cont) rest;
//
//  NegateCont(PTR(Cont) rest);
//  void step_continue() override;
//};

class RightThenAddCont : public Cont {
public:
  PTR(Expr) rhs;
  PTR(Env) env;
  PTR(Cont) rest;
  void step_continue();
  
  RightThenAddCont(PTR(Expr) rhs, PTR(Env) env, PTR(Cont) rest);
};

class AddCont : public Cont {
public:
  PTR(Val) lhs_val;
  PTR(Cont) rest;
  AddCont(PTR(Val) lhs_val, PTR(Cont) rest);
  void step_continue() override;
};

class RightThenMultCont : public Cont {
public:
    PTR(Expr) rhs;
    PTR(Env) env;
    PTR(Cont) rest;
    void step_continue() override;
    RightThenMultCont(PTR(Expr) rhs, PTR(Env) env, PTR(Cont) rest);
};

class MultCont : public Cont {
public:
    PTR(Val) lhs_val;
    PTR(Cont) rest;
    void step_continue();
    MultCont(PTR(Val) lhs_val, PTR(Cont) rest);
};

class RightThenEqCont : public Cont {
public:
    PTR(Expr) rhs;
    PTR(Env) env;
    PTR(Cont) cont;
    void step_continue();
    RightThenEqCont(PTR(Expr)rhs, PTR(Env) env, PTR(Cont) cont);
};


class EqCont : public Cont {
public:
    PTR(Val) lhs_val;
    PTR(Cont) rest;
    void step_continue();
    EqCont(PTR(Val) lhs_val, PTR(Cont) rest);
};

class IfBranchCont: public Cont {
public:
    PTR(Expr) then_part;
    PTR(Expr) else_part;
    PTR(Env) env;
    PTR(Cont) rest;
    void step_continue();
    IfBranchCont(PTR(Expr) then_part, PTR(Expr) else_part, PTR(Env) env, PTR(Cont) rest);
};

class LetBodyCont : public Cont {
public:
    std::string var;
    PTR(Expr) body;
    PTR(Env) env;
    PTR(Cont) rest;
    void step_continue();
    LetBodyCont(std::string var, PTR(Expr) body, PTR(Env) env, PTR(Cont) rest);
};

class ArgThenCallCont : public Cont {
public:
    PTR(Expr) actual_arg;
    PTR(Env) env;
    PTR(Cont) rest;
    void step_continue();
    ArgThenCallCont(PTR(Expr) actual_arg, PTR(Env) env, PTR(Cont) rest);
};

class CallCont : public Cont {
public:
    PTR(Val) to_be_called_val;
    PTR(Cont) rest;
    void step_continue();
    CallCont(PTR(Val) to_be_called_val, PTR(Cont) rest);
};

class DoneCont : public Cont {
public:
    DoneCont();
    void step_continue();
};

class NegateCont : public Cont {
public:
    PTR(Cont) rest;
    NegateCont(PTR(Cont) rest);
    void step_continue();
};

#endif /* cont_hpp */
