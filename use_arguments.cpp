//
// Created by Awerkamp on 1/19/21.
//

#include <iostream>
#include <string>

#define CATCH_CONFIG_RUNNER 
#include "catch.hpp"
#include "expr.h"

using namespace std;

void use_arguments(int argc, char **argv) {

    bool isSeen = false;

    for (int i = 1; i < argc; i++) {

        if (std::string(argv[i]) == "--help") {
            cout << "You can pass in the following arguments" << endl;
            cout << "--test" << endl;
            cout << "--interp" << endl;
            cout << "--print" << endl;
            cout << "--pretty-print" << endl;
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
            while (1) {
                Expr* e = Expr::parse_addend(std::cin);

                e->pretty_print(std::cout);
                std::cout << "\n";

                // skip the whitespace to see if there is a another file
                Expr::skip_whitespace(std::cin);
                if (std::cin.eof()) {
                    exit(0);
                }
            }
        } else if (std::string(argv[i]) == "--print") {
            std::cerr << "Print has not yet been implemented" << endl;
        } else if (std::string(argv[i]) == "--pretty-print") {
            std::cerr << "Pretty print has not yet been implemented" << endl;
        }
        else {
            std::cerr << "Invalid Argument" << endl;
            exit(1);
        }
    }
}