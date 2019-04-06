#include <iostream>
#include <string>
#include <exception>

#include "Interpreter.hpp"

int main() {
  RegisterMachineInterpreter rgvm;

  try {
    const std::string& filename = "two_operand_asm.myasm";
    rgvm.run(filename);
  } catch(const char* pmsg) {
    std::cout << pmsg << "\n";
  } catch(const std::exception& e) {
    std::cout << e.what() << "\n";
  }

  return 0;
}
