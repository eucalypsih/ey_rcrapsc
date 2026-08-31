
qs:
```bash
$ cat source/lib.hpp
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

  std::string name;
};

$ cat source/lib.cpp
#include "lib.hpp"

#include <fmt/format.h>

library::library()
    : name {fmt::format("{}", "myp")}
{
}

$ cat source/main.cpp
#include <iostream>
#include <string>

#include "lib.hpp"

auto main() -> int
{
  auto const lib = library {};
  auto const message = "Hello from " + lib.name + "!";
  std::cout << message << '\n';
  return 0;
}

```

<br>

a-ai:
Kode sumber yang Anda bagikan menunjukkan struktur proyek C++ modern yang memisahkan logika utama ke dalam pustaka (library) agar mudah diuji (testable).

Berikut adalah ringkasan dari kode Anda:

Struktur Komponen Kode
- `source/lib.hpp`: Berisi deklarasi `struct library`. Menggunakan `#pragma once` sebagai *include guard* dan mendefinisikan satu anggota variabel `name` bertipe `std::string`.
- `source/lib.cpp`: Mengimplementasikan konstruktor `library()`. Konstruktor ini mengisi variabel `name` dengan teks `"myp"` menggunakan pustaka pemformatan modern `{fmt} (fmt::format)`.
- `source/main.cpp`: Titik masuk program (`main`). Membuat objek `library`, menggabungkan teks `"Hello from myp!"`, lalu mencetaknya ke `std::out`.

Alur Eksekusi Program
Jika kode ini dikompilasi dan dijalankan, output terminal yang dihasilkan adalah:
```
Hello from myp!

```
Jika Anda ingin melanjutkan pengembangan proyek ini, saya bisa membantu Anda untuk:
- Membuat berkas `CMakeLists.txt` agar kode ini bisa dikompilasi dengan mudah.
- Membuat skrip otomatisasi pengujian menggunakan kerangka kerja seperti **Catch2** atau **Google Test**.
- Menambahkan fitur baru ke dalam struktur pustaka yang sudah ada.
Bagaimana Anda ingin melanjutkan proyek ini?

<br>

qc:
Menambahkan fitur baru ke dalam struktur pustaka yang sudah ada.

<br>

a-ai:
Untuk menambahkan fitur baru ke dalam struktur pustaka yang sudah ada, kita akan **membuat fungsi baru yang menerima argumen nama pengguna, lalu mengembalikan pesan sapaan yang dipersonalisasi**.

Berikut adalah perubahan kode untuk menambahkan fitur tersebut:
1. Perbarui `source/lib.hpp`
Tambahkan deklarasi fungsi baru `greet` di dalam `struct library`.
```cpp
#pragma once

#include <string>

/**
 * @brief The core implementation of the executable
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

```

2. Perbarui `source/lib.cpp`
Implementasikan fungsi `greet` memanfaatkan pustaka `{fmt}` yang sudah Anda gunakan.
```cpp
#include "lib.hpp"

#include <fmt/core.h>

library::library()
    : name {fmt::format("{}", "myp")}
{
}

auto library::greet(std::string const& user_name) const -> std::string
{
  return fmt::format("Halo {}, selamat datang di {}!", user_name, name);
}

```

3. Perbarui `source/main.cpp`
Ubah titik masuk program untuk menggunakan fitur baru ini.
```cpp
#include <iostream>
#include <string>

#include "lib.hpp"

auto main() -> int
{
  auto const lib = library {};
  
  // Menggunakan fitur baru greet()
  auto const personalized_message = lib.greet("Developer");
  
  std::cout << personalized_message << '\n';
  return 0;
}

```
Output Program Setelah Perubahan
Jika dijalankan, program sekarang akan menghasilkan output:
```bash
Halo Developer, selamat datang di myp!

```

---

Jika Anda tertarik, saya bisa membantu untuk melangkah ke tahap berikutnya. Apakah Anda ingin:
- Membuat berkas `CMakeLists.txt` untuk mengompilasi kode dan pustaka `{fmt}` ini?
- Membuat berkas pengujian (unit test) untuk memastikan fungsi `greet` berjalan dengan benar?

<br>

qc:
Menambahkan fitur baru ke dalam struktur pustaka yang sudah ada.

<br>

a-ai:
Mari kita tambahkan fitur baru lainnya untuk membuat pustaka Anda lebih fungsional. Kita akan **menambahkan fitur pencatat versi (versioning) dan fungsi validasi input sederhana untuk memastikan nama pengguna tidak kosong**.

Berikut adalah pembaruan kode terstruktur untuk fitur-fitur baru tersebut:

1. Perbarui `source/lib.hpp`
Kita tambahkan konstanta versi dan fungsi `is_valid_name` untuk memvalidasi input.
```cpp
#pragma once

#include <string>

/**
 * @brief The core implementation of the executable
 */
struct library
{
  library();

  /**
   * @brief Menghasilkan pesan sapaan khusus untuk pengguna
   */
  auto greet(std::string const& user_name) const -> std::string;

  /**
   * @brief Memeriksa apakah nama pengguna valid (tidak kosong atau hanya spasi)
   * @param user_name Nama yang akan diperiksa
   * @return true jika valid, false jika tidak
   */
  auto is_valid_name(std::string const& user_name) const -> bool;

  std::string name;
  std::string const version = "1.1.0"; // Fitur baru: Informasi versi pustaka
};

```

2. Perbarui `source/lib.cpp`
Kita implementasikan logika validasi menggunakan fungsi standar C++.
```cpp
#include "lib.hpp"

#include <fmt/core.h>
#include <algorithm>
#include <cctype>

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

```

3. Perbarui `source/main.cpp`
Kita integrasikan logika penanganan input dan validasi di dalam fungsi `main`.
```cpp
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

```
Simulasi Jalannya Program
- Skenario Input Valid:
```bash
Masukkan nama Anda: Developer
Halo Andi, selamat datang di myp (v1.1.0)!

```
- Skenario Input Kosong:
```bash
Masukkan nama Anda: 
Kesalahan: Nama tidak boleh kosong!

```

<br>

q-ai:
Sekarang struktur pustaka Anda sudah lebih kaya fitur dan interaktif. Langkah apa yang ingin Anda ambil berikutnya?
- Membuat berkas `CMakeLists.txt` untuk otomatisasi kompilasi proyek ini?
- Menulis berkas unit test untuk menguji fungsi `is_valid_name` dengan berbagai variasi input?

<br>

qc:
Menambahkan fitur baru ke dalam struktur pustaka yang sudah ada.

<br>

a-ai:
Mari kita tambahkan fitur pengolah teks (utility text) ke dalam pustaka Anda. Kita akan membuat fitur untuk **mengubah teks menjadi format URL-safe (slug)**. Fitur ini sangat berguna untuk membuat tautan web bersih, mengubah semua huruf menjadi kecil, dan mengganti spasi menjadi tanda hubung (-).

Berikut adalah pembaruan kode untuk fitur baru ini:
1. Perbarui `source/lib.hpp`
Tambahkan deklarasi fungsi `to_slug` di dalam struktur pustaka.
```cpp
#pragma once

#include <string>

/**
 * @brief The core implementation of the executable
 */
struct library
{
  library();

  auto greet(std::string const& user_name) const -> std::string;
  auto is_valid_name(std::string const& user_name) const -> bool;

  /**
   * @brief Mengubah teks input menjadi format slug yang aman untuk URL
   * @param text Teks asli yang akan diubah
   * @return String dalam format lowercase dan menggunakan tanda hubung
   */
  auto to_slug(std::string const& text) const -> std::string;

  std::string name;
  std::string const version = "1.2.0";
};

```
2. Perbarui `source/lib.cpp`
Implementasikan logika transformasi karakter menggunakan fungsi standar C++.
```cpp
#include "lib.hpp"

#include <fmt/core.h>
#include <algorithm>
#include <cctype>

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
  if (user_name.empty()) {
    return false;
  }
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

```
3. Perbarui `source/main.cpp`
Ubah fungsi utama untuk mendemonstrasikan hasil konversi dari fitur slug baru ini.
```cpp
#include <iostream>
#include <string>

#include "lib.hpp"

auto main() -> int
{
  auto const lib = library {};
  
  std::string const title = "C Plus Plus Modern Paling Keren";
  
  // Menggunakan fitur baru to_slug()
  auto const slug_title = lib.to_slug(title);
  
  std::cout << "Judul Asli : " << title << '\n';
  std::cout << "Hasil Slug : " << slug_title << '\n';
  
  return 0;
}

```

Hasil Output Program
Jika dijalankan, program akan menghasilkan teks yang bersih dan aman untuk format tautan:
```text
Judul Asli : C Plus Plus Modern Paling Keren
Hasil Slug : c-plus-plus-modern-paling-keren

```

---

Sekarang struktur pustaka Anda sudah memiliki fitur manipulasi string yang lebih matang. Apakah Anda ingin melanjutkan dengan:
- Membuat berkas `CMakeLists.txt` agar proyek ini bisa langsung dicoba dan dikompilasi?
- Menambahkan **fitur pencatatan waktu (timestamp)** pada sapaan pustaka?

<br>

qc:
Menambahkan fitur baru ke dalam struktur pustaka yang sudah ada.

<br>

a-ai:
Mari kita tambahkan **fitur penghitung statistik teks (text analyzer)** ke dalam pustaka Anda. Fitur ini akan menganalisis string input dan mengembalikan jumlah huruf, jumlah angka, dan jumlah spasi dalam bentuk struktur data terpisah.

Berikut adalah pembaruan kode untuk fitur baru ini:

1. Perbarui `source/lib.hpp`
Kita definisikan sebuah `struct` baru bernama `text_stats` untuk menyimpan hasil analisis, lalu tambahkan fungsi `analyze_text` ke dalam `library`.
```cpp
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
 */
struct library
{
  library();

  auto greet(std::string const& user_name) const -> std::string;
  auto is_valid_name(std::string const& user_name) const -> bool;
  auto to_slug(std::string const& text) const -> std::string;

  /**
   * @brief Menganalisis karakter di dalam teks
   * @param text Teks yang akan dianalisis
   * @return Struktur text_stats berisi jumlah huruf, angka, dan spasi
   */
  auto analyze_text(std::string const& text) const -> text_stats;

  std::string name;
  std::string const version = "1.3.0";
};

```
2. Perbarui `source/lib.cpp`
Implementasikan logika perhitungan karakter menggunakan fungsi bawaan `<cctype>`.
```cpp
#include "lib.hpp"

#include <fmt/core.h>
#include <algorithm>
#include <cctype>

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
  if (user_name.empty()) {
    return false;
  }
  return !std::all_of(user_name.begin(), user_name.end(), [](unsigned char ch) {
    return std::isspace(ch);
  });
}

auto library::to_slug(std::string const& text) const -> std::string
{
  std::string result = text;
  std::transform(result.begin(), result.end(), result.begin(), [](unsigned char c) {
    return std::tolower(c);
  });
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

```
3. Perbarui `source/main.cpp`
Ubah fungsi utama untuk menguji fitur analisis teks ini dan mencetak hasilnya ke layar.
```cpp
#include <iostream>
#include <string>

#include "lib.hpp"

auto main() -> int
{
  auto const lib = library {};
  
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

```

Hasil Output Program
Jika dijalankan, program akan memberikan rincian karakter seperti berikut:
```text
Teks Analisis: "C++ Version 23 diluncurkan pada tahun 2023"
-----------------------------------
Jumlah Huruf : 28
Jumlah Angka : 6
Jumlah Spasi : 6

```

---

Sekarang pustaka Anda sudah memiliki empat fitur utama: sapaan, validasi nama, konversi slug, dan analisis statistik teks.
Langkah pengembangan apa yang ingin Anda lakukan selanjutnya?
- Menulis skrip **unit testing** otomatis untuk menguji seluruh fungsi di atas?

<br>

qc:
Menambahkan fitur baru ke dalam struktur pustaka yang sudah ada.

<br>

a-ai:
Mari kita tambahkan **fitur enkripsi teks sederhana (Caesar Cipher)** ke dalam pustaka Anda. Fitur ini akan mengamankan pesan teks dengan cara menggeser setiap karakter huruf sesuai dengan kunci (key) numerik yang ditentukan.

Berikut adalah pembaruan kode terstruktur untuk menambahkan fitur tersebut:

1. Perbarui `source/lib.hpp`
Tambahkan deklarasi fungsi `encrypt_caesar` di dalam `struct library`.
```cpp
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
 */
struct library
{
  library();

  auto greet(std::string const& user_name) const -> std::string;
  auto is_valid_name(std::string const& user_name) const -> bool;
  auto to_slug(std::string const& text) const -> std::string;
  auto analyze_text(std::string const& text) const -> text_stats;

  /**
   * @brief Mengenkripsi teks menggunakan metode Caesar Cipher
   * @param text Teks asli yang akan dienkripsi
   * @param shift Jumlah pergeseran karakter (kunci enkripsi)
   * @return String teks yang telah disamarkan
   */
  auto encrypt_caesar(std::string const& text, int shift) const -> std::string;

  std::string name;
  std::string const version = "1.4.0";
};

```
2. Perbarui `source/lib.cpp`
Implementasikan logika pergeseran karakter untuk huruf besar (`A-Z`) dan huruf kecil (`a-z`) dengan mengabaikan karakter non-alfabet (seperti spasi dan simbol).
```cpp
#include "lib.hpp"

#include <fmt/core.h>
#include <algorithm>
#include <cctype>

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
  if (user_name.empty()) {
    return false;
  }
  return !std::all_of(user_name.begin(), user_name.end(), [](unsigned char ch) {
    return std::isspace(ch);
  });
}

auto library::to_slug(std::string const& text) const -> std::string
{
  std::string result = text;
  std::transform(result.begin(), result.end(), result.begin(), [](unsigned char c) {
    return std::tolower(c);
  });
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

auto library::encrypt_caesar(std::string const& text, int shift) const -> std::string
{
  std::string result = text;
  // Pastikan shift berada di rentang 0-25
  int const effective_shift = (shift % 26 + 26) % 26;

  for (char& ch : result) {
    if (std::isupper(ch)) {
      ch = static_cast<char>('A' + (ch - 'A' + effective_shift) % 26);
    } else if (std::islower(ch)) {
      ch = static_cast<char>('a' + (ch - 'a' + effective_shift) % 26);
    }
  }
  return result;
}

```
3. Perbarui `source/main.cpp`
Ubah fungsi utama untuk menguji penyandian teks rahasia ini.
```cpp
#include <iostream>
#include <string>

#include "lib.hpp"

auto main() -> int
{
  auto const lib = library {};
  
  std::string const secret_message = "KODE RAHASIA C++ PADA TAHUN 2026";
  int const key = 3; // Menggeser 3 huruf ke depan (A -> D, B -> E, dst.)
  
  // Menggunakan fitur baru encrypt_caesar()
  auto const encrypted = lib.encrypt_caesar(secret_message, key);
  
  std::cout << "Pesan Asli : " << secret_message << '\n';
  std::cout << "Kunci Shift: " << key << '\n';
  std::cout << "Hasil Sandi: " << encrypted << '\n';
  
  return 0;
}

```

Hasil Output Program
Jika dijalankan, program akan menggeser huruf-huruf tersebut secara otomatis:
```bash
Pesan Asli : KODE RAHASIA C++ PADA TAHUN 2026
Kunci Shift: 3
Hasil Sandi: NRGH UDVdvld F++ SDGD WDKXQ 2026

```
Pustaka Anda kini semakin kaya dengan alat pemrosesan teks dan keamanan dasar. Manakah dari langkah berikut yang ingin Anda lakukan sekarang?
- Membuat berkas **unit test** untuk memverifikasi akurasi enkripsi Caesar dengan berbagai nilai kunci?

<br>

qc:
Menambahkan fitur baru ke dalam struktur pustaka yang sudah ada.

<br>

a-ai:
Mari kita lengkapi pustaka Anda dengan fitur manajemen waktu, yaitu fungsi untuk **mengambil penanda waktu saat ini (timestamp formatter)**. Fitur ini akan mengembalikan waktu sistem dalam format string standar ISO 8601 (`YYYY-MM-DD HH:MM:SS`), yang sangat penting untuk pembuatan log aplikasi modern.

Berikut adalah pembaruan kode terstruktur untuk menambahkan fitur tersebut:

1. Perbarui `source/lib.hpp`
Tambahkan deklarasi fungsi `get_current_timestamp` di dalam struktur pustaka. Kita juga membutuhkan pustaka standar `<chrono>` untuk menangani manipulasi waktu sistem.
```cpp
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
 */
struct library
{
  library();

  auto greet(std::string const& user_name) const -> std::string;
  auto is_valid_name(std::string const& user_name) const -> bool;
  auto to_slug(std::string const& text) const -> std::string;
  auto analyze_text(std::string const& text) const -> text_stats;
  auto encrypt_caesar(std::string const& text, int shift) const -> std::string;

  /**
   * @brief Mengambil penanda waktu sistem saat ini
   * @return String waktu dengan format "YYYY-MM-DD HH:MM:SS"
   */
  auto get_current_timestamp() const -> std::string;

  std::string name;
  std::string const version = "1.5.0";
};

```
2. Perbarui `source/lib.cpp`
Implementasikan fungsionalitas waktu menggunakan fitur C++ modern (`std::chrono`) dan gabungkan dengan utilitas format `{fmt}` yang sudah Anda gunakan.
```cpp
#include "lib.hpp"

#include <fmt/core.h>
#include <fmt/chrono.h> // Tambahan untuk memformat waktu secara langsung
#include <algorithm>
#include <cctype>
#include <chrono>

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
  if (user_name.empty()) {
    return false;
  }
  return !std::all_of(user_name.begin(), user_name.end(), [](unsigned char ch) {
    return std::isspace(ch);
  });
}

auto library::to_slug(std::string const& text) const -> std::string
{
  std::string result = text;
  std::transform(result.begin(), result.end(), result.begin(), [](unsigned char c) {
    return std::tolower(c);
  });
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

auto library::encrypt_caesar(std::string const& text, int shift) const -> std::string
{
  std::string result = text;
  int const effective_shift = (shift % 26 + 26) % 26;

  for (char& ch : result) {
    if (std::isupper(ch)) {
      ch = static_cast<char>('A' + (ch - 'A' + effective_shift) % 26);
    } else if (std::islower(ch)) {
      ch = static_cast<char>('a' + (ch - 'a' + effective_shift) % 26);
    }
  }
  return result;
}

auto library::get_current_timestamp() const -> std::string
{
  // Mengambil waktu sekarang dari jam sistem
  auto const now = std::chrono::system_clock::now();
  auto const time_t_now = std::chrono::system_clock::to_time_t(now);
  
  // Mengonversi ke waktu lokal dengan aman menggunakan struktur std::tm
  std::tm const local_time = *std::localtime(&time_t_now);

  // Memanfaatkan pustaka fmt::format untuk mencetak waktu dengan format ISO-like
  return fmt::format("{:%Y-%m-%d %H:%M:%S}", local_time);
}

```
3. Perbarui `source/main.cpp`
Gunakan fungsi baru tersebut di dalam program utama untuk mensimulasikan pencatatan log peristiwa.
```cpp
#include <iostream>
#include <string>

#include "lib.hpp"

auto main() -> int
{
  auto const lib = library {};
  
  // Menggunakan fitur baru get_current_timestamp()
  auto const current_time = lib.get_current_timestamp();
  
  std::cout << "[" << current_time << "] INFO: Aplikasi " << lib.name << " berhasil dimuat.\n";
  std::cout << "[" << current_time << "] USER: Melakukan inisialisasi modul logika...\n";
  
  return 0;
}

```

Hasil Output Program
Jika program dijalankan, baris log akan otomatis mencantumkan waktu nyata saat eksekusi terjadi:
```text
[2026-08-27 05:19:35] INFO: Aplikasi myp berhasil dimuat.
[2026-08-27 05:19:35] USER: Melakukan inisialisasi modul logika...

```

---

Pustaka Anda sekarang telah memiliki fondasi fungsional yang kuat untuk utilitas string, keamanan teks, hingga penanganan waktu log aplikasi.
- Menulis berkas unit test otomatis untuk memvalidasi performa dan ketepatan format dari seluruh fungsi di atas?

<br>

qc:
Menambahkan fitur baru ke dalam struktur pustaka yang sudah ada.

<br>

Mari kita lengkapi pustaka Anda dengan fitur validasi keamanan string, yaitu fungsi untuk **memeriksa kekuatan kata sandi (password strength checker)**. Fitur ini akan mengevaluasi string input dan mengembalikan nilai boolean apakah kata sandi tersebut memenuhi standar keamanan modern.

Standar keamanan yang akan kita terapkan adalah:
- Panjang minimal **8 karakter**.
- Mengandung minimal satu **huruf besar**.
- Mengandung minimal satu **huruf kecil**.
- Mengandung minimal satu **angka**.
Berikut adalah pembaruan kode terstruktur untuk menambahkan fitur tersebut:
1. Perbarui `source/lib.hpp`
Tambahkan deklarasi fungsi `is_strong_password` ke dalam struktur pustaka.
```cpp
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
 */
struct library
{
  library();

  auto greet(std::string const& user_name) const -> std::string;
  auto is_valid_name(std::string const& user_name) const -> bool;
  auto to_slug(std::string const& text) const -> std::string;
  auto analyze_text(std::string const& text) const -> text_stats;
  auto encrypt_caesar(std::string const& text, int shift) const -> std::string;
  auto get_current_timestamp() const -> std::string;

  /**
   * @brief Memeriksa apakah kata sandi memenuhi standar keamanan modern
   * @param password Kata sandi yang akan diuji
   * @return true jika kuat, false jika lemah
   */
  auto is_strong_password(std::string const& password) const -> bool;

  std::string name;
  std::string const version = "1.6.0";
};

```
2. Perbarui `source/lib.cpp`
Implementasikan logika pengecekan karakter menggunakan fungsi bawaan dari `<cctype>`.
```cpp
#include "lib.hpp"

#include <fmt/core.h>
#include <fmt/chrono.h>
#include <algorithm>
#include <cctype>
#include <chrono>

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
  if (user_name.empty()) {
    return false;
  }
  return !std::all_of(user_name.begin(), user_name.end(), [](unsigned char ch) {
    return std::isspace(ch);
  });
}

auto library::to_slug(std::string const& text) const -> std::string
{
  std::string result = text;
  std::transform(result.begin(), result.end(), result.begin(), [](unsigned char c) {
    return std::tolower(c);
  });
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

auto library::encrypt_caesar(std::string const& text, int shift) const -> std::string
{
  std::string result = text;
  int const effective_shift = (shift % 26 + 26) % 26;

  for (char& ch : result) {
    if (std::isupper(ch)) {
      ch = static_cast<char>('A' + (ch - 'A' + effective_shift) % 26);
    } else if (std::islower(ch)) {
      ch = static_cast<char>('a' + (ch - 'a' + effective_shift) % 26);
    }
  }
  return result;
}

auto library::get_current_timestamp() const -> std::string
{
  auto const now = std::chrono::system_clock::now();
  auto const time_t_now = std::chrono::system_clock::to_time_t(now);
  std::tm const local_time = *std::localtime(&time_t_now);
  return fmt::format("{:%Y-%m-%d %H:%M:%S}", local_time);
}

auto library::is_strong_password(std::string const& password) const -> bool
{
  // Syarat 1: Panjang minimal 8 karakter
  if (password.length() < 8) {
    return false;
  }

  bool has_upper = false;
  bool has_lower = false;
  bool has_digit = false;

  for (unsigned char const ch : password) {
    if (std::isupper(ch)) has_upper = true;
    if (std::islower(ch)) has_lower = true;
    if (std::isdigit(ch)) has_digit = true;
  }

  // Harus memenuhi semua kriteria tersisa
  return has_upper && has_lower && has_digit;
}

```
3. Perbarui `source/main.cpp`
Ubah fungsi utama untuk menguji efektivitas validasi kata sandi.
```cpp
#include <iostream>
#include <string>

#include "lib.hpp"

auto main() -> int
{
  auto const lib = library {};
  
  std::string const pass1 = "rahasia";
  std::string const pass2 = "Rahasia2026";
  
  std::cout << std::boolalpha; // Mencetak true/false alih-alih 1/0
  std::cout << "Apakah \"" << pass1 << "\" kuat? " << lib.is_strong_password(pass1) << '\n';
  std::cout << "Apakah \"" << pass2 << "\" kuat? " << lib.is_strong_password(pass2) << '\n';
  
  return 0;
}

```

Hasil Output Program
Jika dijalankan, program akan langsung mengevaluasi kedua sampel kata sandi tersebut:
```text
Apakah "rahasia" kuat? false
Apakah "Rahasia2026" kuat? true

```

---

Pustaka Anda kini sudah dilengkapi dengan sistem pengujian input keamanan yang andal.
Jika Anda sudah selesai menambahkan fitur lokal, mari beralih ke integrasi sistem. Apakah Anda ingin:
- Menulis berkas **unit test** otomatis menggunakan kerangka kerja C++?































<br>
