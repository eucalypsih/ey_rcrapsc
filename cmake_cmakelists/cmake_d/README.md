# cmake_d

`CMAKE_VERSION`: Menyimpan string versi lengkap CMake yang sedang berjalan (contoh: `3.28.1`).
`CMAKE_MAJOR_VERSION`: Mengambil angka komponen **paling depan** dari versi tersebut (contoh: angka `3`).
`CMAKE_MINOR_VERSION`: Mengambil angka komponen **kedua** dari versi tersebut (contoh: angka `28`).
`CMAKE_PATCH_VERSION`: Mengambil angka komponen **terakhir** dari versi tersebut (contoh: angka `1`).

1. Informasi Proyek & Lokasi File
- `PROJECT_NAME`: Menyimpan nama proyek yang dideklarasikan oleh perintah `project()`. Di skrip ini, nilainya akan menjadi `FMT` setelah baris `project(FMT CXX)` dilewati.
- `CMAKE_CURRENT_SOURCE_DIR`: Jalur lengkap ke folder tempat file `CMakeLists.txt` yang sedang diproses berada.
- `CMAKE_CURRENT_BINARY_DIR`: Jalur lengkap ke folder *build* (tempat file objek dan biner hasil kompilasi diletakkan) yang sesuai dengan direktori skrip saat ini.
- `PROJECT_BINARY_DIR`: Jalur lengkap ke folder utama tempat seluruh hasil *build* proyek diletakkan.

2. Konfigurasi Metode & Tipe Kompilasi
- `CMAKE_BUILD_TYPE`: Menentukan tipe optimasi kode yang digunakan (misalnya: `Debug`, `Release`, `RelWithDebInfo`, atau `MinSizeRel`).
- `BUILD_SHARED_LIBS`: Variabel global untuk menentukan apakah library akan dibangun sebagai pustaka dinamis (`.dll`/`.so`) jika bernilai `TRUE`, atau pustaka statis (`.lib`/`.a`) jika bernilai `FALSE`.
- `CMAKE_MODULE_PATH`: Daftar folder tempat CMake akan mencari skrip ekstensi tambahan (`*.cmake`) ketika Anda memanggil perintah `include()` atau `find_package()`.

3. Deteksi Kompilator (Compiler) & Standard Bahasa
- `CMAKE_CXX_COMPILER_ID`: Berisi nama/ID dari kompilator C++ yang sedang mendeteksi kode (contoh: `GNU` untuk `GCC`, `Clang`, atau `MSVC` untuk Microsoft Visual Studio).
- `CMAKE_CXX_COMPILER_VERSION`: Menyimpan string angka versi dari kompilator yang sedang aktif (contoh: `13.2.0`).
- `MSVC`: Variabel bernilai boolean (`TRUE`) yang otomatis aktif jika proyek dikompilasi menggunakan perangkat lunak Microsoft Visual C++.
- `CMAKE_CXX_STANDARD`: Menentukan standard bahasa C++ yang akan digunakan (contoh: `11`, `17`, `20`, atau `26`).
- `CMAKE_CXX_COMPILE_FEATURES`: Daftar internal CMake yang berisi seluruh kemampuan atau fitur modern C++ yang didukung oleh kompilator Anda.

4. Lingkungan Build & Generator
- `CMAKE_GENERATOR`: Menyimpan nama *build tool* yang memproses pembuatan file akhir (contoh: `"Ninja"`, `"Unix Makefiles"`, atau `"Visual Studio 17 2022"`).
- `CMAKE_MAKE_PROGRAM`: Jalur absolut menuju aplikasi eksekusi pembangun kode (seperti `make`, `ninja.exe`, atau `msbuild.exe`).
- `CMAKE_CXX_COMPILER_CLANG_SCAN_DEPS`: Variabel internal khusus kompilator Clang yang menunjuk ke alat pemindai ketergantungan modul C++20 (`clang-scan-deps`).

5. Aturan Instalasi Struktur Folder
Variabel di bawah ini berasal dari modul internal `GNUInstallDirs` untuk standarisasi folder instalasi di Linux/Windows:
- `CMAKE_INSTALL_PREFIX`: Alamat folder utama tujuan akhir saat Anda menjalankan perintah instalasi (biasanya `/usr/local` di Linux).
- `CMAKE_INSTALL_INCLUDEDIR`: Folder standar untuk meletakkan file header `.h` (biasanya bernama `include`).
- `CMAKE_INSTALL_LIBDIR`: Folder standar untuk menaruh berkas library hasil kompilasi seperti `.so` atau `.a` (biasanya bernama `lib`).
- `CMAKE_INSTALL_BINDIR`: Folder standar untuk file eksekusi aplikasi utama (`bin`).

- `PROJECT_SOURCE_DIR`

`cmake -B build -DCMAKE_BUILD_TYPE=Release` Mengubah Tipe Build menjadi Mode Rilis
`cmake -B build -DCMAKE_INSTALL_PREFIX=/home/user/my_libraries/fmt` Mengubah Lokasi Folder Instalasi Akhir
`cmake -B build -DCMAKE_CXX_STANDARD=20` Memaksa Kompilasi Kompatibilitas Modern C++

`$<BUILD_INTERFACE:...>` & `$<INSTALL_INTERFACE:...>`: Menyelesaikan error pertama. Saat Anda mengompilasi proyek di komputer lokal, compiler akan membaca folder source/. Namun, saat proyek diekspor ke orang lain, file ekspor secara otomatis akan dialihkan untuk membaca folder include/myp hasil instalasi.

`$<LINK_ONLY:fmt::fmt>`: Menyelesaikan error kedua. Fungsi pembungkus ini memberi tahu CMake: "Gunakan pustaka fmt ini hanya untuk mencarikan simbol biner saat proses kompilasi executable lokal kita, tapi hapus total nama target fmt ini dari file ekspor agar tidak ditagih ke pengguna luar."








<br>
