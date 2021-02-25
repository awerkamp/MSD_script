//
// Created by Awerkamp on 2/24/21.
//

#include "val.h"
#include "expr.h"

Expr* NumVal::to_expr() {

    return new NumExpr(this->val);
}

Val *NumVal::add_to(Val* other_val) {
    NumVal* other_num = dynamic_cast<NumVal*>(other_val);
    if (other_num == NULL) throw std::runtime_error("add of non-number");
    return new NumVal(val + other_num->val); // todo: not sure if this is right https://www.youtube.com/watch?v=Go5LJu-X_F0&list=PLbdXd8eufjyUWQw3Mqb3SNQEkdjd_S_rB&index=6
}

Val *NumVal::mult_by(Val* other_val) {
    NumVal* other_num = dynamic_cast<NumVal*>(other_val);
    if (other_num == NULL) throw std::runtime_error("multiply of non-number");
    return new NumVal(val * other_num->val);
}

bool NumVal::equals(Val *e) {
    auto *t = dynamic_cast<NumVal*>(e);
    if (t == nullptr) {
        return false;
    } else {
        return (this->val == t->val);
    }
}