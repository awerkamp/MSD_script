//
// Created by Awerkamp on 1/29/21.
//
#include <sstream>
#include "catch.hpp"
#include "expr.h"
#include "val.h"
#include "pointer.h"
#include "step.hpp"

TEST_CASE("equals") {

    PTR(Expr) num1 = NEW(NumExpr)(1);
    PTR(Expr) num2 = NEW(NumExpr)(2);
    PTR(Expr) num3 = NEW(NumExpr)(3);
    PTR(Expr) num4 = NEW(NumExpr)(4);
    PTR(Expr) num5 = NEW(NumExpr)(5);
    PTR(Expr) var1 = NEW(VarExpr)("var1");
    PTR(Expr) var2 = NEW(VarExpr)("var2");

    // add Expr
    CHECK((NEW(AddExpr)(num1, num2))->equals(NEW(AddExpr)(num1, num2)) == true);
    CHECK((NEW(AddExpr)(num1, num5))->equals(NEW(AddExpr)(num3, num3)) == false);
    CHECK((NEW(AddExpr)(num1, num5))->equals(NEW(MultExpr)(num3, num3)) == false);

    // add Val
    CHECK_THROWS_AS(((NEW(BoolVal)(true))->add_to(NEW(BoolVal)(true))), exception);

    // num
    CHECK((num1)->equals(num1) == true);
    CHECK((num1)->equals(num2) == false);
    CHECK((num1)->equals(var1) == false);

    // var
    CHECK((var1)->equals(var1) == true);
    CHECK((var1)->equals(var2) == false);
    CHECK((var1)->equals(num2) == false);


    // mult expr
    CHECK((NEW(MultExpr)(num1, num2))->equals(NEW(MultExpr)(num1, num2)) == true);
    CHECK((NEW(MultExpr)(num1, num2))->equals(NEW(MultExpr)(num1, num3)) == false);
    CHECK((NEW(MultExpr)(num1, num2))->equals(NEW(AddExpr)(num1, num3)) == false);

    // mult val
    CHECK_THROWS_AS(((NEW(BoolVal)(true))->mult_by(NEW(BoolVal)(true))), exception);

    //_let
    CHECK((NEW(LetExpr)("x", num2, NEW(AddExpr)(num2, var1)))->equals(NEW(LetExpr)("x", num2, NEW(AddExpr)(num2, var1))) == true);
    CHECK((NEW(LetExpr)("x", num2, NEW(AddExpr)(num2, var1)))->equals(NEW(MultExpr)(num2, num2)) == false);
    CHECK((NEW(LetExpr)("y", num2, NEW(AddExpr)(num2, var1)))->equals(NEW(LetExpr)("x", num2, NEW(AddExpr)(num2, var1))) == false);
    CHECK((NEW(LetExpr)("x", num2, NEW(AddExpr)(num2, var1)))->equals(NEW(LetExpr)("x", num3, NEW(AddExpr)(num2, var1))) == false);
    CHECK((NEW(LetExpr)("x", num2, NEW(AddExpr)(num2, var1)))->equals(NEW(LetExpr)("x", num3, NEW(AddExpr)(var1, num2))) == false);
    CHECK((NEW(LetExpr)("x", num2, NEW(AddExpr)(num2, var1)))->equals(NEW(LetExpr)("x", num3, NEW(AddExpr)(var1, num2))) == false);

    //BoolVal
    CHECK((NEW(BoolVal)(true))->equals(NEW(BoolVal)(true)) == true);
    CHECK((NEW(BoolVal)(true))->equals(NEW(BoolVal)(true)) == true);
    CHECK((NEW(BoolVal)(true))->equals(NEW(BoolVal)(false)) == false);
    CHECK((NEW(NumVal)(21))->equals(NEW(BoolVal)(false)) == false);
    CHECK((NEW(BoolVal)(true))->equals(NEW(NumVal)(21)) == false);


    //BoolExpr
    CHECK((NEW(BoolExpr)(true))->equals(NEW(BoolExpr)(true)) == true);
    CHECK((NEW(BoolExpr)(true))->equals(NEW(BoolExpr)(false)) == false);
    CHECK((NEW(BoolExpr)(true))->equals(NEW(BoolExpr)(false)) == false);
    PTR(NumExpr) temp = NEW(NumExpr)(4);
    CHECK((NEW(BoolExpr)(true))->equals(temp) == false);

//    CHECK((NEW(BoolExpr)(true))->equals(NEW(NumVal)(21)) == false);


    // eqExp
    PTR(Expr) eqExp = NEW(EqExpr)(num1, num2);
    CHECK(eqExp->equals(NEW(EqExpr)(num1, num2)));
    CHECK(eqExp->equals(NEW(EqExpr)(num1, num3)) == false);

    // ifExp
    PTR(Expr) ifExp = NEW(IfExpr)(NEW(BoolExpr)(true), num1, num2);
    CHECK(ifExp->equals(NEW(IfExpr)(NEW(BoolExpr)(true), num1, num2)) == true);

    PTR(Expr) ifExp2 = NEW(IfExpr)(NEW(BoolExpr)(true), num1, num2);
    CHECK(eqExp->equals(NEW(IfExpr)(NEW(BoolExpr)(true), num1, num2)) == false);

    PTR(Expr) ifExp3 = NEW(IfExpr)(NEW(BoolExpr)(true), num1, num2);
    CHECK(ifExp3->equals(NEW(IfExpr)(NEW(BoolExpr)(true), num1, num3)) == false);

//    CHECK_THROWS_AS(NEW(IfExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2), NEW(NumExpr)(3)), exception);

}

TEST_CASE("Interp") {

    PTR(Expr) num1 = NEW(NumExpr)(1);
    PTR(Expr) num2 = NEW(NumExpr)(2);
    PTR(Expr) num3 = NEW(NumExpr)(3);

    // Add
    CHECK((((NEW(AddExpr)(num1, num2))->interp(Env::empty))->equals(num3->interp(Env::empty))) == true); // 3 == 3
    CHECK((((NEW(AddExpr)(num1, num2))->interp(Env::empty))->equals(num1->interp(Env::empty))) == false); // 3 == 1

    // Variables
//    CHECK_THROWS_WITH((NEW(VarExpr)("x"))->interp(Env::empty), "There is no value for this expression" );
// todo: redo this error
    // Multiply
    CHECK((((NEW(MultExpr)(num1, num2))->interp(Env::empty))->equals(num2->interp(Env::empty))) == true); // 2 == 2
    CHECK((((NEW(MultExpr)(num1, num2))->interp(Env::empty))->equals(num3->interp(Env::empty))) == false); // 2 == 3

    // Num
    CHECK(((num2->interp(Env::empty))->equals(num2->interp(Env::empty))) == true); // 2 == 2
    CHECK(((num2->interp(Env::empty))->equals(num3->interp(Env::empty))) == false); // 2 == 3

    // _let
    CHECK(((NEW(LetExpr)("x", num1, NEW(VarExpr)("x")))->interp(Env::empty)->equals(NEW(NumVal)(1))) == true);
    CHECK((((LetExpr("x", num2, NEW(VarExpr)("x"))).interp(Env::empty)->equals(NEW(NumVal)(1))) == false));
    CHECK((((LetExpr("x", NEW(AddExpr)(num1, num2), NEW(VarExpr)("x"))).interp(Env::empty)->equals(NEW(NumVal)(3))) == true));
    CHECK((((LetExpr("x", NEW(AddExpr)(num1, num2), NEW(AddExpr)(NEW(VarExpr)("x"), num2))).interp(Env::empty)->equals(NEW(NumVal)(5))) == true));
    CHECK((((LetExpr("x", NEW(AddExpr)(num1, num2), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(VarExpr)("x")))).interp(Env::empty)->equals(NEW(NumVal)(6))) == true));
    CHECK_THROWS_AS((((LetExpr("x", NEW(AddExpr)(num1, num2), NEW(AddExpr)(NEW(VarExpr)("y"), NEW(VarExpr)("x")))).interp(Env::empty)->equals(NEW(NumVal)(6))) == true), exception);
    CHECK_THROWS_AS((((LetExpr("x", NEW(AddExpr)(NEW(VarExpr)("x"), num2), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(VarExpr)("x")))).interp(Env::empty)->equals(NEW(NumVal)(6))) == true), exception);
    CHECK((((LetExpr("x", NEW(AddExpr)(num1, num2), NEW(AddExpr)(NEW(NumExpr)(0), NEW(NumExpr)(5)))).interp(Env::empty)->equals(NEW(NumVal)(5))) == true));  // todo: verify this is correct
    CHECK(((LetExpr("x", num1, NEW(VarExpr)("x"))).interp(Env::empty)->equals(NEW(NumVal)(1))) == true);

    // EqExpr
    PTR(Expr) eqExp = NEW(EqExpr)(num1, num2);
    CHECK(eqExp->interp(Env::empty)->equals(NEW(BoolVal)(false)) == true);
    PTR(Expr) eqExp2 = NEW(EqExpr)(num1, num1);
    CHECK(eqExp2->interp(Env::empty)->equals(NEW(BoolVal)(true)) == true);

    // ifExp
    PTR(Expr) ifExp = NEW(IfExpr)(NEW(BoolExpr)(true), num1, num2);
    CHECK(ifExp->interp(Env::empty)->equals(NEW(NumVal)(1)) == true);

    PTR(Expr) ifExp2 = NEW(IfExpr)(NEW(BoolExpr)(false), num1, num2);
    CHECK(ifExp2->interp(Env::empty)->equals(NEW(NumVal)(2)) == true);

    // BoolExpr
    PTR(Expr) boolExpr = NEW(BoolExpr)(true);
    CHECK(boolExpr->interp(Env::empty)->equals(NEW(BoolVal)(true)));
}


TEST_CASE("has_variable") {

    PTR(Expr) num1 = NEW(NumExpr)(1);
    PTR(Expr) num2 = NEW(NumExpr)(2);
    PTR(Expr) num3 = NEW(NumExpr)(3);
    PTR(Expr) var1 = NEW(VarExpr)("var1");
    PTR(Expr) var2 = NEW(VarExpr)("var2");

    // AddExpr
    CHECK(((NEW(AddExpr)(num1, var1))->has_variable()) == true); // var1
    CHECK(((NEW(AddExpr)(num1, num2))->has_variable()) == false); // no var

    // MultExpr
    CHECK(((NEW(MultExpr)(num1, var1))->has_variable()) == true); // var1
    CHECK(((NEW(MultExpr)(num1, num2))->has_variable()) == false); // no var

    // Complex
    CHECK(((NEW(MultExpr)(NEW(AddExpr)(num1, var2), num2))->has_variable()) == true); // var2
    CHECK(((NEW(MultExpr)(NEW(AddExpr)(num1, num2), num2))->has_variable()) == false); // no var

    // VarExpr
    CHECK(var2->has_variable() == true); // var2
    CHECK(num1->has_variable() == false); // var2


    // NumExpr
    CHECK(num3->has_variable() == false); // no var

    // _letExpr
    CHECK(((NEW(LetExpr)("x", num2, var1))->has_variable()) == true);
    CHECK(((NEW(LetExpr)("x", num2, num3))->has_variable()) == false);
    CHECK(((NEW(LetExpr)("x", var1, num2))->has_variable()) == true);

    // boolExpr
    PTR(BoolExpr) trueBoolExpr = NEW(BoolExpr)(true);
    CHECK(trueBoolExpr->has_variable() == false);

    // EqExpr
    PTR(Expr) eqExp = NEW(EqExpr)(num1, num2);
    CHECK(eqExp->has_variable() == false);
    PTR(Expr) eqExp2 = NEW(EqExpr)(var1, var2);
    CHECK(eqExp2->has_variable() == true);
//    PTR(Expr) eqExp2 = NEW(EqExpr)(num1, num1);
//    CHECK(eqExp2->interp(Env::empty)->equals(NEW(BoolVal)(true)) == true);

    // ifExp
    PTR(Expr) ifExp = NEW(IfExpr)(NEW(BoolExpr)(true), num1, num2);
    CHECK(ifExp->has_variable() == false);

    PTR(Expr) ifExp2 = NEW(IfExpr)(NEW(BoolExpr)(false), var1, num2);
    CHECK(ifExp2->has_variable() == true);
}

TEST_CASE("subst") {

    // Var
//    CHECK(((NEW(VarExpr)("x"))->subst("x", NEW(VarExpr)("y"))->equals(NEW(VarExpr)("y"))) == true);
//    CHECK(((NEW(VarExpr)("x"))->subst("x", NEW(VarExpr)("y"))->equals(NEW(VarExpr)("x"))) == false);
//    CHECK(((NEW(VarExpr)("x"))->subst("y", NEW(VarExpr)("y"))->equals(NEW(VarExpr)("y"))) == false);

    // Num
//    CHECK(((NEW(NumExpr)(1))->subst("x", NEW(VarExpr)("y"))->equals(NEW(NumExpr)(1))) == true);

    // Add
//    CHECK( (NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(7)))
//    ->subst("x", NEW(VarExpr)("y"))
//    ->equals(NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(7))) );

    // Mult
//    CHECK( (NEW(MultExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(7)))
//    ->subst("x", NEW(VarExpr)("y"))
//    ->equals(NEW(MultExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(7))) );

    // _let
//    CHECK( (NEW(LetExpr)("x", NEW(NumExpr)(7), NEW(VarExpr)("y")))
//                   ->subst("y", NEW(VarExpr)("x"))
//                   ->equals(NEW(LetExpr)("x", NEW(NumExpr)(7), NEW(VarExpr)("x"))) );

//    CHECK( (NEW(LetExpr)("x", NEW(NumExpr)(7), NEW(VarExpr)("x")))
//                   ->subst("y", NEW(VarExpr)("x"))
//                   ->equals(NEW(LetExpr)("x", NEW(NumExpr)(7), NEW(VarExpr)("x"))) );
//    CHECK((NEW(LetExpr)("t", NEW(NumExpr)(7), NEW(VarExpr)("x")))
//                   ->subst("x", NEW(VarExpr)("z"))
//                   ->equals(NEW(LetExpr)("t", NEW(NumExpr)(7), NEW(VarExpr)("x"))) == false );

    PTR(Expr) let1 = NEW(LetExpr)("x",
                             NEW(NumExpr)(1),
                             NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(2)));
//    CHECK( let1->subst("x", NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(3)))
//                   ->equals(let1) );

    PTR(Expr) let2 = (NEW(LetExpr)("x",
                              NEW(VarExpr)("x"),
                              NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(2))));
//    CHECK( let2->subst("x", NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(3)))
//                   ->equals(NEW(LetExpr)("x",
//                                        NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(3)),
//                                        NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(2)))) );

    // BoolExpr
    PTR(BoolExpr) trueBoolExpr = NEW(BoolExpr)(true);
//    CHECK((trueBoolExpr)
//                  ->subst("x", NEW(VarExpr)("z"))
//                  ->equals(trueBoolExpr) == true);

    // EqExpr
    PTR(Expr) eqExp = NEW(EqExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2));
//    CHECK(eqExp->subst("x", NEW(VarExpr)("z"))->equals(eqExp) == true);
    PTR(Expr) eqExp2 = NEW(EqExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(2));
//    CHECK(eqExp2->subst("x", NEW(VarExpr)("z"))->equals(eqExp) == false);

//    PTR(Expr) eqExp2 = NEW(EqExpr)(var1, var2);
//    CHECK(eqExp2->has_variable() == true);

    // ifExp
    PTR(Expr) ifExp = NEW(IfExpr)(NEW(BoolExpr)(true), NEW(NumExpr)(1), NEW(NumExpr)(2));
//    CHECK(ifExp->subst("x", NEW(VarExpr)("z"))->equals(ifExp) == true);

    PTR(Expr) ifExp2 = NEW(IfExpr)(NEW(BoolExpr)(true), NEW(VarExpr)("x"), NEW(NumExpr)(2));
//    CHECK(ifExp2->subst("x", NEW(NumExpr)(1))->equals(ifExp) == true);
}

TEST_CASE("print") {

    PTR(Expr) num1 = NEW(NumExpr)(1);
    PTR(Expr) num2 = NEW(NumExpr)(2);

    CHECK(((NEW(MultExpr)((NEW(AddExpr)(num1, num2)), NEW(NumExpr)(1)))->to_string()) == "((1+2)*1)");
    CHECK(((NEW(MultExpr)((NEW(MultExpr)(num1, num2)), NEW(NumExpr)(1)))->to_string()) == "((1*2)*1)");
    CHECK(((NEW(MultExpr)((NEW(NumExpr)(1)), NEW(MultExpr)(num1, num2)))->to_string()) == "(1*(1*2))");
    CHECK(((NEW(AddExpr)((NEW(MultExpr)(num1, num2)), NEW(NumExpr)(1)))->to_string()) == "((1*2)+1)");
    CHECK(((NEW(AddExpr)((NEW(AddExpr)(num1, num2)), NEW(MultExpr)(num1, num2)))->to_string()) == "((1+2)+(1*2))");
    CHECK(((NEW(AddExpr)((NEW(AddExpr)(num1, num2)), NEW(AddExpr)(num1, num2)))->to_string()) == "((1+2)+(1+2))");
    CHECK(((NEW(AddExpr)((NEW(AddExpr)(num1, num2)), NEW(AddExpr)(num1, num2)))->to_string()) == "((1+2)+(1+2))");
    CHECK(((NEW(MultExpr)((NEW(MultExpr)(num1, num2)), NEW(MultExpr)(num1, num2)))->to_string()) == "((1*2)*(1*2))");
    CHECK(((NEW(MultExpr)((NEW(AddExpr)(num1, num2)), NEW(AddExpr)(num1, num2)))->to_string()) == "((1+2)*(1+2))");
    CHECK(((NEW(AddExpr)((NEW(MultExpr)(num1, num2)), NEW(MultExpr)(num1, num2)))->to_string()) == "((1*2)+(1*2))");
    CHECK((NEW(VarExpr)("x"))->to_string() == "x");

    // _let
    CHECK(((NEW(LetExpr)("x", NEW(AddExpr)(num1, num2), num1))->to_string()) == "(_let x=(1+2) _in 1)");
    CHECK(((NEW(LetExpr)("x", NEW(AddExpr)(num1 , num2), NEW(AddExpr)(NEW(VarExpr)("x") , num2)))->to_string()) == "(_let x=(1+2) _in (x+2))");
    CHECK((((NEW(LetExpr)("x", NEW(AddExpr)(num1 , num2), NEW(AddExpr)(NEW(VarExpr)("x") , num2)))->to_string()) == "(_let x=(1+2) _in x+2)") == false);

    //BoolExpr
    PTR(BoolExpr) trueBoolExpr = NEW(BoolExpr)(true);
    CHECK(((trueBoolExpr)->to_string()) == "_true");
    PTR(BoolExpr) trueBoolExpr2 = NEW(BoolExpr)(false);
    CHECK(((trueBoolExpr2)->to_string()) == "_false");


    PTR(IfExpr) ifExpr = NEW(IfExpr)(NEW(BoolExpr)(true), NEW(NumExpr)(3), NEW(NumExpr)(4));
    CHECK(((ifExpr)->to_string()) == "(_if _true _then 3 _else 4)");

    PTR(EqExpr) eqExpr = NEW(EqExpr)(NEW(NumExpr)(3), NEW(NumExpr)(3));
    CHECK(((eqExpr)->to_string()) == "(3==3)");

}




//TEST_CASE("pretty_print") {
//
//    Expr* num1 = NEW(NumExpr)(1);
//    Expr* num2 = NEW(NumExpr)(2);
//
//    std::ostream out(nullptr);
//    std::stringbuf str;
//    out.rdbuf(&str);
//
//    CHECK(((NEW(MultExpr)((NEW(AddExpr)(num1, num2)), NEW(NumExpr)(1)))->to_string_pretty()) == "(1 + 2) * 1");
//    CHECK(((NEW(MultExpr)((NEW(MultExpr)(num1, num2)), NEW(NumExpr)(1)))->to_string_pretty()) == "(1 * 2) * 1");
//    CHECK(((NEW(MultExpr)((NEW(NumExpr)(1)), NEW(MultExpr)(num1, num2)))->to_string_pretty()) == "1 * 1 * 2");
//    CHECK(((NEW(AddExpr)((NEW(MultExpr)(num1, num2)), NEW(NumExpr)(1)))->to_string_pretty()) == "1 * 2 + 1");
//    CHECK(((NEW(AddExpr)((NEW(AddExpr)(num1, num2)), NEW(MultExpr)(num1, num2)))->to_string_pretty()) == "(1 + 2) + 1 * 2");
//    CHECK(((NEW(AddExpr)((NEW(AddExpr)(num1, num2)), NEW(AddExpr)(num1, num2)))->to_string_pretty()) == "(1 + 2) + 1 + 2");
//    CHECK(((NEW(MultExpr)((NEW(MultExpr)(num1, num2)), NEW(MultExpr)(num1, num2)))->to_string_pretty()) == "(1 * 2) * 1 * 2");
//    CHECK(((NEW(MultExpr)((NEW(AddExpr)(num1, num2)), NEW(AddExpr)(num1, num2)))->to_string_pretty()) == "(1 + 2) * (1 + 2)");
//    CHECK(((NEW(AddExpr)((NEW(MultExpr)(num1, num2)), NEW(MultExpr)(num1, num2)))->to_string_pretty()) == "1 * 2 + 1 * 2");
//    CHECK(((NEW(AddExpr)((NEW(MultExpr)(num1, num2)), NEW(MultExpr)(num1, num2)))->to_string_pretty()) == "1 * 2 + 1 * 2");
//    CHECK(((NEW(AddExpr)((NEW(AddExpr)(num1, num2)), NEW(AddExpr)(num1, num2)))->to_string_pretty()) == "(1 + 2) + 1 + 2");
//    CHECK((NEW(VarExpr)("x"))->to_string_pretty() == "x");
//
//    // BoolExpr
//    auto* trueBoolExpr = NEW(BoolExpr)(true);
//    CHECK(((trueBoolExpr)->to_string_pretty()) == "_true");
//    auto* trueBoolExpr2 = NEW(BoolExpr)(false);
//    CHECK(((trueBoolExpr2)->to_string_pretty()) == "_false");
//
//    // if Expr
//    auto* ifExpr = NEW(IfExpr)(NEW(BoolExpr)(true), NEW(NumExpr)(3), NEW(NumExpr)(4));
//    CHECK(((ifExpr)->to_string_pretty()) == "(_if _true _then 3 _else 4)");
//
//    // Eq Expr
//    auto* eqExpr = NEW(EqExpr)(NEW(NumExpr)(3), NEW(NumExpr)(3));
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
//    Expr* num1 = NEW(NumExpr)(1);
//    Expr* num2 = NEW(NumExpr)(2);
//
//    std::ostream out(nullptr);
//    std::stringbuf str;
//    out.rdbuf(&str);
//
////    num1->pretty_print_at(out, Expr::print_group_none);
////    CHECK(to_string() == "1");
//
//    out.clear();
//    (NEW(AddExpr)((NEW(AddExpr)(num1, num2)), NEW(AddExpr)(num1, num2)))->pretty_print_at(out, Expr::print_group_none);
//    CHECK(to_string() == "(1 + 2) + 1 + 2");
//
//    CHECK((((NEW(LetExpr)("x", NEW(AddExpr)(num1 , num2), NEW(AddExpr)(NEW(VarExpr)("x") , num2)))->to_string_pretty()) == "(_let x=(1+2) _in x+2)") == false);
//
//}

TEST_CASE("parse_test") {
    CHECK( Expr::parse_str("10")->equals(NEW(NumExpr)(10))); // testing num
//    CHECK( Expr::parse_str("feb")->equals(new Var("feb"))); // testing variable
    PTR(Expr) ten_plus_one = NEW(AddExpr)(NEW(NumExpr)(10), NEW(NumExpr)(1));
    PTR(Expr) neg_ten_plus_one = NEW(AddExpr)(NEW(NumExpr)(-10), NEW(NumExpr)(1));
    CHECK( Expr::parse_str("10+1")->equals(ten_plus_one)); // testing expression Add
    PTR(Expr) five_mult_two = NEW(MultExpr)(NEW(NumExpr)(5), NEW(NumExpr)(2));
    CHECK( Expr::parse_str("5*2")->equals(NEW(MultExpr)(NEW(NumExpr)(5), NEW(NumExpr)(2)))); // testing expression Mult
    CHECK( Expr::parse_str(" 2 ")->equals(NEW(NumExpr)(2))); // testing whitespace
    CHECK( Expr::parse_str("(10+1)")->equals(ten_plus_one)); // testing expression Add
    CHECK_THROWS_AS( Expr::parse_str("(10+1")->equals(ten_plus_one), exception); // testing expression Add
//    CHECK_THROWS_AS( Expr::parse_str("(d+1)")->equals(ten_plus_one), exception); // testing expression Add
    CHECK( Expr::parse_str("(-10+1)")->equals(neg_ten_plus_one)); // testing expression Add

    CHECK( Expr::parse_str("x")->equals(NEW(VarExpr)("x")));

    CHECK( Expr::parse_str("_true")->equals(NEW(BoolExpr)(true)));
    CHECK( Expr::parse_str("_false")->equals(NEW(BoolExpr)(false)));
    CHECK( Expr::parse_str("_if _true _then _true _else _false")->equals(NEW(IfExpr)(NEW(BoolExpr)(true), NEW(BoolExpr)(true), NEW(BoolExpr)(false))));
    CHECK( Expr::parse_str("_let x = 4 _in x + 5")->equals(NEW(LetExpr)("x", NEW(NumExpr)(4), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(5)))));
    CHECK_THROWS_AS( Expr::parse_str("_let 5 = 4 _in x + 5")->equals(NEW(LetExpr)("x", NEW(NumExpr)(4), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(5)))), exception);
    CHECK( Expr::parse_str("5 == 5")->equals(NEW(EqExpr)(NEW(NumExpr)(5), NEW(NumExpr)(5))));
    CHECK( Expr::parse_str("5 == x")->equals(NEW(EqExpr)(NEW(NumExpr)(5), NEW(VarExpr)("x"))));

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
//    CHECK( Expr::parse_str("(_fun (x) x + 1)(10)")->interp()->equals(NEW(NumVal)(11)));




}


TEST_CASE("To String") {

    PTR(NumVal) numVal = NEW(NumVal)(3);
    CHECK(numVal->to_string() == "3");

    PTR(BoolVal) boolVal = NEW(BoolVal)(3);
    CHECK(boolVal->to_string() == "_true");

    PTR(FunVal) funVal = NEW (FunVal)("x", NEW (NumExpr)(1), NEW(EmptyEnv)());
    CHECK(funVal->to_string() == "[function]");


}

//
//TEST_CASE("Add To") {
//
//    PTR(FunExpr) fun1= NEW(FunExpr)("x", NEW(VarExpr)("x"));
//    PTR(FunExpr) fun2= NEW(FunExpr)("y", NEW(VarExpr)(
//            "y"));
//    CHECK_THROWS(fun1->add)
//
//
//
//}



TEST_CASE("To Expr") {

    PTR(BoolVal) trueBoolExpr = NEW(BoolVal)(true);
    PTR(BoolVal) trueBoolExpr2 = NEW(BoolVal)(false);
}

TEST_CASE("fun_expr_new") {

    PTR(FunExpr) fun1= NEW(FunExpr)("x", NEW(VarExpr)("x"));
    PTR(FunExpr) fun2= NEW(FunExpr)("y", NEW(VarExpr)(
            "y"));
    CHECK(fun1->equals(fun2) == false);
    PTR(FunExpr) fun3 = NEW(FunExpr)("x", NEW(VarExpr)(
            "x"));
    CHECK(fun1->equals(fun3) == true);
    PTR(FunExpr) fun4 = NULL;
    CHECK(fun1->equals(fun4) == false);
    std::stringstream empty_string_string("");
    fun1->print(empty_string_string);

    CHECK((empty_string_string.str() == "(_fun (x) x) "));

    CHECK_THROWS(fun1->has_variable(), "not implemented");
}


//TEST_CASE("Continuations") {
//
//    CHECK( Step::interp_by_steps(AddExpr::parse_str("1"))->equals(NEW(NumVal)(1)) ); // todo: Add not working properly
//    CHECK( Step::interp_by_steps(MultExpr::parse_str("5*2"))->equals(NEW(NumVal)(10)) );
//    CHECK( Step::interp_by_steps(EqExpr::parse_str("5 == 5"))->equals(NEW(BoolVal)(true)));
//    CHECK( Step::interp_by_steps(BoolExpr::parse_str("_false"))->equals(NEW(BoolVal)(false)));
//    CHECK( Step::interp_by_steps(BoolExpr::parse_str("_true"))->equals(NEW(BoolVal)(true)));
//    CHECK( Step::interp_by_steps(NumExpr::parse_str("1"))->equals(NEW(NumVal)(1)));
//    CHECK(Step::interp_by_steps(IfExpr::parse_str("(_if _true _then 3 _else 4)"))->equals(NEW(NumVal)(3)));
//    CHECK(Step::interp_by_steps(IfExpr::parse_str("(_if 4==5 _then 3 _else 4)"))->equals(NEW(NumVal)(4)));
//
//
////    PTR(LetExpr) let1 = NEW(LetExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)));
//
//    CHECK( Step::interp_by_steps(AddExpr::parse_str("_let x = 7 _in x + 4"))->equals(NEW(NumVal)(11)));
//
//    CHECK(AddExpr::parse_str("test")->equals(NEW (VarExpr)("test")));
//
//    PTR(CallExpr) callExpr = NEW(CallExpr)(NEW(FunExpr)("x", NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))), NEW(NumExpr)(10));
//    CHECK(Step::interp_by_steps(callExpr)->equals(NEW(NumVal)(11)) == true);
//    CHECK_THROWS(Cont::done->step_continue(), "error in doneCont");
//
////    CHECK(Step::interp_by_steps(FunExpr::parse_str("(_fun (x) x) (10)"))->equals(NEW(NumVal(10))));
////    CHECK( Step::interp_by_steps(Expr::parse_str("_let x = 4 _in x + 5"))->equals(NEW(NumVal(5))));
////    CHECK( Expr::parse_str("_let x = 4 _in x + 5")->equals(NEW(LetExpr)("x", NEW(NumExpr)(4), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(5)))));
//
////    CHECK( Step::interp_by_steps(VarExpr::parse_str("x"))->equals("x"));
//
////    VarExpr* var = NEW (VarExpr)("x");
//
//    CHECK( Step::interp_by_steps(BoolExpr::parse_str("_true"))->equals(NEW(BoolVal)(true)));
//
//
//
//
//
//
////    CHECK( Step::interp_by_steps(Expr::parse_str("_let x = 4 _in x + 5"))->equals(NEW(NumVal(9))));
//
//
//
////    out << "(_let ";
////    out << lhs;
////    out << "=";
////    this->rhs->print(out);
////    out << " _in ";
////    this->body->print(out);
////    out << ")";
//
////    CHECK( Step::interp_by_steps(VarExpr::parse_str("x"))->equals(NEW(BoolVal(true))));
////    CHECK( Step::interp_by_steps(VarExpr::parse_str("x"))->equals("x"));
//}



TEST_CASE("Val") {

    PTR(NumVal) num1 = NEW(NumVal)(34);
    PTR(NumVal) num2 = NEW(NumVal)(50);
    CHECK_THROWS(num1->call(num2), "Cannot call a number");

//    CHECK_THROWS(num1->is_true(), "Not a bool");
//    PTR(CallCont) = new CallCont();
//    CHECK_THROWS(num1->call_step(num2, num1), "Can't call number");




}

TEST_CASE("call_expr") {

    PTR(CallExpr) call1 =  NEW (CallExpr)(NEW (FunExpr)("x", NEW (VarExpr)("x")), NEW (NumExpr)(10));
    PTR(CallExpr) call2 =  NEW (CallExpr)(NEW (FunExpr)("x", NEW (VarExpr)("x")), NEW (NumExpr)(10));
    PTR(CallExpr) call3 = NULL;
    PTR(CallExpr) call4 =  NEW (CallExpr)(NEW (FunExpr)("x", NEW (VarExpr)("x")), NEW (NumExpr)(11));
    CHECK(call1->equals(call3) == false);
    CHECK(call1->equals(call2) == true);
    CHECK(call1->equals(call4) == false);
    CHECK_THROWS_WITH( call1->has_variable(), "invalid call" );
    std::stringstream empty_string_string("");
    call1->print(empty_string_string);
    CHECK( (empty_string_string.str() == "(_fun (x) x) (10)") );
    PTR(Expr) num1 = NEW (NumExpr)(1);
    PTR(VarExpr) y = NEW (VarExpr)("y");
    PTR(Expr) fun1 = NEW (FunExpr)("y", NEW (AddExpr)(y, num1));
    PTR(Expr) call_expr1 = NEW (CallExpr) (fun1, num1);
    CHECK(call_expr1->interp(NEW(ExtendedEnv)("y", NEW(NumVal)(1), NEW(EmptyEnv)()))->equals(NEW (NumVal)(2)));
}


TEST_CASE("fan_val") {
    CHECK_THROWS_WITH((NEW (BoolVal)(true))->call(NULL),"Cannot call bool");
    CHECK((NEW (FunVal)("x", NEW (NumExpr)(1), NEW(EmptyEnv)()))->equals(NEW (FunVal)("x", NEW (NumExpr)(1), NEW(EmptyEnv)())));
    PTR(Expr) n1 = NEW (NumExpr)(1);
    PTR(Val) num1 = NEW (NumVal)(1);
    CHECK((NEW (FunVal)("y", n1,NEW(EmptyEnv)()))->call(num1)->equals(NEW (NumVal)(1)));
}
