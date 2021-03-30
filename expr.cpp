//
// Created by Awerkamp on 1/22/21.
//

#include "expr.h"
#include "val.h"
#include<string>
#include <stdexcept>
#include <iostream>
#include <sstream>

using namespace std;

Expr *Expr::parse_expr(std::istream &in) {
        Expr *e;

        e = parse_comparg(in);  // Chaged to comparg
        skip_whitespace(in);

        int c = in.peek();
        if (c == '=') {
            consume(in, '=');
            consume(in, '=');
            Expr* rhs = parse_expr(in);
            return new EqExpr(e , rhs);
        } else {
            return e;
        }
}

Expr *Expr::parse_comparg(std::istream &in) {
    Expr *e;
    e = parse_addend(in);
    skip_whitespace(in);

    int c = in.peek();
    if (c == '+') {
        consume(in, '+');
        Expr* rhs = parse_comparg(in);
        return new AddExpr(e , rhs);
    } else {
        return e;
    }
}

//Expr *parse_str(std::string s) {// wrapper for testing.
//    // transfer the string into a input the std::istream &in:
//    std::istringstream in(s); // read the stream as a std::istringstream in(s);
//    return Expr::parse_expr(in);
//}

Expr *Expr::parse_str(std::string s) {
    std::istringstream in(s); // read the stream as a std::istringstream in(s);
    return Expr::parse_expr(in);
}

Expr *Expr::parse_addend(std::istream &in) {
    Expr *e = parse_multicand(in);
    skip_whitespace(in);
    int c = in.peek();
    if (c == '*') {
        consume(in, '*');
        Expr* rhs = parse_addend(in);
        return new MultExpr(e, rhs);
    } else {
        return e;
    }
}

Expr *Expr::parse_var(std::istream &in) {

    string var;

    while (1) {
        int c = in.peek();
        if (isalpha(c)) {
//            consume(in, c);
            var += (char)in.get();
        } else
            break;
    }
    return new VarExpr(var);
}

Expr *Expr::parse_multicand(std::istream &in) {
    Expr *expr = parse_inner(in);
    while (in.peek() == '(') {
        consume(in, '(');
        Expr *actual_arg = parse_expr(in);
        consume(in, ')');
        expr = new CallExpr(expr, actual_arg);
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

Expr *Expr::parse_inner(std::istream &in) {
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
            cout << "In the FUN" << endl;
            return parse_fun(in);
        } else {
            throw runtime_error("Invalid input after underscore _ ");
        }
    } else if (isalpha(c)) {
      return parse_var(in);
    } else if (c == '('){
        consume(in, '(');
        c = in.peek();
        Expr *e = parse_comparg(in);
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

static string* check_var(std::istream &in, string &var) {
    if (in.peek() >= 65 && in.peek() <= 90 || in.peek() >= 97 && in.peek() <= 122 ) {
        var = (char)in.get();
        return &var;
    } else {
        throw std::runtime_error("not a variable");
    }
}


Expr *Expr::parse_if(std::istream &in) {

//    consume(in, 'i');
//    consume(in, 'f');

    skip_whitespace(in);

    Expr* boolExpr = parse_expr(in);


    skip_whitespace(in);


    consume(in, '_');
    consume(in, 't');
    consume(in, 'h');
    consume(in, 'e');
    consume(in, 'n');

//    for (char i : "_then") {
//        consume (in, i);
//    }

    Expr* thenExpr = parse_expr(in);

    skip_whitespace(in);

    consume(in, '_');
    consume(in, 'e');
    consume(in, 'l');
    consume(in, 's');
    consume(in, 'e');

    skip_whitespace(in);

    Expr* elseExpr = parse_expr(in);

    return new IfExpr(boolExpr, thenExpr, elseExpr);
}

Expr *Expr::parse_true(std::istream &in) {

//    consume(in, 't');
//    consume(in, 'r');
//    consume(in, 'u');
//    consume(in, 'e');

    return new BoolExpr(true);
}


Expr *Expr::parse_false(std::istream &in) {

//    consume(in, 'f');
//    consume(in, 'a');
//    consume(in, 'l');
//    consume(in, 's');
//    consume(in, 'e');

    return new BoolExpr(false);
}


Expr *Expr::parse_let(std::istream &in) {

//    consume(in, 'l');
//    consume(in, 'e');
//    consume(in, 't');
    skip_whitespace(in);

    string temp;
    string* var = check_var(in, temp);

    skip_whitespace(in);
    consume(in, '=');
    skip_whitespace(in);

    Expr* expr1 = parse_comparg(in);


    skip_whitespace(in);
    consume(in, '_');
    consume(in, 'i');
    consume(in, 'n');
    skip_whitespace(in);

    Expr* expr2 = parse_comparg(in);



    return new LetExpr(var[0], expr1, expr2);
}

Expr *Expr::parse_num(std::istream &in) {
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
            n = n*10 + (c - '0');
        } else
            break;
    }
    if (negative)
        n = -n;
    return new NumExpr(n);
}

Expr* Expr::parse_fun(std::istream &in) {
    skip_whitespace(in);
    AddExpr::consume(in, '(');
    skip_whitespace(in);
    Expr *formal_arg = AddExpr::parse_var(in);
    skip_whitespace(in);
    AddExpr::consume(in, ')');
    skip_whitespace(in);
    Expr *body = AddExpr::parse_comparg(in);

    std::ostream out(nullptr);
    std::stringbuf str;
    out.rdbuf(&str);

    return new FunExpr(formal_arg->to_string(out), body);
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

std::string Expr::to_string(std::ostream &out) {
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

bool NumExpr::equals(Expr *e) {
    auto *t = dynamic_cast<NumExpr*>(e);
    if (t == nullptr) {
        return false;
    } else {
        return (this->rep == t->rep);
    }
}

 Val* NumExpr::interp() {
    return new NumVal(this->rep);
}

bool NumExpr::has_variable() {
    return false;
}

Expr* NumExpr::subst(std::string s, Expr *e) {
    return this;
}

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

bool BoolExpr::equals(Expr *e) {
    auto *t = dynamic_cast<BoolExpr*>(e);
    if (t == nullptr) {
        return false;
    } else {
        return (this->rep == t->rep);
    }
}

Val* BoolExpr::interp() {
    return new BoolVal(this->rep);
}

bool BoolExpr::has_variable() {
    return false;
}

Expr* BoolExpr::subst(std::string s, Expr *e) {
    return this;
}

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

EqExpr::EqExpr(Expr *lhs, Expr *rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}

bool EqExpr::equals(Expr *e) {
    auto *t = dynamic_cast<EqExpr*>(e);
    if (t == nullptr) {
        return false;
    } else {
        return (this->lhs->equals(t->lhs) && this->rhs->equals(t->rhs));
    }
}

Val* EqExpr::interp() {

    if (this->lhs->interp()->equals(this->rhs->interp())) {

        return new BoolVal(true);
    } else {
        return new BoolVal(false);
    }
}

bool EqExpr::has_variable() {
    return (this->lhs->has_variable() || this->rhs->has_variable());
}

Expr* EqExpr::subst(std::string s, Expr *e) {
    return new EqExpr(this->lhs->subst(s, e), this->rhs->subst(s, e));
}

void EqExpr::print(std::ostream &out) {
    out << "(";   //Todo: Do these have parenthesis?
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

AddExpr::AddExpr(Expr *lhs, Expr *rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}

bool AddExpr::equals(Expr *e) {
    AddExpr *t = dynamic_cast<AddExpr*>(e);
    if (t == nullptr) {
        return false;
    } else {
        return (this->lhs->equals(t->lhs) && this->rhs->equals(t->rhs));
    }
}

Val* AddExpr::interp() {
    return this->lhs->interp()->add_to(this->rhs->interp());
}

bool AddExpr::has_variable() {
    return (this->lhs->has_variable() || this->rhs->has_variable());
}

Expr* AddExpr::subst(std::string s, Expr *e) {
    return new AddExpr(this->lhs->subst(s, e), this->rhs->subst(s, e));
}

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

MultExpr::MultExpr(Expr *lhs, Expr *rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}

bool MultExpr::equals(Expr *e) {
    auto *t = dynamic_cast<MultExpr*>(e);
    if (t == nullptr) {
        return false;
    } else {
        return (this->lhs->equals(t->lhs) && this->rhs->equals(t->rhs));
    }
}

Val* MultExpr::interp() {
    return this->lhs->interp()->mult_by(this->rhs->interp());
}

bool MultExpr::has_variable() {
    return (this->lhs->has_variable() || this->rhs->has_variable());
}

Expr* MultExpr::subst(std::string s, Expr *e) {
    return new MultExpr(this->lhs->subst(s, e), this->rhs->subst(s, e));
}

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
    this->name = name;
}

bool VarExpr::equals(Expr *e) {
    auto *t = dynamic_cast<VarExpr*>(e);
    if (t == nullptr) {
        return false;
    } else {
        return (this->name == t->name);
    }
}

Val* VarExpr::interp() {
    throw std::runtime_error("There is no value for this expression");
}

bool VarExpr::has_variable() {
    return true;
}

Expr* VarExpr::subst(std::string s, Expr *e) {
    if (this->name == s) {
        return e;
    } else {
        return this;
    }
}

void VarExpr::print(std::ostream &out) {
    out << this->name;
}

//void VarExpr::pretty_print_at(std::ostream &out, enum printStatus status){
//    out << this->name;
//}

LetExpr::LetExpr(std::string lhs, Expr* rhs, Expr* body) {
    this->lhs = lhs;
    this->rhs = rhs;
    this->body = body;
}

bool LetExpr::equals(Expr *e) {
    auto *t = dynamic_cast<LetExpr*>(e);
    if (t == nullptr) {
        return false;
    } else {
        return (this->lhs == (t->lhs) && this->rhs->equals(t->rhs) && this->body->equals(t->body));
    }
}

Val* LetExpr::interp() {
//    Var *t = dynamic_cast<Var*>(this->lhs);  // Ensures first argument is of type Var
//    if (t == nullptr) {
//        throw std::runtime_error("First argument must be of type Var");
//    }
//    Var* lhs_var = dynamic_cast<Var*>(this->lhs); // Casts lhs to Var

    Val* rhs_val = this->rhs->interp(); // evaluates rhs
//    NumExpr* rhs_interp = new NumExpr(n); // Sets rhs as a Num

    // Substitutes any variable in the body with the rhs evaluation (interp)
    return this->body->subst(lhs, rhs_val->to_expr())->interp();
}

bool LetExpr::has_variable() {
    return this->rhs->has_variable() || this->body->has_variable();
}

Expr* LetExpr::subst(std::string s, Expr *e) {

    if (s != this->lhs) {
        return new LetExpr(this->lhs, this->rhs->subst(s, e), this->body->subst(s, e));
    } else {
        return new LetExpr(this->lhs, this->rhs->subst(s, e), this->body);
    }
}

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

//    auto *r = dynamic_cast<LetExpr*>(this->rhs);
//    auto *b = dynamic_cast<LetExpr*>(this->body);
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

IfExpr::IfExpr(Expr* condition, Expr* statement1, Expr* statement2) {
//    auto *t = dynamic_cast<BoolExpr*>(condition);
//    if (t == nullptr) {
//        throw runtime_error("First parameter of If Expr must be BoolExpr");
//    } else {
//
//    }
    this->condition = condition;
    this->statement1 = statement1;
    this->statement2 = statement2;
}

bool IfExpr::equals(Expr *e) {
    auto *t = dynamic_cast<IfExpr*>(e);
    if (t == nullptr) {
        return false;
    } else {
        return (this->condition->equals(t->condition) && this->statement1->equals(t->statement1) && this->statement2->equals(t->statement2));
    }
}

Val* IfExpr::interp() {

    if (condition->interp()->equals(new BoolVal(true))) {
        return statement1->interp();
    } else {
        return statement2->interp();
    }
}

bool IfExpr::has_variable() {
    return this->condition->has_variable() || this->statement1->has_variable() || this->statement2->has_variable();
}

Expr* IfExpr::subst(std::string s, Expr *e) {

    return new IfExpr(this->condition->subst(s, e), this->statement1->subst(s, e), this->statement2->subst(s,e));
}

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



FunExpr::FunExpr(std::string formal_arg, Expr* body) {
    this->formal_arg = formal_arg;
    this->body = body;
}

bool FunExpr::equals(Expr* e) {
    auto *t = dynamic_cast<FunExpr*>(e);
    if (t == nullptr) {
        return false;
    } else {
        return (formal_arg == t->formal_arg && this->body->equals(t->body));
    }
}

Val* FunExpr::interp() {
    return new FunVal(formal_arg, body);

}

bool FunExpr::has_variable() {
    throw std::runtime_error("not implemented");
}



Expr* FunExpr::subst(std::string s, Expr *e) {

    if(s.compare(formal_arg) == 0) {
        return new FunExpr(this->formal_arg, this->body);
    }
    return new FunExpr(this->formal_arg, this->body);
}

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

CallExpr::CallExpr(Expr* to_be_called, Expr *actual_arg) {
    this -> to_be_called = to_be_called;
    this->actual_arg = actual_arg;
}

bool CallExpr::equals(Expr *e) {
    CallExpr *c = dynamic_cast<CallExpr *>(e);
    if (c == NULL) {
        return false;
    } else {
        return (this->to_be_called->equals(c->to_be_called) && this->actual_arg->equals(c->actual_arg));
    }
}


Val *CallExpr::interp() {
    return to_be_called->interp()->call(actual_arg->interp());
}

bool CallExpr::has_variable() {
    throw std::runtime_error("invalied call");
}

Expr* CallExpr::subst(std::string sub, Expr* expr) {
    Expr *new_to_be_called = to_be_called ->subst(sub, expr);
    Expr *new_actual_arg = actual_arg->subst(sub, expr);
    return new CallExpr(new_to_be_called, new_actual_arg);
}

void CallExpr::print(std::ostream &out) {

    this->to_be_called->print(out);
    out << "(";
    this -> actual_arg -> print(out);
    out << ")";
}



