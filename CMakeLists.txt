cmake_minimum_required(VERSION 3.1..3.13)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
else()
    cmake_policy(VERSION 3.13)
endif()

project(bitfighter VERSION 1.0
                   DESCRIPTION "Veri Nice Project"
                   LANGUAGES CXX)

add_library(calclib STATIC src/calclib.cpp include/calc/lib.hpp)
target_include_directories(calclib PUBLIC include)
target_compile_features(calclib PUBLIC cxx_std_11)

add_library(asmtranslator STATIC src/asmtranslator.cpp include/asmtranslator/translator.hpp)
target_include_directories(asmtranslator PUBLIC include)
target_compile_features(asmtranslator PUBLIC cxx_std_11)

add_library(asminterpreter STATIC src/asminterpreter.cpp include/asminterpreter/interpreter.hpp)
target_include_directories(asminterpreter PUBLIC include)
target_compile_features(asminterpreter PUBLIC cxx_std_11)

add_executable(calc apps/calc.cpp)
target_link_libraries(calc PUBLIC calclib)
target_link_libraries(calc PUBLIC asmtranslator)
target_link_libraries(calc PUBLIC asminterpreter)
