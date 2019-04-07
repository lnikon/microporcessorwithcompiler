#include <fstream>
#include <sstream>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <string>
#include <limits>
#include <cstdlib>
#include <cstring>

#include "Interpreter.hpp"

template <class ForwardIterator>
void debug_printContainer(ForwardIterator begin, ForwardIterator end) {
  using value_type = typename std::decay<decltype(*begin)>::type;
  std::copy(begin, end, std::ostream_iterator<value_type>(std::cout, " "));
}

RegisterMachineInterpreter::RegisterMachineInterpreter() {
  m_regs.resize(REG_COUNT);
  m_flags.fill(false);
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
    std::tuple<std::string, std::string, std::string, std::string> instructionWithOperands = fetch();

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
  auto extension   = std::string{ NOTOP };
  auto operand1    = std::string{ NOTOP };
  auto operand2    = std::string{ NOTOP };

  while(std::getline(inputAssemblyStream, line)) {
    if(line.empty()) {
      m_instructions.push_back(std::make_tuple("NOP", "NOTOP", "NOTOP", "NOTOP"));
      continue;
    }

    if(line[0] == '#') {
      continue;
    }

    auto lineStream = std::istringstream{line};

    // Tokenize instruction line
    std::vector<std::string> lineVector{StringIterator{lineStream},
      StringIterator{}};
    const auto lineVectorSize = lineVector.size();

    // std::cout << "\nlineVector: ";
    // debug_printContainer(std::begin(lineVector), std::end(lineVector));
    // std::cout << "\n";

    // Figure out is instruction supported
    instruction = lineVector[0];
    auto instrInfoIt = m_instructionToInfo.find(instruction);
    if(!isSupportedInstruction(instruction)) {
      std::cerr << "\nUnsupported instruction: " << instruction << "\n";
      std::terminate();
    }

    if(lineVectorSize == 1) {
      m_instructions.push_back(std::make_tuple(instruction, extension, operand1, operand2));
      continue;
    }

    // Check, is second token is extension, if yes,
    // grab its value and save, otherwise set extension value
    // to its default value: DW
    const bool isTokenExt = m_env.isStringExtension(lineVector[1]);
    extension = "DW";
    if(isTokenExt) {
      extension = lineVector[1];
    }
    // Get number of operands
    // Depending on it, instructions are splitted into groups
    // Those are do not work with operands, e.g. NOP
    // Those are work only with one, e.g. PRINT R1
    // And other usual instructions e.g. ADD R1 R4
    auto gotOperandsNumber = std::size_t{}; 
    if(isTokenExt) {
      gotOperandsNumber = lineVector.size() - 2;
    } else {
      gotOperandsNumber = lineVector.size() - 1;
    }

    auto correctOperandsNumber = std::size_t{};
    std::tie(std::ignore, correctOperandsNumber) = instrInfoIt->second;
    if(gotOperandsNumber == correctOperandsNumber) {
      if(gotOperandsNumber == 0) {
        // Defaults values already set to operands
        // This branch is for consistency
        // Nothing to do here
      } else if(gotOperandsNumber == 1) {
        operand1 = lineVector[lineVectorSize - 1];
      } else if(gotOperandsNumber == 2) {
        operand1 = lineVector[lineVectorSize - 2];
        operand2 = lineVector[lineVectorSize - 1];
      }
    } else {
      std::cerr << "\nWrong number of operands #" << gotOperandsNumber 
        << " for specified instruction: " << instruction << "\nAborting...\n";
      std::terminate();
    }

    // std::cout << "\ninstruction: " << instruction << std::endl;
    // std::cout << "extension:   " << extension << std::endl;
    // std::cout << "operand1:    " << operand1 << std::endl;
    // std::cout << "operand2:    " << operand2 << std::endl;
    m_instructions.push_back(std::make_tuple(instruction, extension, operand1, operand2));
  }
}

void RegisterMachineInterpreter::fillInstructionToInfo() {
  m_instructionToInfo["HALT"]   = std::make_tuple(false, 0);
  m_instructionToInfo["NOP"]    = std::make_tuple(false, 0);
  m_instructionToInfo["END"]    = std::make_tuple(false, 0);
  m_instructionToInfo["POP"]    = std::make_tuple(false, 0);

  m_instructionToInfo["JUMP"]   = std::make_tuple(false, 1);
  m_instructionToInfo["PRINT"]  = std::make_tuple(true,  1);
  m_instructionToInfo["NOT"]    = std::make_tuple(true,  1);
  m_instructionToInfo["PUSH"]   = std::make_tuple(true,  1);

  m_instructionToInfo["LOAD"]   = std::make_tuple(true, 2);
  m_instructionToInfo["STORE"]  = std::make_tuple(true, 2);
  m_instructionToInfo["ASSIGN"] = std::make_tuple(true, 2);

  m_instructionToInfo["SWAP"]   = std::make_tuple(true, 2);
  m_instructionToInfo["MOV"]    = std::make_tuple(true, 2);

  m_instructionToInfo["ADD"]    = std::make_tuple(true, 2);
  m_instructionToInfo["SUB"]    = std::make_tuple(true, 2);
  m_instructionToInfo["MUL"]    = std::make_tuple(true, 2);
  m_instructionToInfo["DIV"]    = std::make_tuple(true, 2);
  m_instructionToInfo["AND"]    = std::make_tuple(true, 2);
  m_instructionToInfo["NAND"]   = std::make_tuple(true, 2);
  m_instructionToInfo["OR"]     = std::make_tuple(true, 2);
  m_instructionToInfo["NOR"]    = std::make_tuple(true, 2);
  m_instructionToInfo["XOR"]    = std::make_tuple(true, 2);
}

bool RegisterMachineInterpreter::isSupportedInstruction(const std::string& instrName) {
  return m_instructionToInfo.find(instrName) != std::end(m_instructionToInfo);
}

int RegisterMachineInterpreter::doBinaryInstruction(int value1, int value2, const std::string& instruction) {
  int result = 0;
  if(instruction == "ADD") {
    result = value1 + value2;
    setCarryFlagAdd(value1, value2);
    setZeroFlag(result);
    setSignFlag(result);
    setOverflowFlag(result);

  } else if(instruction == "SUB") {
    result = value1 - value2;
    setCarryFlagMul(value1, value2);
    setZeroFlag(result);
    setSignFlag(result);
    setOverflowFlag(result);

  } else if(instruction == "MUL") {
    result = value1 * value2;
    setCarryFlagMul(value1, value2);
    setZeroFlag(result);
    setSignFlag(result);
    setOverflowFlag(result);

  } else if(instruction == "DIV") {
    result = value1 / value2;
    setZeroFlag(result);

  } else if(instruction == "AND") {
    result = value1 & value2;

  } else if(instruction == "NAND") {
    result = !(value1 & value2);

  } else if(instruction == "OR") {
    result = value1 | value2;

  } else if(instruction == "NOR") {
    result = !(value1 | value2);

  } else if(instruction == "XOR") {
    result = value1 ^ value2;
  }

  return result;
}

void RegisterMachineInterpreter::printCurrentStepInfo() {
  auto lineNumber = m_programCounter - 1;
  auto [instruction, extension, op1, op2] = m_instructions[lineNumber];

  std::string op1valStr = NOTOP;
  if(m_env.isRegisterSupported(op1)) {

    int op1val = 0;
    std::size_t op1offset = m_env.getOffsetForReg(op1);
    unsigned int *puint = reinterpret_cast<unsigned int *>(&m_regs[0]);
    std::memcpy(&op1val, puint + op1offset, sizeof(op1val));

    op1valStr = std::to_string(op1val);
  }

  std::string op2valStr = NOTOP;
  if(m_env.isRegisterSupported(op2)) {
    int op2val = 0;
    std::size_t op2offset = m_env.getOffsetForReg(op2);
    unsigned int *puint = reinterpret_cast<unsigned int *>(&m_regs[0]);
    std::memcpy(&op2val, puint + op2offset, sizeof(op2val));

    op2valStr = std::to_string(op2val);
  }

  std::cout << "\nline_number: " << lineNumber 
    << "\ninstruction: " << instruction 
    << "\n<" << op1 << " : " << op1valStr << ">"
    << "\n<" << op2 << " : " << op2valStr << ">\n";
}

inline void RegisterMachineInterpreter::printFlags() const {
  std::cout << std::boolalpha 
            << "<|CF: " << m_flags[0]
            << "|ZF: " << m_flags[1]
            << "|SF: " << m_flags[2]
            << "|OF: " << m_flags[3] << ">" << std::endl;
}

inline void RegisterMachineInterpreter::setCarryFlagAdd(int op1, int op2) {
  unsigned int test;
  if(__builtin_add_overflow(op1, op2, &test)) {
    m_flags[0] = true;
    return;
  }

  m_flags[0] = false;
}

inline void RegisterMachineInterpreter::setCarryFlagSub(int op1, int op2) {
  unsigned int test;
  if(__builtin_sub_overflow(op1, op2, &test)) {
    m_flags[0] = true;
    return;
  }

  m_flags[0] = false;
}

inline void RegisterMachineInterpreter::setCarryFlagMul(int op1, int op2) {
  unsigned int test;
  if(__builtin_mul_overflow(op1, op2, &test)) {
    m_flags[0] = true;
    return;
  }

  m_flags[0] = false;
}

inline void RegisterMachineInterpreter::setCarryFlagDiv(int, int) { }

inline void RegisterMachineInterpreter::setZeroFlag(int result) {
  if(result == 0) {
    m_flags[1] = true;
    return;
  }
  
  m_flags[1] = false;
}

inline void RegisterMachineInterpreter::setSignFlag(int result) {
  int bits = sizeof(int) * 8;
  int msb = 1 << (bits - 1);
  m_flags[2] = result & msb;
}

inline void RegisterMachineInterpreter::setOverflowFlag(int) {
  m_flags[3] = m_flags[0];
}

std::tuple<std::string, std::string, std::string, std::string> RegisterMachineInterpreter::fetch() {
  return m_instructions[m_programCounter++];
}

void RegisterMachineInterpreter::decode(
    const std::tuple<std::string, std::string, std::string, std::string>& instructionWithOperands) {

  m_instructionRegister = instructionWithOperands;
}

void RegisterMachineInterpreter::execute() {
  auto [instruction, extension, operand1, operand2] = m_instructionRegister;
  // std::cout << "\ninstruction: " << instruction << std::endl;
  // std::cout << "extension:   " << extension << std::endl;
  // std::cout << "operand1:    " << operand1 << std::endl;
  // std::cout << "operand2:    " << operand2 << std::endl;

  auto instrInfoIt = m_instructionToInfo.find(instruction);
  const auto infoTuple = instrInfoIt->second;
  const auto [isExt, operandsNumber] = infoTuple;

  if(operandsNumber == 0) {
    if(instruction == "HALT") {
      std::terminate();
    } else if(instruction == "NOP") {
    } else if(instruction == "END") {
      std::cout << "Programm exited...\n";
      std::exit(0);
    } else if(instruction == "POP") {
      int value = 0;
      std::size_t offset = 0;
      auto value_offset = m_stack.top();
      value = value_offset.first;
      offset = value_offset.second;

      m_stack.pop();

      unsigned int *puch = reinterpret_cast<unsigned int*>(&m_regs[0]);
      std::memcpy(puch + offset, &value, sizeof(value));
    }
  } else if(operandsNumber == 1) {
    if(instruction == "PRINT") {
      bool isRegisterSupported = m_env.isRegisterSupported(operand1);
      if(!isRegisterSupported) {
        std::cerr << msg::error("Register " + operand1 + " is not supported");
        throw;
      }

      auto offsetForExt = m_env.getExtensionSize(extension);
      if(!m_env.isRegisterOffsetCorrect(operand1, offsetForExt)) {
        std::cerr << msg::incorrectOffset(operand1, offsetForExt);
        throw;
      }

      auto offsetForReg = m_env.getOffsetForReg(operand1);

      // std::cout << "\n----> <offsetForReg` " << operand1 << ": " << offsetForReg << ">" << std::endl;
      // std::cout << "----> <offsetForExt` " << extension << ": " << offsetForExt  << ">" << std::endl;

      unsigned int regValue = 0;
      if(offsetForExt == WordSize::B) {
        unsigned char *puchar = &m_regs[0];
        std::memcpy(&regValue, puchar + offsetForReg, sizeof(regValue));

      } else if(offsetForExt == WordSize::W) {
        unsigned short *pushort = reinterpret_cast<unsigned short *>(&m_regs[0]);
        std::memcpy(&regValue, pushort + offsetForReg, sizeof(regValue));

      } else if(offsetForExt == WordSize::DW) {
        unsigned int *puint = reinterpret_cast<unsigned int*>(&m_regs[0]);
        std::memcpy(&regValue, puint + offsetForReg, sizeof(regValue));

      } else if(offsetForExt == WordSize::QW) {
        unsigned long *pulong = reinterpret_cast<unsigned long*>(&m_regs[0]);
        std::memcpy(&regValue, pulong + offsetForReg, sizeof(regValue));
      } else {
        std::cout << msg::error("Unkown extension: " + extension);
        throw;
      }
      std::cout << operand1 << " = " << regValue << std::endl;
      // bool isRegisterSupported = m_env.isRegisterSupported(operand1);
      // if(isRegisterSupported) {
      //   auto offsetForReg = m_env.getOffsetForReg(operand1);
      //   int regValue = 0;
      //   unsigned int *puch = reinterpret_cast<unsigned int *>(&m_regs[0]);
      //   std::memcpy(&regValue, puch + offsetForReg, sizeof(regValue));

      //   std::cout << operand1 << " = " << regValue << std::endl;
      // }
    } else if(instruction == "JUMP") {
      bool isOperandNumber = utility::isStringNumber(operand1);
      if(isOperandNumber) {
        std::size_t instructionIndex = std::stol(operand1) - 1;
        if(instructionIndex > m_instructions.size()) {
          std::cout << "Incorrect line number: " << instructionIndex << "\n";
          std::terminate();
        }

        std::cout << "setting programm counter to: " << instructionIndex << std::endl;
        m_programCounter = instructionIndex;
      }  
    } else if(instruction == "NOT") {
      bool isRegisterSupported = m_env.isRegisterSupported(operand1);
      if(!isRegisterSupported) {
        std::cout << msg::error("Register " + operand1 + " does not supported\n");
        std::terminate();
      }

      auto offsetForExt = m_env.getExtensionSize(extension);
      if(!m_env.isRegisterOffsetCorrect(operand1, offsetForExt)) {
        std::cerr << msg::incorrectOffset(operand1, offsetForExt);
        throw;
      }

      auto offsetForReg = m_env.getOffsetForReg(operand1);

      // std::cout << "\n----> <offsetForReg` " << operand1 << ": " << offsetForReg << ">" << std::endl;
      // std::cout << "----> <offsetForExt` " << extension << ": " << offsetForExt  << ">" << std::endl;

      if(offsetForExt == WordSize::B) {
        unsigned char *puch = &m_regs[0];

        unsigned int operand1Value = 0;
        std::memcpy(&operand1Value, puch + offsetForReg, sizeof(operand1Value));
        operand1Value = ~operand1Value;
        std::memcpy(puch + offsetForReg, &operand1Value, sizeof(operand1Value));  

      } else if(offsetForExt == WordSize::W) {
        unsigned short *puch = reinterpret_cast<unsigned short *>(&m_regs[0]);

        unsigned int operand1Value = 0;
        std::memcpy(&operand1Value, puch + offsetForReg, sizeof(operand1Value));
        operand1Value = ~operand1Value;
        std::memcpy(puch + offsetForReg, &operand1Value, sizeof(operand1Value));  

      } else if(offsetForExt == WordSize::DW) {
        unsigned int *puch = reinterpret_cast<unsigned int*>(&m_regs[0]);

        unsigned int operand1Value = 0;
        std::memcpy(&operand1Value, puch + offsetForReg, sizeof(operand1Value));
        operand1Value = ~operand1Value;
        std::memcpy(puch + offsetForReg, &operand1Value, sizeof(operand1Value));  

      } else if(offsetForExt == WordSize::QW) {
        unsigned long *puch = reinterpret_cast<unsigned long*>(&m_regs[0]);

        unsigned int operand1Value = 0;
        std::memcpy(&operand1Value, puch + offsetForReg, sizeof(operand1Value));
        operand1Value = ~operand1Value;
        std::memcpy(puch + offsetForReg, &operand1Value, sizeof(operand1Value));  

      } else {
        std::cout << msg::error("Unkown extension: " + extension);
        throw;
      }

    } else if(instruction == "PUSH") {
      bool isRegisterSupported = m_env.isRegisterSupported(operand1);
      if(!isRegisterSupported) {
        std::cout << msg::error("Register " + operand1 + " does not supported\n");
        std::terminate();
      }

      auto offsetForExt = m_env.getExtensionSize(extension);
      if(!m_env.isRegisterOffsetCorrect(operand1, offsetForExt)) {
        std::cerr << msg::incorrectOffset(operand1, offsetForExt);
        throw;
      }

      auto offsetForReg = m_env.getOffsetForReg(operand1);

      unsigned int *puch = reinterpret_cast<unsigned int*>(&m_regs[0]);

      unsigned int operand1Value = 0;
      std::memcpy(&operand1Value, puch + offsetForReg, sizeof(operand1Value));
      m_stack.push(std::make_pair(operand1Value, offsetForReg));
    }
  } else if(operandsNumber == 2) {
    /* Required checks */
    /* First operand always should be register */
    bool isFirstOperandSupportedReg = m_env.isRegisterSupported(operand1);
    if(!isFirstOperandSupportedReg) {
      std::cout << "Register " << operand1 << " does not exists\n";
      std::terminate();
    }

    /* Second register can be register OR number */
    bool isSecondOperandRegister = m_env.isRegisterSupported(operand2);
    bool isSecondOperandNumber = utility::isStringNumber(operand2);
    /* END Required checks */

    if(instruction == "LOAD") {
      // What to do here?
    } else if(instruction == "ASSIGN") {
      if(!isSecondOperandNumber) {
        throw "\nSecond operand of ASSIGN should be a number\n";
      }

      auto offsetForExt = m_env.getExtensionSize(extension);
      if(!m_env.isRegisterOffsetCorrect(operand1, offsetForExt)) {
        std::cerr << msg::incorrectOffset(operand1, offsetForExt);
        throw;
      }

      auto offsetForReg = m_env.getOffsetForReg(operand1);

      // std::cout << "\n----> <offsetForReg` " << operand1 << ": " << offsetForReg << ">" << std::endl;
      // std::cout << "----> <offsetForExt` " << extension << ": " << offsetForExt  << ">" << std::endl;

      if(offsetForExt == WordSize::B) {
        unsigned char *puch = &m_regs[0];

        auto operand2Value = std::stoi(operand2); 
        std::memcpy(puch + offsetForReg, &operand2Value, sizeof(operand2Value));  

      } else if(offsetForExt == WordSize::W) {
        unsigned short *puch = reinterpret_cast<unsigned short *>(&m_regs[0]);
        auto operand2Value = std::stoi(operand2); 
        std::memcpy(puch + offsetForReg, &operand2Value, sizeof(operand2Value));  

      } else if(offsetForExt == WordSize::DW) {
        unsigned int *puch = reinterpret_cast<unsigned int*>(&m_regs[0]);
        auto operand2Value = std::stoi(operand2); 
        std::memcpy(puch + offsetForReg, &operand2Value, sizeof(operand2Value));  

      } else if(offsetForExt == WordSize::QW) {
        unsigned long *puch = reinterpret_cast<unsigned long*>(&m_regs[0]);
        auto operand2Value = std::stoi(operand2); 
        std::memcpy(puch + offsetForReg, &operand2Value, sizeof(operand2Value));  
      } else {
        std::cout << msg::error("Unkown extension: " + extension);
        throw;
      }
    } else if(instruction == "ADD"  ||
        instruction == "SUB"  ||
        instruction == "MUL"  ||
        instruction == "DIV"  ||
        instruction == "AND"  ||
        instruction == "NAND" ||
        instruction == "NOR"  ||
        instruction == "OR"   ||
        instruction == "XOR") {
      if(!isSecondOperandRegister) {
        std::cout << msg::error("Operands of ADD always should be registers");
        throw;
      }

      bool isFirstOperandSupportedReg = m_env.isRegisterSupported(operand1);
      if(!isFirstOperandSupportedReg) {
        std::cout << msg::error("Operand " + operand1 + " is not supported");
        throw;
      }

      bool isSecondOperandSupportedReg = m_env.isRegisterSupported(operand2);
      if(!isSecondOperandSupportedReg) {
        std::cout << msg::error("Operand " + operand2 + " is not supported");
        throw;
      } 

      auto offsetForExt = m_env.getExtensionSize(extension);
      unsigned char*  puchar  = nullptr;
      unsigned short* pushort = nullptr;
      unsigned int*   puint   = nullptr;
      unsigned long*  pulong  = nullptr;

      unsigned int operand1Value = 0;
      unsigned int operand2Value = 0;

      std::size_t offsetForOp1 = 0;
      std::size_t offsetForOp2 = 0;

      if(offsetForExt == WordSize::B) {
        puchar = &m_regs[0];

        offsetForOp1 = m_env.getOffsetForReg(operand1);
        std::memcpy(&operand1Value, puchar + offsetForOp1, sizeof(operand1Value));

        offsetForOp2 = m_env.getOffsetForReg(operand2);
        std::memcpy(&operand2Value, puchar + offsetForOp2, sizeof(operand2Value));
      } else if(offsetForExt == WordSize::W) {
        pushort = reinterpret_cast<unsigned short *>(&m_regs[0]);

        operand1Value = 0;
        operand2Value = 0;

        offsetForOp1 = m_env.getOffsetForReg(operand1);
        std::memcpy(&operand1Value, puchar + offsetForOp1, sizeof(operand1Value));

        offsetForOp2 = m_env.getOffsetForReg(operand2);
        std::memcpy(&operand2Value, puchar + offsetForOp2, sizeof(operand2Value));
      } else if(offsetForExt == WordSize::DW) {
        puint = reinterpret_cast<unsigned int*>(&m_regs[0]);

        operand1Value = 0;
        operand2Value = 0;

        offsetForOp1 = m_env.getOffsetForReg(operand1);
          std::memcpy(&operand1Value, puint + offsetForOp1, sizeof(operand1Value));

          offsetForOp2 = m_env.getOffsetForReg(operand2);
          std::memcpy(&operand2Value, puint + offsetForOp2, sizeof(operand2Value));
        } else if(offsetForExt == WordSize::QW) {
          pulong = reinterpret_cast<unsigned long*>(&m_regs[0]);

          operand1Value = 0;
          operand2Value = 0;

          offsetForOp1 = m_env.getOffsetForReg(operand1);
          std::memcpy(&operand1Value, pulong + offsetForOp1, sizeof(operand1Value));

          offsetForOp2 = m_env.getOffsetForReg(operand2);
          std::memcpy(&operand2Value, pulong + offsetForOp2, sizeof(operand2Value));
        } else {
          std::cout << msg::error("Unkown extension: " + extension);
          throw;
        }

      int result = doBinaryInstruction(operand1Value, operand2Value, instruction);

      if(offsetForExt == WordSize::B) {
        std::memcpy(puchar + offsetForOp1, &result, sizeof(operand2Value));

      } else if(offsetForExt == WordSize::W) {
        std::memcpy(pushort + offsetForOp1, &result, sizeof(result));

      } else if(offsetForExt == WordSize::DW) {
        std::memcpy(puint + offsetForOp1, &result, sizeof(result));

      } else if(offsetForExt == WordSize::QW) {
        std::memcpy(pulong + offsetForOp1, &result, sizeof(result));
      }
    }  else if(instruction == "SWAP") {
      if(!isSecondOperandRegister) {
        throw "Operands of ADD always should be registers";
      }

      auto offsetForExt = m_env.getExtensionSize(extension);
      unsigned char* puchar = nullptr;
      unsigned short* pushort = nullptr;
      unsigned int* puint = nullptr;
      unsigned long* pulong = nullptr;

      unsigned int operand1Value = 0;
      unsigned int operand2Value = 0;

      std::size_t offsetForOp1 = 0;
      std::size_t offsetForOp2 = 0;

      if(offsetForExt == WordSize::B) {
        puchar = &m_regs[0];

        operand1Value = 0;
        operand2Value = 0;

        offsetForOp1 = m_env.getOffsetForReg(operand1);
        std::memcpy(&operand1Value, puchar + offsetForOp1, sizeof(operand1Value));

        offsetForOp2 = m_env.getOffsetForReg(operand2);
        std::memcpy(&operand2Value, puchar + offsetForOp2, sizeof(operand2Value));
      } else if(offsetForExt == WordSize::W) {
        pushort = reinterpret_cast<unsigned short *>(&m_regs[0]);

        operand1Value = 0;
        operand2Value = 0;

        offsetForOp1 = m_env.getOffsetForReg(operand1);
        std::memcpy(&operand1Value, pushort + offsetForOp1, sizeof(operand1Value));

        offsetForOp2 = m_env.getOffsetForReg(operand2);
        std::memcpy(&operand2Value, pushort + offsetForOp2, sizeof(operand2Value));
      } else if(offsetForExt == WordSize::DW) {
        puint = reinterpret_cast<unsigned int*>(&m_regs[0]);

        operand1Value = 0;
        operand2Value = 0;

        offsetForOp1 = m_env.getOffsetForReg(operand1);
        std::memcpy(&operand1Value, puint + offsetForOp1, sizeof(operand1Value));

        offsetForOp2 = m_env.getOffsetForReg(operand2);
        std::memcpy(&operand2Value, puint + offsetForOp2, sizeof(operand2Value));
      } else if(offsetForExt == WordSize::QW) {
        pulong = reinterpret_cast<unsigned long*>(&m_regs[0]);

        operand1Value = 0;
        operand2Value = 0;

        offsetForOp1 = m_env.getOffsetForReg(operand1);
        std::memcpy(&operand1Value, pulong + offsetForOp1, sizeof(operand1Value));

        offsetForOp2 = m_env.getOffsetForReg(operand2);
        std::memcpy(&operand2Value, pulong + offsetForOp2, sizeof(operand2Value));
      } else {
        std::cout << msg::error("Unkown extension: " + extension);
        throw;
      }

      if(offsetForExt == WordSize::B) {
        std::memcpy(puchar + offsetForOp1, &operand2Value, sizeof(operand2Value));  
        std::memcpy(puchar + offsetForOp2, &operand1Value, sizeof(operand1Value));  
      } else if(offsetForExt == WordSize::W) {
        std::memcpy(pushort + offsetForOp1, &operand2Value, sizeof(operand2Value));  
        std::memcpy(pushort + offsetForOp2, &operand1Value, sizeof(operand1Value));  
      } else if(offsetForExt == WordSize::DW) {
        std::memcpy(puint + offsetForOp1, &operand2Value, sizeof(operand2Value));  
        std::memcpy(puint + offsetForOp2, &operand1Value, sizeof(operand1Value));  
      } else if(offsetForExt == WordSize::QW) {
        std::memcpy(pulong + offsetForOp1, &operand2Value, sizeof(operand2Value));  
        std::memcpy(pulong + offsetForOp2, &operand1Value, sizeof(operand1Value));  
      }         
    } else if(instruction == "MOV") {
      bool isFirstOperandSupportedReg = m_env.isRegisterSupported(operand1);
      if(!isFirstOperandSupportedReg) {
        std::cout << msg::error("Operand " + operand1 + " is not supported");
        throw;
      }

      bool isSecondOperandSupportedReg = m_env.isRegisterSupported(operand2);
      if(!isSecondOperandSupportedReg) {
        std::cout << msg::error("Operand " + operand2 + " is not supported");
        throw;
      } 

      auto offsetForExt = m_env.getExtensionSize(extension);
      unsigned char*  puchar  = nullptr;
      unsigned short* pushort = nullptr;
      unsigned int*   puint   = nullptr;
      unsigned long*  pulong  = nullptr;

      unsigned int operand1Value = 0;
      unsigned int operand2Value = 0;

      std::size_t offsetForOp1 = 0;
      std::size_t offsetForOp2 = 0;

      if(offsetForExt == WordSize::B) {
        puchar = &m_regs[0];

        offsetForOp1 = m_env.getOffsetForReg(operand1);
        std::memcpy(&operand1Value, puchar + offsetForOp1, sizeof(operand1Value));

        offsetForOp2 = m_env.getOffsetForReg(operand2);
        std::memcpy(&operand2Value, puchar + offsetForOp2, sizeof(operand2Value));
      } else if(offsetForExt == WordSize::W) {
        pushort = reinterpret_cast<unsigned short *>(&m_regs[0]);

        operand1Value = 0;
        operand2Value = 0;

        offsetForOp1 = m_env.getOffsetForReg(operand1);
        std::memcpy(&operand1Value, puchar + offsetForOp1, sizeof(operand1Value));

        offsetForOp2 = m_env.getOffsetForReg(operand2);
        std::memcpy(&operand2Value, puchar + offsetForOp2, sizeof(operand2Value));
      } else if(offsetForExt == WordSize::DW) {
        puint = reinterpret_cast<unsigned int*>(&m_regs[0]);

        operand1Value = 0;
        operand2Value = 0;

        offsetForOp1 = m_env.getOffsetForReg(operand1);
        std::memcpy(&operand1Value, puint + offsetForOp1, sizeof(operand1Value));

        offsetForOp2 = m_env.getOffsetForReg(operand2);
        std::memcpy(&operand2Value, puint + offsetForOp2, sizeof(operand2Value));
      } else if(offsetForExt == WordSize::QW) {
        pulong = reinterpret_cast<unsigned long*>(&m_regs[0]);

        operand1Value = 0;
        operand2Value = 0;

        offsetForOp1 = m_env.getOffsetForReg(operand1);
        std::memcpy(&operand1Value, pulong + offsetForOp1, sizeof(operand1Value));

        offsetForOp2 = m_env.getOffsetForReg(operand2);
        std::memcpy(&operand2Value, pulong + offsetForOp2, sizeof(operand2Value));
      } else {
        std::cout << msg::error("Unkown extension: " + extension);
        throw;
      }

      if(offsetForExt == WordSize::B) {
        std::memcpy(puchar + offsetForOp1, &operand2Value, sizeof(operand2Value));

      } else if(offsetForExt == WordSize::W) {
        std::memcpy(pushort + offsetForOp1, &operand2Value, sizeof(operand2Value));

      } else if(offsetForExt == WordSize::DW) {
        std::memcpy(puint + offsetForOp1, &operand2Value, sizeof(operand2Value));

      } else if(offsetForExt == WordSize::QW) {
        std::memcpy(pulong + offsetForOp1, &operand2Value, sizeof(operand2Value));
      }
    }
  }
  
  printCurrentStepInfo();
  printFlags();
  std::cout << "Hit enter to continue...\n";
  std::cin.ignore(std::numeric_limits <std::streamsize> ::max(), '\n');
}
