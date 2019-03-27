#pragma once

#include <vector>
#include <unordered_map>
#include <set>
#include <string>
#include <tuple>

// Register name -> Register value
// check register exists
// add register 
struct RegisterEnvirnoment {
  RegisterEnvirnoment() {
    m_supportedRegisters.insert("R1");
    m_supportedRegisters.insert("R2");
    m_supportedRegisters.insert("R3");
    m_supportedRegisters.insert("R4");
  }

  bool isRegisterSupported(const std::string& reg) {
    return m_supportedRegisters.count(reg) != 0;

  }
  bool registerExists(const std::string& registerName) {
    return m_registerToValue.find(registerName) != std::end(m_registerToValue);
  }

  private:
  std::unordered_map<std::string, std::string> m_registerToValue;
  std::set<std::string> m_supportedRegisters;
};

struct RegisterMachineInterpreter {
  RegisterMachineInterpreter();
  void run(const std::string& inputAssembly);

  private:
  // Main keywords to use over code
  const std::string NOTOP{ "NAN" };

  std::vector<std::tuple<std::string, std::string, std::string>> m_instructions;
  RegisterEnvirnoment m_env{};

  std::size_t m_programCounter{0};
  std::tuple<std::string, std::string, std::string> m_instructionRegister;
  std::unordered_map<std::string, std::size_t> m_instructionToInfo;

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
