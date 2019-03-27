#include <fstream>
#include <sstream>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <string>

#include "Interpreter.hpp"

template <class ForwardIterator>
void debug_printContainer(ForwardIterator begin, ForwardIterator end) {
  using value_type = typename std::decay<decltype(*begin)>::type;
  std::copy(begin, end, std::ostream_iterator<value_type>(std::cout, " "));
}

RegisterMachineInterpreter::RegisterMachineInterpreter() {
  fillInstructionToInfo();
}

void RegisterMachineInterpreter::run(
    const std::string& inputAssembly) {

  // Parse assembly file and fill instructions vector
  fillInstructions(inputAssembly);
  const std::size_t instructionsSize = m_instructions.size();

  // When programm counter has reached to the end of
  // instructions, execution should halt
  while(m_programCounter != instructionsSize) {
    // Get instruction pointed by @m_programCounter
    std::tuple<std::string, std::string, std::string> instructionWithOperands = fetch();
    
    // Set @m_instructionRegister equal to got instruction
    decode(instructionWithOperands);

    // Execute instruction placed in @m_instructionRegister
    execute();
  }
}

void RegisterMachineInterpreter::fillInstructions(
    const std::string& inputAssembly) {
  using StringIterator = std::istream_iterator<std::string>;

  std::fstream inputAssemblyStream(inputAssembly);

  if(!inputAssemblyStream.is_open()) {
    throw "can't open input assembly from Register Machine Interpreter\n";
  }

  auto line = std::string{};

  auto instruction = std::string{ NOTOP };
  auto operand1    = std::string{ NOTOP };
  auto operand2    = std::string{ NOTOP };

  while(std::getline(inputAssemblyStream, line)) {
    auto lineStream = std::istringstream{line};

    // Tokenize instruction line
    std::vector<std::string> lineVector{StringIterator{lineStream},
      StringIterator{}};

    // Skip empty lines
    // TODO Add comments support
    if(lineVector.empty()) {
      continue;
    }

    std::cout << "\nlineVector: ";
    debug_printContainer(std::begin(lineVector), std::end(lineVector));

    // Figure out is instruction supported
    instruction = lineVector[0];
    auto instrInfoIt = m_instructionToInfo.find(instruction);
    if(instrInfoIt == std::end(m_instructionToInfo)) {
      std::cerr << "\nUnsupported instruction: " << instruction << "\n";
      std::terminate();
    }

    // Get number of operands
    // Depending on it, instructions are splitted into groups
    // Those are do not work with operands, e.g. NOP
    // Those are work only with one, e.g. PRINT R1
    // And other usual instructions e.g. ADD R1 R4
    const auto gotOperandsNumber = lineVector.size() - 1;
    const auto correctOperandsNumber = instrInfoIt->second;
    if(gotOperandsNumber == correctOperandsNumber) {
      if(gotOperandsNumber == 0) {
        // Defaults values already set to operands
        // This branch is for consistency
        // Nothing to do here
      } else if(gotOperandsNumber == 1) {
        operand1 = lineVector[1];
      } else if(gotOperandsNumber == 2) {
        operand1 = lineVector[1];
        operand2 = lineVector[2];
      }
    } else {
      std::cerr << "\nWrong number of operands #" << gotOperandsNumber 
        << " for specified instruction: " << instruction << "\nAborting...\n";
      std::terminate();
    }

    m_instructions.push_back(std::make_tuple(instruction, operand1, operand2));
  }
}

void RegisterMachineInterpreter::fillInstructionToInfo() {
  m_instructionToInfo["LOAD"] = 2;
  m_instructionToInfo["ADD"] = 2;
  m_instructionToInfo["SUB"] = 2;
  m_instructionToInfo["MUL"] = 2;
  m_instructionToInfo["DIV"] = 2;
}

bool RegisterMachineInterpreter::isSupportedInstruction(const std::string& instrName) {
  return m_instructionToInfo.find(instrName) != std::end(m_instructionToInfo);
}

std::tuple<std::string, std::string, std::string> RegisterMachineInterpreter::fetch() {
  return m_instructions[m_programCounter++];
}

void RegisterMachineInterpreter::decode(
    const std::tuple<std::string, std::string, std::string>& instructionWithOperands) {

  m_instructionRegister = instructionWithOperands;
}

void RegisterMachineInterpreter::execute() {
    auto [instruction, operand1, operand2] = m_instructionRegister;
    auto instrInfoIt = m_instructionToInfo.find(instruction);

    const auto operandsNumber = instrInfoIt->second;

    if(operandsNumber == 0) {
      if(instruction == "HALT") {
        // It's a temporary solution
        // For future, consider a return value
        // In case of @false, stop execution of machine
        std::terminate();
      }
    } else if(operandsNumber == 1) {
      if(instruction == "PRINT") {
        
      } 
    } else if(operandsNumber == 2) {
      if(instruction == "LOAD") {
        // What to do here?
      } else if(instruction == "STORE") {
      } else if(instruction == "ADD") {
      } else if(instruction == "SUB") {
      } else if(instruction == "MUL") {
      } else if(instruction == "DIV") {
      }
    }
}
