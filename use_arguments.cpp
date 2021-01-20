//
// Created by Awerkamp on 1/19/21.
//

#include <iostream>
#include <string>

using namespace std;



void use_arguments(int argc, char **argv) {

    bool isSeen = false;

    for (int i = 1; i < argc; i++) {

        if (std::string(argv[i]) == "--help") {
            cout << "You may only pass in the argument --test" << endl;
        } else if (std::string(argv[i]) == "--test" ) {
            if (!isSeen) {
                cout << "Tests Passed" << endl;
                isSeen = true;
            } else {
                std::cerr << "Argument --test has already been entered" << endl;
                exit(1);
            }
        } else {
            std::cerr << "Invalid Argument" << endl;
            exit(1);
        }
    }
}