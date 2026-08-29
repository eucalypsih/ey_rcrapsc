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

  std::string const title = "C Plus Plus Modern Paling Keren";
  
  // Menggunakan fitur baru to_slug()
  auto const slug_title = lib.to_slug(title);
  
  std::cout << "Judul Asli : " << title << '\n';
  std::cout << "Hasil Slug : " << slug_title << '\n';

  std::string const sample_text = "C++ Version 23 diluncurkan pada tahun 2023";
  
  // Menggunakan fitur baru analyze_text()
  auto const stats = lib.analyze_text(sample_text);
  
  std::cout << "Teks Analisis: \"" << sample_text << "\"\n";
  std::cout << "-----------------------------------\n";
  std::cout << "Jumlah Huruf : " << stats.letters << '\n';
  std::cout << "Jumlah Angka : " << stats.digits << '\n';
  std::cout << "Jumlah Spasi : " << stats.spaces << '\n';

  return 0;
}
