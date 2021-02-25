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
        e = parse_addend(in);
        skip_whitespace(in);

        int c = in.peek();
        if (c == '+') {
            consume(in, '+');
            Expr* rhs = parse_expr(in);
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
    skip_whitespace(in);
    int c = in.peek();
    if ((c == '-') || isdigit(c)) {
        return parse_num(in);
    } else if (c == '_') {
        return parse_let(in);
    } else if (isalpha(c)) {
      return parse_var(in);
    } else if (c == '('){
        consume(in, '(');
        Expr *e = parse_expr(in);
        skip_whitespace(in);
        c = in.get();
        if (c != ')') {
            throw std::runtime_error("missing close parenthesis");
        }
        return e;
    }

    else {
//        consume(in, c);
        cout << "This is the error" << (char)c << endl;
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

Expr *Expr::parse_let(std::istream &in) {
    consume(in, '_');
    consume(in, 'l');
    consume(in, 'e');
    consume(in, 't');
    skip_whitespace(in);

    string temp;
    string* var = check_var(in, temp);

    skip_whitespace(in);
    consume(in, '=');
    skip_whitespace(in);

    Expr* expr1 = parse_expr(in);


    skip_whitespace(in);
    consume(in, '_');
    consume(in, 'i');
    consume(in, 'n');
    skip_whitespace(in);

    Expr* expr2 = parse_expr(in);


    cout << var[0] << endl;


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
        cout << "this is what trying to consume" << (char)c << endl;
        throw std::runtime_error("consume mismatch");
    }
}

std::string Expr::to_string(std::ostream &out) {
    this->print(out);
    std::stringstream ss;
    ss << out.rdbuf();
    return ss.str();
}

std::string Expr::to_string_pretty() {
    std::ostream out(nullptr);
    std::stringbuf str;
    out.rdbuf(&str);
    this->pretty_print(out);
    std::stringstream ss;
    ss << out.rdbuf();
    return ss.str();
}

void Expr::pretty_print(std::ostream &out) {
    pretty_print_at(out, print_group_none);
}




NumExpr::NumExpr(int val) {
//    val->Expr
    this->rep = val;
}

bool NumExpr::equals(Expr *e) {
    NumExpr *t = dynamic_cast<NumExpr*>(e);
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

void NumExpr::pretty_print_at(std::ostream &out, enum printStatus status) {
    out << this->rep;
}

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

void AddExpr::pretty_print_at(std::ostream &out, enum printStatus status) {

    if(status == print_group_add || status == print_group_add_or_mult) {
        out << "(";
        this->lhs->pretty_print_at(out, print_group_add);
        out << " + ";
        this->rhs->pretty_print_at(out, print_group_none);
        out << ")";
    } else {
        this->lhs->pretty_print_at(out, print_group_add);
        out << " + ";
        this->rhs->pretty_print_at(out, print_group_none);
    }
}

MultExpr::MultExpr(Expr *lhs, Expr *rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}

bool MultExpr::equals(Expr *e) {
    MultExpr *t = dynamic_cast<MultExpr*>(e);
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


void MultExpr::pretty_print_at(std::ostream &out, enum printStatus status){

    if(status == print_group_add_or_mult) {
        out << "(";
        this->lhs->pretty_print_at(out, print_group_add_or_mult);
        out << " * ";
        this->rhs->pretty_print_at(out, print_group_add);
        out << ")";
    } else {
        this->lhs->pretty_print_at(out, print_group_add_or_mult);
        out << " * ";
        this->rhs->pretty_print_at(out, print_group_add);
    }
}


LetExpr::LetExpr(std::string lhs, Expr* rhs, Expr* body) {
    this->lhs = lhs;
    this->rhs = rhs;
    this->body = body;
}

bool LetExpr::equals(Expr *e) {
    LetExpr *t = dynamic_cast<LetExpr*>(e);
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

void LetExpr::pretty_print_at(std::ostream &out, enum printStatus status){  // todo Make Indented and on new line when let within let

    auto *r = dynamic_cast<LetExpr*>(this->rhs);
    auto *b = dynamic_cast<LetExpr*>(this->body);

    if (status == print_group_let) {
        out << "(_let ";
        out << lhs;
        out << " = ";
        if (r != nullptr) {
            this->rhs->pretty_print_at(out, print_group_let);
        } else {
            this->rhs->pretty_print_at(out, print_group_none);
        }
        out << " in ";
        if (r != nullptr) {
            this->body->pretty_print_at(out, print_group_let);
        } else {
            this->body->pretty_print_at(out, print_group_none);
        }
        out << ")";
    } else {
        out << "_let ";
        out << lhs;
        out << " = ";
        if (r != nullptr) {
            this->rhs->pretty_print_at(out, print_group_let);
        } else {
            this->rhs->pretty_print_at(out, print_group_none);
        }
        out << " in ";
        if (body != nullptr) {
            this->rhs->pretty_print_at(out, print_group_let);
        } else {
            this->body->pretty_print_at(out, print_group_none);
        }
    }
}

VarExpr::VarExpr(std::string name) {
    this->name = name;
}

bool VarExpr::equals(Expr *e) {
    VarExpr *t = dynamic_cast<VarExpr*>(e);
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

void VarExpr::pretty_print_at(std::ostream &out, enum printStatus status){
    out << this->name;
}

