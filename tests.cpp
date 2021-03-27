//
// Created by Awerkamp on 1/29/21.
//
#include <sstream>
#include "catch.hpp"
#include "expr.h"
#include "val.h"

TEST_CASE("equals") {

    Expr* num1 = new NumExpr(1);
    Expr* num2 = new NumExpr(2);
    Expr* num3 = new NumExpr(3);
    Expr* num4 = new NumExpr(4);
    Expr* num5 = new NumExpr(5);
    Expr* var1 = new VarExpr("var1");
    Expr* var2 = new VarExpr("var2");

    // add Expr
    CHECK((new AddExpr(num1, num2))->equals(new AddExpr(num1, num2)) == true);
    CHECK((new AddExpr(num1, num5))->equals(new AddExpr(num3, num3)) == false);
    CHECK((new AddExpr(num1, num5))->equals(new MultExpr(num3, num3)) == false);

    // add Val
    CHECK_THROWS_AS(((new BoolVal(true))->add_to(new BoolVal(true))), exception);

    // num
    CHECK((num1)->equals(num1) == true);
    CHECK((num1)->equals(num2) == false);
    CHECK((num1)->equals(var1) == false);

    // var
    CHECK((var1)->equals(var1) == true);
    CHECK((var1)->equals(var2) == false);
    CHECK((var1)->equals(num2) == false);


    // mult expr
    CHECK((new MultExpr(num1, num2))->equals(new MultExpr(num1, num2)) == true);
    CHECK((new MultExpr(num1, num2))->equals(new MultExpr(num1, num3)) == false);
    CHECK((new MultExpr(num1, num2))->equals(new AddExpr(num1, num3)) == false);

    // mult val
    CHECK_THROWS_AS(((new BoolVal(true))->mult_by(new BoolVal(true))), exception);

    //_let
    CHECK((new LetExpr("x", num2, new AddExpr(num2, var1)))->equals(new LetExpr("x", num2, new AddExpr(num2, var1))) == true);
    CHECK((new LetExpr("x", num2, new AddExpr(num2, var1)))->equals(new MultExpr(num2, num2)) == false);
    CHECK((new LetExpr("y", num2, new AddExpr(num2, var1)))->equals(new LetExpr("x", num2, new AddExpr(num2, var1))) == false);
    CHECK((new LetExpr("x", num2, new AddExpr(num2, var1)))->equals(new LetExpr("x", num3, new AddExpr(num2, var1))) == false);
    CHECK((new LetExpr("x", num2, new AddExpr(num2, var1)))->equals(new LetExpr("x", num3, new AddExpr(var1, num2))) == false);
    CHECK((new LetExpr("x", num2, new AddExpr(num2, var1)))->equals(new LetExpr("x", num3, new AddExpr(var1, num2))) == false);

    //BoolVal
    CHECK((new BoolVal(true))->equals(new BoolVal(true)) == true);
    CHECK((new BoolVal(true))->equals(new BoolVal(true)) == true);
    CHECK((new BoolVal(true))->equals(new BoolVal(false)) == false);
    CHECK((new NumVal(21))->equals(new BoolVal(false)) == false);
    CHECK((new BoolVal(true))->equals(new NumVal(21)) == false);


    //BoolExpr
    CHECK((new BoolExpr(true))->equals(new BoolExpr(true)) == true);
    CHECK((new BoolExpr(true))->equals(new BoolExpr(false)) == false);
    CHECK((new BoolExpr(true))->equals(new BoolExpr(false)) == false);
    auto* temp = new NumExpr(4);
    CHECK((new BoolExpr(true))->equals(temp) == false);

//    CHECK((new BoolExpr(true))->equals(new NumVal(21)) == false);


    // eqExp
    Expr* eqExp = new EqExpr(num1, num2);
    CHECK(eqExp->equals(new EqExpr(num1, num2)));
    CHECK(eqExp->equals(new EqExpr(num1, num3)) == false);

    // ifExp
    Expr* ifExp = new IfExpr(new BoolExpr(true), num1, num2);
    CHECK(ifExp->equals(new IfExpr(new BoolExpr(true), num1, num2)) == true);

    Expr* ifExp2 = new IfExpr(new BoolExpr(true), num1, num2);
    CHECK(eqExp->equals(new IfExpr(new BoolExpr(true), num1, num2)) == false);

    Expr* ifExp3 = new IfExpr(new BoolExpr(true), num1, num2);
    CHECK(ifExp3->equals(new IfExpr(new BoolExpr(true), num1, num3)) == false);

//    CHECK_THROWS_AS(new IfExpr(new NumExpr(1), new NumExpr(2), new NumExpr(3)), exception);
}

TEST_CASE("Interp") {

    Expr* num1 = new NumExpr(1);
    Expr* num2 = new NumExpr(2);
    Expr* num3 = new NumExpr(3);

    // Add
    CHECK((((new AddExpr(num1, num2))->interp())->equals(num3->interp())) == true); // 3 == 3
    CHECK((((new AddExpr(num1, num2))->interp())->equals(num1->interp())) == false); // 3 == 1

    // Variables
    CHECK_THROWS_WITH((new VarExpr("x"))->interp(), "There is no value for this expression" );

    // Multiply
    CHECK((((new MultExpr(num1, num2))->interp())->equals(num2->interp())) == true); // 2 == 2
    CHECK((((new MultExpr(num1, num2))->interp())->equals(num3->interp())) == false); // 2 == 3

    // Num
    CHECK(((num2->interp())->equals(num2->interp())) == true); // 2 == 2
    CHECK(((num2->interp())->equals(num3->interp())) == false); // 2 == 3

    // _let
    CHECK(((new LetExpr("x", num1, new VarExpr("x")))->interp()->equals(new NumVal(1))) == true);
    CHECK((((LetExpr("x", num2, new VarExpr("x"))).interp()->equals(new NumVal(1))) == false));
    CHECK((((LetExpr("x", new AddExpr(num1, num2), new VarExpr("x"))).interp()->equals(new NumVal(3))) == true));
    CHECK((((LetExpr("x", new AddExpr(num1, num2), new AddExpr(new VarExpr("x"), num2))).interp()->equals(new NumVal(5))) == true));
    CHECK((((LetExpr("x", new AddExpr(num1, num2), new AddExpr(new VarExpr("x"), new VarExpr("x")))).interp()->equals(new NumVal(6))) == true));
    CHECK_THROWS_AS((((LetExpr("x", new AddExpr(num1, num2), new AddExpr(new VarExpr("y"), new VarExpr("x")))).interp()->equals(new NumVal(6))) == true), exception);
    CHECK_THROWS_AS((((LetExpr("x", new AddExpr(new VarExpr("x"), num2), new AddExpr(new VarExpr("x"), new VarExpr("x")))).interp()->equals(new NumVal(6))) == true), exception);
    CHECK((((LetExpr("x", new AddExpr(num1, num2), new AddExpr(new NumExpr(0), new NumExpr(5)))).interp()->equals(new NumVal(5))) == true));  // todo: verify this is correct
    CHECK(((LetExpr("x", num1, new VarExpr("x"))).interp()->equals(new NumVal(1))) == true);

    // EqExpr
    Expr* eqExp = new EqExpr(num1, num2);
    CHECK(eqExp->interp()->equals(new BoolVal(false)) == true);
    Expr* eqExp2 = new EqExpr(num1, num1);
    CHECK(eqExp2->interp()->equals(new BoolVal(true)) == true);

    // ifExp
    Expr* ifExp = new IfExpr(new BoolExpr(true), num1, num2);
    CHECK(ifExp->interp()->equals(new NumVal(1)) == true);

    Expr* ifExp2 = new IfExpr(new BoolExpr(false), num1, num2);
    CHECK(ifExp2->interp()->equals(new NumVal(2)) == true);

    // BoolExpr
    Expr* boolExpr = new BoolExpr(true);
    CHECK(boolExpr->interp()->equals(new BoolVal(true)));
}


TEST_CASE("has_variable") {

    Expr* num1 = new NumExpr(1);
    Expr* num2 = new NumExpr(2);
    Expr* num3 = new NumExpr(3);
    Expr* var1 = new VarExpr("var1");
    Expr* var2 = new VarExpr("var2");

    // AddExpr
    CHECK(((new AddExpr(num1, var1))->has_variable()) == true); // var1
    CHECK(((new AddExpr(num1, num2))->has_variable()) == false); // no var

    // MultExpr
    CHECK(((new MultExpr(num1, var1))->has_variable()) == true); // var1
    CHECK(((new MultExpr(num1, num2))->has_variable()) == false); // no var

    // Complex
    CHECK(((new MultExpr(new AddExpr(num1, var2), num2))->has_variable()) == true); // var2
    CHECK(((new MultExpr(new AddExpr(num1, num2), num2))->has_variable()) == false); // no var

    // VarExpr
    CHECK(var2->has_variable() == true); // var2
    CHECK(num1->has_variable() == false); // var2


    // NumExpr
    CHECK(num3->has_variable() == false); // no var

    // _letExpr
    CHECK(((new LetExpr("x", num2, var1))->has_variable()) == true);
    CHECK(((new LetExpr("x", num2, num3))->has_variable()) == false);
    CHECK(((new LetExpr("x", var1, num2))->has_variable()) == true);

    // boolExpr
    auto* trueBoolExpr = new BoolExpr(true);
    CHECK(trueBoolExpr->has_variable() == false);

    // EqExpr
    Expr* eqExp = new EqExpr(num1, num2);
    CHECK(eqExp->has_variable() == false);
    Expr* eqExp2 = new EqExpr(var1, var2);
    CHECK(eqExp2->has_variable() == true);
//    Expr* eqExp2 = new EqExpr(num1, num1);
//    CHECK(eqExp2->interp()->equals(new BoolVal(true)) == true);

    // ifExp
    Expr* ifExp = new IfExpr(new BoolExpr(true), num1, num2);
    CHECK(ifExp->has_variable() == false);

    Expr* ifExp2 = new IfExpr(new BoolExpr(false), var1, num2);
    CHECK(ifExp2->has_variable() == true);
}

TEST_CASE("subst") {

    // Var
    CHECK(((new VarExpr("x"))->subst("x", new VarExpr("y"))->equals(new VarExpr("y"))) == true);
    CHECK(((new VarExpr("x"))->subst("x", new VarExpr("y"))->equals(new VarExpr("x"))) == false);
    CHECK(((new VarExpr("x"))->subst("y", new VarExpr("y"))->equals(new VarExpr("y"))) == false);

    // Num
    CHECK(((new NumExpr(1))->subst("x", new VarExpr("y"))->equals(new NumExpr(1))) == true);

    // Add
    CHECK( (new AddExpr(new VarExpr("x"), new NumExpr(7)))
    ->subst("x", new VarExpr("y"))
    ->equals(new AddExpr(new VarExpr("y"), new NumExpr(7))) );

    // Mult
    CHECK( (new MultExpr(new VarExpr("x"), new NumExpr(7)))
    ->subst("x", new VarExpr("y"))
    ->equals(new MultExpr(new VarExpr("y"), new NumExpr(7))) );

    // _let
    CHECK( (new LetExpr("x", new NumExpr(7), new VarExpr("y")))
                   ->subst("y", new VarExpr("x"))
                   ->equals(new LetExpr("x", new NumExpr(7), new VarExpr("x"))) );

    CHECK( (new LetExpr("x", new NumExpr(7), new VarExpr("x")))
                   ->subst("y", new VarExpr("x"))
                   ->equals(new LetExpr("x", new NumExpr(7), new VarExpr("x"))) );
    CHECK((new LetExpr("t", new NumExpr(7), new VarExpr("x")))
                   ->subst("x", new VarExpr("z"))
                   ->equals(new LetExpr("t", new NumExpr(7), new VarExpr("x"))) == false );

    Expr *let1 = new LetExpr("x",
                             new NumExpr(1),
                             new AddExpr(new VarExpr("x"), new NumExpr(2)));
    CHECK( let1->subst("x", new AddExpr(new VarExpr("y"), new NumExpr(3)))
                   ->equals(let1) );

    Expr *let2 = (new LetExpr("x",
                              new VarExpr("x"),
                              new AddExpr(new VarExpr("x"), new NumExpr(2))));
    CHECK( let2->subst("x", new AddExpr(new VarExpr("y"), new NumExpr(3)))
                   ->equals(new LetExpr("x",
                                        new AddExpr(new VarExpr("y"), new NumExpr(3)),
                                        new AddExpr(new VarExpr("x"), new NumExpr(2)))) );

    // BoolExpr
    auto* trueBoolExpr = new BoolExpr(true);
    CHECK((trueBoolExpr)
                  ->subst("x", new VarExpr("z"))
                  ->equals(trueBoolExpr) == true);

    // EqExpr
    Expr* eqExp = new EqExpr(new NumExpr(1), new NumExpr(2));
    CHECK(eqExp->subst("x", new VarExpr("z"))->equals(eqExp) == true);
    Expr* eqExp2 = new EqExpr(new VarExpr("x"), new NumExpr(2));
    CHECK(eqExp2->subst("x", new VarExpr("z"))->equals(eqExp) == false);

//    Expr* eqExp2 = new EqExpr(var1, var2);
//    CHECK(eqExp2->has_variable() == true);

    // ifExp
    Expr* ifExp = new IfExpr(new BoolExpr(true), new NumExpr(1), new NumExpr(2));
    CHECK(ifExp->subst("x", new VarExpr("z"))->equals(ifExp) == true);

    Expr* ifExp2 = new IfExpr(new BoolExpr(true), new VarExpr("x"), new NumExpr(2));
    CHECK(ifExp2->subst("x", new NumExpr(1))->equals(ifExp) == true);
}

TEST_CASE("print") {

    Expr* num1 = new NumExpr(1);
    Expr* num2 = new NumExpr(2);
//
    std::ostream out(nullptr);
    std::stringbuf str;
    out.rdbuf(&str);

    CHECK(((new MultExpr((new AddExpr(num1, num2)), new NumExpr(1)))->to_string(out)) == "((1+2)*1)");
    CHECK(((new MultExpr((new MultExpr(num1, num2)), new NumExpr(1)))->to_string(out)) == "((1*2)*1)");
    CHECK(((new MultExpr((new NumExpr(1)), new MultExpr(num1, num2)))->to_string(out)) == "(1*(1*2))");
    CHECK(((new AddExpr((new MultExpr(num1, num2)), new NumExpr(1)))->to_string(out)) == "((1*2)+1)");
    CHECK(((new AddExpr((new AddExpr(num1, num2)), new MultExpr(num1, num2)))->to_string(out)) == "((1+2)+(1*2))");
    CHECK(((new AddExpr((new AddExpr(num1, num2)), new AddExpr(num1, num2)))->to_string(out)) == "((1+2)+(1+2))");
    CHECK(((new AddExpr((new AddExpr(num1, num2)), new AddExpr(num1, num2)))->to_string(out)) == "((1+2)+(1+2))");
    CHECK(((new MultExpr((new MultExpr(num1, num2)), new MultExpr(num1, num2)))->to_string(out)) == "((1*2)*(1*2))");
    CHECK(((new MultExpr((new AddExpr(num1, num2)), new AddExpr(num1, num2)))->to_string(out)) == "((1+2)*(1+2))");
    CHECK(((new AddExpr((new MultExpr(num1, num2)), new MultExpr(num1, num2)))->to_string(out)) == "((1*2)+(1*2))");
    CHECK((new VarExpr("x"))->to_string(out) == "x");

    // _let
    CHECK(((new LetExpr("x", new AddExpr(num1, num2), num1))->to_string(out)) == "(_let x=(1+2) _in 1)");
    CHECK(((new LetExpr("x", new AddExpr(num1 , num2), new AddExpr(new VarExpr("x") , num2)))->to_string(out)) == "(_let x=(1+2) _in (x+2))");
    CHECK((((new LetExpr("x", new AddExpr(num1 , num2), new AddExpr(new VarExpr("x") , num2)))->to_string(out)) == "(_let x=(1+2) _in x+2)") == false);







    //BoolExpr
    auto* trueBoolExpr = new BoolExpr(true);
    CHECK(((trueBoolExpr)->to_string(out)) == "_true");
    auto* trueBoolExpr2 = new BoolExpr(false);
    CHECK(((trueBoolExpr2)->to_string(out)) == "_false");


    auto* ifExpr = new IfExpr(new BoolExpr(true), new NumExpr(3), new NumExpr(4));
    CHECK(((ifExpr)->to_string(out)) == "(_if _true _then 3 _else 4)");

    auto* eqExpr = new EqExpr(new NumExpr(3), new NumExpr(3));
    CHECK(((eqExpr)->to_string(out)) == "(3==3)");

}

TEST_CASE("fun_expr") {
//    _let f = _fun (x) x
//    _in  f(2)
    //equals
    FunExpr *fun1 = new FunExpr("x", new VarExpr("x"));
    FunExpr *fun2 = new FunExpr("y", new VarExpr(
            "y"));// creating new Num object to assign memory for that object Making a pointer to the Expr class
    //NumExpr* num = new NumExpr(2);
    CHECK(fun1->equals(fun2) == false);
    //equals
    FunExpr *fun3 = new FunExpr("x", new VarExpr(
            "x"));// creating new Num object to assign memory for that object Making a pointer to the Expr class
    //NumExpr* num = new NumExpr(2);
    CHECK(fun1->equals(fun3) == true);
    ////equals NULL
    FunExpr *fun4 = NULL;
    CHECK(fun1->equals(fun4) == false);
    //Has_Variable
    //Subst
    CHECK(fun1->subst("x", fun1)->equals(new FunExpr("x", new VarExpr("x"))));
}


TEST_CASE("call_expr") {
//    (_fun (x) x)(10)
    //equals
    CallExpr* call1 =  new CallExpr(new FunExpr("x", new VarExpr("x")), new NumExpr(10));
    CallExpr* call2 =  new CallExpr(new FunExpr("x", new VarExpr("x")), new NumExpr(10));
    CallExpr* call3 = NULL;
    CallExpr* call4 =  new CallExpr(new FunExpr("x", new VarExpr("x")), new NumExpr(11));
    CHECK(call1->equals(call3) == false);
    CHECK(call1->equals(call2) == true);
    CHECK(call1->equals(call4) == false);
    //Has_Variable
    //print:
    std::stringstream empty_string_string(""); // empty but gets set to fun1
    call1->print(empty_string_string);
    //_fun (x) x
    //std::cout << empty_string_string.str() << std::endl;
//    CHECK( (empty_string_string.str() == "(_fun (x) x) (10)") );
}


//TEST_CASE("pretty_print") {
//
//    Expr* num1 = new NumExpr(1);
//    Expr* num2 = new NumExpr(2);
//
//    std::ostream out(nullptr);
//    std::stringbuf str;
//    out.rdbuf(&str);
//
//    CHECK(((new MultExpr((new AddExpr(num1, num2)), new NumExpr(1)))->to_string_pretty()) == "(1 + 2) * 1");
//    CHECK(((new MultExpr((new MultExpr(num1, num2)), new NumExpr(1)))->to_string_pretty()) == "(1 * 2) * 1");
//    CHECK(((new MultExpr((new NumExpr(1)), new MultExpr(num1, num2)))->to_string_pretty()) == "1 * 1 * 2");
//    CHECK(((new AddExpr((new MultExpr(num1, num2)), new NumExpr(1)))->to_string_pretty()) == "1 * 2 + 1");
//    CHECK(((new AddExpr((new AddExpr(num1, num2)), new MultExpr(num1, num2)))->to_string_pretty()) == "(1 + 2) + 1 * 2");
//    CHECK(((new AddExpr((new AddExpr(num1, num2)), new AddExpr(num1, num2)))->to_string_pretty()) == "(1 + 2) + 1 + 2");
//    CHECK(((new MultExpr((new MultExpr(num1, num2)), new MultExpr(num1, num2)))->to_string_pretty()) == "(1 * 2) * 1 * 2");
//    CHECK(((new MultExpr((new AddExpr(num1, num2)), new AddExpr(num1, num2)))->to_string_pretty()) == "(1 + 2) * (1 + 2)");
//    CHECK(((new AddExpr((new MultExpr(num1, num2)), new MultExpr(num1, num2)))->to_string_pretty()) == "1 * 2 + 1 * 2");
//    CHECK(((new AddExpr((new MultExpr(num1, num2)), new MultExpr(num1, num2)))->to_string_pretty()) == "1 * 2 + 1 * 2");
//    CHECK(((new AddExpr((new AddExpr(num1, num2)), new AddExpr(num1, num2)))->to_string_pretty()) == "(1 + 2) + 1 + 2");
//    CHECK((new VarExpr("x"))->to_string_pretty() == "x");
//
//    // BoolExpr
//    auto* trueBoolExpr = new BoolExpr(true);
//    CHECK(((trueBoolExpr)->to_string_pretty()) == "_true");
//    auto* trueBoolExpr2 = new BoolExpr(false);
//    CHECK(((trueBoolExpr2)->to_string_pretty()) == "_false");
//
//    // if Expr
//    auto* ifExpr = new IfExpr(new BoolExpr(true), new NumExpr(3), new NumExpr(4));
//    CHECK(((ifExpr)->to_string_pretty()) == "(_if _true _then 3 _else 4)");
//
//    // Eq Expr
//    auto* eqExpr = new EqExpr(new NumExpr(3), new NumExpr(3));
//    CHECK(((eqExpr)->to_string_pretty()) == "3 == 3");
//}
//
//std::string to_string(std::ostream &out) {
//    std::stringstream ss;
//    ss << out.rdbuf();
//    return ss.str();
//}
//
//TEST_CASE("pretty_print_at") {
//
//    Expr* num1 = new NumExpr(1);
//    Expr* num2 = new NumExpr(2);
//
//    std::ostream out(nullptr);
//    std::stringbuf str;
//    out.rdbuf(&str);
//
////    num1->pretty_print_at(out, Expr::print_group_none);
////    CHECK(to_string(out) == "1");
//
//    out.clear();
//    (new AddExpr((new AddExpr(num1, num2)), new AddExpr(num1, num2)))->pretty_print_at(out, Expr::print_group_none);
//    CHECK(to_string(out) == "(1 + 2) + 1 + 2");
//
//    CHECK((((new LetExpr("x", new AddExpr(num1 , num2), new AddExpr(new VarExpr("x") , num2)))->to_string_pretty()) == "(_let x=(1+2) _in x+2)") == false);
//
//}

TEST_CASE("parse_test") {
    CHECK( Expr::parse_str("10")->equals(new NumExpr(10))); // testing num
//    CHECK( Expr::parse_str("feb")->equals(new Var("feb"))); // testing variable
    Expr *ten_plus_one = new AddExpr(new NumExpr(10), new NumExpr(1));
    Expr *neg_ten_plus_one = new AddExpr(new NumExpr(-10), new NumExpr(1));
    CHECK( Expr::parse_str("10+1")->equals(ten_plus_one)); // testing expression Add
    Expr *five_mult_two = new MultExpr(new NumExpr(5), new NumExpr(2));
    CHECK( Expr::parse_str("5*2")->equals(new MultExpr(new NumExpr(5), new NumExpr(2)))); // testing expression Mult
    CHECK( Expr::parse_str(" 2 ")->equals(new NumExpr(2))); // testing whitespace
    CHECK( Expr::parse_str("(10+1)")->equals(ten_plus_one)); // testing expression Add
    CHECK_THROWS_AS( Expr::parse_str("(10+1")->equals(ten_plus_one), exception); // testing expression Add
//    CHECK_THROWS_AS( Expr::parse_str("(d+1)")->equals(ten_plus_one), exception); // testing expression Add
    CHECK( Expr::parse_str("(-10+1)")->equals(neg_ten_plus_one)); // testing expression Add

    CHECK( Expr::parse_str("x")->equals(new VarExpr("x")));

    CHECK( Expr::parse_str("_true")->equals(new BoolExpr(true)));
    CHECK( Expr::parse_str("_false")->equals(new BoolExpr(false)));
    CHECK( Expr::parse_str("_if _true _then _true _else _false")->equals(new IfExpr(new BoolExpr(true), new BoolExpr(true), new BoolExpr(false))));
    CHECK( Expr::parse_str("_let x = 4 _in x + 5")->equals(new LetExpr("x", new NumExpr(4), new AddExpr(new VarExpr("x"), new NumExpr(5)))));
    CHECK_THROWS_AS( Expr::parse_str("_let 5 = 4 _in x + 5")->equals(new LetExpr("x", new NumExpr(4), new AddExpr(new VarExpr("x"), new NumExpr(5)))), exception);
    CHECK( Expr::parse_str("5 == 5")->equals(new EqExpr(new NumExpr(5), new NumExpr(5))));
    CHECK( Expr::parse_str("5 == x")->equals(new EqExpr(new NumExpr(5), new VarExpr("x"))));

    // Invalid Input
    CHECK_THROWS_AS( Expr::parse_str("<(10+1>")->equals(ten_plus_one), exception); // testing expression Add
    CHECK_THROWS_AS( Expr::parse_str("__ test")->equals(ten_plus_one), exception); // testing expression Add
    CHECK_THROWS_AS( Expr::parse_str("_it test")->equals(ten_plus_one), exception); // testing expression Add

//_let f = _fun (x) x*x_in  f(2):
//    // testing for parse_let (add other types later)
//    _let x = 1 _ in x + 2
//     CHECK( parse_str("_let x = 1 _ in x + 2 ")->equals( (new _let(new Variable("x"),
//    //new Num(1), new Add(new Variable("x"), new Num(2))));)); // testing whitespace


// todo: uncomment this
//    CHECK( Expr::parse_str("(_fun (x) x + 1)(10)")->interp()->equals(new NumVal(11)));


}


TEST_CASE("To Expr") {

    auto* trueBoolExpr = new BoolVal(true);
    CHECK(trueBoolExpr->to_expr()->equals(new BoolExpr(true)));
    auto* trueBoolExpr2 = new BoolVal(false);
    CHECK(trueBoolExpr2->to_expr()->equals(new BoolExpr(true)) == false);
}