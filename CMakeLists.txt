#  __   __  _______  ___      ___      _______          _______  ______    ___   _______  __    _  ______   ______
# |  | |  ||       ||   |    |   |    |       |        |       ||    _ |  |   | |       ||  |  | ||      | |      |
# |  |_|  ||    ___||   |    |   |    |   _   |        |    ___||   | ||  |   | |    ___||   |_| ||  _    ||___   |
# |       ||   |___ |   |    |   |    |  | |  |        |   |___ |   |_||_ |   | |   |___ |       || | |   |  __|  |
# |       ||    ___||   |___ |   |___ |  |_|  | ___    |    ___||    __  ||   | |    ___||  _    || |_|   | |_____|
# |   _   ||   |___ |       ||       ||       ||_  |   |   |    |   |  | ||   | |   |___ | | |   ||       |   __
# |__| |__||_______||_______||_______||_______|  |_|   |___|    |___|  |_||___| |_______||_|  |__||______|   |__|
#

cmake_minimum_required(VERSION 3.1..3.13)


################ START Main Settings ###################################
if(${CMAKE_VERSION} VERSION_LESS 3.12)                                 #
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})#
else()                                                                 #
    cmake_policy(VERSION 3.13)                                         #
endif()                                                                #
                                                                       #
project(bitfighter VERSION 1.0                                         #
                   DESCRIPTION "Assembly translator for Miyazaki-1988" #
                   LANGUAGES CXX)                                      #
# Set build type to debug build                                        #
# For the future, parametrize this option                              #
set(CMAKE_BUILD_TYPE Debug)                                            #
################ END Main Settings #####################################


################ START Calculator #######################################
file(GLOB CALCULATOR_HPP "${CMAKE_SOURCE_DIR}/include/calculator/*.hpp")#
file(GLOB CALCULATOR_CPP "${CMAKE_SOURCE_DIR}/src/calculator/*.cpp")    #
                                                                        #
add_library(calculator STATIC ${CALCULATOR_HPP} ${CALCULATOR_CPP})      #
                                                                        #
target_include_directories(calculator PUBLIC include/calculator)        #
target_compile_features(calculator PUBLIC cxx_std_17)                   #
# set_target_properties(calculator PROPERTIES CXX_STANDARD 17)            #
################ END Calculator #########################################


################ START ASMTranslator #####################################################
file(GLOB ASMTRANSLATOR_HPP "${CMAKE_SOURCE_DIR}/include/asmtranslator/*.hpp")           #
file(GLOB ASMTRANSLATOR_CPP "${CMAKE_SOURCE_DIR}/src/asmtranslator/*.cpp")               #
add_library(asmtranslator STATIC ${ASMTRANSLATOR_HPP} ${ASMTRANSLATOR_CPP})              #
                                                                                         #
                                                                                         #
target_include_directories(asmtranslator PUBLIC include/asmtranslator include/calculator)#
target_compile_features(asmtranslator PUBLIC cxx_std_17)                                 #
# set_target_properties(asmtranslator PROPERTIES CXX_STANDARD 17)                        #
################ END ASMTranslator #######################################################


################ START ASMInterpreter ###########################################
file(GLOB ASMINTERPRETER_HPP "${CMAKE_SOURCE_DIR}/include/asminterpreter/*.hpp")#
file(GLOB ASMINTERPRETER_CPP "${CMAKE_SOURCE_DIR}/src/asminterpreter/*.cpp")    #
                                                                                #
add_library(asminterpreter STATIC ${ASMINTERPRETER_HPP} ${ASMINTERPRETER_CPP})  #
                                                                                #
target_include_directories(asminterpreter PUBLIC include/asminterpreter)        #
target_link_libraries(asminterpreter PUBLIC asmtranslator)                      #
target_compile_features(asminterpreter PUBLIC cxx_std_17)                       #
# set_target_properties(asminterpreter PROPERTIES CXX_STANDARD 17)              #
################ END ASMInterpreter #############################################

################ START RegisterMachineVM ###############################################
file(GLOB REGISTERMACHINEVM_HPP "${CMAKE_SOURCE_DIR}/include/registermachinevm/*.hpp") #
file(GLOB REGISTERMACHINEVM_CPP "${CMAKE_SOURCE_DIR}/src/registermachinevm/*.cpp")     #
                                                                                       #
add_library(registermachinevm STATIC ${REGISTERMACHINEVM_HPP} ${REGISTERMACHINEVM_CPP})#
                                                                                       #
target_include_directories(registermachinevm PUBLIC include/registermachinevm)         #
# target_link_libraries(registermachinevm PUBLIC asmtranslator)                        #
target_compile_features(registermachinevm PUBLIC cxx_std_17)                           #
# set_target_properties(asminterpreter PROPERTIES CXX_STANDARD 17)                     #
################ END RegisterMachineVM #################################################

################ START Executables ###############
add_executable(calc apps/calc.cpp)               #
target_link_libraries(calc PUBLIC calculator)    #
target_link_libraries(calc PUBLIC asmtranslator) #
target_link_libraries(calc PUBLIC asminterpreter)#

add_executable(two_operand_asm apps/two_operand_asm.cpp)               #
target_link_libraries(two_operand_asm PUBLIC calculator)    #
target_link_libraries(two_operand_asm PUBLIC asmtranslator) #
target_link_libraries(two_operand_asm PUBLIC asminterpreter)#
################ END Executables #################

