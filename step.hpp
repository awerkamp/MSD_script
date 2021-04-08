#ifndef step_hpp
#define step_hpp

#include "pointer.h"
#include "env.h"

class Val;

class Expr;
class Cont;

class Step {
public:
  typedef enum {
    interp_mode,
    continue_mode
  } mode_t;

  /* Mode insicates whether the next step is to
     start interpreting an expression or start
     delivering a value to a continuation. */
  static mode_t mode;

  /* The expression to interpret, meaningful
    only when `mode` is `interp_mode`: */
  static PTR(Expr) expr;

  static PTR(Env) env;
  
  /* The value to be delivered to the continuation,
    meaningful only when `mode` is `continue_mode`: */
//  static int val;
  static PTR(Val) val;

  /* The continuation to receive a value, meaningful
     only when `mode` is `continue_mode`: */
  static PTR(Cont) cont;

  static PTR(Val) interp_by_steps(PTR(Expr) e);
  
  /* Function to interpret an expression by stepping.
     The function should only be called once to start
     an expression's interpretation. It must not be
     called by `step_interp` or `step_continue` (i.e.,
     it must not be called recursively, since the whole
     point is to avoid rcursive calls at the C++ level). */
};

#endif /* step_hpp */
