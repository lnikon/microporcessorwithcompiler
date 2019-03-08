cmake_minimum_required(VERSION 3.1..3.13)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
else()
    cmake_policy(VERSION 3.13)
endif()

project(bitfighter VERSION 1.0
                   DESCRIPTION "Assembly translator for Miyazaki-1988"
                   LANGUAGES CXX)
set(CMAKE_BUILD_TYPE Debug)

file(GLOB CALCULATOR_HPP "${CMAKE_SOURCE_DIR}/include/calculator/*.hpp")
file(GLOB CALCULATOR_CPP "${CMAKE_SOURCE_DIR}/src/calculator/*.cpp")

add_library(calculator STATIC ${CALCULATOR_HPP} ${CALCULATOR_CPP})
#add_library(calculator STATIC  src/BinaryExpressionBuilder.cpp
#                            src/NumericElementNode.cpp
#                            src/BinaryOperationNode.cpp
#                            src/Tokenizer.cpp
#                            include/calculator/BinaryExpressionBuilder.hpp
#                            include/calculator/NumericElementNode.hpp
#                            include/calculator/BinaryOperationNode.hpp
#                            include/calculator/ExpressionElementNode.hpp
#                            include/calculator/Tokenizer.hpp
#                            include/calculator/Token.hpp
#                            )

target_include_directories(calculator PUBLIC include/calculator)
target_compile_features(calculator PUBLIC cxx_std_17)
set_target_properties(calculator PROPERTIES CXX_STANDARD 17)

file(GLOB ASMTRANSLATOR_HPP "${CMAKE_SOURCE_DIR}/include/asmtranslator/*.hpp")
file(GLOB ASMTRANSLATOR_CPP "${CMAKE_SOURCE_DIR}/src/asmtranslator/*.cpp")

# add_library(asmtranslator STATIC src/ASMTranslator.cpp include/asmtranslator/ASMTranslator.hpp)
add_library(asmtranslator STATIC ${ASMTRANSLATOR_HPP} ${ASMTRANSLATOR_CPP})
target_include_directories(asmtranslator PUBLIC include/asmtranslator include/calculator)
target_compile_features(asmtranslator PUBLIC cxx_std_17)
set_target_properties(asmtranslator PROPERTIES CXX_STANDARD 17)

file(GLOB ASMINTERPRETER_HPP "${CMAKE_SOURCE_DIR}/include/asminterpreter/*.hpp")
file(GLOB ASMINTERPRETER_CPP "${CMAKE_SOURCE_DIR}/src/asminterpreter/*.cpp")
# add_library(asminterpreter STATIC src/ASMInterpreter.cpp include/asminterpreter/ASMInterpreter.hpp)
add_library(asminterpreter STATIC ${ASMINTERPRETER_HPP} ${ASMINTERPRETER_CPP})
target_include_directories(asminterpreter PUBLIC include/asminterpreter)
target_compile_features(asminterpreter PUBLIC cxx_std_17)
set_target_properties(asminterpreter PROPERTIES CXX_STANDARD 17)


add_executable(calc apps/calc.cpp)
target_link_libraries(calc PUBLIC calculator)
target_link_libraries(calc PUBLIC asmtranslator)
target_link_libraries(calc PUBLIC asminterpreter)
