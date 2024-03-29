#include <stdexcept>

#include "cont.hpp"
#include "step.hpp"


PTR(Cont) Cont::done = NEW(DoneCont)();

DoneCont::DoneCont() { }

void DoneCont::step_continue() {
  throw std::runtime_error("error in doneCont");
}

void IfBranchCont::step_continue() {
    PTR(Val) test_val = Step::val;
    Step::mode = Step::interp_mode;
    if (test_val->is_true()) {
        Step::expr = then_part;
    } else {
        Step::expr = else_part;
    }
    Step::env = env;
    Step::cont = rest;
}

IfBranchCont::IfBranchCont(PTR(Expr) then_part, PTR(Expr) else_part, PTR(Env) env, PTR(Cont) rest) {
    this->then_part = then_part;
    this->else_part = else_part;
    this->env = env;
    this->rest = rest;
}

void LetBodyCont::step_continue() {
    Step::mode = Step::interp_mode;
    Step::expr = body;
    Step::cont = rest;
    Step::env = NEW(ExtendedEnv)(var, Step::val, env);
}

LetBodyCont::LetBodyCont(std::string var, PTR(Expr) body, PTR(Env) env, PTR(Cont) rest) {
    this->var = var;
    this->body = body;
    this->env = env;
    this->rest = rest;
}

void ArgThenCallCont::step_continue() {
    Step::mode = Step::interp_mode;
    Step::expr = actual_arg;
    Step::cont = NEW(CallCont)(Step::val, rest);
    Step::env = env;
}

ArgThenCallCont::ArgThenCallCont(PTR(Expr) actual_arg, PTR(Env) env, PTR(Cont) rest) {
    this->actual_arg = actual_arg;
    this->env = env;
    this->rest = rest;
}

void CallCont::step_continue() {
    to_be_called_val->call_step(Step::val, rest);
}

CallCont::CallCont(PTR(Val) to_be_called_val, PTR(Cont) rest) {
    this->to_be_called_val = to_be_called_val;
    this->rest = rest;
}

//NegateCont::NegateCont(PTR(Cont) rest) {
//    this->rest = rest;
//}
//
//void NegateCont::step_continue() {
//    Step::mode = Step::continue_mode;
//    Step::val = Step::val;
//    Step::cont = rest;
//}

RightThenAddCont::RightThenAddCont(PTR(Expr) rhs, PTR(Env) env, PTR(Cont) rest) {
    this->rhs = rhs;
    this->env = env;
    this->rest = rest;
}

void RightThenAddCont::step_continue() {
    Step::mode = Step::interp_mode;
    Step::expr = rhs;
    Step::env = env;
    Step::cont = NEW(AddCont)(Step::val, rest);
}

AddCont::AddCont(PTR(Val) lhs_val, PTR(Cont) rest) {
    this->lhs_val = lhs_val;
    this-> rest = rest;
}
void AddCont::step_continue() {
    Step::mode = Step::continue_mode;
    Step::val = lhs_val->add_to(Step::val);
    Step::cont = rest;
}

RightThenMultCont::RightThenMultCont(PTR(Expr) rhs, PTR(Env) env, PTR(Cont) rest) {
    this->rhs = rhs;
    this->env = env;
    this->rest = rest;
}

void RightThenMultCont::step_continue() {
    Step::mode = Step::interp_mode;
    Step::expr = rhs;
    Step::env = env;
    Step::cont = NEW(MultCont)(Step::val, rest);
}

void MultCont::step_continue() {
    Step::mode = Step::continue_mode;
    Step::val = lhs_val->mult_by(Step::val);
    Step::cont = rest;
}

MultCont::MultCont(PTR(Val) lhs_val, PTR(Cont) rest) {
    this->lhs_val = lhs_val;
    this->rest = rest;
}

RightThenEqCont::RightThenEqCont(PTR(Expr) rhs, PTR(Env) env, PTR(Cont) cont) {
    this->rhs = rhs;
    this->env = env;
    this->cont = cont;
}

void RightThenEqCont::step_continue() {
    Step::mode = Step::interp_mode;
    Step::expr = rhs;
    Step::env = env;
    Step::cont = NEW(EqCont)(Step::val, cont);
}

void EqCont::step_continue() {
    Step::mode = Step::continue_mode;
    Step::val = NEW(BoolVal)(lhs_val->equals(Step::val));
    Step::cont = rest;
}

EqCont::EqCont(PTR(Val) lhs_val, PTR(Cont) rest) {
    this->lhs_val = lhs_val;
    this->rest = rest;
}