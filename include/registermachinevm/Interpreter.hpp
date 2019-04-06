#pragma once

#include <vector>
#include <unordered_map>
#include <set>
#include <string>
#include <tuple>
#include <algorithm>

namespace WordSize {

static const std::size_t B  = 8;
static const std::size_t W  = 2 * B;
static const std::size_t DW = 2 * W;
static const std::size_t QW = 2 * DW;

}

struct utility {
  static bool isStringNumber(const std::string& str) {
    return !str.empty() 
      && std::all_of(std::begin(str), std::end(str), ::isdigit);
  }
};

struct msg {
  static std::string incorrectOffset(const std::string& regName, std::size_t offset) {
    return std::string{"\nIncorrect offset #" + std::to_string(offset) 
                        + " for register " + regName + "\n"};
  }

  static std::string error(const std::string& msg) {
    return std::string {"\nError: " + msg + "\n"};
  }
};

struct RegisterEnvirnoment {
  private:
    std::unordered_map<std::string, std::string> m_registerToValue;
    std::set<std::string> m_supportedRegisters;

    // Map register name to its offset from regs array begin 
    std::unordered_map< std::string, 
                        std::size_t> m_registerToOffset;

  public:
    const std::size_t REGISTERS_COUNT = 256;
    RegisterEnvirnoment() {
      for(std::size_t i = 0; i < REGISTERS_COUNT; i++) {
        m_supportedRegisters.insert("R" + std::to_string(i));
        m_registerToOffset["R" + std::to_string(i)] = i;
      }
    }
    
    bool isRegisterOffsetCorrect(const std::string& reg, std::size_t offsetSize) {
      return !static_cast<bool>(m_registerToOffset[reg] % offsetSize);
    }

    bool isRegisterSupported(const std::string& reg) {
      return m_supportedRegisters.count(reg) != 0;
    }

    void addRegisterToValue(const std::string& reg, const std::string& val) {
      m_registerToValue[reg] = val;
    }

    std::optional<decltype(m_registerToValue)::iterator> getRegisterToValueIt(const std::string& reg) {
      if(isRegisterExists(reg)) {
        return m_registerToValue.find(reg);
      }

      return std::nullopt;
    }

    bool isRegisterExists(const std::string& registerName) {
      return m_registerToValue.find(registerName) != std::end(m_registerToValue);
    }

    bool isStringExtension(const std::string& str) {
      return str == "B" || str == "W" || str == "DW" || str == "QW";
    }

    std::size_t getExtensionSize(const std::string& ext) {
      if(ext == "B")  return WordSize::B;
      if(ext == "W")  return WordSize::W;
      if(ext == "DW") return WordSize::DW;
      if(ext == "QW") return WordSize::QW;

      return std::size_t {};
    }

    std::size_t getOffsetForReg(const std::string& reg) {
      return m_registerToOffset[reg];
    }
};

struct RegisterMachineInterpreter {
  RegisterMachineInterpreter();
  void run(const std::string& inputAssembly);

  private:
  // Main keywords to use over code
  const std::string NOTOP{ "NAN" };
  const std::size_t REG_COUNT = 256;

  std::vector<std::tuple<std::string, std::string, std::string, std::string>> m_instructions;
  RegisterEnvirnoment m_env{};
  std::vector<unsigned char> m_regs;
  std::size_t m_programCounter{0};
  std::tuple<std::string, std::string, std::string, std::string> m_instructionRegister;
  std::unordered_map< std::string, 
                      std::tuple<bool, std::size_t>> m_instructionToInfo;

  private:
  // Main cycle implementation functions
  std::tuple<std::string, std::string, std::string, std::string> fetch();
  void decode(const std::tuple<std::string, std::string, std::string, std::string>&);
  void execute();

  // Helper functions
  void fillInstructions(const std::string& inputAssembly);
  void fillInstructionToInfo();

  bool isSupportedInstruction(const std::string& instrName);

  int doBinaryInstruction(int value1, int value2, const std::string& instruction);
};
