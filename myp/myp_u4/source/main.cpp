#include <iostream>
#include <string>

#include "lib.hpp"

auto main() -> int
{
  auto const lib = library {};
  auto const message = "Hello from " + lib.name + "!";
  std::cout << message << '\n';

  // Menggunakan fitur baru greet()
  auto const personalized_message = lib.greet("Developer");
  
  std::cout << personalized_message << '\n';
  return 0;
}
