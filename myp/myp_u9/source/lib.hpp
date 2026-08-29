#pragma once

#include <string>

/**
 * @brief Menyimpan data statistik hasil analisis teks
 */
struct text_stats
{
  int letters = 0;
  int digits = 0;
  int spaces = 0;
};

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

  /**
   * @brief Memeriksa apakah nama pengguna valid (tidak kosong atau hanya spasi)
   * @param user_name Nama yang akan diperiksa
   * @return true jika valid, false jika tidak
   */
  auto is_valid_name(std::string const& user_name) const -> bool;

  /**
   * @brief Mengubah teks input menjadi format slug yang aman untuk URL
   * @param text Teks asli yang akan diubah
   * @return String dalam format lowercase dan menggunakan tanda hubung
   */
  auto to_slug(std::string const& text) const -> std::string;

  /**
   * @brief Menganalisis karakter di dalam teks
   * @param text Teks yang akan dianalisis
   * @return Struktur text_stats berisi jumlah huruf, angka, dan spasi
   */
  auto analyze_text(std::string const& text) const -> text_stats;

  /**
   * @brief Mengenkripsi teks menggunakan metode Caesar Cipher
   * @param text Teks asli yang akan dienkripsi
   * @param shift Jumlah pergeseran karakter (kunci enkripsi)
   * @return String teks yang telah disamarkan
   */
  auto encrypt_caesar(std::string const& text, int shift) const -> std::string;

  /**
   * @brief Mengambil penanda waktu sistem saat ini
   * @return String waktu dengan format "YYYY-MM-DD HH:MM:SS"
   */
  auto get_current_timestamp() const -> std::string;

  std::string name;
  std::string const version = "1.1.0"; // Fitur baru: Informasi versi pustaka
};
