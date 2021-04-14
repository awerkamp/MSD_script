#include "step.hpp"
//#include "expr.h"
//#include "cont.hpp"
//#include "pointer.h"
//#include "cont.hpp"

class Val;
class Cont;

Step::mode_t Step::mode;

PTR(Cont) Step::cont;

PTR(Expr) Step::expr; /* only for Step::interp_mode */
PTR(Val) Step::val;        /* only for Step::continue_mode */
PTR(Env) Step::env;


PTR(Val) Step::interp_by_steps(PTR(Expr) e) {
  Step::mode = Step::interp_mode;
  Step::expr = e;
  Step::env = Env::empty;
  Step::val = nullptr;
  Step::cont = Cont::done;

  while (1) {
    if (Step::mode == Step::interp_mode)
      Step::expr->step_interp();
    else {
      if (Step::cont == Cont::done)
        return Step::val;
      else {
          Step::cont->step_continue();
      }
    }
  }
}
