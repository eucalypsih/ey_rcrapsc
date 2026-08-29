#pragma once

#include <string>

/**
 * @brief The core implementation of the executable
 *
 * This class makes up the library part of the executable, which means that the
 * main logic is implemented here. This kind of separation makes it easy to
 * test the implementation for the executable, because the logic is nicely
 * separated from the command-line logic implemented in the main function.
 */
struct library
{
  /**
   * @brief Simply initializes the name member to the name of the project
   */
  library();

  /**
   * @brief Menghasilkan pesan sapaan khusus untuk pengguna
   * @param user_name Nama pengguna yang akan disapa
   * @return String pesan sapaan format baru
   */
  auto greet(std::string const& user_name) const -> std::string;

  std::string name;
};
