cmake_minimum_required(VERSION 3.17)
project(MSD_script)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(MSD_script
        cmdline.h
        main.cpp
        tests.cpp
        use_arguments.cpp
        expr.cpp
        val.cpp
        env.cpp
        step.cpp
        cont.cpp
        )

