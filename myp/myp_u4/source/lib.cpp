#include "lib.hpp"

#include <fmt/format.h>

library::library()
    : name {fmt::format("{}", "myp")}
{
}

auto library::greet(std::string const& user_name) const -> std::string
{
  return fmt::format("Halo {}, selamat datang di {}!", user_name, name);
}
