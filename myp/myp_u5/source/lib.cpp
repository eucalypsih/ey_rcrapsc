#include "lib.hpp"

#include <fmt/format.h>

library::library()
    : name {fmt::format("{}", "myp")}
{
}

auto library::greet(std::string const& user_name) const -> std::string
{
  return fmt::format("Halo {}, selamat datang di {} (v{})!", user_name, name, version);
}

auto library::is_valid_name(std::string const& user_name) const -> bool
{
  // Validasi: tidak boleh kosong
  if (user_name.empty()) {
    return false;
  }
  
  // Validasi: tidak boleh hanya berisi karakter spasi/whitespace
  return !std::all_of(user_name.begin(), user_name.end(), [](unsigned char ch) {
    return std::isspace(ch);
  });
}
