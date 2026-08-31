# cmake_cmakelists


if (${CMAKE_VERSION} VERSION_LESS 3.12)
  cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif ()

Potongan kode tersebut merupakan **mekanisme fallback (pilihan cadangan) untuk mengatur kebijakan perilaku (policies) CMake ke versi yang lebih baru** ketika proyek dikompilasi menggunakan CMake versi lama (di bawah versi 3.12).

Berikut adalah penjelasan detail mengenai cara kerja baris kode tersebut:

1. `if (${CMAKE_VERSION} VERSION_LESS 3.12)`
Baris ini memeriksa versi aplikasi CMake yang saat ini sedang berjalan di sistem pengguna. Jika versi CMake yang digunakan **lebih kecil / lebih tua dari versi 3.12**, maka blok perintah di dalam `if` akan dieksekusi.
2. `cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})`
Di dalam blok `if`, perintah ini dipanggil untuk memaksa CMake menggunakan semua aturan perilaku (*policies*) yang sesuai dengan versi CMake maksimal yang saat ini terinstal di sistem pengguna.
- `${CMAKE_MAJOR_VERSION}` dan `${CMAKE_MINOR_VERSION}` adalah variabel bawaan CMake yang menyimpan angka versi CMake yang sedang aktif saat itu (misalnya: jika menggunakan CMake 3.11, maka nilainya adalah `3` dan `11`).

Mengapa Blok Kode Ini Diperlukan?
Pada baris pertama file `CMakeLists.txt`, terdapat deklarasi:
```cmake
cmake_minimum_required(VERSION 3.8...3.28)

```
- Pada CMake 3.12 ke atas: Fitur rentang versi (`3.8...3.28`) ini didukung secara resmi. CMake akan otomatis mengaktifkan semua *policies* baru hingga versi `3.28` (atau versi tertinggi yang terpasang di sistem pengguna jika di bawah 3.28).
- Pada CMake di bawah 3.12: CMake versi lama belum mengenali simbol titik tiga (`...`). Mereka akan mengabaikan bagian `...3.28` dan **hanya membaca versi `3.8`**. Akibatnya, CMake hanya akan mengaktifkan kebijakan (policies) lama bawaan versi 3.8, meskipun pengguna sebenarnya memiliki versi yang sedikit lebih baru (seperti CMake 3.10 atau 3.11).
Kesimpulan:
Sepotong kode tersebut berfungsi sebagai jembatan agar CMake versi 3.9 hingga 3.11 tetap dapat menggunakan policies terbaru yang tersedia di sistem mereka, meniru perilaku bawaan yang seharusnya baru ada di CMake 3.12.

---

qs:
FMT_MASTER_PROJECT

dimanan aq bisa menemukan variabel itu ?

---

aa:
Anda dapat menemukan atau mengatur variabel `FMT_MASTER_PROJECT` di **dua tempat utama**, tergantung pada apakah Anda ingin melihat bagaimana variabel tersebut didefinisikan secara internal, atau Anda ingin mengubah nilainya dari luar proyek.

Berikut adalah panduan tempat untuk menemukannya:

1. Di Dalam File `CMakeLists.txt` Internal milik `{fmt}`
Jika Anda mencari baris kode tempat variabel ini pertama kali diproses secara internal, Anda bisa menemukannya langsung di file `CMakeLists.txt` yang Anda kirimkan sebelumnya (tepatnya di bagian atas file):
```cmake
# Determine if fmt is built as a subproject (using add_subdirectory) or if it is
# the master project.
if (NOT DEFINED FMT_MASTER_PROJECT)
  set(FMT_MASTER_PROJECT OFF)
  # Checking project name is more reliable than checking source directories.
  if (NOT DEFINED PROJECT_NAME)
    set(FMT_MASTER_PROJECT ON)
    message(STATUS "CMake version: ${CMAKE_VERSION}")
  endif ()
endif ()

```
- Logika Internal: Blok kode di atas memeriksa apakah variabel `FMT_MASTER_PROJECT` sudah didefinisikan sebelumnya. Jika belum (`NOT DEFINED`), sistem secara otomatis akan mendeteksinya:
    - Nilai diatur ke `ON` jika proyek `{fmt}` dibangun secara mandiri sebagai proyek utama (berdiri sendiri).
    - Nilai diatur ke `OFF` jika `{fmt}` dimasukkan sebagai sub-proyek (misalnya melalui perintah `add_subdirectory()`) di dalam aplikasi utama Anda.

---

2. Di Dalam File `CMakeLists.txt` Milik Anda Sendiri (Sebagai Proyek Utama)
Jika Anda mengintegrasikan library `{fmt}` ke dalam aplikasi Anda (menggunakan perintah seperti `add_subdirectory()` atau `FetchContent`), Anda tidak perlu mengedit file `CMakeLists.txt` bawaan milik `{fmt}`.

Anda bisa **mendefinisikan variabel ini di dalam file `CMakeLists.txt` milik proyek Anda sendiri** tepat sebelum Anda memanggil library `{fmt}`:
```cmake
cmake_minimum_required(VERSION 3.15)
project(AplikasiSaya CXX)

# Tulis variabel FMT_MASTER_PROJECT di sini untuk memaksa nilainya
set(FMT_MASTER_PROJECT ON CACHE BOOL "Force fmt to act as master project")

# Baru kemudian panggil folder library fmt
add_subdirectory(path/to/fmt) 

```

---

3. Melalui Baris Perintah (Terminal / Command Line) saat Kompilasi
Anda tidak wajib menuliskan variabel ini di dalam file teks. Anda juga bisa menemukannya atau menyuntikkannya langsung saat menjalankan perintah `cmake` di terminal menggunakan argumen `-D`:
```bash
cmake -DFMT_MASTER_PROJECT=ON ..

```
Perintah ini akan langsung mendaftarkan variabel `FMT_MASTER_PROJECT` ke dalam sistem *cache* CMake untuk proses kompilasi tersebut.

<br>

qa:
Apakah Anda sedang mencoba mengatasi masalah kompilasi tertentu (seperti target pengujian atau instalasi `{fmt}` yang tidak muncul)? Beritahu saya agar saya bisa membantu **mengarahkan konfigurasi yang tepat** untuk proyek Anda.

---

- Mengatur Melalui Proyek Induk (Jika `{fmt}` adalah Sub-proyek)
Jika Anda memasukkan folder `{fmt}` ke dalam proyek aplikasi Anda sendiri menggunakan perintah `add_subdirectory()`, Anda bisa memodifikasi variabel tersebut di dalam `CMakeLists.txt` milik **proyek Anda**, tepat **sebelum** perintah `add_subdirectory()` dipanggil.
```cmake
cmake_minimum_required(VERSION 3.15)
project(AplikasiUtamaSaya CXX)

# 1. Modifikasi standar C++ untuk seluruh proyek termasuk {fmt}
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 2. Modifikasi agar {fmt} dibangun sebagai library dinamis (.dll/.so)
set(BUILD_SHARED_LIBS ON CACHE BOOL "Build shared libraries" FORCE)

# 3. Panggil folder library {fmt}
add_subdirectory(third_party/fmt)

# 4. Hubungkan dengan aplikasi Anda
add_executable(AplikasiSaya main.cpp)
target_link_libraries(AplikasiSaya PRIVATE fmt::fmt)

```















<br>
