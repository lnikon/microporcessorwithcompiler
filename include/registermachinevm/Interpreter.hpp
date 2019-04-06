#pragma once

#include <vector>
#include <unordered_map>
#include <set>
#include <string>
#include <tuple>
#include <algorithm>

namespace WordSize {

static const std::size_t B  = 8;
static const std::size_t W  = 8;
static const std::size_t DW = 8;
static const std::size_t QW = 8;

}

namespace utility {
  static bool isStringNumber(const std::string& str) {
    return !str.empty() 
      && std::all_of(std::begin(str), std::end(str), ::isdigit);
  }
};

struct RegisterEnvirnoment {
  private:
    std::unordered_map<std::string, std::string> m_registerToValue;
    std::set<std::string> m_supportedRegisters;

    // Map register name to its properties
    std::unordered_map< std::string, 
                        std::size_t> m_registerToProperties;

  public:
    const std::size_t REGISTERS_COUNT = 256;
    RegisterEnvirnoment() {
      for(std::size_t i = 0; i < REGISTERS_COUNT; i++) {
        m_supportedRegisters.insert("R" + std::to_string(i));
      }

      for(std::size_t i = 0; i < REGISTERS_COUNT; i++) {
        m_registerToProperties["R" + std::to_string(i)] = i;
      }
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
};

struct RegisterMachineInterpreter {
  RegisterMachineInterpreter();
  void run(const std::string& inputAssembly);

  private:
  // Main keywords to use over code
  const std::string NOTOP{ "NAN" };
  const std::size_t REG_COUNT = 256;

  std::vector<std::tuple<std::string, std::string, std::string>> m_instructions;
  RegisterEnvirnoment m_env{};
  std::vector<unsigned char> m_regs;
  std::size_t m_programCounter{0};
  std::tuple<std::string, std::string, std::string> m_instructionRegister;
  std::unordered_map< std::string, 
                      std::size_t> m_instructionToInfo;

  private:
  // Main cycle implementation functions
  std::tuple<std::string, std::string, std::string> fetch();
  void decode(const std::tuple<std::string, std::string, std::string>&);
  void execute();

  // Helper functions
  void fillInstructions(const std::string& inputAssembly);
  void fillInstructionToInfo();

  bool isSupportedInstruction(const std::string& instrName);
};
