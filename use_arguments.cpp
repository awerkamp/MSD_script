//
// Created by Awerkamp on 1/19/21.
//

#include <iostream>
#include <string>

#define CATCH_CONFIG_RUNNER // deactivate when testing in Clion
#include "catch.hpp" // deactivate when testing in Clion
#include "expr.cpp" // deactivate when testing in Clion

using namespace std;

void use_arguments(int argc, char **argv) {

    bool isSeen = false;

    for (int i = 1; i < argc; i++) {

        if (std::string(argv[i]) == "--help") {
            cout << "You may only pass in the argument --test" << endl;
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
        }
        else {
            std::cerr << "Invalid Argument" << endl;
            exit(1);
        }
    }
}