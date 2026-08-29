#include <iostream>
#include <string>

#include "lib.hpp"

auto main() -> int
{
  auto const lib = library {};

  std::string input_name;
  std::cout << "Masukkan nama Anda: ";
  std::getline(std::cin, input_name);

  // Menggunakan fitur baru validasi
  if (!lib.is_valid_name(input_name)) {
    std::cout << "Kesalahan: Nama tidak boleh kosong!\n";
    return 1;
  }
  
  auto const personalized_message = lib.greet(input_name);
  
  std::cout << personalized_message << '\n';
  return 0;
}
