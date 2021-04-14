//
// Created by Awerkamp on 1/22/21.
//

#include "expr.h"
#include "val.h"
#include "env.h"
#include<string>
#include <stdexcept>
#include <iostream>
#include <sstream>
#include "step.hpp"

using namespace std;

PTR(Expr) Expr::parse_expr(std::istream &in) {
        PTR(Expr) e;

        e = parse_comparg(in);  // Chaged to comparg
        skip_whitespace(in);

        int c = in.peek();
        if (c == '=') {
            consume(in, '=');
            consume(in, '=');
            PTR(Expr) rhs = parse_expr(in);
            return NEW(EqExpr)(e , rhs);
        } else {
            return e;
        }
}

PTR(Expr) Expr::parse_comparg(std::istream &in) {
    PTR(Expr) e;
    e = parse_addend(in);
    skip_whitespace(in);

    int c = in.peek();
    if (c == '+') {
        consume(in, '+');
        PTR(Expr) rhs = parse_comparg(in);
        return NEW(AddExpr)(e , rhs);
    } else {
        return e;
    }
}

//PTR(Expr) parse_str(std::string s) {// wrapper for testing.
//    // transfer the string into a input the std::istream &in:
//    std::istringstream in(s); // read the stream as a std::istringstream in(s);
//    return Expr::parse_expr(in);
//}

PTR(Expr) Expr::parse_str(std::string s) {
    std::istringstream in(s); // read the stream as a std::istringstream in(s);
    return Expr::parse_expr(in);
}

PTR(Expr) Expr::parse_addend(std::istream &in) {
    PTR(Expr) e = parse_multicand(in);
    skip_whitespace(in);
    int c = in.peek();
    if (c == '*') {
        consume(in, '*');
        PTR(Expr) rhs = parse_addend(in);
        return NEW(MultExpr)(e, rhs);
    } else {
        return e;
    }
}

PTR(Expr) Expr::parse_var(std::istream &in) {

    string var;

    while (1) {
        int c = in.peek();
        if (isalpha(c)) {
//            consume(in, c);
            var += (char)in.get();
        } else
            break;
    }
    return NEW(VarExpr)(var);
}

//
//PTR(Expr) parse_var(std::istream &in) {
//    std::string name;
//    while(1) {
//        char c = in.peek();
//        if (isalpha(c)) {
//            Expr::consume(in, c);
//            name = name + c;
//        }
//        else {
//            break;
//        }
//    }
//    return NEW(VarExpr)(name);
//}

PTR(Expr) Expr::parse_multicand(std::istream &in) {
    PTR(Expr) expr = parse_inner(in);
    while (in.peek() == '(') {
        consume(in, '(');
        PTR(Expr) actual_arg = parse_expr(in);
        consume(in, ')');
        expr = NEW(CallExpr)(expr, actual_arg);
    }
    return expr;
}

static std::string parse_alphabetic(std::istream &in, std::string prefix) {
    std::string name = prefix;
    while (1) {
        char c = in.peek();
        if (!isalpha(c)) {
            break;
        } else {
            name += in.get();
        }

    }
    return name;
}

std::string parse_keyword(std::istream &in) {
    in.get();
    return parse_alphabetic(in, "_");
}

PTR(Expr) Expr::parse_inner(std::istream &in) {
    skip_whitespace(in);
    int c = in.peek();

    if ((c == '-') || isdigit(c)) {
        return parse_num(in);
    } else if (c == '_') {
//        consume(in, '_');
        string key_word = parse_keyword(in);
//        c = in.peek();
        if (key_word == "_if"){
            return parse_if(in);
        } else if (key_word == "_true") {
            return parse_true(in);
        } else if (key_word == "_false") {
            return parse_false(in);
        } else if (key_word == "_let") {
            return parse_let(in);
        } else if (key_word == "_fun") {
            return parse_fun(in);
        } else {
            throw runtime_error("Invalid input after underscore _ ");
        }
    } else if (isalpha(c)) {
      return parse_var(in);
    } else if (c == '('){
        consume(in, '(');
//        c = in.peek();
        PTR(Expr) e = parse_comparg(in);
        skip_whitespace(in);
        c = in.get();
        if (c != ')') {
            throw std::runtime_error("missing close parenthesis");
        }
        return e;
    }

    else {
//        consume(in, c);
        cout << "This is the error" << c << endl;
        throw std::runtime_error("invalid input 2 ");
    }
}

//string check_var(std::istream &in, string &var) {
//    if (in.peek() >= 65 && in.peek() <= 90 || in.peek() >= 97 && in.peek() <= 122 ) {
//        var = (char)in.get();
//        return var;
//    } else {
//        throw std::runtime_error("not a variable");
//    }
//}
///







PTR(Expr) Expr::parse_if(std::istream &in) {

    skip_whitespace(in);

    PTR(Expr) ifExpr = parse_expr(in);

    PTR(NumExpr) numExpr = CAST(NumExpr)(ifExpr);
    if (numExpr != nullptr) {
        throw std::runtime_error("If Conditional should not be a number");
    }

    skip_whitespace(in);

    std::string thenString = parse_keyword(in);
    if (thenString != "_then") {
        throw std::runtime_error("missing thenstring");
    }

    PTR(Expr) thenExpr = parse_expr(in);


    skip_whitespace(in);

    std::string elseString = parse_keyword(in);

    if (elseString != "_else") {
        throw std::runtime_error("Missing else string");
    }

    PTR(Expr) elseExpr = parse_expr(in);

    return NEW(IfExpr)(ifExpr, thenExpr, elseExpr);
}

PTR(Expr) Expr::parse_true(std::istream &in) {
    return NEW(BoolExpr)(true);
}


PTR(Expr) Expr::parse_false(std::istream &in) {
    return NEW(BoolExpr)(false);
}


PTR(Expr) Expr::parse_let(std::istream &in) {

    skip_whitespace(in);

    PTR(Expr) var = parse_var(in);
    string varString = var->to_string();

    skip_whitespace(in);
    consume(in, '=');
    skip_whitespace(in);

    PTR(Expr) rhs = parse_comparg(in);

//    skip_whitespace(in);
    std::string inString = parse_keyword(in);

    if (inString != "_in") {
        throw std::runtime_error("Missing In");
    }
    skip_whitespace(in);

    PTR(Expr) body = parse_comparg(in);

    return NEW(LetExpr)(varString, rhs, body);
}



PTR(Expr) Expr::parse_num(std::istream &in) {
    int n = 0;
    bool negative = false;
    if (in.peek() == '-') {
        negative = true;
        consume(in, '-');
    }
    while (1) {
        int c = in.peek();
        if (isdigit(c)) {
            consume(in, c);
            unsigned long long test = n;

            n = (unsigned) n*10 + (c - '0');
            if (n < 0) {
                throw runtime_error("Number too large");
            }
        } else
            break;
    }
    if (negative)
        n = -n;
    return NEW(NumExpr)(n);
}

PTR(Expr) Expr::parse_fun(std::istream &in) {
    skip_whitespace(in);
    AddExpr::consume(in, '(');
    skip_whitespace(in);
    PTR(Expr) formal_arg = AddExpr::parse_var(in);
    skip_whitespace(in);
    AddExpr::consume(in, ')');
    skip_whitespace(in);
    PTR(Expr) body = AddExpr::parse_comparg(in);

    std::ostream out(nullptr);
    std::stringbuf str;
    out.rdbuf(&str);

    return NEW( FunExpr)(formal_arg->to_string(), body);
}

void Expr::skip_whitespace(std::istream &in) {
    while (1) {
        int c = in.peek();
        if (!isspace(c))
            break;
        consume(in, c);
    }
}

void Expr::consume(std::istream &in, int expect) {
    int c = in.get();
    if (c != expect) {
        cout << "this is what trying to consume" << c << endl;
        cout << "this is what was expected" << expect << endl;
        throw std::runtime_error("consume mismatch");
    }
}

std::string Expr::to_string() {
    std::ostream out(nullptr);
    std::stringbuf str;
    out.rdbuf(&str);
    this->print(out);
    std::stringstream ss;
    ss << out.rdbuf();
    return ss.str();
}

//std::string Expr::to_string_pretty() {
//    std::ostream out(nullptr);
//    std::stringbuf str;
//    out.rdbuf(&str);
//    this->pretty_print(out);
//    std::stringstream ss;
//    ss << out.rdbuf();
//    return ss.str();
//}

//void Expr::pretty_print(std::ostream &out) {
//    pretty_print_at(out, print_group_none);
//}




NumExpr::NumExpr(int val) {
//    val->Expr
    this->rep = val;
}

bool NumExpr::equals(PTR(Expr) e) {
    PTR(NumExpr)t = CAST(NumExpr)(e);
    if (t == nullptr) {
        return false;
    } else {
        return (this->rep == t->rep);
    }
}

 PTR(Val) NumExpr::interp(PTR(Env) env) {
    return NEW(NumVal)(this->rep);
}

void NumExpr::step_interp() {
    Step::mode = Step::continue_mode;
    Step::val = NEW(NumVal)(rep);
    Step::cont = Step::cont;
}

bool NumExpr::has_variable() {
    return false;
}

//PTR(Expr) NumExpr::subst(std::string s, PTR(Expr) e) {
//    return THIS;
//}

void NumExpr::print(std::ostream &out) {
    out << this->rep;
}
//
//void NumExpr::pretty_print_at(std::ostream &out, enum printStatus status) {
//    out << this->rep;
//}

BoolExpr::BoolExpr(bool val) {
//    val->Expr
    this->rep = val;

//    if (!val || val) {
//        throw runtime_error("Boolean Value must be a boolean");
//    }
//
//    if (val != "_true" && val != "_false") {
//        throw runtime_error("Boolean Value must be either _true or _false");
//    }
}

bool BoolExpr::equals(PTR(Expr) e) {
    PTR(BoolExpr)t = CAST(BoolExpr)(e);
    if (t == nullptr) {
        return false;
    } else {
        return (this->rep == t->rep);
    }
}

PTR(Val) BoolExpr::interp(PTR(Env) env) {
    return NEW(BoolVal)(this->rep);
}

void BoolExpr::step_interp() {
    Step::mode = Step::continue_mode;
    Step::val = NEW(BoolVal)(rep);
    Step::cont = Step::cont;
}

bool BoolExpr::has_variable() {
    return false;
}

//PTR(Expr) BoolExpr::subst(std::string s, PTR(Expr) e) {
//    return THIS;
//}

void BoolExpr::print(std::ostream &out) {
    if (rep) {
        out << "_true";
    } else {
        out << "_false";
    }
}

//void BoolExpr::pretty_print_at(std::ostream &out, enum printStatus status) {
//    if (rep) {
//        out << "_true";
//    } else {
//        out << "_false";
//    }
//}

EqExpr::EqExpr(PTR(Expr) lhs, PTR(Expr) rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}

bool EqExpr::equals(PTR(Expr) e) {
    PTR(EqExpr)t = CAST(EqExpr)(e);
    if (t == nullptr) {
        return false;
    } else {
        return (this->lhs->equals(t->lhs) && this->rhs->equals(t->rhs));
    }
}

PTR(Val) EqExpr::interp(PTR(Env) env) {

    if (this->lhs->interp(env)->equals(this->rhs->interp(env))) {

        return NEW(BoolVal)(true);
    } else {
        return NEW(BoolVal)(false);
    }
}

void EqExpr::step_interp() {
    Step::mode = Step::interp_mode;
    Step::expr = lhs;
    Step::cont = NEW(RightThenEqCont)(rhs, Step::env, Step::cont);
}



bool EqExpr::has_variable() {
    return (this->lhs->has_variable() || this->rhs->has_variable());
}

//PTR(Expr) EqExpr::subst(std::string s, PTR(Expr) e) {
//    return NEW(EqExpr)(this->lhs->subst(s, e), this->rhs->subst(s, e));
//}

void EqExpr::print(std::ostream &out) {
    out << "(";
    this->lhs->print(out);
    out << "==";
    this->rhs->print(out);
    out << ")";
}

//void EqExpr::pretty_print_at(std::ostream &out, enum printStatus status) {
//
//    if(status == print_group_add || status == print_group_add_or_mult) {
//        out << "(";
//        this->lhs->pretty_print_at(out, print_group_add);
//        out << " == ";
//        this->rhs->pretty_print_at(out, print_group_none);
//        out << ")";
//    } else {
//        this->lhs->pretty_print_at(out, print_group_add);
//        out << " == ";
//        this->rhs->pretty_print_at(out, print_group_none);
//    }
//}

AddExpr::AddExpr(PTR(Expr) lhs, PTR(Expr) rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}

bool AddExpr::equals(PTR(Expr) e) {
    PTR(AddExpr)t = CAST(AddExpr)(e);
    if (t == nullptr) {
        return false;
    } else {
        return (this->lhs->equals(t->lhs) && this->rhs->equals(t->rhs));
    }
}

PTR(Val) AddExpr::interp(PTR(Env) env) {
    return this->lhs->interp(env)->add_to(this->rhs->interp(env));
}

void AddExpr::step_interp() {
    Step::mode = Step::interp_mode;
    Step::expr = lhs;
    Step::env = Step::env;
    Step::cont = NEW(RightThenAddCont)(rhs, Step::env, Step::cont);
}

bool AddExpr::has_variable() {
    return (this->lhs->has_variable() || this->rhs->has_variable());
}

//PTR(Expr) AddExpr::subst(std::string s, PTR(Expr) e) {
//    return NEW(AddExpr)(this->lhs->subst(s, e), this->rhs->subst(s, e));
//}

void AddExpr::print(std::ostream &out) {
    out << "(";
    this->lhs->print(out);
    out << "+";
    this->rhs->print(out);
    out << ")";
}

//void AddExpr::pretty_print_at(std::ostream &out, enum printStatus status) {
//
//    if(status == print_group_add || status == print_group_add_or_mult) {
//        out << "(";
//        this->lhs->pretty_print_at(out, print_group_add);
//        out << " + ";
//        this->rhs->pretty_print_at(out, print_group_none);
//        out << ")";
//    } else {
//        this->lhs->pretty_print_at(out, print_group_add);
//        out << " + ";
//        this->rhs->pretty_print_at(out, print_group_none);
//    }
//}

MultExpr::MultExpr(PTR(Expr) lhs, PTR(Expr) rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}

bool MultExpr::equals(PTR(Expr) e) {
    PTR(MultExpr)t = CAST(MultExpr)(e);
    if (t == nullptr) {
        return false;
    } else {
        return (this->lhs->equals(t->lhs) && this->rhs->equals(t->rhs));
    }
}

PTR(Val) MultExpr::interp(PTR(Env) env) {
    return this->lhs->interp(env)->mult_by(this->rhs->interp(env));
}

void MultExpr::step_interp() {
    Step::mode = Step::interp_mode;
    Step::expr = lhs;
    Step::cont = NEW(RightThenMultCont)(rhs, Step::env, Step::cont);
}

bool MultExpr::has_variable() {
    return (this->lhs->has_variable() || this->rhs->has_variable());
}

//PTR(Expr) MultExpr::subst(std::string s, PTR(Expr) e) {
//    return NEW(MultExpr)(this->lhs->subst(s, e), this->rhs->subst(s, e));
//}

void MultExpr::print(std::ostream &out) {
    out << "(";
    this->lhs->print(out);
    out << "*";
    this->rhs->print(out);
    out << ")";
}


//void MultExpr::pretty_print_at(std::ostream &out, enum printStatus status){
//
//    if(status == print_group_add_or_mult) {
//        out << "(";
//        this->lhs->pretty_print_at(out, print_group_add_or_mult);
//        out << " * ";
//        this->rhs->pretty_print_at(out, print_group_add);
//        out << ")";
//    } else {
//        this->lhs->pretty_print_at(out, print_group_add_or_mult);
//        out << " * ";
//        this->rhs->pretty_print_at(out, print_group_add);
//    }
//}

VarExpr::VarExpr(std::string name) {
    this->val = name;
}

bool VarExpr::equals(PTR(Expr) e) {
    PTR(VarExpr)t = CAST(VarExpr)(e);
    if (t == nullptr) {
        return false;
    } else {
        return (this->val == t->val);
    }
}

PTR(Val) VarExpr::interp(PTR(Env) env) {
    return env->lookup(val);
}

void VarExpr::step_interp() {
    Step::mode = Step::continue_mode;
    Step::val = Step::env->lookup(val);
    Step::cont = Step::cont;
}

bool VarExpr::has_variable() {
    return true;
}

//PTR(Expr) VarExpr::subst(std::string s, PTR(Expr) e) {
//    if (this->name == s) {
//        return e;
//    } else {
//        return THIS;
//    }
//}

void VarExpr::print(std::ostream &out) {
    out << this->val;
}

//void VarExpr::pretty_print_at(std::ostream &out, enum printStatus status){
//    out << this->name;
//}

LetExpr::LetExpr(std::string lhs, PTR(Expr) rhs, PTR(Expr) body) {
    this->lhs = lhs;
    this->rhs = rhs;
    this->body = body;
}

bool LetExpr::equals(PTR(Expr) e) {
    PTR(LetExpr)t = CAST(LetExpr)(e);
    if (t == nullptr) {
        return false;
    } else {
        return (this->lhs == (t->lhs) && this->rhs->equals(t->rhs) && this->body->equals(t->body));
    }
}

PTR(Val) LetExpr::interp(PTR(Env) env) {
//    Var *t = CAST(Var)(this->lhs);  // Ensures first argument is of type Var
//    if (t == nullptr) {
//        throw std::runtime_error("First argument must be of type Var");
//    }
//    Var* lhs_var = CAST(Var)(this->lhs); // Casts lhs to Var


    PTR(Val) rhs_val = this->rhs->interp(env); // evaluates rhs
//    NumExpr* rhs_interp = NEW(NumExpr)(n); // Sets rhs as a Num
    PTR(Env) new_env = NEW(ExtendedEnv)(lhs, rhs_val, env);
    // Substitutes any variable in the body with the rhs evaluation (interp)
    return body->interp(new_env);

//    return this->body->subst(lhs, rhs_val->to_expr())->interp(env);
}

void LetExpr::step_interp() {
    Step::mode = Step::interp_mode;
    Step::expr = rhs;
    Step::env = Step::env;
    Step::cont = NEW(LetBodyCont)(lhs, body, Step::env, Step::cont);
}

bool LetExpr::has_variable() {
    return this->rhs->has_variable() || this->body->has_variable();
}

//PTR(Expr) LetExpr::subst(std::string s, PTR(Expr) e) {
//
//    if (s != this->lhs) {
//        return NEW(LetExpr)(this->lhs, this->rhs->subst(s, e), this->body->subst(s, e));
//    } else {
//        return NEW(LetExpr)(this->lhs, this->rhs->subst(s, e), this->body);
//    }
//}

void LetExpr::print(std::ostream &out) {
    out << "(_let ";
    out << lhs;
    out << "=";
    this->rhs->print(out);
    out << " _in ";
    this->body->print(out);
    out << ")";
}

//void LetExpr::pretty_print_at(std::ostream &out, enum printStatus status){  // todo Make Indented and on new line when let within let
//
//    print(out);

//    auto *r = CAST(LetExpr)(this->rhs);
//    auto *b = CAST(LetExpr)(this->body);
//
//    if (status == print_group_let) {
//        out << "(_let ";
//        out << lhs;
//        out << " = ";
//        if (r != nullptr) {
//            this->rhs->pretty_print_at(out, print_group_let);
//        } else {
//            this->rhs->pretty_print_at(out, print_group_none);
//        }
//        out << " in ";
//        if (r != nullptr) {
//            this->body->pretty_print_at(out, print_group_let);
//        } else {
//            this->body->pretty_print_at(out, print_group_none);
//        }
//        out << ")";
//    } else {
//        out << "_let ";
//        out << lhs;
//        out << " = ";
//        if (r != nullptr) {
//            this->rhs->pretty_print_at(out, print_group_let);
//        } else {
//            this->rhs->pretty_print_at(out, print_group_none);
//        }
//        out << " in ";
//        if (body != nullptr) {
//            this->rhs->pretty_print_at(out, print_group_let);
//        } else {
//            this->body->pretty_print_at(out, print_group_none);
//        }
//    }
//}

IfExpr::IfExpr(PTR(Expr) condition, PTR(Expr) statement1, PTR(Expr) statement2) {
//    auto *t = CAST(BoolExpr)(condition);
//    if (t == nullptr) {
//        throw runtime_error("First parameter of If Expr must be BoolExpr");
//    } else {
//
//    }
    this->condition = condition;
    this->statement1 = statement1;
    this->statement2 = statement2;
}

bool IfExpr::equals(PTR(Expr) e) {
    PTR(IfExpr)t = CAST(IfExpr)(e);
    if (t == nullptr) {
        return false;
    } else {
        return (this->condition->equals(t->condition) && this->statement1->equals(t->statement1) && this->statement2->equals(t->statement2));
    }
}

PTR(Val) IfExpr::interp(PTR(Env) env) {

    if (condition->interp(env)->equals(NEW(BoolVal)(true))) {
        return statement1->interp(env);
    } else {
        return statement2->interp(env);
    }
}

void IfExpr::step_interp() {
    Step::mode = Step::interp_mode;
    Step::expr = condition;
    Step::env = Step::env;
    Step::cont = NEW(IfBranchCont)(statement1, statement2, Step::env, Step::cont);
}



bool IfExpr::has_variable() {
    return this->condition->has_variable() || this->statement1->has_variable() || this->statement2->has_variable();
}

//PTR(Expr) IfExpr::subst(std::string s, PTR(Expr) e) {
//
//    return NEW(IfExpr)(this->condition->subst(s, e), this->statement1->subst(s, e), this->statement2->subst(s,e));
//}

void IfExpr::print(std::ostream &out) {
    out << "(_if ";
    condition->print(out);
    out << " _then ";
    statement1->print(out);
    out << " _else ";
    statement2->print(out);
    out << ")";
}

//void IfExpr::pretty_print_at(std::ostream &out, enum printStatus status){  // todo Make Indented and on new line when let within let
//
//    print(out);
//}



FunExpr::FunExpr(std::string formal_arg, PTR(Expr) body) {
    this->formal_arg = formal_arg;
    this->body = body;
}

bool FunExpr::equals(PTR(Expr) e) {
    PTR(FunExpr)t = CAST(FunExpr)(e);
    if (t == nullptr) {
        return false;
    } else {
        return (formal_arg == t->formal_arg && this->body->equals(t->body));
    }
}

PTR(Val) FunExpr::interp(PTR(Env) env) {
    return NEW(FunVal) (formal_arg, body, env);
}

void FunExpr::step_interp() {
    Step::mode = Step::continue_mode;
    Step::val = NEW(FunVal)(formal_arg, body, Step::env);
    Step::cont = Step::cont;
}

bool FunExpr::has_variable() {
    throw std::runtime_error("not implemented");
}



//PTR(Expr) FunExpr::subst(std::string s, PTR(Expr) e) {
//
//    if(s.compare(formal_arg) == 0) {
//        return NEW( FunExpr)(this->formal_arg, this->body);
//    }
//    return NEW( FunExpr)(this->formal_arg, this->body);
//}

void FunExpr::print(std::ostream &out) {
    out << "(_fun (";
    out << formal_arg;
    out << ") ";
    this->body->print(out);
    out << ") ";
}

//void FunExpr::pretty_print(std::ostream &out) {
//    out << "(fun ( ";
//    out << formal_arg;
//    out << " )";
//    this->body->print(out);
//    out << ") ";
//
//
//}

CallExpr::CallExpr(PTR(Expr) to_be_called, PTR(Expr) actual_arg) {
    this -> to_be_called = to_be_called;
    this->actual_arg = actual_arg;
}

bool CallExpr::equals(PTR(Expr) e) {
    PTR(CallExpr)c = CAST(CallExpr )(e);
    if (c == NULL) {
        return false;
    } else {
        return (this->to_be_called->equals(c->to_be_called) && this->actual_arg->equals(c->actual_arg));
    }
}


PTR(Val) CallExpr::interp(PTR(Env) env) {
    return to_be_called->interp(env)->call(actual_arg->interp(env));
}

void CallExpr::step_interp() {
    Step::mode = Step::interp_mode;
    Step::expr = to_be_called;
    Step::cont = NEW(ArgThenCallCont)(actual_arg, Step::env, Step::cont);
}

bool CallExpr::has_variable() {
    throw std::runtime_error("invalid call");
}

//PTR(Expr)  CallExpr::subst(std::string sub, PTR(Expr) expr) {
//    PTR(Expr) new_to_be_called = to_be_called ->subst(sub, expr);
//    PTR(Expr) new_actual_arg = actual_arg->subst(sub, expr);
//    return NEW(CallExpr)(new_to_be_called, new_actual_arg);
//}

void CallExpr::print(std::ostream &out) {

    this->to_be_called->print(out);
    out << "(";
    this -> actual_arg -> print(out);
    out << ")";
}



