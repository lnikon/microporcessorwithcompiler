#pragma once

#include <vector>
#include <unordered_map>
#include <string>
#include <tuple>

struct RegisterMachineInterpreter {
	RegisterMachineInterpreter();
	void run(const std::string& inputAssembly);

private:
	std::vector<std::tuple<std::string, std::string, std::string>> m_instructions;

	std::size_t m_programCounter{0};
	std::tuple<std::string, std::string, std::string> m_instructionRegister;
	std::unordered_map<std::string, int> m_instructionToInfo;

private:
	// Main cycle implementation functions
	std::tuple<std::string, std::string, std::string> fetch();
	void decode(const std::tuple<std::string, std::string, std::string>&);
	void execute();

	// Helper functions
	void fillInstructions(const std::string& inputAssembly);
	void fillInstructionToInfo();
};
