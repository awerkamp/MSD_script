//
// Created by Awerkamp on 1/29/21.
//
#include <sstream>
#include "catch.hpp"
#include "expr.h"

TEST_CASE("equals") {

    Expr* num1 = new Num(1);
    Expr* num2 = new Num(2);
    Expr* num3 = new Num(3);
    Expr* num4 = new Num(4);
    Expr* num5 = new Num(5);
    Expr* var1 = new Var("var1");
    Expr* var2 = new Var("var2");

    CHECK((new Add(num1, num2))->equals(new Add(num1, num2)) == true);
    CHECK((new Add(num1, num5))->equals(new Add(num3, num3)) == false);
    CHECK((num1)->equals(num1) == true);
    CHECK((num1)->equals(num2) == false);
    CHECK((var1)->equals(var1) == true);
    CHECK((var1)->equals(var2) == false);
    CHECK((new Mult(num1, num2))->equals(new Mult(num1, num2)) == true);
    CHECK((new Mult(num1, num2))->equals(new Mult(num1, num3)) == false);
}

TEST_CASE("Interp") {

    Expr* num1 = new Num(1);
    Expr* num2 = new Num(2);
    Expr* num3 = new Num(3);

    // Add
    CHECK((((new Add(num1, num2))->interp())==(num3->interp())) == true); // 3 == 3
    CHECK((((new Add(num1, num2))->interp())==(num1->interp())) == false); // 3 == 1

    // Variables
    CHECK_THROWS_WITH( (new Var("x"))->interp(), "There is no value for this expression" );

    // Multiply
    CHECK((((new Mult(num1, num2))->interp())==(num2->interp())) == true); // 2 == 2
    CHECK((((new Mult(num1, num2))->interp())==(num3->interp())) == false); // 2 == 3

    //Num
    CHECK(((num2->interp())==(num2->interp())) == true); // 2 == 2
    CHECK(((num2->interp())==(num3->interp())) == false); // 2 == 3
}

TEST_CASE("has_variable") {

    Expr* num1 = new Num(1);
    Expr* num2 = new Num(2);
    Expr* num3 = new Num(3);
    Expr* var1 = new Var("var1");
    Expr* var2 = new Var("var2");

    // Add
    CHECK(((new Add(num1, var1))->has_variable()) == true); // var1
    CHECK(((new Add(num1, num2))->has_variable()) == false); // no var

    // Mult
    CHECK(((new Mult(num1, var1))->has_variable()) == true); // var1
    CHECK(((new Mult(num1, num2))->has_variable()) == false); // no var

    // Complex
    CHECK(((new Mult(new Add(num1, var2), num2))->has_variable()) == true); // var2
    CHECK(((new Mult(new Add(num1, num2), num2))->has_variable()) == false); // no var

    // Var
    CHECK(var2->has_variable() == true); // var2

    // Num
    CHECK(num3->has_variable() == false); // no var
}

TEST_CASE("subst") {

    // Var
    CHECK(((new Var("x"))->subst("x", new Var("y"))->equals(new Var("y"))) == true);
    CHECK(((new Var("x"))->subst("x", new Var("y"))->equals(new Var("x"))) == false);

    // Num
    CHECK(((new Num(1))->subst("x", new Var("y"))->equals(new Num(1))) == true);

    // Add
    CHECK( (new Add(new Var("x"), new Num(7)))
    ->subst("x", new Var("y"))
    ->equals(new Add(new Var("y"), new Num(7))) );

    // Mult
    CHECK( (new Mult(new Var("x"), new Num(7)))
    ->subst("x", new Var("y"))
    ->equals(new Mult(new Var("y"), new Num(7))) );
}

TEST_CASE("print") {

    Expr* num1 = new Num(1);
    Expr* num2 = new Num(2);
//
    std::ostream out(nullptr);
    std::stringbuf str;
    out.rdbuf(&str);

    CHECK(((new Mult((new Add(num1, num2)), new Num(1)))->to_string(out)) == "((1+2)*1)");
    CHECK(((new Mult((new Mult(num1, num2)), new Num(1)))->to_string(out)) == "((1*2)*1)");
    CHECK(((new Mult((new Num(1)), new Mult(num1, num2)))->to_string(out)) == "(1*(1*2))");
    CHECK(((new Add((new Mult(num1, num2)), new Num(1)))->to_string(out)) == "((1*2)+1)");
    CHECK(((new Add((new Add(num1, num2)), new Mult(num1, num2)))->to_string(out)) == "((1+2)+(1*2))");
    CHECK(((new Add((new Add(num1, num2)), new Add(num1, num2)))->to_string(out)) == "((1+2)+(1+2))");
    CHECK(((new Add((new Add(num1, num2)), new Add(num1, num2)))->to_string(out)) == "((1+2)+(1+2))");
    CHECK(((new Mult((new Mult(num1, num2)), new Mult(num1, num2)))->to_string(out)) == "((1*2)*(1*2))");
    CHECK(((new Mult((new Add(num1, num2)), new Add(num1, num2)))->to_string(out)) == "((1+2)*(1+2))");
    CHECK(((new Add((new Mult(num1, num2)), new Mult(num1, num2)))->to_string(out)) == "((1*2)+(1*2))");
}


TEST_CASE("pretty_print") {

    Expr* num1 = new Num(1);
    Expr* num2 = new Num(2);
//
    std::ostream out(nullptr);
    std::stringbuf str;
    out.rdbuf(&str);

    CHECK(((new Mult((new Add(num1, num2)), new Num(1)))->to_string_pretty(out)) == "(1 + 2) * 1");
    CHECK(((new Mult((new Mult(num1, num2)), new Num(1)))->to_string_pretty(out)) == "(1 * 2) * 1");
    CHECK(((new Mult((new Num(1)), new Mult(num1, num2)))->to_string_pretty(out)) == "1 * 1 * 2");
    CHECK(((new Add((new Mult(num1, num2)), new Num(1)))->to_string_pretty(out)) == "1 * 2 + 1");
    CHECK(((new Add((new Add(num1, num2)), new Mult(num1, num2)))->to_string_pretty(out)) == "1 + 2 + 1 * 2");
    CHECK(((new Add((new Add(num1, num2)), new Add(num1, num2)))->to_string_pretty(out)) == "1 + 2 + 1 + 2");
    CHECK(((new Add((new Add(num1, num2)), new Add(num1, num2)))->to_string_pretty(out)) == "1 + 2 + 1 + 2");
    CHECK(((new Mult((new Mult(num1, num2)), new Mult(num1, num2)))->to_string_pretty(out)) == "(1 * 2) * 1 * 2");
    CHECK(((new Mult((new Add(num1, num2)), new Add(num1, num2)))->to_string_pretty(out)) == "(1 + 2) * (1 + 2)");
    CHECK(((new Add((new Mult(num1, num2)), new Mult(num1, num2)))->to_string_pretty(out)) == "1 * 2 + 1 * 2");
}