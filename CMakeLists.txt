cmake_minimum_required(VERSION 3.1..3.13)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
else()
    cmake_policy(VERSION 3.13)
endif()

project(bitfighter VERSION 1.0
                   DESCRIPTION "Assembly translator for Miyazaki-1988"
                   LANGUAGES CXX)

add_library(calclib STATIC  src/BinaryExpressionBuilder.cpp
                            src/NumericElementNode.cpp
                            src/BinaryOperationNode.cpp
                            include/calc/BinaryExpressionBuilder.hpp
                            include/calc/NumericElementNode.hpp
                            include/calc/BinaryOperationNode.hpp
                            include/calc/ExpressionElementNode.hpp
                            )
target_include_directories(calclib PUBLIC include/calc)
target_compile_features(calclib PUBLIC cxx_std_17)
set_target_properties(calclib PROPERTIES CXX_STANDARD 17)

add_library(asmtranslator STATIC src/ASMTranslator.cpp include/asmtranslator/ASMTranslator.hpp)
target_include_directories(asmtranslator PUBLIC include/asmtranslator include/calc)
target_compile_features(asmtranslator PUBLIC cxx_std_17)
set_target_properties(asmtranslator PROPERTIES CXX_STANDARD 17)


add_library(asminterpreter STATIC src/ASMInterpreter.cpp include/asminterpreter/ASMInterpreter.hpp)
target_include_directories(asminterpreter PUBLIC include/asminterpreter)
target_compile_features(asminterpreter PUBLIC cxx_std_17)
set_target_properties(asminterpreter PROPERTIES CXX_STANDARD 17)


add_executable(calc apps/calc.cpp)
target_link_libraries(calc PUBLIC calclib)
target_link_libraries(calc PUBLIC asmtranslator)
target_link_libraries(calc PUBLIC asminterpreter)
