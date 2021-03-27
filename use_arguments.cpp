//
// Created by Awerkamp on 1/19/21.
//

#include <iostream>
#include <string>

#define CATCH_CONFIG_RUNNER 
#include "catch.hpp"
#include "expr.h"
#include "val.h"

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

                Expr* e = Expr::parse_expr(std::cin);  // todo: changed to parse_expr
                Val* v = e->interp();
                e = v->to_expr();

                std::cout << e->to_string(cout) << "\n";
                exit(0);


        } else if (std::string(argv[i]) == "--print") {
            Expr* e = Expr::parse_comparg(std::cin);
            e->print(std::cout);
            exit(0);

        }
        else {
            std::cerr << "Invalid Argument" << endl;
            exit(1);
        }
    }
}