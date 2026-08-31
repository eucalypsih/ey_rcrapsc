# ey_rcraps

```bash
owner="eucalypsih";repo="ey_rcrapsc";git clone -q --filter=blob:none --sparse git@github.com:${owner}/${repo}.git && sleep 0.5 && cd $repo && sleep 0.5 && un="eucalypsih";ue="eucalypsih@gmail.com";git checkout main && sleep 0.5 && git config user.name "$un" && sleep 0.5 && git config user.email "$ue" && sleep 0.5 && git config gpg.format ssh && sleep 0.5 && git config user.signingkey ~/.ssh/id_rsa.pub && sleep 0.5 && git config commit.gpgsign true && sleep 0.5 && git config gpg.ssh.allowedSignersFile ~/.ssh/allowed_signers

```
`owner="eucalypsih";repo="ey_rcrapsc";git clone -q --filter=blob:none --sparse git@github.com:${owner}/${repo}.git && sleep 0.5 && cd $repo && sleep 0.5 && un="eucalypsih";ue="eucalypsih@gmail.com";git checkout main && sleep 0.5 && git config user.name "$un" && sleep 0.5 && git config user.email "$ue" && sleep 0.5 && git config gpg.format ssh && sleep 0.5 && git config user.signingkey ~/.ssh/id_rsa.pub && sleep 0.5 && git config commit.gpgsign true && sleep 0.5 && git config gpg.ssh.allowedSignersFile ~/.ssh/allowed_signers`

---

```bash
cmake -B build -S . -DCMAKE_INSTALL_PREFIX=./dist -DCMAKE_BUILD_TYPE=Debug

```
`cmake -B build -S . -DCMAKE_INSTALL_PREFIX=./dist -DCMAKE_BUILD_TYPE=Debug`



```bash
cmake --build build

```
`cmake --build build`



```bash
cmake --install build

```
`cmake --install build`





---

Secara bawaan (*built-in*), CMake tidak memiliki perintah resmi seperti `cmake init` untuk membuat boilerplate secara interaktif. Namun, Anda bisa membuat fungsi tersebut sendiri agar bekerja persis seperti `npm init` atau `go mod init`.

Berikut adalah cara membuat perintah `cmake-init` kustom di komputer Anda.

---

Cara 1: Menggunakan Perintah Alias Otomatis (Instan & Cepat)
Anda bisa mendaftarkan skrip pembuat `CMakeLists.txt` ke dalam konfigurasi terminal Anda sebagai perintah baru.

🛠️ Untuk Linux / macOS (Bash atau Zsh)
1. Buka file konfigurasi terminal Anda (`~/.bashrc` atau `~/.zshrc`).
2. Tambahkan baris fungsi berikut di bagian paling bawah:
```bash
cmake-init() {
    local proj_name=${1:-MyProject}
    cat << EOF > CMakeLists.txt
cmake_minimum_required(VERSION 3.15)
project($proj_name VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SOURCES "*.cpp")
add_executable(\${PROJECT_NAME} \${SOURCES})
EOF
    echo "✨ CMakeLists.txt berhasil diinisialisasi untuk proyek: $proj_name"
}

```
3. Simpan dan muat ulang terminal dengan mengetik `source ~/.zshrc` (atau `.bashrc`).
4. Cara Penggunaan: Cukup ketik `cmake-init nama_proyek_anda` di folder mana saja.

🛠️ Untuk Windows (PowerShell)
1. Buka profil PowerShell Anda dengan mengetik `notepad $PROFILE`.
2. Masukkan fungsi berikut ke dalam file tersebut:
```powershell
function cmake-init ($ProjName = "MyProject") {
@'
cmake_minimum_required(VERSION 3.15)
project({0} VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SOURCES "*.cpp")
add_executable(${PROJECT_NAME} ${SOURCES})
'@ -f $ProjName | Out-File -FilePath CMakeLists.txt -Encoding utf8
    Write-Host "✨ CMakeLists.txt berhasil diinisialisasi untuk proyek: $ProjName" -ForegroundColor Green
}

```
3. Simpan, tutup notepad, lalu buka ulang PowerShell Anda.
4. Cara Penggunaan: Cukup ketik `cmake-init nama_proyek_anda` di folder proyek Anda.

---

Cara 2: Menggunakan Paket Pihak Ketiga Resmi (`cmake-init`)
Jika Anda menginginkan alat interaktif yang jauh lebih canggih (menanyakan opsi arsitektur, struktur folder src/include, hingga setup testing), komunitas open-source telah membuat alat khusus bernama `cmake-init`.

Anda bisa menginstalnya menggunakan manajer paket Python (PIP):
```bash
pip install cmake-init

```
Setelah terinstal, Anda tinggal masuk ke folder proyek baru Anda dan mengetik:
```bash
cmake-init

```
Alat ini akan memandu Anda melalui beberapa pertanyaan interaktif persis seperti alur kerja pada `npm init`.

<br>

q-ai:
Apakah Anda lebih menyukai solusi **skrip alias yang simpel (Cara 1)** atau ingin mengeksplorasi pembuatan sistem pengujian (*testing setup*) menggunakan **perangkat canggih (Cara 2)**?

<br>

qs:
cara mengcompile hasil generate dari `cmake-init`

<br>

a-ai:
Untuk mengompilasi proyek hasil generate dari `cmake-init`, Anda perlu melakukan dua tahap utama, yaitu **konfigurasi (generating) dan pembangunan (building)**. Karena `cmake-init` secara default menghasilkan struktur proyek modern, cara terbaik dan paling aman adalah menggunakan perintah bawaan CMake yang bersifat lintas platform.

Berikut adalah langkah-langkah cepat untuk mengompilasi proyek Anda langsung dari terminal/command prompt:

1. Masuk ke Direktori Proyek
Buka terminal dan pastikan Anda berada di dalam folder utama proyek (tempat file `CMakeLists.txt` berada).
```bash
cd /jalur/ke/folder/proyek-anda

```
2. Tahap Konfigurasi (Generate Build Files)
Jalankan perintah berikut untuk membuat folder bernama `build` dan menghasilkan file konfigurasi sistem build (seperti *Makefile* atau *Visual Studio Solution*) di dalamnya:
```bash
cmake -S . -B build

```
- `-S .`: Menentukan direktori sumber (source) saat ini.
- `-B build`: Menentukan folder `build` sebagai tempat hasil kompilasi.
> (Catatan: Jika Anda ingin mengompilasi dalam mode rilis untuk performa maksimal, tambahkan flag berikut: `cmake -S . -B build -D CMAKE_BUILD_TYPE=Release`).
3. Tahap Kompilasi (Build)
Setelah proses konfigurasi selesai tanpa *error*, jalankan perintah build universal ini:
```bash
cmake --build build

```
Perintah di atas akan mendeteksi kompiler bawaan komputer Anda (seperti `make`, `ninja`, atau` MSBuild`) dan langsung mengompilasi kodenya hingga menjadi file *executable* (aplikasi) atau library.

---

Pro-Tips Tambahan 💡
- Kompilasi Lebih Cepat (Multi-core): Jika proyek Anda cukup besar, manfaatkan semua core CPU Anda saat kompilasi dengan menambahkan flag `-j`:
```bash
cmake --build build -j 4

```
> (Ganti angka 4 dengan jumlah core CPU Anda).

Menjalankan Aplikasi: Setelah proses di atas selesai 100%, file binary/aplikasi Anda biasanya akan muncul di dalam folder `build`. Anda bisa mencarinya dan menjalankannya dengan perintah:
```bash
./build/nama_aplikasi_anda

```


- Lokasi Hasil Kompilasi: Hasil file jadi (executable) biasanya akan otomatis berada di dalam subfolder `build` atau `build/Debug` / `build/Release`.
- Menjalankan Unit Test (Jika Ada): Proyek dari `cmake-init` umumnya dilengkapi dengan pengujian bawaan. Anda bisa langsung mengujinya setelah kompilasi selesai dengan perintah:  `bash cd build` && `ctest`

<br>

q-ai:
Jika Anda mengalami kendala saat menjalankan langkah-langkah di atas, beri tahu saya **sistem operasi** apa yang Anda gunakan (Windows/Linux/macOS) serta **pesan _error_** yang muncul agar saya bisa membantu memperbaikinya!

<br>

qs:
```bash
$ cmake -S . -B build
-- The CXX compiler identification is Clang 21.1.8
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: /data/data/com.termux/files/usr/bin/c++ - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
CMake Error at CMakeLists.txt:31 (find_package):
  By not providing "Findfmt.cmake" in CMAKE_MODULE_PATH this project has
  asked CMake to find a package configuration file provided by "fmt", but
  CMake did not find one.

  Could not find a package configuration file provided by "fmt" with any of
  the following names:

    fmt.cps
    fmtConfig.cmake
    fmt-config.cmake

  Add the installation prefix of "fmt" to CMAKE_PREFIX_PATH or set "fmt_DIR"
  to a directory containing one of the above files.  If "fmt" provides a
  separate development package or SDK, be sure it has been installed.


-- Configuring incomplete, errors occurred!

```

<br>

a-ai:
Error tersebut terjadi karena proyek Anda membutuhkan pustaka (library) bernama `fmt`, tetapi sistem belum menginstalnya di lingkungan Termux Anda. `cmake-init` secara default sering kali menyertakan `fmt` untuk manajemen string dan output.

Karena Anda menggunakan **Termux**, Anda bisa menyelesaikannya dengan dua cara di bawah ini. Pilih cara yang paling sesuai dengan kebutuhan Anda.

Cara 1: Menginstal `fmt` Langsung di Termux (Paling Cepat)
Anda cukup menginstal paket `fmt` yang sudah disediakan oleh repositori resmi Termux agar CMake bisa langsung mendeteksinya.
1. Buka sesi terminal baru atau bersihkan terminal Anda.
2. Jalankan perintah instalasi berikut:
```bash
pkg update && pkg install fmt

```
3. Setelah instalasi selesai, hapus folder `build` lama Anda agar konfigurasi CMake diulang dari awal:
```bash
rm -rf build

```
4. Jalankan kembali perintah konfigurasi CMake Anda:
```bash
cmake -S . -B build

```

---

Cara 2: Menggunakan FetchContent (Otomatis Download via CMake)
Jika Anda ingin proyek ini bisa dikompilasi di perangkat lain tanpa perlu menginstal paket `fmt` secara manual di sistem, Anda bisa menyuruh CMake untuk mengunduhnya secara otomatis saat proses konfigurasi.
1. Buka file `CMakeLists.txt` utama proyek Anda
2. Cari baris `find_package(fmt REQUIRED)` (biasanya di sekitar baris 31 sesuai pesan error).
3. **Ganti** baris `find_package(fmt REQUIRED)` tersebut dengan kode berikut
```txt
include(FetchContent)
FetchContent_Declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt
  GIT_TAG        12.2.0 # 10.2.1 # Atau versi stabil lainnya
)
FetchContent_MakeAvailable(fmt)

```
4. Simpan file tersebut, lalu jalankan kembali perintah:
```bash
cmake -S . -B build

```

---

Setelah proses konfigurasi di atas selesai tanpa error, Anda bisa langsung melanjutkan ke tahap kompilasi akhir dengan perintah:
```bash
cmake --build build

```

<br>

q-ai:
Jika muncul error baru terkait dependensi lain setelah Anda mencoba cara di atas, beri tahu saya **nama paket/library** yang error agar bisa kita selesaikan bersama!

<br>

qs:
```bash
$ cat CMakeLists.txt
cmake_minimum_required(VERSION 3.14)

include(cmake/prelude.cmake)

project(
    myp
    VERSION 0.1.0
    DESCRIPTION "My Simple Program"
    HOMEPAGE_URL "https://exam.co"
    LANGUAGES CXX
)

include(cmake/project-is-top-level.cmake)
include(cmake/variables.cmake)

# ---- Declare library ----

add_library(
    myp_lib OBJECT
    source/lib.cpp
)

target_include_directories(
    myp_lib ${warning_guard}
    PUBLIC
    "\$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/source>"
)

target_compile_features(myp_lib PUBLIC cxx_std_20)

# find_package(fmt REQUIRED)
include(FetchContent)
FetchContent_Declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt
  GIT_TAG        12.2.0 # 10.2.1 # Atau versi stabil lainnya
)
FetchContent_MakeAvailable(fmt)

target_link_libraries(myp_lib PRIVATE fmt::fmt)

# ---- Declare executable ----

add_executable(myp_exe source/main.cpp)
add_executable(myp::exe ALIAS myp_exe)

set_property(TARGET myp_exe PROPERTY OUTPUT_NAME myp)

target_compile_features(myp_exe PRIVATE cxx_std_20)

target_link_libraries(myp_exe PRIVATE myp_lib)

# ---- Install rules ----

if(NOT CMAKE_SKIP_INSTALL_RULES)
  include(cmake/install-rules.cmake)
endif()

# ---- Developer mode ----

if(NOT myp_DEVELOPER_MODE)
  return()
elseif(NOT PROJECT_IS_TOP_LEVEL)
  message(
      AUTHOR_WARNING
      "Developer mode is intended for developers of myp"
  )
endif()

include(cmake/dev-mode.cmake)

```

<br>

a-ai:
Berikut adalah beberapa rekomendasi peningkatan fitur dan perbaikan struktur pada file `CMakeLists.txt` Anda agar lebih optimal, aman, dan mengikuti praktik terbaik *Modern CMake*:

🛠️ Perbaikan & Peningkatan Rekomendasi
1. Gunakan Mode Header-Only Jika Sesuai Kebutuhan: Pustaka `{fmt}` dapat digunakan sebagai pustaka terkompilasi (`fmt::fmt`) atau *header-only* (`fmt::fmt-header-only`). Pada konfigurasi Anda, `myp_lib` dideklarasikan sebagai OBJECT library, yang berarti ia tidak menghasilkan file `.a` atau `.lib` fisik secara langsung melainkan hanya kumpulan file objek (`.o`/`.obj`).
    - Jika Anda ingin proses kompilasi awal berjalan lebih cepat tanpa mengompilasi ulang source code `{fmt}`, Anda bisa mengubah `fmt::fmt` menjadi `fmt::fmt-header-only`.
2. Matikan Pengujian Internal Pustaka Eksternal: Secara bawaan, menambahkan pustaka via `FetchContent` akan ikut mengeksekusi script pengujian pustaka tersebut jika ada. Tambahkan instruksi untuk mematikan unit test `{fmt}` agar proses konfigurasi CMake Anda jauh lebih cepat.
3. Optimasi Posisi FetchContent: Praktik terbaik dalam CMake adalah meletakkan deklarasi unduhan pihak ketiga di bagian atas setelah deklarasi proyek, atau mengelompokkannya ke dalam file terpisah agar kode target utama tetap bersih.

📝 Pembaruan Kode `CMakeLists.txt`
Berikut adalah kode `CMakeLists.txt` yang telah diperbarui dengan rapi:
```cmake
cmake_minimum_required(VERSION 3.14)

include(cmake/prelude.cmake)

project(
    myp
    VERSION 0.1.0
    DESCRIPTION "My Simple Program"
    HOMEPAGE_URL "https://exam.co"
    LANGUAGES CXX
)

include(cmake/project-is-top-level.cmake)
include(cmake/variables.cmake)

# ---- Pustaka Pihak Ketiga (Dependencies) ----

include(FetchContent)

# Menonaktifkan kompilasi unit test internal milik pustaka fmt
set(FMT_TEST OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt
  GIT_TAG        12.2.0 
)
FetchContent_MakeAvailable(fmt)

# ---- Declare library ----

add_library(
    myp_lib OBJECT
    source/lib.cpp
)

target_include_directories(
    myp_lib ${warning_guard}
    PUBLIC
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/source>"
)

target_compile_features(myp_lib PUBLIC cxx_std_20)

# Menghubungkan pustaka fmt ke target library proyek Anda
# Gunakan 'fmt::fmt' untuk versi compiled library (bawaan) 
# atau 'fmt::fmt-header-only' jika ingin mode tanpa kompilasi terpisah.
target_link_libraries(myp_lib PRIVATE fmt::fmt)

# ---- Declare executable ----

add_executable(myp_exe source/main.cpp)
add_executable(myp::exe ALIAS myp_exe)

set_property(TARGET myp_exe PROPERTY OUTPUT_NAME myp)

target_compile_features(myp_exe PRIVATE cxx_std_20)

target_link_libraries(myp_exe PRIVATE myp_lib)

# ---- Install rules ----

if(NOT CMAKE_SKIP_INSTALL_RULES)
  include(cmake/install-rules.cmake)
endif()

# ---- Developer mode ----

if(NOT myp_DEVELOPER_MODE)
  return()
elseif(NOT PROJECT_IS_TOP_LEVEL)
  message(
      AUTHOR_WARNING
      "Developer mode is intended for developers of myp"
  )
endif()

include(cmake/dev-mode.cmake)

```

---

🔍 Catatan Tambahan Penting
- Kompilasi String (C++20): Karena proyek Anda menggunakan standar` cxx_std_20`, pustaka `{fmt}` secara otomatis mendukung pemeriksaan validitas *format string* pada saat kompilasi (compile-time check). Jika terdapat ketidakcocokan tipe data pada `fmt::format()`, compiler akan langsung memunculkan *error* sebelum program dijalankan.
- Pembersihan Cache: Jika setelah memperbarui file ini Anda menemui kendala unduhan, disarankan untuk menghapus folder `build` Anda dan melakukan konfigurasi ulang (`cmake -B build -S .`) untuk membersihkan sisa cache `FetchContent` yang lama.

<br>

q-ai:
Apakah Anda berencana menambahkan pustaka eksternal lainnya (seperti GoogleTest atau spdlog), atau ingin melakukan konfigurasi khusus pada fitur instalasi pustaka ini?

<br>

qc:
Saya ingin melakukan konfigurasi khusus pada fitur instalasi pustaka ini?

<br>

a-ai:
Untuk melakukan konfigurasi instalasi khusus pada proyek C++ modern yang terstruktur seperti ini, fokus utama kita berada di dalam file internal `cmake/install-rules.cmake` yang dipanggil oleh `CMakeLists.txt` Anda.

Karena target `myp_lib` Anda dideklarasikan sebagai **`OBJECT` library**, ada aturan khusus dalam CMake yang harus diperhatikan agar file objek, file *header*, dan file konfigurasi CMake (`mypConfig.cmake`) dapat diinstal dengan benar supaya bisa digunakan oleh proyek lain (*downstream projects*).

Berikut adalah panduan langkah demi langkah dan templat kode untuk mengonfigurasi fitur instalasi Anda.

---

1. Struktur Folder Instalasi Standar
Konfigurasi ini akan mengikuti standar GNU untuk tata letak direktori:
- `bin/` : Berisi file eksekusi (`myp_exe` / `myp`).
- `lib/` : Berisi file objek/arsip (jika ada).
- `include/myp/` : Berisi file *header* publik Anda (misal: `lib.hpp`).
- `lib/cmake/myp/` : Berisi file konfigurasi agar proyek lain bisa menggunakan perintah `find_package(myp REQUIRED)`.

2. Implementasi Kode `cmake/install-rules.cmake`
Silakan buat atau perbarui isi file `cmake/install-rules.cmake` Anda dengan kode terstruktur di bawah ini:
```cmake
if(NOT DEFINED CMAKE_INSTALL_BINDIR)
  include(GNUInstallDirs)
endif()

# ==============================================================================
# 1. Ekspor Nama Target (Export Set)
# ==============================================================================
# Kita mengelompokkan target proyek ke dalam satu set ekspor bernama "mypTargets"
set(export_name "mypTargets")

# ==============================================================================
# 2. Aturan Instalasi Target (Executable & Library)
# ==============================================================================
install(
    TARGETS myp_exe myp_lib
    EXPORT ${export_name}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    # Khusus untuk OBJECT library, CMake 3.14+ membutuhkan ini agar objeknya ikut terinstal jika diperlukan
    OBJECTS DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

# ==============================================================================
# 3. Aturan Instalasi File Header (.hpp / .h)
# ==============================================================================
# Asumsi: Anda meletakkan file header publik di dalam folder 'source/'
install(
    DIRECTORY "${PROJECT_SOURCE_DIR}/source/"
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/myp
    FILES_MATCHING
    PATTERN "*.hpp"
    PATTERN "*.h"
)

# ==============================================================================
# 4. Pembuatan File Konfigurasi CMake untuk Proyek Lain (find_package)
# ==============================================================================
# Folder tempat menyimpan file konfigurasi CMake hasil instalasi
set(cmake_config_dest "${CMAKE_INSTALL_LIBDIR}/cmake/myp")

# Menghasilkan file target (mypTargets.cmake) yang mendaftarkan target 'myp::exe' dsb.
install(
    EXPORT ${export_name}
    NAMESPACE myp::
    DESTINATION ${cmake_config_dest}
)

# Membuat file versi paket (mypConfigVersion.cmake) untuk memeriksa kecocokan versi
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${PROJECT_BINARY_DIR}/mypConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

# Menginstal file versi tersebut
install(
    FILES "${PROJECT_BINARY_DIR}/mypConfigVersion.cmake"
    DESTINATION ${cmake_config_dest}
)

# (Opsional) Jika Anda butuh file mypConfig.cmake kustom, Anda bisa membuatnya di folder cmake/
# Jika tidak ada logika rumit, cukup instal file minimal seperti di bawah ini:
file(WRITE "${PROJECT_BINARY_DIR}/mypConfig.cmake"
[[
include(CMakeFindDependencyMacro)
# Jika proyek lain membutuhkan fmt saat menggunakan myp, sertakan dependency di sini:
# find_dependency(fmt REQUIRED) 

include("${CMAKE_CURRENT_LIST_DIR}/mypTargets.cmake")
]]
)

install(
    FILES "${PROJECT_BINARY_DIR}/mypConfig.cmake"
    DESTINATION ${cmake_config_dest}
)

```

3. Cara Menguji dan Menjalankan Instalasi
Setelah file di atas dikonfigurasi, Anda dapat menguji proses instalasi ke folder lokal komputer Anda dengan perintah terminal berikut:
    1. Konfigurasi Proyek (Tentukan folder tujuan instalasi menggunakan `CMAKE_INSTALL_PREFIX`):
```bash
cmake -B build -S . -DCMAKE_INSTALL_PREFIX=./dist

```
    2. Kompilasi Kode:
```bash
cmake --build build

```
    3. Eksekusi Instalasi:
```bash
cmake --install build

```
Setelah menjalankan perintah ketiga, folder baru bernama `dist/` akan muncul di direktori Anda, berisi folder `bin`, `include`, dan `lib` yang sudah siap didistribusikan.

---

q-ai:
Jika Anda ingin menyesuaikan proses instalasi ini lebih lanjut, beri tahu saya:
- Apakah pustaka `myp_lib` ini nantinya akan **dipakai oleh proyek C++ lain**, atau instalasi ini hanya bertujuan untuk mendistribusikan file `myp_exe` (aplikasi final) saja?
- Apakah Anda memerlukan skema penamaan file khusus atau penanganan otomatis untuk **pustaka `{fmt}`** agar ikut tersalin ke folder instalasi?

<br>

qc:
Saya memerlukan skema penamaan file khusus atau penanganan otomatis untuk **pustaka `{fmt}`** agar ikut tersalin ke folder instalasi?

<br>

a-ai:
Berikut adalah solusi otomatis untuk menangani pustaka `{fmt}` agar ikut terinstal bersama proyek Anda.

Karena Anda mengunduh `{fmt}` menggunakan `FetchContent`, secara bawaan CMake tidak akan menginstal target pihak ketiga tersebut ke dalam folder `CMAKE_INSTALL_PREFIX` Anda. Kita harus mengonfigurasinya secara eksplisit di dalam kode CMake.

Ada dua pendekatan terbaik yang bisa Anda gunakan tergantung pada bagaimana proyek lain akan menggunakan pustaka Anda:

---

Pendekatan 1: Menginstal `{fmt}` Bersama Proyek (Bundling)
Jika Anda ingin file library/header dari `{fmt}` **ikut disalin secara fisik** ke dalam folder instalasi (`lib/` dan `include/`), Anda harus mengizinkan aturan instalasi internal milik `{fmt}` berjalan, lalu mengekspornya ke dalam paket CMake Anda.

Perbarui file `CMakeLists.txt` pada bagian `FetchContent` menjadi seperti ini:
```cmake
# (Di dalam file cmake/install-rules.cmake)

# Salin isi dari mypConfig.cmake yang dibuat otomatis agar menyertakan fmt
file(WRITE "${PROJECT_BINARY_DIR}/mypConfig.cmake"
[[
include(CMakeFindDependencyMacro)

# Memastikan file target fmt yang ikut terinstal dapat ditemukan oleh proyek luar
if(NOT TARGET fmt::fmt)
    include("${CMAKE_CURRENT_LIST_DIR}/fmt-targets.cmake" OPTIONAL)
endif()

include("${CMAKE_CURRENT_LIST_DIR}/mypTargets.cmake")
]]
)

install(
    FILES "${PROJECT_BINARY_DIR}/mypConfig.cmake"
    DESTINATION ${cmake_config_dest}
)

```
> Keuntungan: Folder instalasi Anda bersifat mandiri (self-contained). Siapa pun yang mengambil folder hasil instalasi Anda tidak perlu mengunduh {fmt} lagi secara terpisah.

---

Pendekatan 2: Menggunakan Skema Penamaan Kustom (Custom Suffix/Prefix)
Jika Anda ingin memberikan nama file kustom pada hasil kompilasi executable proyek Anda (misal: menambahkan akhiran `-d` untuk mode Debug, atau memberikan nama spesifik berdasarkan arsitektur), Anda bisa memanfaatkan properti target CMake di `CMakeLists.txt`.

Tambahkan konfigurasi properti berikut tepat di bawah deklarasi `add_executable`:
```cmake
# ---- Declare executable ----

add_executable(myp_exe source/main.cpp)
add_executable(myp::exe ALIAS myp_exe)

# 1. Skema nama file dasar hasil kompilasi
set_property(TARGET myp_exe PROPERTY OUTPUT_NAME "myp-app")

# 2. Skema penamaan dinamis berdasarkan konfigurasi Build (Kustomisasi Fitur)
# Menambahkan suffix '-d' jika dicompile dalam mode Debug (contoh hasil: myp-app-d.exe)
set_target_properties(myp_exe PROPERTIES
    DEBUG_POSTFIX "-d"
    RELEASE_POSTFIX ""
    MINSIZEREL_POSTFIX "-min"
)

```

---

🚀 Cara Eksekusi dan Verifikasi Hasil
Jalankan perintah ini di terminal Anda untuk melihat pemisahan struktur dan penyalinan otomatis pustaka `{fmt}`:
```bash
# 1. Konfigurasi dengan folder output instalasi di "./dist"
cmake -B build -S . -DCMAKE_INSTALL_PREFIX=./dist -DCMAKE_BUILD_TYPE=Debug

# 2. Compile proyek
cmake --build build

# 3. Lakukan instalasi
cmake --install build

```
Setelah proses selesai, periksa folder `dist/`. Anda akan melihat struktur otomatis seperti berikut:
- `dist/bin/myp-app-d` (Menggunakan skema nama kustom Debug)
- `dist/include/fmt/` (Header milik `{fmt}` otomatis tersalin)
- `dist/lib/` (File library `fmt` atau objek proyek Anda otomatis tersalin)
- `dist/lib/cmake/fmt/` (Konfigurasi CMake milik `{fmt}`)

<br>

q-ai:
Jika Anda ingin beralih ke skema instalasi yang **tidak menyalin file fisik** melainkan meminta komputer target mengunduhnya via manajer paket (seperti *vcpkg* atau *Conan*), atau butuh penyesuaian penamaan lainnya, beri tahu saya!

<br>

qc:
Saya ingin beralih ke skema instalasi yang **tidak menyalin file fisik** melainkan meminta komputer target mengunduhnya via manajer paket *Conan*

<br>

a-ai:
Beralih ke manajer paket seperti **Conan** adalah keputusan yang sangat tepat untuk manajemen dependensi C++ modern. Dengan skema ini, kita akan **menghapus instruksi `FetchContent`** dari CMake dan menyerahkan tugas pengunduhan serta manajemen pustaka `{fmt}` sepenuhnya kepada Conan.

Ketika proyek Anda diinstal dan dipakai oleh pengguna lain (*downstream*), komputer target cukup menjalankan perintah Conan untuk menarik dependensi yang dibutuhkan secara otomatis.

Berikut adalah langkah-langkah migrasi dan perubahan kodenya:

---

1. Buat File Resep Conan (`conanfile.txt`)
Buat file baru bernama `conanfile.txt` di direktori utama proyek Anda (sejajar dengan `CMakeLists.txt`). File ini berfungsi untuk memberi tahu Conan pustaka apa saja yang dibutuhkan dan *generator* apa yang harus digunakan:
```ini
[requires]
fmt/12.2.0

[generators]
CMakeDeps
CMakeToolchain

```

---

2. Perbarui sCMakeLists.txt` (Hapus FetchContent)
Ganti blok kode `FetchContent` lama Anda dengan perintah `find_package` standar. Conan akan menghasilkan file konfigurasi yang membuat perintah `find_package` ini bekerja secara otomatis.

Perbarui `CMakeLists.txt` Anda menjadi seperti ini:
```cmake
cmake_minimum_required(VERSION 3.15) # Conan sangat merekomendasikan minimal CMake 3.15+

include(cmake/prelude.cmake)

project(
    myp
    VERSION 0.1.0
    DESCRIPTION "My Simple Program"
    HOMEPAGE_URL "https://exam.co"
    LANGUAGES CXX
)

include(cmake/project-is-top-level.cmake)
include(cmake/variables.cmake)

# ---- Pustaka Pihak Ketiga via Conan ----

# Mencari pustaka fmt yang disediakan oleh Conan di komputer target
find_package(fmt REQUIRED)

# ---- Declare library ----

add_library(
    myp_lib OBJECT
    source/lib.cpp
)

target_include_directories(
    myp_lib ${warning_guard}
    PUBLIC
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/source>"
)

target_compile_features(myp_lib PUBLIC cxx_std_20)

# Menghubungkan target fmt resmi dari paket Conan
target_link_libraries(myp_lib PRIVATE fmt::fmt)

# ---- Declare executable ----

add_executable(myp_exe source/main.cpp)
add_executable(myp::exe ALIAS myp_exe)

set_property(TARGET myp_exe PROPERTY OUTPUT_NAME "myp")
target_compile_features(myp_exe PRIVATE cxx_std_20)
target_link_libraries(myp_exe PRIVATE myp_lib)

# ---- Install rules ----

if(NOT CMAKE_SKIP_INSTALL_RULES)
  include(cmake/install-rules.cmake)
endif()

# ---- Developer mode ----
if(NOT myp_DEVELOPER_MODE)
  return()
endif()

include(cmake/dev-mode.cmake)

```

---

3. Perbarui `cmake/install-rules.cmake`
Karena kita ingin komputer target mencari `{fmt}` sendiri, kita harus memasukkan instruksi `find_dependency(fmt REQUIRED)` ke dalam file konfigurasi CMake proyek Anda (`mypConfig.cmake`) saat diinstal.

Perbarui bagian pembuatan file konfigurasi di `cmake/install-rules.cmake`:
```cmake
# (Di dalam file cmake/install-rules.cmake)

set(cmake_config_dest "${CMAKE_INSTALL_LIBDIR}/cmake/myp")

# ... (bagian install TARGETS dan HEADERS tetap sama seperti sebelumnya) ...

# Tulis file konfigurasi yang akan mendeteksi dependensi di komputer target
file(WRITE "${PROJECT_BINARY_DIR}/mypConfig.cmake"
[[
include(CMakeFindDependencyMacro)

# Memaksa komputer target untuk mencari pustaka fmt (lewat Conan atau sistem mereka)
find_dependency(fmt REQUIRED) 

include("${CMAKE_CURRENT_LIST_DIR}/mypTargets.cmake")
]]
)

install(
    FILES "${PROJECT_BINARY_DIR}/mypConfig.cmake"
    DESTINATION ${cmake_config_dest}
)

```

---

🚀 Cara Kompilasi Menggunakan Conan 2.x
Sekarang, alur kerja kompilasi Anda akan melibatkan Conan untuk memasang dependensi terlebih dahulu sebelum CMake dijalankan. Ikuti perintah terminal berikut:
```bash
# 1. Install dependensi lewat Conan (ini akan menghasilkan file CMakeToolchain)
# Perintah ini mendeteksi compiler Anda dan mengunduh fmt binary yang cocok
conan install . --output-folder=build --build=missing -s build_type=Release

# 2. Konfigurasi CMake dengan menyertakan toolchain dari Conan
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=build/conan_toolchain.cmake -DCMAKE_INSTALL_PREFIX=./dist -DCMAKE_BUILD_TYPE=Release

# 3. Kompilasi Proyek
cmake --build build

# 4. Instalasi Proyek
cmake --install build

```

---

Outputs Terminal:
```bash
$ cmake --install build
-- Install configuration: "RelWithDebInfo"
-- Installing: /data/data/com.termux/files/home/myp_u2/dist/bin/myp
-- Installing: /data/data/com.termux/files/home/myp_u2/dist/lib/objects-RelWithDebInfo/myp_lib/source/lib.cpp.o
-- Installing: /data/data/com.termux/files/home/myp_u2/dist/include/myp
-- Installing: /data/data/com.termux/files/home/myp_u2/dist/include/myp/lib.hpp
-- Installing: /data/data/com.termux/files/home/myp_u2/dist/lib/cmake/myp/mypTargets.cmake
-- Installing: /data/data/com.termux/files/home/myp_u2/dist/lib/cmake/myp/mypTargets-relwithdebinfo.cmake
-- Installing: /data/data/com.termux/files/home/myp_u2/dist/lib/cmake/myp/mypConfigVersion.cmake
-- Installing: /data/data/com.termux/files/home/myp_u2/dist/lib/cmake/myp/mypConfig.cmake

```
📂 Isi dari Folder `dist` Anda:
Di dalam folder `dist` tersebut, CMake telah menyusun hasil kompilasi program Anda ke dalam struktur folder standar:
- 📂 `bin/`: Berisi file eksekutif utama bernama `myp`. Anda bisa menjalankannya dengan perintah `./bin/myp`.
- 📂 `include/myp/`: Berisi file header C++ (`lib.hpp`) agar pustaka (*library*) ini bisa digunakan oleh proyek lain.
- 📂 `lib/`: Berisi objek kompilasi dan konfigurasi CMake (`mypConfig.cmake`, dll) agar proyek lain dapat menemukan pustaka ini dengan perintah `find_package(myp)`.

q-ai:
Apakah Anda ingin **menjalankan file eksekutif `myp`** tersebut sekarang, atau ingin **menghubungkannya ke proyek C++ lain**?

<br>

q-ai:
Apakah Anda ingin mengubah lokasi instalasi ini ke folder sistem yang umum (seperti /usr/local), atau apakah Anda mengalami kendala saat menjalankan tahap --install?

<br>

qc:
Saya ingin mengubah lokasi instalasi ini ke folder sistem yang umum

<br>

a-ai:
Untuk mengubah lokasi instalasi ke folder sistem yang umum di Termux, Anda harus mengubah nilai `CMAKE_INSTALL_PREFIX` menjadi `/data/data/com.termux/files/usr` (setara dengan `/usr/local` di Linux biasa).

Termux memiliki struktur folder sistem sendiri karena tidak menggunakan root Android standar.

🛠️ Langkah-Langkah Mengubah Lokasi
1. Atur ulang konfigurasi CMake
Jalankan perintah konfigurasi ulang dengan mengarahkan *prefix* ke folder sistem Termux:
```bash
cmake -B build -S . -DCMAKE_INSTALL_PREFIX=/data/data/com.termux/files/usr

```
2. Kompilasi ulang proyek
```bash
cmake --build build

```
3. Lakukan instalasi ke sistem
```bash
cmake --install build

```

📂 Di Mana File Anda Sekarang?
Setelah langkah di atas selesai, file Anda akan tersebar secara otomatis ke folder sistem global Termux:
- Aplikasi utama (myp): Masuk ke `/data/data/com.termux/files/usr/bin/myp`. Anda sekarang bisa menjalankan program cukup dengan mengetik myp dari folder mana saja.
- File Header (lib.hpp): Masuk ke `/data/data/com.termux/files/usr/include/myp/`.
- Konfigurasi CMake: Masuk ke `/data/data/com.termux/files/usr/lib/cmake/myp/`. Proyek C++ lain di Termux kini bisa langsung mendeteksinya via `find_package(myp)`.

<br>

q-ai:
Apakah Anda ingin **menguji coba menjalankan perintah `myp`** langsung dari terminal, atau ingin **menghapus sisa folder `dist` lama** yang berada di folder *home*?

<br>

qc:
ingin membersihkan sisa file kompilasi temporer

<br>

a-ai:
Untuk membersihkan sisa file kompilasi temporer, Anda bisa menghapus folder `build` yang dibuat oleh CMake sebelumnya.

💡 Alternatif Menggunakan Perintah Bawaan CMake
Jika di masa mendatang Anda ingin membersihkan hasil kompilasi *tanpa* menghapus seluruh folder konfigurasi `build`, Anda bisa menggunakan perintah bawaan CMake berikut:
```bash
cmake --build build --target clean

```
> Catatan: Perintah clean di atas hanya menghapus file objek kompilasi seperti `.o`, tetapi tetap mempertahankan file konfigurasi dasar CMake.


---



📊 Hasil Akhir Struktur Folder `dist/`
Setelah Anda menjalankan `cmake --install`, folder `dist/` hanya akan berisi file asli dari proyek Anda sendiri:
- `dist/bin/myp` (Aplikasi Anda)
- `dist/include/myp/` (Header Anda sendiri)
- `dist/lib/cmake/myp/mypConfig.cmake` (File konfigurasi yang menginstruksikan `find_dependency(fmt REQUIRED)`)
File fisik dari `{fmt}` **tidak akan ikut tersalin** ke folder `dist/`, menjaga ukuran paket instalasi Anda tetap kecil dan bersih. Ketika orang lain menggunakan perintah `find_package(myp)`, CMake mereka akan otomatis ikut mencari `{fmt}` di komputer mereka melalui Conan.

<br>

q-ai:
Apakah Anda ingin membuat proyek ini agar bisa **dibuat menjadi paket Conan sendiri** (sehingga orang lain bisa mengetik `requires = myp/0.1.0` di proyek mereka), atau ada konfigurasi profil arsitektur tertentu yang ingin disesuaikan?

<br>

qc:
Saya ingin membuat proyek ini agar bisa **dibuat menjadi paket Conan sendiri** (sehingga orang lain bisa mengetik `requires = myp/0.1.0` di proyek mereka)

<br>

a-ai:
Untuk mengubah proyek Anda menjadi paket Conan mandiri yang dapat digunakan oleh proyek lain dengan menulis `requires = myp/0.1.0`, kita perlu membuat file resep paket C++ resmi menggunakan **Conan 2.x**. File ini dinamakan `conanfile.py`.

Dengan skema ini, fungsi instalasi fisik yang sebelumnya ditangani oleh `cmake/install-rules.cmake` akan diambil alih secara otomatis dan dibungkus dengan standar manajemen paket Conan.

Berikut adalah langkah-langkah implementasinya:

---

1. Buat File Resep Paket (`conanfile.py`)
Hapus file `conanfile.txt` yang lama (jika ada), lalu buat file bernama `conanfile.py` di direktori utama proyek Anda (sejajar dengan `CMakeLists.txt`). Masukkan kode python standar Conan 2.x di bawah ini:
```python
from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout, CMakeDeps

class MypRecipe(ConanFile):
    name = "myp"
    version = "0.1.0"
    package_type = "library"

    # Informasi Metadata Paket
    description = "My Simple Program"
    homepage = "https://exam.co"
    license = "MIT" # Sesuaikan dengan lisensi proyek Anda
    topics = ("cpp", "fmt", "library")

    # Pengaturan Arsitektur dan Compiler target
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {"shared": False, "fPIC": True}

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def configure(self):
        if self.options.shared:
            self.options.rm_safe("fPIC")

    def layout(self):
        # Mengatur tata letak folder build standar CMake & Conan
        cmake_layout(self)

    def requirements(self):
        # Deklarasi dependensi yang dibutuhkan oleh paket myp
        # 'transitive_headers=True' memastikan header 'fmt' ikut diekspos ke proyek downstream
        self.requires("fmt/12.2.0", transitive_headers=True)

    def generate(self):
        # Menghasilkan toolchain dan dependensi CMake otomatis saat kompilasi paket
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self)
        tc.generate()

    def build(self):
        # Proses kompilasi kode program menggunakan CMake
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        # Mengambil file hasil kompilasi dan instalasi dari CMake
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        # Memberi tahu proyek downstream target apa saja yang tersedia
        # Karena di CMakeLists Anda mendaftarkan ALIAS myp::exe untuk myp_exe
        self.cpp_info.set_property("cmake_target_name", "myp::myp")
        
        # Konfigurasi jika proyek downstream menggunakan komponen library objek Anda
        self.cpp_info.libs = ["myp_lib"]

```

---

2. Penyesuaian Minimal pada `cmake/install-rules.cmake`
Agar mekanisme `cmake.install()` milik Conan di atas dapat berjalan dengan sempurna, pastikan file `cmake/install-rules.cmake` Anda menggunakan variabel path standar CMake (`CMAKE_INSTALL_INCLUDEDIR`, dll) seperti yang sudah kita buat sebelumnya.

Conan akan menyuntikkan folder penyimpanan paket internalnya secara otomatis ke variabel `CMAKE_INSTALL_PREFIX`. Jadi Anda tidak perlu lagi melakukan hardcode lokasi direktori luar.

---

🚀 Cara Membuat Paket dan Mengujinya Secara Lokal
Untuk mengemas proyek Anda ke dalam penyimpanan lokal komputer Anda (*Conan Local Cache*), jalankan perintah berikut di terminal:
```bash
# Membuat paket myp/0.1.0 di lokal cache komputer Anda
conan create . --build=missing

```
Perintah di atas akan mengeksekusi siklus hidup Conan: mendownload `{fmt}`, mengompilasi proyek `myp` Anda, mengekstrak file binary/header hasil kompilasi, lalu menyimpannya sebagai paket resmi.

---

💡 Cara Proyek Lain Menggunakan Paket Anda
Setelah perintah `conan create` berhasil, proyek C++ lain di komputer Anda sekarang sudah bisa memanggil paket Anda. Mereka hanya perlu membuat konfigurasi seperti ini:

Contoh `conanfile.txt` di proyek lain:
```txt
[requires]
myp/0.1.0

[generators]
CMakeDeps
CMakeToolchain

```
Contoh CMakeLists.txt` di proyek lain:
```cmake
cmake_minimum_required(VERSION 3.15)
project(DownstreamApp LANGUAGES CXX)

find_package(myp REQUIRED)

add_executable(app main.cpp)
# Menghubungkan ke paket buatan Anda yang ditarik otomatis oleh Conan
target_link_libraries(app PRIVATE myp::myp) 

```

<br>

q-ai:
Apakah Anda ingin panduan lebih lanjut tentang cara **mengunggah (upload) paket ini ke server remote** seperti JFrog Artifactory atau ConanCenter agar bisa diakses oleh tim Anda di komputer berbeda?























<br>
