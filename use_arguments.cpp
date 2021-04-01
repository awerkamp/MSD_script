//
// Created by Awerkamp on 1/19/21.
//



#define CATCH_CONFIG_RUNNER 
#include "catch.hpp"
#include "expr.h"
#include "val.h"
#include "pointer.h"
#include "env.h"
#include <iostream>
#include <string>

using namespace std;

void use_arguments(int argc, char **argv) {

    bool isSeen = false;

    for (int i = 1; i < argc; i++) {

        if (std::string(argv[i]) == "--help") {
            cout << "You can pass in the following arguments" << endl;
            cout << "--test" << endl;
            cout << "--interp" << endl;
            cout << "--print" << endl;
        } else if (std::string(argv[i]) == "--test" ) {
            if (!isSeen) {
                int returnStatus = Catch::Session().run(1,argv);
                if(returnStatus == 1) {
                    std::cerr << "One or more of the tests failed. Goodbye."  << endl;
                    exit(1);
                }
                isSeen = true;
            } else {
                std::cerr << "Argument --test has already been entered. Goodbye" << endl;
                exit(1);
            }
        } else if (std::string(argv[i]) == "--interp") {

//                PTR(Expr) y = Expr::parse_expr(std::cin);  // todo: changed to parse_expr
                PTR(Expr) y = Expr::parse_comparg(std::cin);
                PTR(Val) v = y->interp(Env::empty);
//                e = v->to_expr();

                std::cout << v->to_string() << "\n";
                exit(0);


        } else if (std::string(argv[i]) == "--print") {
            PTR(Expr) e = Expr::parse_comparg(std::cin);
            e->print(std::cout);
            exit(0);

        }
        else {
            std::cerr << "Invalid Argument" << endl;
            exit(1);
        }
    }
}