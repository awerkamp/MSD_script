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

    // add
    CHECK((new Add(num1, num2))->equals(new Add(num1, num2)) == true);
    CHECK((new Add(num1, num5))->equals(new Add(num3, num3)) == false);
    CHECK((new Add(num1, num5))->equals(new Mult(num3, num3)) == false);

    // num
    CHECK((num1)->equals(num1) == true);
    CHECK((num1)->equals(num2) == false);
    CHECK((num1)->equals(var1) == false);

    // var
    CHECK((var1)->equals(var1) == true);
    CHECK((var1)->equals(var2) == false);
    CHECK((var1)->equals(num2) == false);

    // mult
    CHECK((new Mult(num1, num2))->equals(new Mult(num1, num2)) == true);
    CHECK((new Mult(num1, num2))->equals(new Mult(num1, num3)) == false);
    CHECK((new Mult(num1, num2))->equals(new Add(num1, num3)) == false);

    //_let
    CHECK((new _let("x", num2, new Add(num2, var1)))->equals(new _let("x", num2, new Add(num2, var1))) == true);
    CHECK((new _let("x", num2, new Add(num2, var1)))->equals(new Mult(num2, num2)) == false);
    CHECK((new _let("y", num2, new Add(num2, var1)))->equals(new _let("x", num2, new Add(num2, var1))) == false);
    CHECK((new _let("x", num2, new Add(num2, var1)))->equals(new _let("x", num3, new Add(num2, var1))) == false);
    CHECK((new _let("x", num2, new Add(num2, var1)))->equals(new _let("x", num3, new Add(var1, num2))) == false);
    CHECK((new _let("x", num2, new Add(num2, var1)))->equals(new _let("x", num3, new Add(var1, num2))) == false);
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

    // Num
    CHECK(((num2->interp())==(num2->interp())) == true); // 2 == 2
    CHECK(((num2->interp())==(num3->interp())) == false); // 2 == 3

    // _let
    CHECK((((_let("x", num1, new Var("x"))).interp() == 1) == true));
    CHECK((((_let("x", num2, new Var("x"))).interp() == 1) == false));
    CHECK((((_let("x", new Add(num1, num2), new Var("x"))).interp() == 3) == true));
    CHECK((((_let("x", new Add(num1, num2), new Add(new Var("x"), num2))).interp() == 5) == true));
    CHECK((((_let("x", new Add(num1, num2), new Add(new Var("x"), new Var("x")))).interp() == 6) == true));
    CHECK_THROWS_AS((((_let("x", new Add(num1, num2), new Add(new Var("y"), new Var("x")))).interp() == 6) == true), exception);
    CHECK_THROWS_AS((((_let("x", new Add(new Var("x"), num2), new Add(new Var("x"), new Var("x")))).interp() == 6) == true), exception);
    CHECK((((_let("x", new Add(num1, num2), new Add(new Num(0), new Num(5)))).interp() == 5) == true));  // todo: verify this is correct
    CHECK(((_let("x", num1, new Var("x"))).interp() == 1) == true);
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
    CHECK(num1->has_variable() == false); // var2


    // Num
    CHECK(num3->has_variable() == false); // no var

    // _let
    CHECK(((new _let("x", num2, var1))->has_variable()) == true);
    CHECK(((new _let("x", num2, num3))->has_variable()) == false);
    CHECK(((new _let("x", var1, num2))->has_variable()) == true);
}

TEST_CASE("subst") {

    // Var
    CHECK(((new Var("x"))->subst("x", new Var("y"))->equals(new Var("y"))) == true);
    CHECK(((new Var("x"))->subst("x", new Var("y"))->equals(new Var("x"))) == false);
    CHECK(((new Var("x"))->subst("y", new Var("y"))->equals(new Var("y"))) == false);

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

    // _let
    CHECK( (new _let("x", new Num(7), new Var("y")))
                   ->subst("y", new Var("x"))
                   ->equals(new _let("x", new Num(7), new Var("x"))) );

    CHECK( (new _let("x", new Num(7), new Var("x")))
                   ->subst("y", new Var("x"))
                   ->equals(new _let("x", new Num(7), new Var("x"))) );
    CHECK( (new _let("t", new Num(7), new Var("x")))
                   ->subst("x", new Var("z"))
                   ->equals(new _let("t", new Num(7), new Var("x"))) == false );

    Expr *let1 = new _let("x",
                          new Num(1),
                          new Add(new Var("x"), new Num(2)));
    CHECK( let1->subst("x", new Add(new Var("y"), new Num(3)))
                   ->equals(let1) );

    Expr *let2 = (new _let("x",
                          new Var("x"),
                          new Add(new Var("x"), new Num(2))));
    CHECK( let2->subst("x", new Add(new Var("y"), new Num(3)))
                   ->equals(new _let("x",
                                    new Add(new Var("y"), new Num(3)),
                                    new Add(new Var("x"), new Num(2)))) );
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
    CHECK((new Var("x"))->to_string(out) == "x");

    // _let
    CHECK(((new _let("x", new Add(num1, num2), num1))->to_string(out)) == "(_let x=(1+2) _in 1)");
    CHECK(((new _let("x", new Add(num1 , num2), new Add(new Var("x") , num2)))->to_string(out)) == "(_let x=(1+2) _in (x+2))");
    CHECK((((new _let("x", new Add(num1 , num2), new Add(new Var("x") , num2)))->to_string(out)) == "(_let x=(1+2) _in x+2)") == false);
}

TEST_CASE("pretty_print") {

    Expr* num1 = new Num(1);
    Expr* num2 = new Num(2);

    std::ostream out(nullptr);
    std::stringbuf str;
    out.rdbuf(&str);

    CHECK(((new Mult((new Add(num1, num2)), new Num(1)))->to_string_pretty()) == "(1 + 2) * 1");
    CHECK(((new Mult((new Mult(num1, num2)), new Num(1)))->to_string_pretty()) == "(1 * 2) * 1");
    CHECK(((new Mult((new Num(1)), new Mult(num1, num2)))->to_string_pretty()) == "1 * 1 * 2");
    CHECK(((new Add((new Mult(num1, num2)), new Num(1)))->to_string_pretty()) == "1 * 2 + 1");
    CHECK(((new Add((new Add(num1, num2)), new Mult(num1, num2)))->to_string_pretty()) == "(1 + 2) + 1 * 2");
    CHECK(((new Add((new Add(num1, num2)), new Add(num1, num2)))->to_string_pretty()) == "(1 + 2) + 1 + 2");
    CHECK(((new Mult((new Mult(num1, num2)), new Mult(num1, num2)))->to_string_pretty()) == "(1 * 2) * 1 * 2");
    CHECK(((new Mult((new Add(num1, num2)), new Add(num1, num2)))->to_string_pretty()) == "(1 + 2) * (1 + 2)");
    CHECK(((new Add((new Mult(num1, num2)), new Mult(num1, num2)))->to_string_pretty()) == "1 * 2 + 1 * 2");
    CHECK(((new Add((new Mult(num1, num2)), new Mult(num1, num2)))->to_string_pretty()) == "1 * 2 + 1 * 2");
    CHECK(((new Add((new Add(num1, num2)), new Add(num1, num2)))->to_string_pretty()) == "(1 + 2) + 1 + 2");
    CHECK((new Var("x"))->to_string_pretty() == "x");
}

std::string to_string(std::ostream &out) {
    std::stringstream ss;
    ss << out.rdbuf();
    return ss.str();
}

TEST_CASE("pretty_print_at") {

    Expr* num1 = new Num(1);
    Expr* num2 = new Num(2);

    std::ostream out(nullptr);
    std::stringbuf str;
    out.rdbuf(&str);

    num1->pretty_print_at(out, Expr::print_group_none);
    CHECK(to_string(out) == "1");

    out.clear();
    (new Add((new Add(num1, num2)), new Add(num1, num2)))->pretty_print_at(out, Expr::print_group_none);
    CHECK(to_string(out) == "(1 + 2) + 1 + 2");
}

TEST_CASE("parse_test") {
    CHECK( Expr::parse_str("10")->equals(new Num(10))); // testing num
//    CHECK( Expr::parse_str("feb")->equals(new Var("feb"))); // testing variable
    Expr *ten_plus_one = new Add(new Num(10), new Num(1));
    Expr *neg_ten_plus_one = new Add(new Num(-10), new Num(1));
    CHECK( Expr::parse_str("10+1")->equals(ten_plus_one)); // testing expression Add
    Expr *five_mult_two = new Mult(new Num(5), new Num(2));
    CHECK( Expr::parse_str("5*2")->equals(new Mult(new Num(5), new Num(2)))); // testing expression Mult
    CHECK( Expr::parse_str(" 2 ")->equals(new Num(2))); // testing whitespace
    CHECK( Expr::parse_str("(10+1)")->equals(ten_plus_one)); // testing expression Add
    CHECK_THROWS_AS( Expr::parse_str("(10+1")->equals(ten_plus_one), exception); // testing expression Add
//    CHECK_THROWS_AS( Expr::parse_str("(d+1)")->equals(ten_plus_one), exception); // testing expression Add
    CHECK( Expr::parse_str("(-10+1)")->equals(neg_ten_plus_one)); // testing expression Add



    // testing for parse_let (add other types later)
    //_let x = 1 _ in x + 2
    // CHECK( parse_str("_let x = 1 _ in x + 2 ")->equals( (new _let(new Variable("x"),
    //new Num(1), new Add(new Variable("x"), new Num(2))));)); // testing whitespace
}