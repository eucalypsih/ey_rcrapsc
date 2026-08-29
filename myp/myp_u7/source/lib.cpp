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

auto library::to_slug(std::string const& text) const -> std::string
{
  std::string result = text;
  
  // 1. Ubah semua huruf menjadi kecil (lowercase)
  std::transform(result.begin(), result.end(), result.begin(), [](unsigned char c) {
    return std::tolower(c);
  });

  // 2. Ganti spasi menjadi tanda hubung '-'
  std::transform(result.begin(), result.end(), result.begin(), [](char c) {
    return (c == ' ') ? '-' : c;
  });

  return result;
}

auto library::analyze_text(std::string const& text) const -> text_stats
{
  text_stats stats;

  for (unsigned char const ch : text) {
    if (std::isalpha(ch)) {
      stats.letters++;
    } else if (std::isdigit(ch)) {
      stats.digits++;
    } else if (std::isspace(ch)) {
      stats.spaces++;
    }
  }

  return stats;
}
