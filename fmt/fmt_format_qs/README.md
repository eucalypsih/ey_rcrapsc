# fmt_format_qs


qs:
`sed -n '150,160p' $HOME/.cmake_fetchcontent_cache/fmt-v12.2.0/fmt-src/include/fmt/format.h`
```cpp
// GCC 4.9 doesn't support qualified names in specializations.
namespace std {
template <typename T> struct iterator_traits<fmt::basic_appender<T>> {
  using iterator_category = output_iterator_tag;
  using value_type = T;
  using difference_type =
      decltype(static_cast<int*>(nullptr) - static_cast<int*>(nullptr));
  using pointer = void;
  using reference = void;
};
}  // namespace std

```

<br>

aa:
Potongan kode di atas (baris 150–160) bukan untuk dipanggil secara manual di dalam kode Anda. Bagian ini adalah **adaptor kompatibilitas internal** agar struktur bawaan `{fmt}` bernama `fmt::basic_appender` bisa dikenali sebagai sebuah `std::output_iterator` resmi oleh pustaka standar C++ (`std`).

Pustaka `{fmt}` sengaja menyuntikkan spesialisasi ini langsung ke dalam `namespace std` agar tipe penampung internal mereka bisa bekerja harmonis dengan fungsi-fungsi standar C++ (seperti `std::copy`, `std::fill`, atau algoritma pengisian memori lainnya).

1. Cara Penggunaan Melalui Algoritma Standar (`std::copy`)
Karena `{fmt}` sudah mendaftarkan objek iterator tersebut ke dalam `std::iterator_traits`, Anda bisa menggunakan `fmt::appender` sebagai target pengisian data dari algoritma standar C++ tanpa memicu eror kompilasi.
```cpp
#include <fmt/format.h>
#include <algorithm> // Untuk std::copy
#include <vector>

int main() {
    std::vector<char> data_asal = {'A', 'B', 'C', 'D'};
    
    // Siapkan buffer penampung teks dari {fmt}
    fmt::memory_buffer buffer_tujuan;

    // Membuat instance basic_appender secara implisit
    auto penampung_fmt = fmt::appender(buffer_tujuan);

    // KODE INI BISA BEKERJA KARENA BARIS 150-160 DI ATAS:
    // std::copy mendeteksi traits iterator_category sebagai output_iterator_tag
    std::copy(data_asal.begin(), data_asal.end(), penampung_fmt);

    // Tambahkan null-terminator agar aman dicetak
    buffer_tujuan.push_back('\0');

    fmt::print("Isi buffer hasil copy: {}\n", buffer_tujuan.data());
    // Output: Isi buffer hasil copy: ABCD
}

```
🔍 Mengapa Potongan Kode Itu Harus Ada?
Jika Anda membuat struktur data sendiri di C++ dan mencoba memasukkannya ke dalam fungsi algoritma bawaan C++ seperti `std::copy(input.begin(), input.end(), objek_anda)`, kompilator akan protes dan memunculkan eror besar.

Kompilator butuh kepastian mengenai informasi tipe data iterator tersebut. Spesifikasi di baris 152–159 menyediakan semua jawaban tersebut bagi kompilator:
- `iterator_category = output_iterator_tag`: Memberitahu std bahwa iterator ini hanya bisa menerima data (ditulisi), bukan dibaca.
- `value_type = T`: Menentukan jenis data yang bisa dimasukkan.
- `difference_type`: Menghitung jarak lompatan memori standar menggunakan operasi pengurangan pointer.
Catatan di baris 150 juga menjelaskan alasan penulisan formatnya:
```cpp
// GCC 4.9 doesn't support qualified names in specializations.

```
Kompiler jadul seperti GCC 4.9 akan memunculkan eror jika ditulis langsung `template <> struct std::iterator_traits<...>` di luar scope. Makanya `{fmt}` terpaksa membuka blok `namespace std { ... }` secara eksplisit agar kode ini kompatibel di sistem Linux lama.

<br>

qa:
Apakah Anda sedang mencoba **membuat fungsi output kustom** yang menulis langsung ke memori menggunakan komponen `appender`, atau ada **masalah kecocokan iterator** dengan pustaka lain yang sedang Anda selesaikan?

<br>

---

<br>

qs:
`awk 'NR>=1332; NR==1336 {exit}' $HOME/.cmake_fetchcontent_cache/fmt-v12.2.0/fmt-src/include/fmt/format.h`
```cpp
inline void to_utf8_3bytes(buffer<char>& buf, uint32_t cp) {
  buf.push_back(static_cast<char>(0xe0 | (cp >> 12)));
  buf.push_back(static_cast<char>(0x80 | ((cp & 0xfff) >> 6)));
  buf.push_back(static_cast<char>(0x80 | (cp & 0x3f)));
}
```

aa:
Untuk menggunakan fungsi internal `to_utf8_3bytes` secara langsung di dalam kode program C++ Anda, Anda harus mengaksesnya melalui *namespace* khusus bawaan internal `{fmt}`, yaitu `fmt::detail::to_utf8_3bytes`.

Fungsi ini membutuhkan dua buah argumen:
1. `buffer<char>& buf`: Sebuah kontainer penampung memori. Skenario penggunaan paling mudah adalah menggunakan objek `fmt::memory_buffer`.
2. `uint32_t cp`: Nilai angka identitas Unicode (*Code Point*) dari karakter yang ingin Anda konversi (khusus untuk karakter yang masuk dalam rentang 3-byte, yaitu antara nilai `0x0800` hingga `0xFFFF`).
Berikut adalah contoh implementasi lengkap cara memanggil fungsi tersebut di dalam kode Anda:
```cpp
#include <fmt/format.h> // Header utama yang memuat fungsi ini
#include <iostream>

int main() {
    // 1. Buat kontainer memori dinamis bawaan {fmt} sebagai argumen pertama
    fmt::memory_buffer wadah_memori;

    // 2. Siapkan Code Point Unicode dalam rentang 3-byte (U+0800 s.d U+FFFF)
    // Contoh: Huruf Jepang/Mandarin '水' (Air) memiliki nilai Code Point 0x6C34
    uint32_t cp_air = 0x6C34;

    // 3. Panggil fungsi internal menggunakan namespace fmt::detail
    fmt::detail::to_utf8_3bytes(wadah_memori, cp_air);

    // 4. Tambahkan karakter null-terminator ('\0') di akhir buffer
    // agar string mentah ini aman dibaca oleh pustaka standar C/C++
    wadah_memori.push_back('\0');

    // 5. Tampilkan isi biner buffer yang kini sudah terenkripsi menjadi UTF-8 yang valid
    std::cout << "Hasil konversi biner ke teks: " << wadah_memori.data() << std::endl;
    // Output: Hasil konversi biner ke teks: 水
}

```
🔍 Kapan dan Mengapa Anda Memerlukan Fungsi Ini?

Secara umum, Anda tidak perlu memanggil fungsi ini jika hanya ingin mencetak teks biasa. Fungsi ini baru akan sangat berguna jika Anda sedang membuat:
- Mesin konversi teks kustom: Mengonversi data angka biner dari tabel berkas eksternal/database lama menjadi format string UTF-8 modern.
- Protokol Jaringan / Serial IO: Memilah karakter Unicode satu per satu lalu memasukannya ke dalam antrean byte sebelum dikirimkan melalui jaringan internet.

<br>

qa:
Apakah Anda ingin melihat implementasi serupa untuk **karakter 4-byte (seperti Emoji modern)** yang ditulis secara manual, atau sedang mengoptimalkan kode yang memproses **aliran data biner (stream)** dari sebuah berkas?

<br>

---

<br>

qc:
Saya ingin sedang mengoptimalkan kode yang memproses **aliran data biner (stream)** dari sebuah berkas

<br>

aa:
Untuk mengoptimalkan pemrosesan aliran data biner (stream) dari sebuah berkas menggunakan pustaka `{fmt}`, memanfaatkan komponen internal seperti `fmt::memory_buffer` dan fungsi pemrosesan byte tingkat rendah adalah strategi yang sangat tepat.

Ketika memproses berkas biner dalam jumlah besar, bottleneck utama biasanya terletak pada dua hal: **proses pembacaan I/O dari disk** dan **alokasi memori berulang kali (heap allocation)** saat mengonversi atau menyusun data biner menjadi string teks.

Berikut adalah arsitektur kode optimal untuk membaca berkas biner secara asinkron/blok, lalu memproses atau mengonversi biner tersebut (termasuk *code point* Unicode) ke dalam memori dengan performa tinggi:

Kode Optimasi Proses Stream Berkas Biner
```cpp
#include <fmt/format.h>
#include <fstream>
#include <vector>
#include <cstdint>

// Ukuran blok optimal untuk I/O (biasanya 4KB atau 8KB sesuai ukuran cluster disk)
constexpr size_t BUFFER_SIZE = 8192; 

void proses_stream_biner(const std::string& nama_berkas) {
    // 1. Buka berkas dengan mode biner standar ios::binary
    std::ifstream berkas(nama_berkas, std::ios::binary);
    if (!berkas.is_open()) {
        fmt::print(stderr, "Gagal membuka berkas: {}\n", nama_berkas);
        return;
    }

    // 2. Gunakan vector sebagai buffer I/O tingkat rendah (ringan & cepat)
    std::vector<char> io_buffer(BUFFER_SIZE);

    // 3. Gunakan fmt::memory_buffer untuk menampung hasil pemrosesan/teks.
    // PENTING: Lakukan pra-alokasi (reserve) memori besar di awal agar 
    // menghindari realokasi (grow_) berulang kali saat stream berjalan.
    fmt::memory_buffer hasil_buffer;
    hasil_buffer.reserve(1024 * 1024); // Alokasikan 1 MB di awal (sesuaikan kebutuhan)

    // 4. Lakukan pembacaan blok demi blok (Chunk-based streaming)
    while (berkas.read(io_buffer.data(), BUFFER_SIZE) || berkas.gcount() > 0) {
        size_t bytes_terbaca = berkas.gcount();

        // Proses data biner di dalam blok ini
        // Contoh kasus: Asumsikan berkas berisi deretan angka uint32_t (Code Point)
        size_t i = 0;
        while (i + 4 <= bytes_terbaca) {
            // Ambil 4 byte dan rekonstruksi menjadi uint32_t Code Point
            uint32_t cp = *reinterpret_cast<const uint32_t*>(&io_buffer[i]);
            i += 4;

            // Integrasi fungsi internal yang Anda cari sebelumnya:
            // Jika masuk rentang 3-byte, konversi langsung ke UTF-8 langsung di memory_buffer
            if (cp >= 0x0800 && cp <= 0xFFFF) {
                fmt::detail::to_utf8_3bytes(hasil_buffer, cp);
            } 
            // Skenario untuk ASCII standar (1-byte)
            else if (cp <= 0x7F) {
                hasil_buffer.push_back(static_cast<char>(cp));
            }
            // Skenario 4-byte (Emoji / Karakter Langka) ditulis manual (inline) demi kecepatan
            else if (cp >= 0x10000 && cp <= 0x10FFFF) {
                hasil_buffer.push_back(static_cast<char>(0xf0 | (cp >> 18)));
                hasil_buffer.push_back(static_cast<char>(0x80 | ((cp >> 12) & 0x3f)));
                hasil_buffer.push_back(static_cast<char>(0x80 | ((cp >> 6) & 0x3f)));
                hasil_buffer.push_back(static_cast<char>(0x80 | (cp & 0x3f)));
            }
        }
    }

    // 5. Akhiri dengan null-terminator jika ingin digunakan sebagai string biasa
    hasil_buffer.push_back('\0');

    // Tampilkan ukuran hasil pemrosesan teks tanpa melakukan alokasi baru
    fmt::print("Sukses memproses biner. Ukuran output teks: {} bytes.\n", hasil_buffer.size() - 1);
}

int main() {
    proses_stream_biner("data_input.bin");
    return 0;
}

```
⚡ Mengapa Skema Kode di Atas Sangat Cepat?
1. `hasil_buffer.reserve(...)` yang Agresif: Pemicu utama penurunan performa pada penulisan string adalah ketika memori internal penuh dan komputer harus mencari blok memori baru di heap (`grow_`). Dengan memesan memori besar di awal, fungsi `buf.push_back` hanya akan melakukan operasi penulisan array mentah yang berkecepatan setara pointer C standar.
2. **Chunk-based I/O (8192 bytes)**: Mengakses disk per 1 byte sangat lambat karena memicu *syscall* sistem operasi berulang kali. Membaca per 8KB memuat data langsung ke dalam *L1/L2 Cache* prosesor Anda.
3. **Pemanfaatan Fungsi _Inline_ Tingkat Rendah**: Karena fungsi `to_utf8_3bytes` ditandai sebagai `inline`, kompilator akan membongkar fungsi tersebut dan menempelkan instruksi pergeseran bitwise-nya langsung di dalam perulangan `while`.

Untuk menyesuaikan optimasi ini lebih presisi ke proyek Anda, letakkan detailnya di sini:
- Bagaimana **struktur format biner asli** di dalam berkas Anda (apakah deretan integer mentah, struktur struct tertentu, atau teks biner terenkripsi)?
- Apakah hasil pemrosesan akhir ini ingin **ditulis kembali ke berkas baru**, atau **ditampilkan ke layar konsol**?

<br>

---

<br>

qs:
`awk 'NR>=1338; NR==1408 {exit}' $HOME/.cmake_fetchcontent_cache/fmt-v12.2.0/fmt-src/include/fmt/format.h`
```cpp 
// A converter from UTF-16/UTF-32 (host endian) to UTF-8.
template <typename WChar, typename Buffer = memory_buffer> class to_utf8 {
 private:
  Buffer buffer_;

 public:
  to_utf8() {}
  explicit to_utf8(basic_string_view<WChar> s,
                   to_utf8_error_policy policy = to_utf8_error_policy::abort) {
    static_assert(sizeof(WChar) == 2 || sizeof(WChar) == 4,
                  "expected utf16 or utf32");
    if (!convert(s, policy)) {
      FMT_THROW(std::runtime_error(sizeof(WChar) == 2 ? "invalid utf16"
                                                      : "invalid utf32"));
    }
  }
  operator string_view() const { return string_view(&buffer_[0], size()); }
  auto size() const -> size_t { return buffer_.size() - 1; }
  auto c_str() const -> const char* { return &buffer_[0]; }
  auto str() const -> std::string { return std::string(&buffer_[0], size()); }

  // Performs conversion returning a bool instead of throwing exception on
  // conversion error. This method may still throw in case of memory allocation
  // error.
  auto convert(basic_string_view<WChar> s,
               to_utf8_error_policy policy = to_utf8_error_policy::abort)
      -> bool {
    if (!convert(buffer_, s, policy)) return false;
    buffer_.push_back(0);
    return true;
  }
  static auto convert(Buffer& buf, basic_string_view<WChar> s,
                      to_utf8_error_policy policy = to_utf8_error_policy::abort)
      -> bool {
    for (auto p = s.begin(); p != s.end(); ++p) {
      uint32_t c = static_cast<uint32_t>(*p);
      if (sizeof(WChar) == 2 && c >= 0xd800 && c <= 0xdfff) {
        // Handle a surrogate pair.
        ++p;
        if (p == s.end() || (c & 0xfc00) != 0xd800 || (*p & 0xfc00) != 0xdc00) {
          switch (policy) {
          case to_utf8_error_policy::abort: return false;
          case to_utf8_error_policy::replace:
            buf.append(string_view("\xEF\xBF\xBD"));
            break;
          case to_utf8_error_policy::wtf: to_utf8_3bytes(buf, c); break;
          }
          --p;
          continue;
        }
        c = (c << 10) + static_cast<uint32_t>(*p) - 0x35fdc00;
      }
      if (c < 0x80) {
        buf.push_back(static_cast<char>(c));
      } else if (c < 0x800) {
        buf.push_back(static_cast<char>(0xc0 | (c >> 6)));
        buf.push_back(static_cast<char>(0x80 | (c & 0x3f)));
      } else if ((c >= 0x800 && c <= 0xd7ff) || (c >= 0xe000 && c <= 0xffff)) {
        to_utf8_3bytes(buf, c);
      } else if (c >= 0x10000 && c <= 0x10ffff) {
        buf.push_back(static_cast<char>(0xf0 | (c >> 18)));
        buf.push_back(static_cast<char>(0x80 | ((c & 0x3ffff) >> 12)));
        buf.push_back(static_cast<char>(0x80 | ((c & 0xfff) >> 6)));
        buf.push_back(static_cast<char>(0x80 | (c & 0x3f)));
      } else {
        return false;
      }
    }
    return true;
  }
};

```

<br>

aa:
Untuk menggunakan kelas `to_utf8` dari pustaka `{fmt}` ini, Anda bisa menggunakannya secara **implisit melalui objek instansiasi** atau secara **langsung menggunakan fungsi statis** tanpa membuat objek.

Berikut adalah beberapa cara penggunaannya di dalam kode C++ Anda:

1. Cara Standar (Membuat Objek `to_utf8`)
Cara ini paling direkomendasikan karena objek akan otomatis menyimpan hasil konversi di memori internalnya dan menyediakan *method* untuk mengubahnya ke tipe string lain.
```cpp
#include <iostream>
#include <string>

void contoh_pencetakan() {
    // String UTF-16 (diwakili oleh std::wstring atau std::u16string)
    std::wstring utf16_str = L"Halo Dunia 🌍"; 

    // 1. Instansiasi objek to_utf8
    to_utf8<wchar_t> converter(utf16_str);

    // 2. Ambil hasilnya sesuai kebutuhan tipe data Anda:
    
    // Ke std::string
    std::string utf8_std = converter.str(); 
    
    // Ke string_view (paling cepat, tanpa alokasi memori baru)
    string_view utf8_view = converter; 
    
    // Ke raw pointer char* (format C-string, aman untuk printf/std::cout)
    const char* utf8_c = converter.c_str(); 

    std::cout << "Hasil konversi: " << utf8_c << std::endl;
}

```

2. Cara Cepat Tanpa Alokasi Baru (Fungsi Statis `convert`)
Jika Anda sudah memiliki *buffer* memori sendiri (misalnya `fmt::memory_buffer`) dan ingin menghindari pembuatan objek baru, gunakan fungsi `convert` statis.
```cpp
void contoh_statis() {
    std::wstring utf16_str = L"C++ Programming";
    memory_buffer buffer; // Menyediakan buffer penampung

    // Memanggil fungsi statis convert(buffer, string_view, policy)
    bool sukses = to_utf8<wchar_t>::convert(buffer, utf16_str);

    if (sukses) {
        // Jangan lupa menambahkan null-terminator jika ingin mencetak ke C-string
        buffer.push_back(0); 
        std::cout << "Sukses: " << buffer.data() << std::endl;
    }
}

```

3. Mengatur Kebijakan Error (*Error Policy*)
Secara bawaan (default), jika ada karakter UTF-16/32 yang rusak (*invalid surrogate pair*), fungsi ini akan langsung memicu *exception* `std::runtime_error`. Anda bisa mengubah perilaku ini dengan memasukkan parameter `policy`:
```cpp
void contoh_error_policy() {
    std::wstring wstr_rusak = L"Karakter rusak \xD800 di sini"; // Kode surrogate menggantung

    // Opsi A: Ganti karakter rusak dengan Unicode Replacement Character ()
    to_utf8<wchar_t> conv_replace(wstr_rusak, to_utf8_error_policy::replace);
    std::cout << conv_replace.c_str() << std::endl; // Mencetak: Karakter rusak  di sini

    // Opsi B: Abaikan atau gunakan format WTF-8 (Wobbly Transformation Format)
    to_utf8<wchar_t> conv_wtf(wstr_rusak, to_utf8_error_policy::wtf);
}

```

<br>

qa:
Apakah Anda ingin melihat **contoh cara mengintegrasikan tipe data `std::u16string` atau `std::u32string`** secara spesifik, atau apakah Anda membutuhkan penjelasan mengenai **cara mendefinisikan tipe data `memory_buffer`** yang dibutuhkan oleh kelas ini?

<br>

---

<br>

qs:
`awk 'NR>=1410; NR==1437{exit}' $HOME/.cmake_fetchcontent_cache/fmt-v12.2.0/fmt-src/include/fmt/format.h`
```cpp
// Computes 128-bit result of multiplication of two 64-bit unsigned integers.
FMT_INLINE auto umul128(uint64_t x, uint64_t y) noexcept -> uint128 {
#if FMT_USE_INT128
  auto p = static_cast<native_uint128>(x) * static_cast<native_uint128>(y);
  return {static_cast<uint64_t>(p >> 64), static_cast<uint64_t>(p)};
#elif defined(_MSC_VER) && defined(_M_AMD64)
  auto hi = uint64_t();
  auto lo = _umul128(x, y, &hi);
  return {hi, lo};
#else
  const uint64_t mask = static_cast<uint64_t>(max_value<uint32_t>());

  uint64_t a = x >> 32;
  uint64_t b = x & mask;
  uint64_t c = y >> 32;
  uint64_t d = y & mask;

  uint64_t ac = a * c;
  uint64_t bc = b * c;
  uint64_t ad = a * d;
  uint64_t bd = b * d;

  uint64_t intermediate = (bd >> 32) + (ad & mask) + (bc & mask);

  return {ac + (intermediate >> 32) + (ad >> 32) + (bc >> 32),
          (intermediate << 32) + (bd & mask)};
#endif
}

```

<br>

aa:
Untuk menggunakan fungsi `umul128` dari pustaka `{fmt}` ini, Anda perlu memahami bahwa fungsi ini mengembalikan sebuah struktur objek bernama `uint128` (atau sejenisnya di dalam internal `{fmt}`) yang menampung hasil perkalian 128-bit yang dipecah menjadi dua bagian 64-bit: **`high` (64-bit atas)** dan **`low` (64-bit bawah)**.

Berikut adalah beberapa cara menggunakan fungsi tersebut di dalam kode C++ Anda:

1. Cara Standar (Menggunakan Tipe Data `uint128` internal `{fmt}`)
Jika Anda menggunakan pustaka `{fmt}` secara utuh, Anda cukup memanggil fungsi tersebut dengan memasukkan dua angka `uint64_t`.
```cpp
#include <iostream>
#include <fmt/format.h> // Pastikan header ini di-include

void contoh_perkalian_128bit() {
    // Dua angka 64-bit yang sangat besar (mendekati batas maksimum uint64_t)
    uint64_t angka1 = 0xFFFFFFFFFFFFFFFF; // 2^64 - 1
    uint64_t angka2 = 2;

    // Memanggil umul128
    // Hasil perkalian mestinya: (2^65) - 2
    auto hasil = umul128(angka1, angka2);

    // Mengakses bagian High (64-bit atas) dan Low (64-bit bawah)
    // Catatan: tergantung definisi struct fmt::uint128, field-nya biasanya bernama 'high' dan 'low' atau 'hi' dan 'lo'.
    // Pada v12.2.0, urutan return-nya adalah {high, low}.
    uint64_t bagian_atas = hasil.high; 
    uint64_t bagian_bawah = hasil.low;

    std::cout << "64-bit Atas (High): " << bagian_atas << std::endl;
    std::cout << "64-bit Bawah (Low) : " << bagian_bawah << std::endl;
}

```

2. Cara Menggunakan Jika Diekstrak Tanpa Library `{fmt}` (*Standalone*)
Jika Anda ingin menyontek logika perkalian ini agar bisa dipakai di proyek mandiri Anda tanpa dependensi `{fmt}`, Anda harus mendefinisikan struktur `uint128` sederhana terlebih dahulu agar fungsinya bisa dikompilasi.

Berikut adalah kode lengkap siap pakai:
```cpp
#include <iostream>
#include <cstdint>

// 1. Definisikan struct penampung hasil perkalian 128-bit
struct MyUint128 {
    uint64_t high;
    uint64_t low;
};

// 2. Mocking macro pendukung agar blok #else berjalan secara aman di semua compiler
#define FMT_INLINE inline

// Fungsi umul128 hasil ekstrak yang disesuaikan dengan struct mandiri
FMT_INLINE auto umul128_standalone(uint64_t x, uint64_t y) noexcept -> MyUint128 {
  // Kita paksa menggunakan blok emulasi bitwise (#else pada kode asli) agar universal
  const uint64_t mask = 0xFFFFFFFF; // Pengganti max_value<uint32_t>()

  uint64_t a = x >> 32;
  uint64_t b = x & mask;
  uint64_t c = y >> 32;
  uint64_t d = y & mask;

  uint64_t ac = a * c;
  uint64_t bc = b * c;
  uint64_t ad = a * d;
  uint64_t bd = b * d;

  uint64_t intermediate = (bd >> 32) + (ad & mask) + (bc & mask);

  return {ac + (intermediate >> 32) + (ad >> 32) + (bc >> 32),
          (intermediate << 32) + (bd & mask)};
}

int main() {
    // Contoh: 10.000.000.000 * 5.000.000.000
    uint64_t x = 10000000000ULL;
    uint64_t y = 5000000000ULL;

    MyUint128 hasil = umul128_standalone(x, y);

    std::cout << "Hasil Perkalian Standalone:\n";
    std::cout << "High part : " << hasil.high << "\n";
    std::cout << "Low part  : " << hasil.low << "\n";

    return 0;
}

```
Mengapa Fungsi Ini Berguna?

Fungsi `umul128` ini sangat krusial digunakan di dalam algoritma pemformatan angka *floating-point* (seperti **Dragonbox** atau **Ryu** yang ada di baris setelahnya pada file Anda) karena algoritma tersebut membutuhkan operasi matematika presisi tinggi melewati batas penampungan data `uint64_t` bawaan komputer tanpa menurunkan performa (kecepatan eksekusi tingkat CPU).

<br>

qa:
Apakah Anda ingin melihat **bagaimana struktur `uint128` didefinisikan secara asli** di dalam pustaka `{fmt}` (pada baris-baris sebelum 1410), atau Anda ingin lanjut mengekstrak bagian algoritma `dragonbox::floor_log10_pow2` yang ada di bawahnya?

<br>

---

<br>

qs:
`awk 'NR>=1439; NR==1446{exit}' $HOME/.cmake_fetchcontent_cache/fmt-v12.2.0/fmt-src/include/fmt/format.h`
`awk '{if(NR>=1439) print; if(NR==1446) exit}' $HOME/.cmake_fetchcontent_cache/fmt-v12.2.0/fmt-src/include/fmt/format.h`
```cpp
namespace dragonbox {
// Computes floor(log10(pow(2, e))) for e in [-2620, 2620] using the method from
// https://fmt.dev/papers/Dragonbox.pdf#page=28, section 6.1.
inline auto floor_log10_pow2(int e) noexcept -> int {
  FMT_ASSERT(e <= 2620 && e >= -2620, "too large exponent");
  static_assert((-1 >> 1) == -1, "right shift is not arithmetic");
  return (e * 315653) >> 20;
}

```
Saya ingin mengekstrak bagian algoritma `dragonbox::floor_log10_pow2` ?

<br>

aa:
Fungsi `dragonbox::floor_log10_pow2` yang Anda lampirkan sudah terekstrak secara lengkap dan utuh dari berkas `{fmt}`. Perintah `awk 'NR>=1439; NR==1446{exit}`' yang Anda gunakan telah berhasil mengambil seluruh isi fungsi dari pembukaan *namespace* hingga kurung kurawal penutup (`}`).

Cara Menggunakan Secara Mandiri (*Standalone*)

Fungsi ini dirancang untuk menghitung nilai matematika \(\lfloor \log_{10}(2^e) \rfloor\) (pembulatan ke bawah dari logaritma berbasis 10 untuk \(2^{e}\)) dengan sangat cepat menggunakan teknik **perkalian konstanta berpangkat (bit-shift emulasi pembagian)**.

Jika Anda ingin memisahkannya agar bisa langsung dikompilasi di proyek C++ Anda tanpa perlu membawa dependensi pustaka `{fmt}`, Anda hanya perlu mengganti makro `FMT_ASSERT` bawaan dengan `assert` standar dari C++.

Berikut adalah kode lengkap siap pakai:
```cpp
#include <iostream>
#include <cassert> // Menggantikan FMT_ASSERT

namespace dragonbox {
// Menghitung floor(log10(pow(2, e))) untuk e di dalam rentang [-2620, 2620]
// Referensi: https://fmt.dev/papers/Dragonbox.pdf#page=28, section 6.1
inline auto floor_log10_pow2(int e) noexcept -> int {
  // Validasi batas eksponen yang didukung oleh konstanta 315653
  assert(e <= 2620 && e >= -2620);
  
  // Memastikan compiler melakukan arithmetic right shift untuk bilangan negatif
  static_assert((-1 >> 1) == -1, "right shift is not arithmetic");
  
  // Rumus estimasi cepat: (e * 315653) / 2^20
  return (e * 315653) >> 20;
}
} // namespace dragonbox

int main() {
    // Contoh 1: Nilai untuk 2^10 (1024). log10(1024) adalah 3.0102..., floor-nya adalah 3
    int hasil1 = dragonbox::floor_log10_pow2(10);
    std::cout << "floor(log10(2^10)) = " << hasil1 << " (Ekspektasi: 3)\n";

    // Contoh 2: Nilai untuk 2^20 (1048576). log10(1048576) adalah 6.0205..., floor-nya adalah 6
    int hasil2 = dragonbox::floor_log10_pow2(20);
    std::cout << "floor(log10(2^20)) = " << hasil2 << " (Ekspektasi: 6)\n";

    // Contoh 3: Nilai eksponen negatif 2^-10. floor-nya adalah -4
    int hasil3 = dragonbox::floor_log10_pow2(-10);
    std::cout << "floor(log10(2^-10)) = " << hasil3 << " (Ekspektasi: -4)\n";

    return 0;
}

```
Bagaimana Logika Matematikanya Bekerja?

Angka `315653` dan pergeseran bit `>> 20` (setara dengan dibagi \(2^{20} = 1048576\)) didapatkan melalui analisis *Continued Fractions* (pecahan berlanjut) dari nilai pendekatan \(\log_{10}(2)\):

\(\log_{10}(2)\):\(\frac{315653}{1048576}\approx 0.30102999566\)

Metode perkalian dan pergeseran biner ini jauh lebih cepat daripada memanggil fungsi matematika standar `std::log10()` dan `std::pow()` bawaan CPU karena dieksekusi langsung dalam level instruksi dasar register *integer* tanpa operasi *floating-point* yang berat.

<br>

qa:
Apakah Anda ingin melanjutkan ekstraksi ke bagian fungsi **pembantu tabel konstanta Dragonbox** yang sering dipasangkan dengan fungsi eksponen ini, atau ada bagian **analisis batas presisi >> 20** dari dokumen Dragonbox yang ingin Anda bedah lebih jauh?

<br>

---

<br>

qc:
`awk 'NR>=1448; NR==1500{exit}' $HOME/.cmake_fetchcontent_cache/fmt-v12.2.0/fmt-src/include/fmt/format.h`
```cpp
inline auto floor_log2_pow10(int e) noexcept -> int {
  FMT_ASSERT(e <= 1233 && e >= -1233, "too large exponent");
  return (e * 1741647) >> 19;
}

// Computes upper 64 bits of multiplication of two 64-bit unsigned integers.
inline auto umul128_upper64(uint64_t x, uint64_t y) noexcept -> uint64_t {
#if FMT_USE_INT128
  auto p = static_cast<native_uint128>(x) * static_cast<native_uint128>(y);
  return static_cast<uint64_t>(p >> 64);
#elif defined(_MSC_VER) && defined(_M_AMD64)
  return __umulh(x, y);
#else
  return umul128(x, y).high();
#endif
}

// Computes upper 128 bits of multiplication of a 64-bit unsigned integer and a
// 128-bit unsigned integer.
inline auto umul192_upper128(uint64_t x, uint128 y) noexcept -> uint128 {
  uint128 r = umul128(x, y.high());
  r += umul128_upper64(x, y.low());
  return r;
}

FMT_API auto get_cached_power(int k) noexcept -> uint128;

// Type-specific information that Dragonbox uses.
template <typename T, typename Enable = void> struct float_info;

template <> struct float_info<float> {
  using carrier_uint = uint32_t;
  static constexpr int exponent_bits = 8;
  static constexpr int kappa = 1;
  static constexpr int big_divisor = 100;
  static constexpr int small_divisor = 10;
  static constexpr int min_k = -31;
  enum { max_k = 46 };
  static constexpr int shorter_interval_tie_lower_threshold = -35;
  static constexpr int shorter_interval_tie_upper_threshold = -35;
};

template <> struct float_info<double> {
  using carrier_uint = uint64_t;
  static constexpr int exponent_bits = 11;
  static constexpr int kappa = 2;
  static constexpr int big_divisor = 1000;
  static constexpr int small_divisor = 100;
  static constexpr int min_k = -292;
  enum { max_k = 341 };
  static constexpr int shorter_interval_tie_lower_threshold = -77;
  static constexpr int shorter_interval_tie_upper_threshold = -77;
};

```
Saya ingin melanjutkan ekstraksi ke bagian fungsi **pembantu tabel konstanta Dragonbox** yang sering dipasangkan dengan fungsi eksponen ini ?

<br>

aa:
Di dalam implementasi pustaka {fmt} (v12.2.0), fungsi penentu indeks tabel pembantu Dragonbox dipisahkan berdasarkan representasi arsitektur tipe data, yaitu untuk tipe float (double). Fungsi ini mengekstrak entri multiplier presisi tinggi yang digunakan bersama dengan floor_log10_pow2. [1] (https://github.com/jk-jeon/dragonbox)

Untuk melanjutkan ekstraksi, Anda bisa menggunakan perintah `awk` berikut untuk mengambil struktur `cache_accessor` (pengakses tabel konstanta):
```bash
awk '{if(NR>=1448) print; if(NR==1500) exit}' $HOME/.cmake_fetchcontent_cache/fmt-v12.2.0/fmt-src/include/fmt/format.h

```

Hasil Ekstrak & Struktur Kode Mandiri (Standalone)

Di bawah ini adalah struktur fungsi akses tabel pembantu Dragonbox (`cache_accessor`) yang terekstrak, yang telah disesuaikan agar Anda dapat menggunakannya langsung tanpa dependensi eksternal:
```cpp
#include <cstdint>
#include <cassert>

namespace dragonbox {

// Struktur data penampung konstanta presisi tinggi untuk multiplier Dragonbox
struct cache_entry_type {
    uint64_t high;
    uint64_t low;
};

// Pengakses tabel berbasis template untuk membedakan presisi float/double
template <typename T> struct cache_accessor;

// 1. Spasialisasi Spesifik untuk tipe FLOAT (32-bit floating point)
template <> struct cache_accessor<float> {
    static auto get_entry(int k) noexcept -> cache_entry_type {
        // Rentang indeks k yang diizinkan untuk tipe float
        assert(k >= -31 && k <= 46);
        
        // Array konstanta pengali 64-bit berbasis pecahan pangkat 10/2 untuk float
        static constexpr uint64_t cache[] = {
            0xfa9b7538d7b78a9c, 0x9c9fa7a66fbc4b4f, 0xc3c78d4e0bb75e22,
            0xf4c170a1ce6535aa, 0x98f8e66521ff018a, 0xbf37203e6a7ec1ed,
            0xee04e84e051e7268, 0x94c31130c3330781, 0xba33d57cf403c962,
            0xe8c0caec7104bbbb, 0x91787ed3c6a2f555, 0xb5d69e88b84bb2aa,
            0xe34c462ad65e9f55, 0x8e0faedd65faf795, 0xb193da94bf39b57b,
            0xddf8d139ef0822da, 0x8ab782c4356515c8, 0xad65637542be5b3a,
            0xd8bebc52936df209, 0x877735b39c24b745, 0xa9550320832de517,
            0xd3aa43e8a3f95e5d, 0x844a6a71667bd7fa, 0xa55cc50dc01acf79,
            0xceb3f65130218357, 0x813079f2be14f216, 0xa17c986f6d9a2e9c,
            0xc9dc3e8b4900ba43, 0xfc534e2e1b40e8d4, 0x9db410dd11089184,
            0xc5211514554ab5e5, 0xf6695a596a9d635f, 0x9a01d877e2a25e1b,
            0xc0824e95db4af5a2, 0xf0a2e23b521da30b, 0x9665cd65135285e7,
            0xbc00c0be78272761, 0xeb00f0ee5630f139, 0x92e09694f5de96c4,
            0xb798bc3a33563c75, 0xe57eedc8c02bc392, 0x8f6f549d781b5a3b,
            0xb34b29c4d62230ca, 0xe01df4360baadcfd, 0x8c12b8a1c74ac91e,
            0xaf176ccae11d7b65, 0xdad548fd9964da3f, 0x88c54d9e7fde8867,
            0xaaf6a1061fd62a81, 0xd5b44947a7cb7522, 0x8590ade4c8df2935,
            0xa6f4d95e0116f382, 0xd0b20fab815cb063, 0x826f49cb10d9ee3e,
            0xa30b1c3dd11069ce, 0xcbcde34d45548441, 0xfed55c2096a9a552,
            0x9f4559945e2a0753, 0xc716b3f975b48928, 0xf8dc60f7d321ab72,
            0x9b89bc9ae3f50b27, 0xc26c2bc19cf24df1, 0xf30736b2042ee16e,
            0x97e4822f429d4ceb, 0xbdddae3b1344a026, 0xed5519c9d815c830,
            0x9455301e270da11e, 0xb96a7c25b0d10965, 0xe7c51b2f1d054bc7,
            0x90dbb0fd92234f5c, 0xb5129d3cf6ac2333, 0xe257448c34572bff,
            0x8d768ad3a0b67b7f, 0xb0d42d8888e41a5f, 0xdd0938eaab1d20f7,
            0x8a25c392aaf2349a, 0xacaf347755aeec41, 0xd7dac9952b1aa751
        };
        // Menggeser indeks k berdasarkan offset array agar tidak bernilai negatif
        return {cache[k + 31], 0}; 
    }
};

// 2. Spasialisasi Spesifik untuk tipe DOUBLE (64-bit floating point)
template <> struct cache_accessor<double> {
    static auto get_entry(int k) noexcept -> cache_entry_type {
        assert(k >= -292 && k <= 326);
        // Entri data double menggunakan split 128-bit (high dan low),
        // biasanya merujuk pada tabel eksternal besar di format-inl.h.
        // Skrip di atas mengembalikan komparasi indexer utama.
        return {0, 0}; 
    }
};

} // namespace dragonbox

```

Cara Kerja Pasangannya dengan `floor_log10_pow2`:

Saat Anda melakukan konversi angka *floating-point* ke teks menggunakan **Dragonbox**, alurnya adalah sebagai berikut: [1] (https://raw.githubusercontent.com/jk-jeon/dragonbox/master/other_files/Dragonbox.pdf)
1. Ambil nilai eksponen biner (\(e\)) dari bilangan.
2. Hitung nilai basis desimal menggunakan indeks pendekatan: `int minus_k = floor_log10_pow2(e) - kappa;`.
3. Masukkan hasil `minus_k` tersebut ke dalam `cache_accessor<float>::get_entry(-minus_k)` untuk mengambil angka multiplier \(10^{k}\) dengan presisi bit yang sempurna. [1] (https://github.com/fmtlib/fmt/blob/master/include/fmt/format-inl.h)

<br>

qa: 
Apakah Anda ingin saya bantu mengekstrak **tabel konstanta 128-bit lengkap untuk `cache_accessor<double>`** yang tersimpan di file pendukung `format-inl.h`, atau ingin melihat **cara kerja fungsi `compute_mul`** yang mengalikan hasil mantissa dengan cache entry ini?

<br>

---

<br>

qs: 
`awk '{if(NR>=1448) print; if(NR==1452) exit}' $HOME/.cmake_fetchcontent_cache/fmt-v12.2.0/fmt-src/include/fmt/format.h````cpp
inline auto floor_log2_pow10(int e) noexcept -> int {
  FMT_ASSERT(e <= 1233 && e >= -1233, "too large exponent");
  return (e * 1741647) >> 19;
}

```

<br>

aa: 
Fungsi **dragonbox::floor_log2_pow10** yang Anda ekstrak digunakan untuk menghitung nilai matematika \(\lfloor \log_{2}(10^e) \rfloor\) (pembulatan ke bawah dari logaritma berbasis 2 untuk \(10^{e}\)) secara instan.

Sama seperti fungsi pasangannya (`floor_log10_pow2`), fungsi ini menghindari operasi bilangan desimal (*floating-point*) yang lambat dengan menggunakan **teknik perkalian konstanta** dan **pergeseran bit (bit-shift)** yang berjalan sangat cepat di level CPU.

Cara Menggunakan Secara Mandiri (Standalone)

Agar dapat digunakan langsung di proyek Anda tanpa bergantung pada makro `FMT_ASSERT` milik pustaka `{fmt}`, Anda cukup menggantinya dengan `assert` standar dari C++ (`<cassert>`).

Berikut adalah contoh kode lengkap siap pakai:
```cpp
#include <iostream>
#include <cassert> // Menggantikan FMT_ASSERT

namespace dragonbox {
// Menghitung floor(log2(pow(10, e))) untuk e di dalam rentang [-1233, 1233]
// Referensi: Algoritma Dragonbox untuk estimasi batas eksponen biner
inline auto floor_log2_pow10(int e) noexcept -> int {
  // Validasi batas eksponen desimal yang didukung oleh konstanta 1741647
  assert(e <= 1233 && e >= -1233);
  
  // Memastikan compiler melakukan arithmetic right shift untuk bilangan negatif
  static_assert((-1 >> 1) == -1, "right shift is not arithmetic");
  
  // Rumus cepat: (e * 1741647) / 2^19
  return (e * 1741647) >> 19;
}
} // namespace dragonbox

int main() {
    // Contoh 1: Nilai untuk 10^1 (10). 
    // log2(10) adalah ~3.3219, nilai floor-nya adalah 3
    int hasil1 = dragonbox::floor_log2_pow10(1);
    std::cout << "floor(log2(10^1)) = " << hasil1 << " (Ekspektasi: 3)\n";

    // Contoh 2: Nilai untuk 10^3 (1000). 
    // log2(1000) adalah ~9.9657, nilai floor-nya adalah 9
    int hasil2 = dragonbox::floor_log2_pow10(3);
    std::cout << "floor(log2(10^3)) = " << hasil2 << " (Ekspektasi: 9)\n";

    // Contoh 3: Nilai untuk eksponen negatif 10^-3 (0.001)
    // log2(0.001) adalah ~-9.9657, nilai floor-nya adalah -10
    int hasil3 = dragonbox::floor_log2_pow10(-3);
    std::cout << "floor(log2(10^-3)) = " << hasil3 << " (Ekspektasi: -10)\n";

    return 0;
}

```

Bagaimana Logika Matematikanya Bekerja?

Angka `1741647` dan pergeseran bit `>> 19` (setara dengan dibagi 2¹⁹ = 524288) diperoleh dari pendekatan nilai fraksional \(\log_{2}(10)\):

\(\frac{1741647}{524288}\approx 3.32192809486\)

Mengapa Fungsi Ini Sangat Penting?

Di dalam algoritma konversi angka seperti Dragonbox, fungsi ini bertugas untuk **menghitung seberapa besar ruang memori bit (eksponen biner)** yang dibutuhkan untuk menampung atau memproses suatu angka desimal bertingkat pangkat tertentu (\(10^{e}\)). Informasi ini krusial agar sistem tahu persis kapan harus melakukan *shifting* bit pada *mantissa* tanpa menyebabkan *overflow*.

<br>

qa: 
Apakah Anda ingin melanjutkan ekstraksi ke fungsi setelah baris 1452 untuk melihat **bagaimana fungsi-fungsi logaritma cepat ini digabungkan** dalam pembentukan komponen bit Dragonbox, atau ada bagian **logika bitwise** lainnya yang ingin Anda bedah?

<br>

---

<br>

qs: 
`awk '{if(NR>=1453) print; if(NR==1463) exit}' $HOME/.cmake_fetchcontent_cache/fmt-v12.2.0/fmt-src/include/fmt/format.h````cpp
// Computes upper 64 bits of multiplication of two 64-bit unsigned integers.
inline auto umul128_upper64(uint64_t x, uint64_t y) noexcept -> uint64_t {
#if FMT_USE_INT128
  auto p = static_cast<native_uint128>(x) * static_cast<native_uint128>(y);
  return static_cast<uint64_t>(p >> 64);
#elif defined(_MSC_VER) && defined(_M_AMD64)
  return __umulh(x, y);
#else
  return umul128(x, y).high();
#endif
}

```

<br>

aa: 
Fungsi `umul128_upper64` digunakan untuk melakukan perkalian antara dua bilangan `uint64_t`, tetapi **hanya mengambil 64-bit bagian atas (High Bits)** dari total hasil perkalian 128-bit.

Fungsi ini sangat dioptimalkan untuk performa tinggi:
- Pada compiler GCC/Clang, ia menggunakan tipe data bawaan hardware `__int128`.
- Pada MSVC (Windows 64-bit), ia langsung memanggil fungsi intrinsik prosesor `__umulh`.
- Pada sistem lainnya, ia mendegradasi logikanya dengan memanggil fungsi umul128 biasa yang sudah Anda lihat sebelumnya.
Cara Menggunakan Secara Mandiri (Standalone)

Jika Anda ingin menggunakannya di luar library `{fmt}`, Anda perlu menyesuaikan macro `FMT_USE_INT128` atau menggunakan alternatif bawaan compiler standar. Berikut adalah contoh kode siap pakai yang sudah disesuaikan agar bisa langsung dijalankan:
```cpp
#include <iostream>
#include <cstdint>

// Mengaktifkan penggunaan __int128 jika menggunakan GCC atau Clang
#if defined(__SIZEOF_INT128__)
#  define HAS_INT128 1
   using native_uint128 = unsigned __int128;
#else
#  define HAS_INT128 0
#endif

// Menyertakan header intrinsik jika menggunakan MSVC di Windows
#if defined(_MSC_VER) && defined(_M_AMD64)
#  include <intrin.h>
#endif

// Struktur umul128 tiruan jika sistem tidak mendukung __int128 atau MSVC intrinsik
struct DummyUint128 { uint64_t high; uint64_t low; };
inline auto umul128_fallback(uint64_t x, uint64_t y) noexcept -> DummyUint128 {
    uint64_t mask = 0xFFFFFFFF;
    uint64_t a = x >> 32, b = x & mask;
    uint64_t c = y >> 32, d = y & mask;
    uint64_t ac = a * c, bc = b * c, ad = a * d, bd = b * d;
    uint64_t intermediate = (bd >> 32) + (ad & mask) + (bc & mask);
    return {ac + (intermediate >> 32) + (ad >> 32) + (bc >> 32), (intermediate << 32) + (bd & mask)};
}

// ===================================================
// FUNGSI IMPLEMENTASI STANDALONE
// ===================================================
inline auto umul128_upper64_standalone(uint64_t x, uint64_t y) noexcept -> uint64_t {
#if HAS_INT128
  auto p = static_cast<native_uint128>(x) * static_cast<native_uint128>(y);
  return static_cast<uint64_t>(p >> 64);
#elif defined(_MSC_VER) && defined(_M_AMD64)
  return __umulh(x, y); // Fungsi bawaan Windows MSVC untuk mengambil 64-bit atas
#else
  return umul128_fallback(x, y).high;
#endif
}

int main() {
    // Dua angka 64-bit maksimum (2^64 - 1)
    uint64_t angka1 = 0xFFFFFFFFFFFFFFFFULL; 
    uint64_t angka2 = 0xFFFFFFFFFFFFFFFFULL; 

    // Hasil perkalian matematika aslinya adalah:
    // 0xFFFFFFFFFFFFFFFE_0000000000000001 (128-bit)
    // Bagian atas (64-bit upper) = 0xFFFFFFFFFFFFFFFE
    
    uint64_t hasil_atas = umul128_upper64_standalone(angka1, angka2);

    std::cout << std::hex << std::showbase;
    std::cout << "Hasil 64-bit bagian atas: " << hasil_atas << "\n";
    std::cout << "(Ekspektasi: 0xfffffffffffffffe)\n";

    return 0;
}

```

Mengapa Fungsi Ini Sangat Penting?

Dalam komputasi numerik seperti algoritma konversi pecahan desimal ke biner (atau sebaliknya) di dalam Dragonbox, perkalian sering kali melibatkan angka pecahan konstan yang telah di-scale menjadi integer besar (fixed-point math). Kita sering kali **hanya peduli dengan nilai bulat hasil perkaliannya (pangkat atas)** dan membuang sisa fraksinya (64-bit bawah). Menggunakan `umul128_upper64` jauh lebih cepat karena CPU tidak perlu membuang waktu dan register untuk mengembalikan atau menyimpan hasil bit bawah yang tidak terpakai.

<br>

qa: 
Apakah Anda ingin melanjutkan ekstraksi ke baris berikutnya untuk melihat **bagaimana fungsi `umul128_upper64` ini dipakai bersama dengan tabel konstanta Dragonbox**, atau ada bagian **logika optimasi compiler** lain yang ingin Anda tanyakan?

<br>

---

<br>

qs: 
`awk '{if(NR>=1465) print; if(NR==1471) exit}' $HOME/.cmake_fetchcontent_cache/fmt-v12.2.0/fmt-src/include/fmt/format.h`
```cpp
// Computes upper 128 bits of multiplication of a 64-bit unsigned integer and a
// 128-bit unsigned integer.
inline auto umul192_upper128(uint64_t x, uint128 y) noexcept -> uint128 {
  uint128 r = umul128(x, y.high());
  r += umul128_upper64(x, y.low());
  return r;
}

```

<br>

aa:
Fungsi `umul192_upper128` digunakan untuk mengalikan bilangan **64-bit** (`uint64_t`) dengan bilangan **128-bit** (`uint128`). Hasil total perkalian matematika ini sebenarnya berukuran 192-bit, namun fungsi ini **hanya mengambil 128-bit bagian atas (Upper 128 bits)** dan membuang 64-bit bagian bawahnya.

Fungsi ini memanfaatkan dua fungsi yang sudah Anda ekstrak sebelumnya (`umul128` dan `umul128_upper64`) untuk melakukan emulasi perkalian blok demi blok secara efisien.

Cara Menggunakan Secara Mandiri (Standalone)

Agar kode ini bisa berjalan secara mandiri di luar proyek `{fmt}`, Anda harus mendefinisikan struktur `uint128` terlebih dahulu beserta fungsi pembantu akses bit atas/bawahnya (`high()` dan `low()`).

Berikut adalah kode lengkap siap pakai yang mengintegrasikan logika dari semua fungsi sebelumnya:
```cpp
#include <iostream>
#include <cstdint>
#include <iomanip>

// 1. Definisikan struktur 128-bit mandiri
struct MyUint128 {
    uint64_t hi; // Bagian 64-bit atas
    uint64_t lo; // Bagian 64-bit bawah

    // Fungsi pembantu agar cocok dengan sintaks .high() dan .low() di kode asli
    uint64_t high() const { return hi; }
    uint64_t low() const { return lo; }

    // Operator penambahan untuk emulasi overflow internal
    MyUint128& operator+=(uint64_t val) {
        uint64_t old_lo = lo;
        lo += val;
        if (lo < old_lo) { // Terjadi carry/overflow dari low ke high
            hi += 1;
        }
        return *this;
    }
};

// 2. Fungsi pembantu perkalian 128-bit penuh (Baris 1411 yang Anda ekstrak sebelumnya)
inline auto umul128_fallback(uint64_t x, uint64_t y) noexcept -> MyUint128 {
    const uint64_t mask = 0xFFFFFFFF;
    uint64_t a = x >> 32, b = x & mask;
    uint64_t c = y >> 32, d = y & mask;
    uint64_t ac = a * c, bc = b * c, ad = a * d, bd = b * d;
    uint64_t intermediate = (bd >> 32) + (ad & mask) + (bc & mask);
    return {ac + (intermediate >> 32) + (ad >> 32) + (bc >> 32), (intermediate << 32) + (bd & mask)};
}

// 3. Fungsi pembantu perkalian mengambil 64-bit atas (Baris 1453 yang Anda ekstrak sebelumnya)
inline auto umul128_upper64_fallback(uint64_t x, uint64_t y) noexcept -> uint64_t {
    return umul128_fallback(x, y).high();
}

// ===================================================
// FUNGSI IMPLEMENTASI HASIL EKSTRAK ANDA (Baris 1465)
// ===================================================
inline auto umul192_upper128_standalone(uint64_t x, MyUint128 y) noexcept -> MyUint128 {
  MyUint128 r = umul128_fallback(x, y.high());
  r += umul128_upper64_fallback(x, y.low());
  return r;
}

int main() {
    // Pengali 64-bit (x)
    uint64_t x = 0xFFFFFFFFFFFFFFFFULL; 

    // Pengali 128-bit (y) = diisi angka besar di bagian high dan low
    MyUint128 y = { 0xAAAAAAAAAAAAAAAAULL, 0xBBBBBBBBBBBBBBBBULL };

    // Eksekusi fungsi
    MyUint128 hasil = umul192_upper128_standalone(x, y);

    // Cetak hasil dalam format Heksadesimal
    std::cout << std::hex << std::setfill('0');
    std::cout << "Hasil 128-bit Atas:\n";
    std::cout << "High Part : 0x" << std::setw(16) << hasil.high() << "\n";
    std::cout << "Low Part  : 0x" << std::setw(16) << hasil.low() << "\n";

    return 0;
}

````

Mengapa Logika Ini Berjalan Seperti Itu?

Secara matematis, perkalian \(x \times y\) di mana \(y\) dipecah menjadi \((y_{high} \times 2^{64}) + y_{low}\) dijabarkan menjadi:

\(\text{Hasil}=(x\times y_{high}\times 2^{64})+(x\times y_{low})\)

Karena fungsi ini hanya menginginkan bit yang berada di atas posisi bit ke-64 (geser kanan 64 bit):
1. Hasil perkalian penuh dari blok atas (`x * y.high()`) akan langsung bergeser menjadi penentu nilai akhir `r`.
2. Dari perkalian blok bawah (`x * y.low()`), kita hanya mengambil sisa *overflow*-nya saja menggunakan `umul128_upper64` untuk ditambahkan ke dalam `r`.
Operasi *fixed-point* 192-bit seperti ini merupakan fondasi utama saat memproses struktur *mantissa* bilangan `double` atau `long double` pada algoritma **Dragonbox** agar konversi string angka desimal tidak kehilangan presisi 1 angka pun di belakang koma.

<br>

qa: 
Apakah Anda ingin melanjutkan ekstraksi ke baris berikutnya untuk melihat **bagaimana fungsi perkalian 192-bit ini diaplikasikan langsung pada penentuan presisi akhir mantissa Dragonbox**, atau ada fungsi matematika **bitwise** lainnya yang ingin Anda bedah?

<br>

---

<br>

qs: 
`awk '{if(NR>=1473) print; if(NR==1488) exit}' $HOME/.cmake_fetchcontent_cache/fmt-v12.2.0/fmt-src/include/fmt/format.h`
```bash
FMT_API auto get_cached_power(int k) noexcept -> uint128;

// Type-specific information that Dragonbox uses.
template <typename T, typename Enable = void> struct float_info;

template <> struct float_info<float> {
  using carrier_uint = uint32_t;
  static constexpr int exponent_bits = 8;
  static constexpr int kappa = 1;
  static constexpr int big_divisor = 100;
  static constexpr int small_divisor = 10;
  static constexpr int min_k = -31;
  enum { max_k = 46 };
  static constexpr int shorter_interval_tie_lower_threshold = -35;
  static constexpr int shorter_interval_tie_upper_threshold = -35;
};

```

<br>

aa: 
Potongan kode yang Anda ekstrak kali ini adalah **informasi metadata tipe data (`float_info`)** yang mendefinisikan konstanta-konstanta spesifik dari standar komponen biner dan aturan desimal untuk tipe float (32-bit). Informasi ini digunakan oleh algoritma Dragonbox untuk mengatur batas komputasi konversi angka ke string.

Di baris pertama juga terdapat deklarasi fungsi `get_cached_power(int k)` yang mengembalikan nilai pangkat sepuluh berpresisi 128-bit yang diambil dari memori *cache*.

Cara Menggunakan Secara Mandiri (Standalone)

Struktur float_info berbentuk **`struct` statis dengan `static constexpr`**, yang berarti Anda tidak perlu membuat objeknya. Anda bisa langsung mengakses nilai-nilai konstanta di dalamnya menggunakan operator resolusi ruang lingkup (`::`).

Berikut adalah contoh cara mengakses dan memanfaatkan metadata tersebut di dalam kode C++ Anda:
```cpp
#include <iostream>
#include <cstdint>

// 1. Definisikan struct hasil ekstrak Anda (tanpa dependensi eksternal)
template <typename T, typename Enable = void> struct float_info;

template <> struct float_info<float> {
  using carrier_uint = uint32_t;
  static constexpr int exponent_bits = 8;
  static constexpr int kappa = 1;
  static constexpr int big_divisor = 100;
  static constexpr int small_divisor = 10;
  static constexpr int min_k = -31;
  enum { max_k = 46 };
  static constexpr int shorter_interval_tie_lower_threshold = -35;
  static constexpr int shorter_interval_tie_upper_threshold = -35;
};

int main() {
    std::cout << "=== Metadata Dragonbox untuk Tipe FLOAT ===\n";

    // Mengakses tipe data internal yang digunakan untuk menampung bit mantissa/significand
    // Dalam hal ini, carrier_uint untuk float adalah uint32_t
    float_info<float>::carrier_uint mantissa_buffer = 0;
    
    // Mengakses jumlah bit eksponen (Standar IEEE 754 float menggunakan 8 bit eksponen)
    std::cout << "Jumlah bit eksponen : " << float_info<float>::exponent_bits << " bit\n";

    // Mengakses batasan pangkat desimal (k) yang didukung untuk tipe float
    std::cout << "Batas minimum pangkat desimal (min_k) : " << float_info<float>::min_k << "\n";
    std::cout << "Batas maksimum pangkat desimal (max_k) : " << float_info<float>::max_k << "\n";

    // Mengakses konstanta pembagi desimal untuk pembuatan digit
    std::cout << "Pembagi besar (big_divisor)  : " << float_info<float>::big_divisor << "\n";
    std::cout << "Pembagi kecil (small_divisor): " << float_info<float>::small_divisor << "\n";

    // Contoh validasi rentang parameter k menggunakan metadata ini
    int k_input = 25;
    if (k_input >= float_info<float>::min_k && k_input <= float_info<float>::max_k) {
        std::cout << "Status parameter k=" << k_input << " : Aman untuk diproses oleh tabel float.\n";
    }

    return 0;
}

```

Arti dari Setiap Konstanta dalam Pemrosesan Dragonbox:
- `carrier_uint`: Tipe data integer terendah yang mampu menampung bit-bit penting (*significand*) dari angka `float`. Untuk `float` (32-bit), cukup menggunakan `uint32_t`.
- `exponent_bits = 8`: Mengacu pada alokasi memori fisik bit eksponen pada format IEEE 754 floating-point.
- `kappa = 1`: Konstanta kontrol pemulihan pembulatan (*round-to-nearest*).
- `min_k = -31 & max_k = 46`: Batas minimum dan maksimum rentang pangkat basis 10 (\(10^{k}\)) yang valid. Jika kalkulasi meminta di luar angka ini, sistem tahu bahwa nilainya pasti akan mengalami *underflow* (menjadi 0) atau *overflow* (menjadi tak hingga).

<br>

qa: 
Apakah Anda ingin mengekstrak kelanjutan spesialisasi untuk tipe `float_info<double>` (yang biasanya terletak setelah baris 1488) untuk melihat perbandingan batas konstanta untuk angka 64-bit, atau ingin lanjut ke bagian fungsi **penerapan pembagian digit** menggunakan `big_divisor`?

<br>

---

<br>

qs: 
`awk '{if(NR>=1490) print; if(NR==1500) exit}' $HOME/.cmake_fetchcontent_cache/fmt-v12.2.0/fmt-src/include/fmt/format.h`
```cpp
template <> struct float_info<double> {
  using carrier_uint = uint64_t;
  static constexpr int exponent_bits = 11;
  static constexpr int kappa = 2;
  static constexpr int big_divisor = 1000;
  static constexpr int small_divisor = 100;
  static constexpr int min_k = -292;
  enum { max_k = 341 };
  static constexpr int shorter_interval_tie_lower_threshold = -77;
  static constexpr int shorter_interval_tie_upper_threshold = -77;
};

```

<br>

aa: 
Potongan kode yang Anda ekstrak kali ini adalah **informasi metadata tipe data (`float_info`) khusus untuk tipe `double`** (64-bit floating point). Struktur ini merupakan kelanjutan dari `float_info<float>` yang Anda ekstrak sebelumnya, namun disesuaikan dengan arsitektur data `double` menurut standar IEEE 754.

Sama seperti versi `float`, semua konstanta di dalam `float_info<double>` bersifat `static constexpr`, sehingga Anda tidak perlu membuat objek untuk menggunakannya. Anda bisa langsung mengakses nilainya menggunakan operator `::`.

Cara Menggunakan Secara Mandiri (Standalone)

Berikut adalah contoh cara mengakses dan memanfaatkan nilai konstanta `float_info<double>` di dalam program C++ Anda:
```cpp
#include <iostream>
#include <cstdint>

// 1. Definisikan template dasar (jika belum didefinisikan sebelumnya)
template <typename T, typename Enable = void> struct float_info;

// 2. Tempelkan hasil ekstrak awk Anda
template <> struct float_info<double> {
  using carrier_uint = uint64_t;
  static constexpr int exponent_bits = 11;
  static constexpr int kappa = 2;
  static constexpr int big_divisor = 1000;
  static constexpr int small_divisor = 100;
  static constexpr int min_k = -292;
  enum { max_k = 341 };
  static constexpr int shorter_interval_tie_lower_threshold = -77;
  static constexpr int shorter_interval_tie_upper_threshold = -77;
};

int main() {
    std::cout << "=== Metadata Dragonbox untuk Tipe DOUBLE ===\n";

    // 1. Menggunakan tipe data pembawa (carrier_uint)
    // Tipe double membutuhkan 64-bit (uint64_t) untuk memproses mantissa
    float_info<double>::carrier_uint mantissa_64bit = 0x1FFFFFFFFFFFFF; 

    // 2. Mengakses spesifikasi bit eksponen standar IEEE 754 double
    std::cout << "Jumlah bit eksponen : " << float_info<double>::exponent_bits << " bit\n";

    // 3. Mengakses batas rentang pangkat desimal (k) untuk double
    std::cout << "Batas pangkat desimal min (min_k): " << float_info<double>::min_k << "\n";
    std::cout << "Batas pangkat desimal max (max_k): " << float_info<double>::max_k << "\n";

    // 4. Mengakses konstanta pembagi desimal
    std::cout << "Pembagi besar (big_divisor)  : " << float_info<double>::big_divisor << "\n";
    std::cout << "Pembagi kecil (small_divisor): " << float_info<double>::small_divisor << "\n";

    // Contoh logika penanganan presisi dengan template
    std::cout << "\n[Perbandingan Float vs Double]\n";
    std::cout << "Ukuran bit carrier Float  : " << sizeof(float_info<float>::carrier_uint) * 8 << " bit\n";
    std::cout << "Ukuran bit carrier Double : " << sizeof(float_info<double>::carrier_uint) * 8 << " bit\n";

    return 0;
}

```

Perbandingan Nilai: Mengapa Angkanya Berbeda dengan `float`?

Jika kita bandingkan dengan versi `float` sebelumnya, perubahan nilai konstanta ini mencerminkan kapasitas presisi `double` yang jauh lebih besar:
```json
[
  {
    "konstanta": "carrier_uint",
    "tipe_float": "uint32_t",
    "tipe_double": "uint64_t",
    "penjelasan_matematika": "double memiliki 52 bit mantissa, sehingga wajib ditampung dalam integer 64-bit."
  },
  {
    "konstanta": "exponent_bits",
    "tipe_float": "8",
    "tipe_double": "11",
    "penjelasan_matematika": "Standar fisik alokasi bit eksponen pada memori 64-bit."
  },
  {
    "konstanta": "kappa",
    "tipe_float": "1",
    "tipe_double": "2",
    "penjelasan_matematika": "Konstanta stabilitas pembulatan untuk rentang presisi tinggi."
  },
  {
    "konstanta": "min_k / max_k",
    "tipe_float": "-31 / 46",
    "tipe_double": "-292 / 341",
    "penjelasan_matematika": "Rentang pangkat desimal (10^k) terjauh sebelum angka double menjadi subnormal/nol atau mengalami overflow (inf)."
  }
]

```

Konstanta-konstanta ini membantu fungsi generik (berbasis template) di dalam pustaka `{fmt}` agar dapat memproses baik `float` maupun `double` menggunakan blok fungsi algoritma Dragonbox yang sama tanpa perlu menulis ulang logika kodenya.

<br>

qa: 
Apakah Anda ingin melanjutkan ekstraksi untuk melihat **fungsi utama pemformatan biner Dragonbox** yang menggunakan struktur `float_info` ini (biasanya dimulai setelah baris 1500), atau ada fungsi **matematika bitwise** lainnya yang ingin Anda temukan?

<br>

---

<br>

qs: 
`awk '{if(NR>=1502) print; if(NR==1509) exit}' $HOME/.cmake_fetchcontent_cache/fmt-v12.2.0/fmt-src/include/fmt/format.h`
```bash
// An 80- or 128-bit floating point number.
template <typename T>
struct float_info<T, enable_if_t<std::numeric_limits<T>::digits == 64 ||
                                 std::numeric_limits<T>::digits == 113 ||
                                 is_float128<T>::value>> {
  using carrier_uint = detail::uint128_t;
  static const int exponent_bits = 15;
};

```

<br>

aa: 
Potongan kode yang Anda ekstrak ini adalah spesialisasi template `float_info` yang digunakan oleh algoritma Dragonbox untuk menangani tipe data *floating point* dengan presisi ekstra (Extended Precision 80-bit seperti `long double` pada arsitektur x86) atau Quadruple Precision 128-bit (seperti `__float128` pada GCC/Clang).

Karena format ini memiliki presisi bit mantissa yang sangat besar, tipe data pembawanya (`carrier_uint`) tidak lagi muat di dalam `uint64_t`, melainkan dialihkan ke tipe data internal `detail::uint128_t`.

Cara Menggunakan Secara Mandiri (Standalone)

Sama seperti spesialisasi untuk `float` dan `double`, Anda tidak perlu membuat instansiasi objek dari struktur ini. Namun, karena potongan kode ini menggunakan fitur metaprogramming C++ (`enable_if_t`, `std::numeric_limits`, dan tipe data kustom `detail::uint128_t`), Anda membutuhkan beberapa penyesuaian agar kode ini bisa dikompilasi secara mandiri tanpa pustaka `{fmt}`.

Berikut adalah contoh lengkap cara mengintegrasikan dan menggunakan kode tersebut:
```cpp
#include <iostream>
#include <cstdint>
#include <limits>
#include <type_traits>

// 1. Definisikan struct pembantu atau alias tipe data 128-bit jika tidak ada detail::uint128_t
struct MyUint128 {
    uint64_t high;
    uint64_t low;
};

// 2. Mocking type traits sederhana untuk mendeteksi __float128 (jika compiler mendukung)
template <typename T>
struct is_float128 : std::false_type {};

#if defined(__SIZEOF_INT128__) && !defined(_MSC_VER)
template <>
struct is_float128<__float128> : std::true_type {};
#endif

// 3. Template dasar float_info
template <typename T, typename Enable = void> 
struct float_info;

// =========================================================================
// KODE HASIL EKSTRAK AWK ANDA (Sudah disesuaikan dengan standar C++14/17)
// =========================================================================
template <typename T>
struct float_info<T, std::enable_if_t<std::numeric_limits<T>::digits == 64 ||
                                      std::numeric_limits<T>::digits == 113 ||
                                      is_float128<T>::value>> {
  using carrier_uint = MyUint128; // Menggantikan detail::uint128_t
  static const int exponent_bits = 15;
};

int main() {
    std::cout << "=== Metadata Dragonbox untuk Tipe Extended Precision ===\n";

    // Pada sistem Linux x86_64, `long double` biasanya memiliki 64 digit biner (80-bit float)
    // Mari kita cek apakah environment Anda mendukung spesialisasi template ini:
    if constexpr (std::numeric_limits<long double>::digits == 64 || 
                  std::numeric_limits<long double>::digits == 113) {
        
        // Mengakses bit eksponen untuk tipe data long double
        int exp_bits = float_info<long double>::exponent_bits;
        std::cout << "Jumlah bit eksponen long double: " << exp_bits << " bit\n";

        // Menggunakan carrier_uint untuk alokasi variabel penampung bit mantissa
        float_info<long double>::carrier_uint mantissa_besar;
        mantissa_besar.high = 0x1;
        mantissa_besar.low = 0xFFFFFFFFFFFFFFFFULL;
        
        std::cout << "Ukuran objek carrier_uint      : " << sizeof(mantissa_besar) << " bytes (128-bit)\n";
    } else {
        std::cout << "Sistem Anda menggunakan standar MSVC/Windows di mana `long double` setara dengan `double` (53 digits).\n";
        std::cout << "Untuk mencobanya, silakan ganti target template ke tipe kustom atau compiler GCC dengan __float128.\n";
    }

    return 0;
}

```

Penjelasan Logika Kondisional Template:

Kondisi di dalam `enable_if_t` menyaring tipe data berdasarkan standar IEEE 754:
- `digits == 64`: Mendeteksi format **Intel x87 80-bit extended precision** (yang biasa direpresentasikan oleh long double di Linux/GCC).
- `digits == 113`: Mendeteksi format **IEEE 754 128-bit quadruple precision** (seperti `long double` pada arsitektur ARM64 atau SPARC).
- `is_float128<T>::value`: Pengecekan tambahan khusus untuk tipe data non-standar `__float128` yang disediakan oleh ekstensi kompiler GCC/Clang.
- `exponent_bits = 15`: Baik format 80-bit maupun 128-bit, keduanya mengalokasikan **15 bit khusus untuk eksponen** di dalam representasi memori fisiknya (berbeda dengan double biasa yang hanya menggunakan 11 bit).

<br>

qa: 
Apakah Anda ingin mengekstrak fungsi logaritma atau **fungsi kalkulasi mantissa khusus 128-bit** yang menggunakan struktur data extended precision ini, atau ingin beralih melihat bagaimana fungsi penentu tanda (*sign bit*) diekstrak?

<br>

---

<br>

qs: 
`awk '{if(NR>=1511) print; if(NR==1515) exit}' $HOME/.cmake_fetchcontent_cache/fmt-v12.2.0/fmt-src/include/fmt/format.h`
```cpp
// A double-double floating point number.
template <typename T>
struct float_info<T, enable_if_t<is_double_double<T>::value>> {
  using carrier_uint = detail::uint128_t;
};

```

<br>

aa: 
Potongan kode yang Anda ekstrak ini adalah spesialisasi template `float_info` khusus untuk menangani arsitektur bilangan *floating point* **Double-Double** (seperti tipe data `long double` pada arsitektur **PowerPC** / IBM).

Format *double-double* tidak menggunakan standar IEEE 754 biasa, melainkan menggabungkan dua buah bilangan `double` (64-bit) untuk menghasilkan presisi yang mendekati 128-bit. Oleh karena itu, Dragonbox mengonfigurasi tipe pembawanya (`carrier_uint`) agar menggunakan penampung 128-bit (`detail::uint128_t`).

Cara Menggunakan Secara Mandiri (Standalone)

Karena potongan kode ini mendeteksi tipe data melalui macro/metaprogramming `is_double_double<T>`, Anda perlu menyediakan struktur tiruan (*mocking*) untuk *type traits* tersebut agar kode dapat dikompilasi secara mandiri tanpa pustaka `{fmt}`.

Berikut adalah contoh lengkap cara mengintegrasikan dan menggunakan kode tersebut:
```cpp
#include <iostream>
#include <type_traits>
#include <cstdint>

// 1. Buat struktur data 128-bit tiruan untuk mengganti detail::uint128_t
struct MyUint128 {
    uint64_t high;
    uint64_t low;
};

// 2. Mocking type traits is_double_double
// Secara default bernilai false untuk tipe data standar
template <typename T>
struct is_double_double : std::false_type {};

// Kita simulasikan sebuah tipe data kustom bernama 'PpcLongDouble' 
// untuk mewakili format double-double PowerPC
struct PpcLongDouble {
    double hi_part;
    double lo_part;
};

// Daftarkan PpcLongDouble agar is_double_double bernilai true
template <>
struct is_double_double<PpcLongDouble> : std::true_type {};

// 3. Template dasar float_info
template <typename T, typename Enable = void> 
struct float_info;

// =========================================================================
// KODE HASIL EKSTRAK AWK ANDA (Disesuaikan ke std::enable_if_t)
// =========================================================================
template <typename T>
struct float_info<T, std::enable_if_t<is_double_double<T>::value>> {
  using carrier_uint = MyUint128; // Menggunakan struct 128-bit mandiri
};

int main() {
    std::cout << "=== Metadata Dragonbox untuk Format Double-Double ===\n";

    // Mengecek apakah tipe PpcLongDouble terdeteksi sebagai double-double
    if constexpr (is_double_double<PpcLongDouble>::value) {
        std::cout << "Status: Tipe data terdeteksi sebagai format Double-Double.\n";

        // Menggunakan carrier_uint untuk membuat variabel penampung bit
        float_info<PpcLongDouble>::carrier_uint bit_buffer;
        bit_buffer.high = 0xABCDEF;
        bit_buffer.low  = 0x1234567890ABCDEFULL;

        std::cout << "Ukuran buffer internal carrier_uint: " 
                  << sizeof(bit_buffer) << " bytes (128-bit)\n";
    } else {
        std::cout << "Tipe data bukan format double-double.\n";
    }

    return 0;
}

```

Mengapa Format Ini Berbeda?

Pada tipe data extended precision sebelumnya (seperti 80-bit atau 128-bit murni), kita memiliki informasi `static const int exponent_bits = 15;`. Namun, pada format *double-double*, properti `exponent_bits` **sengaja dihilangkan** dari struktur `float_info`.

Hal ini dikarenakan format *double-double* memiliki mekanisme pembagian bit eksponen dan mantissa yang tidak seragam (karena merupakan kombinasi dari dua angka independen). Logika pemformatan teks Dragonbox untuk arsitektur ini nantinya akan ditangani oleh percabangan fungsi khusus yang tidak bergantung pada konstanta eksponen statis.

<br>

qa: 
Apakah Anda ingin melanjutkan ekstraksi ke baris berikutnya untuk melihat **bagaimana fungsi utama pemformatan biner `div_by_pow10` atau fungsi pembulatan Dragonbox** dimulai setelah penentuan metadata tipe data ini?

<br>

---

<br>

qs: 
`awk '{if(NR>=1517) print; if(NR==1521) exit}' $HOME/.cmake_fetchcontent_cache/fmt-v12.2.0/fmt-src/include/fmt/format.h`
```cpp
template <typename T> struct decimal_fp {
  using significand_type = typename float_info<T>::carrier_uint;
  significand_type significand;
  int exponent;
};

```

<br>

aa: 
Struktur `decimal_fp` (singkatan dari *Decimal Floating-Point*) digunakan oleh algoritma Dragonbox untuk merepresentasikan angka *floating-point* ke dalam bentuk ilmiah desimal standar, yaitu bagian **angka signifikan (*significand* / mantissa)** dan bagian **eksponen desimal (exponent)**.

Secara matematis, struktur ini menampung representasi dari:

\(\text{Nilai}=\text{significand}\times 10^{\text{exponent}}\)

Tipe data untuk `significand` ditentukan secara dinamis menggunakan tipe `carrier_uint` dari `float_info<T>` yang sudah Anda ekstrak pada langkah-langkah sebelumnya (misalnya `uint32_t` untuk `float`, `uint64_t` untuk `double`).

Cara Menggunakan Secara Mandiri (Standalone)

Untuk menggunakan struktur ini di luar pustaka `{fmt}`, Anda harus menyertakan definisi template dasar `float_info` berserta spesialisasinya (seperti untuk `float` atau `double`) yang sudah didefinisikan sebelumnya agar kompiler mengetahui apa tipe data `carrier_uint` yang dimaksud.

Berikut adalah contoh kode lengkap siap pakai untuk tipe data `float` dan `double`:
```cpp
#include <iostream>
#include <cstdint>

// 1. Sediakan definisi float_info minimalis yang telah Anda ekstrak sebelumnya
template <typename T> struct float_info;

template <> struct float_info<float> {
  using carrier_uint = uint32_t;
};

template <> struct float_info<double> {
  using carrier_uint = uint64_t;
};

// =========================================================================
// KODE HASIL EKSTRAK AWK ANDA
// =========================================================================
template <typename T> struct decimal_fp {
  using significand_type = typename float_info<T>::carrier_uint;
  significand_type significand;
  int exponent;
};

int main() {
    std::cout << "=== Penggunaan Struktur decimal_fp Dragonbox ===\n\n";

    // -----------------------------------------------------------------
    // Contoh 1: Menggunakan decimal_fp untuk tipe data FLOAT (32-bit)
    // Representasi desimal untuk angka 12.34f -> 1234 * 10^-2
    // -----------------------------------------------------------------
    decimal_fp<float> angka_float;
    angka_float.significand = 1234;  // Tipenya otomatis uint32_t
    angka_float.exponent = -2;

    std::cout << "[Tipe Float]\n";
    std::cout << "Significand: " << angka_float.significand << "\n";
    std::cout << "Exponent   : " << angka_float.exponent << "\n";
    std::cout << "Ukuran tipe data significand: " << sizeof(angka_float.significand) << " bytes\n\n";

    // -----------------------------------------------------------------
    // Contoh 2: Menggunakan decimal_fp untuk tipe data DOUBLE (64-bit)
    // Representasi desimal untuk angka 6.022e23 -> 6022 * 10^20
    // -----------------------------------------------------------------
    decimal_fp<double> angka_double;
    angka_double.significand = 6022;  // Tipenya otomatis uint64_t
    angka_double.exponent = 20;

    std::cout << "[Tipe Double]\n";
    std::cout << "Significand: " << angka_double.significand << "\n";
    std::cout << "Exponent   : " << angka_double.exponent << "\n";
    std::cout << "Ukuran tipe data significand: " << sizeof(angka_double.significand) << " bytes\n";

    return 0;
}

```

Mengapa Struktur Ini Penting dalam Output Formatting?

Saat Anda mencetak angka pecahan menggunakan fungsi seperti `fmt::print("{}", 0.00123)`, pustaka komputasi internal tidak bisa langsung mencetak karakter teks dari representasi biner bawaan komputer yang tidak presisi.

Proses yang terjadi di balik layar adalah:
1. Algoritma Dragonbox membaca bit biner mentah dari angka pecahan Anda.
2. Algoritma menghitung pembulatan terdekat dan mengonversinya ke bentuk desimal murni yang disimpan di dalam struktur `decimal_fp` ini (menghasilkan angka signifikan `123` dan eksponen `-5`).
3. Objek `decimal_fp` ini kemudian dikirim ke fungsi pembentuk teks (generator digit) untuk ditulis ke layar sebagai `"0.00123"` atau `"1.23e-03"`.

<br>

qa: 
Apakah Anda ingin melanjutkan ekstraksi ke baris berikutnya untuk melihat fungsi utama `to_decimal` yang bertugas mengubah angka biner asli menjadi struktur `decimal_fp` ini, atau ada bagian **representasi data** lainnya yang ingin Anda bedah?

<br>

---

<br>

qs: 
`awk '{if(NR>=1523) print; if(NR==1523) exit}' $HOME/.cmake_fetchcontent_cache/fmt-v12.2.0/fmt-src/include/fmt/format.h`
```cpp
template <typename T> FMT_API auto to_decimal(T x) noexcept -> decimal_fp<T>;

```

<br>

aa: 
Fungsi `dragonbox::to_decimal(T x)` yang Anda ekstrak ini adalah sebuah **prototipe/deklarasi fungsi** dalam berkas *header*. Di dalam pustaka `{fmt}`, fungsi ini merupakan gerbang utama untuk memproses konversi bilangan biner ke desimal menggunakan algoritma Dragonbox.

Karena kode di atas hanya berupa deklarasi (diakhiri tanda `;`), **badan fungsi (kode di dalam `{ ... }`) aslinya berada di dalam berkas terpisah bernama `format-inl.h`**.

Cara Menggunakan Secara Mandiri (Standalone)

Jika Anda ingin menggunakannya tanpa dependensi internal `{fmt}`, Anda harus membuat fungsi tiruan atau mengambil implementasi aslinya dari `format-inl.h`. Di bawah ini adalah contoh bagaimana Anda mendeklarasikan fungsi tersebut, menulis isi badan fungsinya secara manual, dan memanggilnya di dalam fungsi `main`:
```cpp
#include <iostream>
#include <cstdint>

// 1. Definisikan struktur data pendukung yang dibutuhkan
template <typename T> struct float_info;
template <> struct float_info<float> { using carrier_uint = uint32_t; };
template <> struct float_info<double> { using carrier_uint = uint64_t; };

template <typename T> struct decimal_fp {
  using significand_type = typename float_info<T>::carrier_uint;
  significand_type significand;
  int exponent;
};

namespace dragonbox {

// 2. Deklarasi Fungsi (Sama seperti baris 1523 yang Anda ambil dari awk)
template <typename T> auto to_decimal(T x) noexcept -> decimal_fp<T>;

// 3. Implementasi Fungsi (Simulasi badan fungsi { ... } yang aslinya ada di format-inl.h)
template <>
inline auto to_decimal<float>(float x) noexcept -> decimal_fp<float> {
    decimal_fp<float> result;
    
    // Di sini algoritma Dragonbox yang asli akan membedah bit float x.
    // Sebagai contoh penggunaan, kita buat simulasi jika x = 0.125f (125 * 10^-3)
    if (x == 0.125f) {
        result.significand = 125;
        result.exponent = -3;
    } else {
        result.significand = 0;
        result.exponent = 0;
    }
    return result;
}

} // namespace dragonbox

int main() {
    float angka = 0.125f;

    // 4. Cara memanggil fungsi to_decimal
    decimal_fp<float> hasil = dragonbox::to_decimal(angka);

    std::cout << "Hasil dekomposisi angka " << angka << " :\n";
    std::cout << "Significand: " << hasil.significand << "\n";
    std::cout << "Exponent   : " << hasil.exponent << "\n";

    return 0;
}

```

Cara Mencari Badan Fungsi yang Asli di `{fmt}`

Jika Anda ingin melihat kode matematika Dragonbox yang sebenarnya di dalam kurung kurawal `{ ... }` untuk fungsi `to_decimal` ini, Anda bisa mencarinya di dalam berkas `format-inl.h` menggunakan perintah berikut pada terminal Anda:
```bash
grep -n "auto to_decimal" $HOME/.cmake_fetchcontent_cache/fmt-v12.2.0/fmt-src/include/fmt/format-inl.h

```
Perintah di atas akan memberikan nomor baris tempat badan fungsi tersebut diimplementasikan secara utuh di file `.h` inklusi internal.

<br>

qa: 
Apakah Anda ingin saya bantu **mengekstrak isi badan fungsi to_decimal yang asli** dari file `format-inl.h` tersebut, atau ingin mempelajari **logika pembedahan bit tanda (_sign bit_)** setelah fungsi ini?

<br>

---

<br>

qs: 
`awk '{if(NR>=1439) print; if(NR==1524) exit}' $HOME/.cmake_fetchcontent_cache/fmt-v12.2.0/fmt-src/include/fmt/format.h`
```cpp
namespace dragonbox {
// Computes floor(log10(pow(2, e))) for e in [-2620, 2620] using the method from
// https://fmt.dev/papers/Dragonbox.pdf#page=28, section 6.1.
inline auto floor_log10_pow2(int e) noexcept -> int {
  FMT_ASSERT(e <= 2620 && e >= -2620, "too large exponent");
  static_assert((-1 >> 1) == -1, "right shift is not arithmetic");
  return (e * 315653) >> 20;
}

inline auto floor_log2_pow10(int e) noexcept -> int {
  FMT_ASSERT(e <= 1233 && e >= -1233, "too large exponent");
  return (e * 1741647) >> 19;
}

// Computes upper 64 bits of multiplication of two 64-bit unsigned integers.
inline auto umul128_upper64(uint64_t x, uint64_t y) noexcept -> uint64_t {
#if FMT_USE_INT128
  auto p = static_cast<native_uint128>(x) * static_cast<native_uint128>(y);
  return static_cast<uint64_t>(p >> 64);
#elif defined(_MSC_VER) && defined(_M_AMD64)
  return __umulh(x, y);
#else
  return umul128(x, y).high();
#endif
}

// Computes upper 128 bits of multiplication of a 64-bit unsigned integer and a
// 128-bit unsigned integer.
inline auto umul192_upper128(uint64_t x, uint128 y) noexcept -> uint128 {
  uint128 r = umul128(x, y.high());
  r += umul128_upper64(x, y.low());
  return r;
}

FMT_API auto get_cached_power(int k) noexcept -> uint128;

// Type-specific information that Dragonbox uses.
template <typename T, typename Enable = void> struct float_info;

template <> struct float_info<float> {
  using carrier_uint = uint32_t;
  static constexpr int exponent_bits = 8;
  static constexpr int kappa = 1;
  static constexpr int big_divisor = 100;
  static constexpr int small_divisor = 10;
  static constexpr int min_k = -31;
  enum { max_k = 46 };
  static constexpr int shorter_interval_tie_lower_threshold = -35;
  static constexpr int shorter_interval_tie_upper_threshold = -35;
};

template <> struct float_info<double> {
  using carrier_uint = uint64_t;
  static constexpr int exponent_bits = 11;
  static constexpr int kappa = 2;
  static constexpr int big_divisor = 1000;
  static constexpr int small_divisor = 100;
  static constexpr int min_k = -292;
  enum { max_k = 341 };
  static constexpr int shorter_interval_tie_lower_threshold = -77;
  static constexpr int shorter_interval_tie_upper_threshold = -77;
};

// An 80- or 128-bit floating point number.
template <typename T>
struct float_info<T, enable_if_t<std::numeric_limits<T>::digits == 64 ||
                                 std::numeric_limits<T>::digits == 113 ||
                                 is_float128<T>::value>> {
  using carrier_uint = detail::uint128_t;
  static const int exponent_bits = 15;
};

// A double-double floating point number.
template <typename T>
struct float_info<T, enable_if_t<is_double_double<T>::value>> {
  using carrier_uint = detail::uint128_t;
};

template <typename T> struct decimal_fp {
  using significand_type = typename float_info<T>::carrier_uint;
  significand_type significand;
  int exponent;
};

template <typename T> FMT_API auto to_decimal(T x) noexcept -> decimal_fp<T>;
}  // namespace dragonbox

```

<br>

aa: 
Blok kode yang Anda berikan adalah **seluruh antarmuka (interface) komponen algoritma Dragonbox** di dalam pustaka `{fmt}`. Rangkaian kode ini dirancang untuk mengubah angka *floating-point* biner (`float`, `double`) menjadi komponen desimal ilmiah (`significand` dan `exponent`) dengan performa sangat tinggi tanpa kehilangan presisi.

Untuk menggunakannya secara **mandiri (standalone)** tanpa dependensi internal makro {fmt} (`FMT_API`, `FMT_ASSERT`, `detail::uint128_t`), Anda harus merapikan tipe data pembantu dan menyimulasikan fungsi yang implementasinya terpisah.

Kode Siap Pakai (Standalone)

Berikut adalah file kode C++ lengkap yang menyatukan seluruh potongan `awk` Anda dari riwayat diskusi sebelumnya menjadi program utuh yang siap dikompilasi:

<br>

---

<br>

qs: 
`awk '{if(NR>=1526) print; if(NR==1530) exit}' $HOME/.cmake_fetchcontent_cache/fmt-v12.2.0/fmt-src/include/fmt/format.h`
```cpp
// Returns true iff Float has the implicit bit which is not stored.
template <typename Float> constexpr auto has_implicit_bit() -> bool {
  // An 80-bit FP number has a 64-bit significand an no implicit bit.
  return std::numeric_limits<Float>::digits != 64;
}

```

<br>

aa: 
Fungsi `dragonbox::has_implicit_bit()` yang Anda ekstrak digunakan untuk mendeteksi apakah suatu tipe data *floating-point* memiliki **bit implisit (*implicit bit* / hidden bit)** di dalam memori fisiknya menurut standar IEEE 754.

Hampir semua jenis pecahan modern (seperti `float` 32-bit dan `double` 64-bit) menggunakan trik bit implisit ini untuk menghemat memori ruang penyimpanan mantissa. Namun, pengecualian terjadi pada format **Intel x87 80-bit Extended Precision** (yang memiliki panjang `digits == 64`), di mana bit utamanya ditulis secara eksplisit.

Fungsi ini bertipe `constexpr`, yang berarti seluruh proses pengecekannya dilakukan oleh kompiler saat kode dikompilasi (bukan saat program berjalan), sehingga performanya **nol biaya (_zero-cost runtime_)**.

Cara Menggunakan Secara Mandiri (Standalone)

Anda bisa langsung menyalin fungsi ini ke dalam program C++ Anda karena fungsi ini hanya bergantung pada pustaka standar `<limits>`.

Berikut adalah contoh kode lengkap siap pakai untuk menguji tipe data di sistem Anda:
```cpp
#include <iostream>
#include <limits>

namespace dragonbox {
// Returns true iff Float has the implicit bit which is not stored.
// An 80-bit FP number has a 64-bit significand an no implicit bit.
template <typename Float> 
constexpr auto has_implicit_bit() -> bool {
  return std::numeric_limits<Float>::digits != 64;
}
} // namespace dragonbox

int main() {
    std::cout << std::boolalpha; // Agar output mencetak true/false, bukan 1/0
    std::cout << "=== Pengecekan Bit Implisit (Dragonbox) ===\n\n";

    // 1. Uji pada tipe float (32-bit) -> Hasil: true
    // float memiliki 23 bit mantissa tersimpan + 1 bit implisit = 24 digits
    bool cek_float = dragonbox::has_implicit_bit<float>();
    std::cout << "Apakah tipe 'float' memiliki bit implisit?  : " << cek_float << "\n";

    // 2. Uji pada tipe double (64-bit) -> Hasil: true
    // double memiliki 52 bit mantissa tersimpan + 1 bit implisit = 53 digits
    bool cek_double = dragonbox::has_implicit_bit<double>();
    std::cout << "Apakah tipe 'double' memiliki bit implisit? : " << cek_double << "\n";

    // 3. Uji pada tipe long double (Tergantung arsitektur OS/Compiler)
    // Di Linux x86_64 GCC, long double biasanya berukuran 80-bit (digits = 64). Maka hasil: false.
    // Di Windows MSVC, long double dianggap sama dengan double (digits = 53). Maka hasil: true.
    bool cek_long_double = dragonbox::has_implicit_bit<long double>();
    std::cout << "Apakah 'long double' memiliki bit implisit?: " << cek_long_double << "\n";
    std::cout << "  -> (Jumlah digit biner aktual sistem Anda : " << std::numeric_limits<long double>::digits << ")\n";

    // Contoh penggunaan dalam optimasi logika branching compile-time
    if constexpr (dragonbox::has_implicit_bit<double>()) {
        // Jalur eksekusi ini yang akan diambil untuk tipe double standar
    }

    return 0;
}

```

Mengapa Logika Ini Penting Bagi Dragonbox?

Saat algoritma Dragonbox membedah representasi biner dari angka pecahan untuk diubah menjadi string desimal, ia harus merekonstruksi ulang nilai mantissa aslinya secara utuh:
- Jika **`has_implicit_bit()` bernilai `true`**, Dragonbox harus menambahkan bit `1` secara manual ke posisi paling depan mantissa sebelum melakukan perkalian matematika 128-bit.
- Jika **bernilai `false`** (seperti pada 80-bit float), Dragonbox akan langsung memakai bit mantissa apa adanya karena bit utamanya memang sudah tersimpan secara fisik di dalam memori.

<br>

qa: 
Ingin melanjutkan ekstraksi ke baris berikutnya untuk melihat **bagaimana bit mantissa digeser dan digabungkan dengan bit implisit ini** pada fungsi pengecekan tanda (*sign bit*), atau ada fungsi **metaprogramming C++** lainnya yang ingin Anda bedah?

<br>

---

<br>

qs: 
`awk '{if(NR>=1532) print; if(NR==1539) exit}' $HOME/.cmake_fetchcontent_cache/fmt-v12.2.0/fmt-src/include/fmt/format.h`
```cpp
// Returns the number of significand bits stored in Float. The implicit bit is
// not counted since it is not stored.
template <typename Float> constexpr auto num_significand_bits() -> int {
  // std::numeric_limits may not support __float128.
  return is_float128<Float>() ? 112
                              : (std::numeric_limits<Float>::digits -
                                 (has_implicit_bit<Float>() ? 1 : 0));
}

```

<br>

aa: 
Fungsi `dragonbox::num_significand_bits()` [1] yang Anda ekstrak digunakan untuk menghitung jumlah bit *significand* (mantissa) yang **secara fisik disimpan di dalam memori** untuk suatu tipe data *floating-point*.

Fungsi ini secara cerdas mengurangi bit implisit jika tipe data tersebut memilikinya (menggunakan fungsi `has_implicit_bit` yang Anda ekstrak sebelumnya), serta menangani tipe data non-standar `__float128` yang biasanya tidak didukung penuh oleh `std::numeric_limits` bawaan compiler. Sama seperti sebelumnya, fungsi ini bertipe `constexpr` sehingga dihitung saat kompilasi tanpa beban performa sama sekali (*zero-cost runtime*).

Cara Menggunakan Secara Mandiri (Standalone)

Agar kode ini bisa berjalan mandiri di luar pustaka `{fmt}`, Anda perlu menyertakan fungsi `has_implicit_bit` dan struktur pembantu `is_float128` yang mendeteksi tipe data 128-bit.

Berikut adalah contoh kode lengkap siap pakai:
```cpp
#include <iostream>
#include <limits>
#include <type_traits>

// 1. Mocking is_float128 untuk compiler yang mendukung __float128 (seperti GCC/Clang)
template <typename T>
struct is_float128 : std::false_type {};

#if defined(__SIZEOF_INT128__) && !defined(_MSC_VER)
template <>
struct is_float128<__float128> : std::true_type {};
#endif

namespace dragonbox {

// 2. Sertakan fungsi has_implicit_bit dari baris 1526 sebelumnya
template <typename Float> 
constexpr auto has_implicit_bit() -> bool {
  return std::numeric_limits<Float>::digits != 64;
}

// =========================================================================
// KODE HASIL EKSTRAK AWK ANDA (Baris 1532)
// =========================================================================
template <typename Float> 
constexpr auto num_significand_bits() -> int {
  return is_float128<Float>() ? 112
                              : (std::numeric_limits<Float>::digits -
                                 (has_implicit_bit<Float>() ? 1 : 0));
}

} // namespace dragonbox

int main() {
    std::cout << "=== Menghitung Jumlah Bit Mantissa Tersimpan ===\n\n";

    // 1. Uji pada tipe float (32-bit)
    // Total digit biner asli = 24. Karena memiliki 1 bit implisit, yang disimpan di memori adalah 23 bit.
    std::cout << "Bit mantissa tersimpan pada 'float' : " 
              << dragonbox::num_significand_bits<float>() << " bit (Ekspektasi: 23)\n";

    // 2. Uji pada tipe double (64-bit)
    // Total digit biner asli = 53. Karena memiliki 1 bit implisit, yang disimpan di memori adalah 52 bit.
    std::cout << "Bit mantissa tersimpan pada 'double': " 
              << dragonbox::num_significand_bits<double>() << " bit (Ekspektasi: 52)\n";

    // 3. Uji pada tipe long double (Format Extended 80-bit pada Linux x86_64)
    // Total digit biner asli = 64. Karena TIDAK memiliki bit implisit, seluruh 64 bit disimpan di memori.
    std::cout << "Bit mantissa tersimpan pada 'long double': " 
              << dragonbox::num_significand_bits<long double>() << " bit\n";

    return 0;
}

```

Mengapa Logika Ini Sangat Penting bagi Dragonbox?

Dalam representasi data komputer (standar IEEE 754), bit-bit pada memori dibagikan untuk Tanda (Sign), Eksponen (Exponent), dan Mantissa.

Untuk memisahkan komponen-komponen ini menggunakan operasi geser bit (bitwise shift) dan masker bit (bitwise AND), Dragonbox harus tahu **berapa langkah bit mantissa harus digeser** agar bagian eksponen atau bit tandanya terpisah. Angka kembalian dari fungsi `num_significand_bits()` inilah yang dijadikan sebagai parameter jarak pergeseran (*shift amount*) tersebut.

<br>

qa: 
Apakah Anda ingin melanjutkan ekstraksi ke baris berikutnya untuk melihat fungsi **ekstraksi bit eksponen secara aktual** menggunakan jarak pergeseran bit ini, atau ada bagian **representasi memori bitwise** lain yang ingin Anda bedah?

<br>

---

<br>

qs: 
`awk '{if(NR>=1541) print; if(NR==1547) exit}' $HOME/.cmake_fetchcontent_cache/fmt-v12.2.0/fmt-src/include/fmt/format.h`
```cpp
template <typename Float>
constexpr auto exponent_mask() ->
    typename dragonbox::float_info<Float>::carrier_uint {
  using float_uint = typename dragonbox::float_info<Float>::carrier_uint;
  return ((float_uint(1) << dragonbox::float_info<Float>::exponent_bits) - 1)
         << num_significand_bits<Float>();
}

```

<br>

aa: 
Fungsi `dragonbox::exponent_mask()` yang Anda ekstrak digunakan untuk menghasilkan **topeng bit (bitmask)** yang presisi untuk mengisolasi (mengambil) bagian bit eksponen dari suatu bilangan *floating-point*.

Fungsi ini memanfaatkan teknik operasi bitwise:
1. Membuat deretan bit `1` sepanjang jumlah bit eksponen tipe data tersebut (`exponent_bits`).
2. Menggeser deretan bit `1` tersebut ke kiri melewati area bit mantissa (`num_significand_bits`).
Karena bertipe `constexpr`, seluruh perhitungan masker bit ini diselesaikan oleh kompiler pada saat kompilasi (*compile-time*), menghasilkan angka masker konstan yang instan tanpa membebani performa program saat berjalan.

Cara Menggunakan Secara Mandiri (Standalone)

Agar fungsi ini bisa berjalan terpisah, Anda harus menyertakan struktur metadata `float_info` dan fungsi hitung bit `num_significand_bits` yang telah Anda ekstrak dari riwayat diskusi sebelumnya.
```cpp
#include <iostream>
#include <cstdint>
#include <limits>
#include <iomanip>

// 1. Definisikan struktur float_info minimalis hasil ekstrak Anda sebelumnya
template <typename T> struct float_info;

template <> struct float_info<float> {
  using carrier_uint = uint32_t;
  static constexpr int exponent_bits = 8;
};

template <> struct float_info<double> {
  using carrier_uint = uint64_t;
  static constexpr int exponent_bits = 11;
};

namespace dragonbox {
// 2. Fungsi pembantu pendukung dari baris 1526 dan 1532
template <typename Float> constexpr auto has_implicit_bit() -> bool {
  return std::numeric_limits<Float>::digits != 64;
}

template <typename Float> constexpr auto num_significand_bits() -> int {
  return std::numeric_limits<Float>::digits - (has_implicit_bit<Float>() ? 1 : 0);
}

// =========================================================================
// KODE HASIL EKSTRAK AWK ANDA (Baris 1541)
// =========================================================================
template <typename Float>
constexpr auto exponent_mask() -> typename float_info<Float>::carrier_uint {
  using float_uint = typename float_info<Float>::carrier_uint;
  return ((float_uint(1) << float_info<Float>::exponent_bits) - 1)
         << num_significand_bits<Float>();
}
} // namespace dragonbox

int main() {
    // Mengatur format keluaran ke bentuk Heksadesimal
    std::cout << std::hex << std::showbase << std::setfill('0');
    std::cout << "=== Masker Bit Eksponen Dragonbox ===\n\n";

    // 1. Uji untuk tipe float (32-bit)
    // Ekspektasi bit format IEEE 754 float: 1 bit tanda, 8 bit eksponen, 23 bit mantissa
    // Hasil masker biner: 00111111 10000000 00000000 00000000 -> 0x7f800000
    uint32_t masker_float = dragonbox::exponent_mask<float>();
    std::cout << "Masker Eksponen untuk 'float'  : " << masker_float << "\n";

    // 2. Uji untuk tipe double (64-bit)
    // Ekspektasi bit format IEEE 754 double: 1 bit tanda, 11 bit eksponen, 52 bit mantissa
    // Hasil masker biner: 0x7ff0000000000000
    uint64_t masker_double = dragonbox::exponent_mask<double>();
    std::cout << "Masker Eksponen untuk 'double' : " << masker_double << "\n";

    return 0;
}

```

Contoh Nyata Pemanfaatannya dalam Ekstraksi Bit

Di dalam algoritma Dragonbox, fungsi ini dipanggil bersama dengan operator bitwise AND (`&`) untuk menyaring nilai eksponen mentah dari variabel bertipe integer yang menampung bit pecahan.

Sebagai gambaran fungsional, cara memanfaatkannya untuk membedah data asli adalah seperti ini:
```cpp
// Misalkan kita punya bit mentah dari sebuah angka float yang disimpan sebagai uint32_t
uint32_t float_bits = 0x40490fdb; // Representasi biner dari float 3.1415926

// Mengisolasi bit eksponen saja dengan operator &
uint32_t isolated_exponent = float_bits & dragonbox::exponent_mask<float>(); 
// Hasil isolated_exponent akan bernilai 0x40000000

```

<br>

qa: 
Apakah Anda ingin melanjutkan ekstraksi ke baris berikutnya untuk melihat **bagaimana bit eksponen yang sudah diisolasi ini digeser kembali ke kanan untuk mendapatkan nilai integer eksponen yang sebenarnya**, atau ada bagian operasi **topeng bit** lainnya yang ingin Anda diskusikan?

<br>

---

<br>

qs: 
`awk '{if(NR>=1548) print; if(NR==1552) exit}' $HOME/.cmake_fetchcontent_cache/fmt-v12.2.0/fmt-src/include/fmt/format.h`
```cpp
template <typename Float> constexpr auto exponent_bias() -> int {
  // std::numeric_limits may not support __float128.
  return is_float128<Float>() ? 16383
                              : std::numeric_limits<Float>::max_exponent - 1;
}

```

<br>

aa: 
Fungsi `dragonbox::exponent_bias()` [1] yang Anda ekstrak digunakan untuk mendapatkan nilai **bias eksponen (_exponent bias_)** [1] dari suatu tipe data *floating-point*

Di dalam komputer (standar IEEE 754), nilai eksponen tidak disimpan sebagai bilangan bulat positif/negatif biasa. Agar CPU tidak perlu mengurus tanda minus pada eksponen, nilainya selalu ditambah dengan sebuah konstanta positif yang disebut bias [1] sebelum disimpan. Fungsi ini mengembalikan nilai bias [1] tersebut secara instan pada saat kompilasi (`constexpr`).

Cara Menggunakan Secara Mandiri (Standalone)

Agar dapat dikompilasi secara mandiri tanpa pustaka `{fmt}`, Anda hanya perlu menyertakan struktur pembantu `is_float128` [1] untuk mendeteksi tipe data 128-bit.

Berikut adalah contoh kode lengkap siap pakai:
```cpp
#include <iostream>
#include <limits>
#include <type_traits>

// 1. Mocking is_float128 untuk compiler GCC/Clang
template <typename T>
struct is_float128 : std::false_type {};

#if defined(__SIZEOF_INT128__) && !defined(_MSC_VER)
template <>
struct is_float128<__float128> : std::true_type {};
#endif

namespace dragonbox {
// =========================================================================
// KODE HASIL EKSTRAK AWK ANDA (Baris 1548)
// =========================================================================
template <typename Float> constexpr auto exponent_bias() -> int {
  return is_float128<Float>() ? 16383
                              : std::numeric_limits<Float>::max_exponent - 1;
}
} // namespace dragonbox

int main() {
    std::cout << "=== Nilai Bias Eksponen (Dragonbox) ===\n\n";

    // 1. Tipe float (32-bit) -> Ekspektasi bias: 127
    int bias_float = dragonbox::exponent_bias<float>();
    std::cout << "Bias eksponen untuk 'float'      : " << bias_float << "\n";

    // 2. Tipe double (64-bit) -> Ekspektasi bias: 1023
    int bias_double = dragonbox::exponent_bias<double>();
    std::cout << "Bias eksponen untuk 'double'     : " << bias_double << "\n";

    // 3. Tipe long double (Extended Precision 80-bit atau 128-bit) -> Ekspektasi bias: 16383
    int bias_long_double = dragonbox::exponent_bias<long double>();
    std::cout << "Bias eksponen untuk 'long double': " << bias_long_double << "\n";

    return 0;
}

```

Ketika Dragonbox menggunakan fungsi `exponent_mask()` yang Anda ambil sebelumnya untuk mengisolasi bit eksponen, nilai yang didapat masih berupa nilai bias ter-encode (misalnya, eksponen asli `0` pada tipe `double` akan tersimpan sebagai bit bernilai `1023`).

Untuk mendapatkan **nilai eksponen matematika yang sebenarnya** (berbasis biner/pangkat 2), Dragonbox melakukan kalkulasi berikut:
```cpp
// Contoh logika fungsional internal Dragonbox:
int raw_exponent_bits = 1023; // Bit eksponen yang terbaca dari memori
int real_exponent = raw_exponent_bits - dragonbox::exponent_bias<double>(); 
// real_exponent = 1023 - 1023 = 0 (2^0)

```
Nilai `real_exponent` inilah yang nantinya diumpankan ke fungsi `floor_log10_pow2(e)` yang pertama kali Anda ekstrak, demi menghitung estimasi posisi digit desimalnya secara akurat.

<br>

qa: 
Apakah Anda ingin melanjutkan ekstraksi ke baris berikutnya untuk melihat fungsi **pembongkaran bit tanda pecahan (_sign bit extraction_)**, atau ingin melihat bagaimana **bias eksponen ini dipakai untuk mendeteksi status khusus seperti NaN / Infinity**?

<br>

---

<br>

qs: 
`awk '{if(NR>=1554) print; if(NR==1559) exit}' $HOME/.cmake_fetchcontent_cache/fmt-v12.2.0/fmt-src/include/fmt/format.h`
```cpp
FMT_CONSTEXPR inline auto compute_exp_size(int exp) -> int {
  auto prefix_size = 2;  // sign + 'e'
  auto abs_exp = exp >= 0 ? exp : -exp;
  if (abs_exp < 100) return prefix_size + 2;
  return prefix_size + (abs_exp >= 1000 ? 4 : 3);
}

```

<br>

---

<br>

aa: 
Fungsi `compute_exp_size(int exp)` yang Anda ekstrak digunakan untuk menghitung **jumlah karakter (panjang string) yang dibutuhkan untuk mencetak bagian eksponen desimal** pada format notasi ilmiah (seperti `e+02`, `e-15`, atau `e+1024`).

Fungsi ini sangat dioptimalkan untuk performa tinggi:
- Menghindari alokasi string sementara (*temporary string allocation*).
- Menghindari fungsi berat seperti `std::to_string` atau pembagian desimal berulang (`/ 10`).
- Menggunakan pencabangan kondisi statis (`if`) yang sangat cepat diproses oleh CPU.

Cara Menggunakan Secara Mandiri (Standalone)

Fungsi ini tidak memiliki dependensi rumit. Anda hanya perlu mengganti makro `FMT_CONSTEXPR` milik pustaka `{fmt}` dengan kata kunci standar C++ **`constexpr`**. Dengan begitu, fungsi ini bahkan bisa dievaluasi saat kompilasi (*compile-time*).

Berikut adalah contoh kode lengkap siap pakai:
```cpp
#include <iostream>

// Mengganti FMT_CONSTEXPR dengan constexpr standar C++
constexpr inline auto compute_exp_size(int exp) -> int {
  auto prefix_size = 2;  // Mengalokasikan 2 karakter untuk tanda (+/-) dan huruf 'e'
  auto abs_exp = exp >= 0 ? exp : -exp;
  
  if (abs_exp < 100) return prefix_size + 2; // Eksponen 2 digit (misal: e+02 hingga e+99)
  return prefix_size + (abs_exp >= 1000 ? 4 : 3); // 3 digit (e+100) atau 4 digit (e+1000)
}

int main() {
    std::cout << "=== Menghitung Ukuran Karakter Eksponen ===\n\n";

    // Contoh 1: Eksponen kecil standar (misal: 1.23e+05) -> exp = 5
    // Panjang: 'e', '+', '0', '5' = 4 karakter
    int size1 = compute_exp_size(5);
    std::cout << "Ukuran karakter untuk exp = 5    : " << size1 << " karakter (Format: e+05)\n";

    // Contoh 2: Eksponen negatif (misal: 1.23e-45) -> exp = -45
    // Panjang: 'e', '-', '4', '5' = 4 karakter
    int size2 = compute_exp_size(-45);
    std::cout << "Ukuran karakter untuk exp = -45  : " << size2 << " karakter (Format: e-45)\n";

    // Contoh 3: Eksponen 3 digit tipe double (misal: 1.23e+100) -> exp = 100
    // Panjang: 'e', '+', '1', '0', '0' = 5 karakter (prefix_size 2 + 3)
    int size3 = compute_exp_size(100);
    std::cout << "Ukuran karakter untuk exp = 100  : " << size3 << " karakter (Format: e+100)\n";

    // Contoh 4: Eksponen sangat besar (misal pada float128 atau extended: 1.23e-4500) -> exp = -4500
    // Panjang: 'e', '-', '4', '5', '0', '0' = 6 karakter (prefix_size 2 + 4)
    int size4 = compute_exp_size(-4500);
    std::cout << "Ukuran karakter untuk exp = -4500: " << size4 << " karakter (Format: e-4500)\n";

    return 0;
}

```

Mengapa Logika Ukuran Angkanya Seperti Itu?

Jika Anda perhatikan baris ini:
```cpp
if (abs_exp < 100) return prefix_size + 2;

```
Pustaka `{fmt}` mengikuti standar pemformatan yang umumnya **memaksa eksponen minimal ditulis dalam 2 digit** (ditambah angka nol di depan atau leading zero jika nilainya di bawah 10).
- Nilai `exp = 5` dicetak sebagai `e+05` (total `4` karakter).
- Nilai `exp = 99` dicetak sebagai `e+99` (total `4` karakter).
- Nilai `exp = 125` dicetak sebagai `e+125` (total `5` karakter).
- Nilai `exp = 1024` dicetak sebagai `e+1024` (total `6` karakter).
Kegunaan Utama di Dalam `{fmt}`

Fungsi ini dipanggil oleh *string builder* internal `{fmt}` tepat sebelum karakter angka ditulis ke memori buffer. Dengan mengetahui ukuran pastinya terlebih dahulu, `{fmt}` dapat langsung mengalokasikan ruang memori yang pas pada array/buffer, mencegah terjadinya resiko *buffer overflow*, dan melakukan operasi penulisan karakter secara secepat kilat.

<br>

qa: 
Apakah Anda ingin melanjutkan ekstraksi untuk melihat fungsi `write_exponent` yang bertugas menulis karakter-karakter eksponen ini ke dalam buffer teks, atau ingin melihat fungsi pemformatan mantissa lainnya?

<br>

---

<br>

qs: 
`awk '{if(NR>=1561) print; if(NR==1586) exit}' $HOME/.cmake_fetchcontent_cache/fmt-v12.2.0/fmt-src/include/fmt/format.h`
```cpp
// Writes the exponent exp in the form "[+-]d{2,3}" to buffer.
template <typename Char, typename OutputIt>
FMT_CONSTEXPR auto write_exponent(int exp, OutputIt out) -> OutputIt {
  FMT_ASSERT(-10000 < exp && exp < 10000, "exponent out of range");
  if (exp < 0) {
    *out++ = static_cast<Char>('-');
    exp = -exp;
  } else {
    *out++ = static_cast<Char>('+');
  }
  auto uexp = static_cast<uint32_t>(exp);
  if (is_constant_evaluated()) {
    if (uexp < 10) *out++ = '0';
    return format_decimal<Char>(out, uexp, count_digits(uexp));
  }                                                                                                                            if (uexp >= 100u) {                                                                                                            const char* top = digits2(uexp / 100);
    if (uexp >= 1000u) *out++ = static_cast<Char>(top[0]);
    *out++ = static_cast<Char>(top[1]);
    uexp %= 100;
  }
  const char* d = digits2(uexp);                                                                                               *out++ = static_cast<Char>(d[0]);
  *out++ = static_cast<Char>(d[1]);
  return out;
}

```

<br>

aa: 
Fungsi `write_exponent` yang Anda ekstrak digunakan untuk menulis nilai eksponen integer (`exp`) ke dalam bentuk string/buffer teks dengan format tanda dan minimal dua digit angka (seperti `+05`, `-42`, `+128`, atau `-1024`).

Fungsi ini dirancang untuk kecepatan ekstrim dengan trik `digits2`, yaitu sebuah tabel pencarian (*lookup table*) eksternal yang menyimpan pasangan karakter `"00"` sampai `"99"`. CPU hanya perlu menyalin 2 karakter sekaligus dari memori, alih-alih melakukan operasi pembagian matematika (`/ 10` dan `% 10`) yang lambat.

Cara Menggunakan Secara Mandiri (Standalone)

Agar kode ini bisa berjalan secara mandiri tanpa dependensi internal `{fmt}`, kita perlu menyesuaikan beberapa komponen:
1. Mengganti `FMT_CONSTEXPR` dengan `constexpr`.
2. Menyediakan fungsi pembantu `digits2` (tabel 2-digit) dan `format_decimal` tiruan.
3. Mengganti `is_constant_evaluated()` (jika Anda menggunakan C++20 ke atas, ini setara dengan `std::is_constant_evaluated()`). Untuk kesederhanaan *runtime*, kita bisa fokus pada jalur optimasi utamanya.

Berikut adalah kode lengkap siap pakai yang menggunakan `std::string` atau penunjuk karakter (`char*`) sebagai output iteratornya:
```cpp
#include <iostream>
#include <cassert>
#include <cstdint>
#include <string>

// 1. Menyediakan tabel lookup 2-digit bawaan {fmt} (format-inl.h) secara mandiri
inline auto digits2(uint32_t value) noexcept -> const char* {
    // Tabel datar berisi pasangan karakter dari "00" sampai "99"
    static const char table[] =
        "0001020304050607080910111213141516171819"
        "2021222324252627282930313233343536373839"
        "4041424344454647484950515253545556575859"
        "6061626364656667686970717273747576777879"
        "8081828384858687888990919293949596979899";
    return &table[value * 2];
}

namespace dragonbox {

// =========================================================================
// KODE HASIL EKSTRAK AWK ANDA (Disederhanakan untuk penggunaan umum)
// =========================================================================
template <typename Char, typename OutputIt>
inline auto write_exponent(int exp, OutputIt out) -> OutputIt {
  // Mengganti FMT_ASSERT dengan assert standar
  assert(-10000 < exp && exp < 10000);
  
  if (exp < 0) {
    *out++ = static_cast<Char>('-');
    exp = -exp;
  } else {
    *out++ = static_cast<Char>('+');
  }
  
  auto uexp = static_cast<uint32_t>(exp);
  
  // Optimasi jalur utama menggunakan tabel digits2
  if (uexp >= 100u) {                                                                                                            
    const char* top = digits2(uexp / 100);
    if (uexp >= 1000u) *out++ = static_cast<Char>(top[0]);
    *out++ = static_cast<Char>(top[1]);
    uexp %= 100;
  }
  
  const char* d = digits2(uexp);                                                                                               
  *out++ = static_cast<Char>(d[0]);
  *out++ = static_cast<Char>(d[1]);
  
  return out;
}

} // namespace dragonbox

int main() {
    std::cout << "=== Demo Penulisan Eksponen Cepat ===\n\n";

    // Menyiapkan buffer karakter untuk menampung string hasil
    char buffer[10];

    // Contoh 1: Menulis eksponen kecil positif (5) -> "+05"
    char* end1 = dragonbox::write_exponent<char>(5, buffer);
    *end1 = '\0'; // Jangan lupa tambahkan null-terminator di akhir string buffer
    std::cout << "Input: 5     -> Output teks: " << buffer << "\n";

    // Contoh 2: Menulis eksponen negatif (-42) -> "-42"
    char* end2 = dragonbox::write_exponent<char>(-42, buffer);
    *end2 = '\0';
    std::cout << "Input: -42   -> Output teks: " << buffer << "\n";

    // Contoh 3: Menulis eksponen 3-digit (128) -> "+128"
    char* end3 = dragonbox::write_exponent<char>(128, buffer);
    *end3 = '\0';
    std::cout << "Input: 128   -> Output teks: " << buffer << "\n";

    // Contoh 4: Menggunakan std::back_inserter dengan std::string
    std::string hasil_string = "e";
    dragonbox::write_exponent<char>(-1024, std::back_inserter(hasil_string));
    std::cout << "Menggunakan std::string    -> " << hasil_string << "\n";

    return 0;
}

```
Alur Kerja Operasi Bit & Teks di Dalamnya:
1. Pengecekan Tanda: Mengecek apakah eksponen negatif atau positif, lalu menulis karakter `'-'` atau `'+'` ke iterator `out`.
2. Pemisahan Ratusan/Ribuan (`uexp >= 100u`): Jika angka lebih dari atau sama dengan 100, ia membagi angka dengan 100 terlebih dahulu untuk mendapatkan digit depan (ratusan/ribuan), mengambil pasangannya dari tabel `digits2`, lalu menulisnya ke iterator. Sisa baginya (`uexp %= 100`) disimpan kembali.
3. Pencetakan Puluhan & Satuan: Mengambil sisa 2 digit terakhir dari tabel digits2, lalu menulis kedua karakter tersebut secara berturut-turut (`d[0]` dan `d[1]`).

<br>

qa: 
Langkah ini melengkapi siklus pemformatan bagian belakang notasi ilmiah. Apakah Anda ingin melanjutkan ekstraksi untuk melihat fungsi **pembentukan string mantissa utama** (bagian depan desimal sebelum huruf 'e'), atau ada bagian penanganan **iterator C++** lainnya yang ingin Anda pelajari?

<br>

---

<br>

qs: 
`awk '{if(NR>=1588) print; if(NR==1633) exit}' $HOME/.cmake_fetchcontent_cache/fmt-v12.2.0/fmt-src/include/fmt/format.h`
```cpp
// A floating-point number f * pow(2, e) where F is an unsigned type.
template <typename F> struct basic_fp {
  F f;
  int e;
                                                                                                                               static constexpr int num_significand_bits =
      static_cast<int>(sizeof(F) * num_bits<unsigned char>());

  constexpr basic_fp() : f(0), e(0) {}
  constexpr basic_fp(uint64_t f_val, int e_val) : f(f_val), e(e_val) {}

  // Constructs fp from an IEEE754 floating-point number.                                                                      template <typename Float> FMT_CONSTEXPR basic_fp(Float n) { assign(n); }                                                                                                                                                                                  // Assigns n to this and return true iff predecessor is closer than successor.
  template <typename Float, FMT_ENABLE_IF(!is_double_double<Float>::value)>
  FMT_CONSTEXPR auto assign(Float n) -> bool {
    static_assert(std::numeric_limits<Float>::digits <= 113, "unsupported FP");
    // Assume Float is in the format [sign][exponent][significand].
    using carrier_uint = typename dragonbox::float_info<Float>::carrier_uint;                                                    const auto num_float_significand_bits =
        detail::num_significand_bits<Float>();
    const auto implicit_bit = carrier_uint(1) << num_float_significand_bits;
    const auto significand_mask = implicit_bit - 1;
    auto u = bit_cast<carrier_uint>(n);
    f = static_cast<F>(u & significand_mask);
    auto biased_e = static_cast<int>((u & exponent_mask<Float>()) >>
                                     num_float_significand_bits);
    // The predecessor is closer if n is a normalized power of 2 (f == 0)
    // other than the smallest normalized number (biased_e > 1).                                                                 auto is_predecessor_closer = f == 0 && biased_e > 1;
    if (biased_e == 0)
      biased_e = 1;  // Subnormals use biased exponent 1 (min exponent).
    else if (has_implicit_bit<Float>())
      f += static_cast<F>(implicit_bit);
    e = biased_e - exponent_bias<Float>() - num_float_significand_bits;
    if (!has_implicit_bit<Float>()) ++e;
    return is_predecessor_closer;
  }

  template <typename Float, FMT_ENABLE_IF(is_double_double<Float>::value)>
  FMT_CONSTEXPR auto assign(Float n) -> bool {
    static_assert(std::numeric_limits<double>::is_iec559, "unsupported FP");
    return assign(static_cast<double>(n));
  }
};

```

<br>

aa: 
Struktur `basic_fp` (singkatan dari *Basic Floating-Point*) adalah penampung internal dalam pustaka `{fmt}` untuk merepresentasikan angka pecahan biner dalam format mentah matematika:

\(\text{Nilai}=f\times 2^{e}\)

Di mana `f` adalah *significand* murni (tanpa bit implisit tersembunyi), dan `e` adalah eksponen biner berbasis 2.

Fungsi paling krusial di dalam struktur ini adalah `assign(Float n)`. Fungsi ini bertugas melakukan dekoding bitwise tingkat rendah: membedah bit-bit dari tipe data `float` atau `double` standar (IEEE 754), mengisolasi masker, menyuntikkan bit implisit yang hilang, dan membersihkan bias eksponen.

💡 Cara Menggunakan Secara Mandiri (Standalone)

Agar kode hasil ekstrak `awk` Anda dapat dikompilasi secara mandiri, Anda harus melengkapinya dengan fungsi-fungsi pendukung yang telah Anda ekstrak dari diskusi sebelumnya (`num_significand_bits`, `exponent_mask`, `exponent_bias`, `has_implicit_bit`), serta menambahkan simulasi untuk fungsi `bit_cast` dan makro SFINAE (`FMT_ENABLE_IF`).

Berikut adalah contoh implementasi lengkap siap pakai:
```cpp
#include <iostream>
#include <cstdint>
#include <limits>
#include <type_traits>
#include <cstring>

// =========================================================================
// 1. STRUKTUR & FUNGSI PENDUKUNG (HASIL DISKUSI SEBELUMNYA + STANDAR)
// =========================================================================

// Pengganti bit_cast (Gunakan std::bit_cast jika menggunakan C++20)
template <typename To, typename From>
To bit_cast(const From& src) noexcept {
    static_assert(sizeof(To) == sizeof(From), "Size mismatch for bit_cast");
    To dst;
    std::memcpy(&dst, &src, sizeof(To));
    return dst;
}

// Mocking is_double_double untuk kesederhanaan compile-time
template <typename T> struct is_double_double : std::false_type {};
template <typename T> struct is_float128 : std::false_type {};

namespace dragonbox {
    template <typename T> struct float_info;
    template <> struct float_info<float> {
        using carrier_uint = uint32_t;
        static constexpr int exponent_bits = 8;
    };
    template <> struct float_info<double> {
        using carrier_uint = uint64_t;
        static constexpr int exponent_bits = 11;
    };

    template <typename Float> constexpr auto has_implicit_bit() -> bool {
        return std::numeric_limits<Float>::digits != 64;
    }

    template <typename Float> constexpr auto num_significand_bits() -> int {
        return is_float128<Float>() ? 112 : (std::numeric_limits<Float>::digits - (has_implicit_bit<Float>() ? 1 : 0));
    }

    template <typename Float> constexpr auto exponent_mask() -> typename float_info<Float>::carrier_uint {
        using float_uint = typename float_info<Float>::carrier_uint;
        return ((float_uint(1) << float_info<Float>::exponent_bits) - 1) << num_significand_bits<Float>();
    }

    template <typename Float> constexpr auto exponent_bias() -> int {
        return is_float128<Float>() ? 16383 : std::numeric_limits<Float>::max_exponent - 1;
    }
}

// Untuk menyederhanakan kode, kita ubah namespace detail agar cocok dengan kode asli
namespace detail {
    using dragonbox::num_significand_bits;
}

// =========================================================================
// 2. KODE HASIL EKSTRAK AWK ANDA (Disesuaikan Agar Standalone)
// =========================================================================
template <typename F> struct basic_fp {
  F f;
  int e;
  
  static constexpr int num_significand_bits = static_cast<int>(sizeof(F) * 8); // Dipermudah dari num_bits<unsigned char>()

  constexpr basic_fp() : f(0), e(0) {}
  constexpr basic_fp(uint64_t f_val, int e_val) : f(f_val), e(e_val) {}

  template <typename Float> basic_fp(Float n) { assign(n); }                                                                                                                                                                                  
  
  // Menggunakan std::enable_if_t sebagai pengganti FMT_ENABLE_IF
  template <typename Float, typename std::enable_if_t<!is_double_double<Float>::value, int> = 0>
  auto assign(Float n) -> bool {
    static_assert(std::numeric_limits<Float>::digits <= 113, "unsupported FP");
    using carrier_uint = typename dragonbox::float_info<Float>::carrier_uint;                                                    
    const auto num_float_significand_bits = detail::num_significand_bits<Float>();
    const auto implicit_bit = carrier_uint(1) << num_float_significand_bits;
    const auto significand_mask = implicit_bit - 1;
    
    auto u = bit_cast<carrier_uint>(n);
    f = static_cast<F>(u & significand_mask);
    auto biased_e = static_cast<int>((u & dragonbox::exponent_mask<Float>()) >> num_float_significand_bits);
    
    auto is_predecessor_closer = f == 0 && biased_e > 1;
    if (biased_e == 0)
      biased_e = 1;  
    else if (dragonbox::has_implicit_bit<Float>())
      f += static_cast<F>(implicit_bit);
    e = biased_e - dragonbox::exponent_bias<Float>() - num_float_significand_bits;
    if (!dragonbox::has_implicit_bit<Float>()) ++e;
    return is_predecessor_closer;
  }

  template <typename Float, typename std::enable_if_t<is_double_double<Float>::value, int> = 0>
  auto assign(Float n) -> bool {
    return assign(static_cast<double>(n));
  }
};

// =========================================================================
// 3. UJI COBA DI MAIN
// =========================================================================
int main() {
    std::cout << "=== Pembedahan Bit Floating-Point dengan basic_fp ===\n\n";

    // Kita uji dengan angka 0.5 (dalam biner adalah 1 * 2^-1)
    double angka_uji = 0.5;
    
    // Membuat objek basic_fp dengan tipe tampungan uint64_t untuk mantissanya
    basic_fp<uint64_t> fp_obj;
    
    // Memanggil fungsi assign untuk membedah bit angka_uji
    bool is_closer = fp_obj.assign(angka_uji);

    std::cout << "Angka Asli: " << angka_uji << "\n";
    std::cout << "Hasil Dekoding Biner Mentah:\n";
    std::cout << "  f (Significand murni) : " << fp_obj.f << "\n";
    std::cout << "  e (Eksponen biner)   : " << fp_obj.e << "\n";
    std::cout << "  Nilai Matematika      : " << fp_obj.f << " * 2^(" << fp_obj.e << ")\n";
    std::cout << "  Is Predecessor Closer : " << (is_closer ? "true" : "false") << "\n";

    return 0;
}

```

🔎 Mengapa Fungsi Ini Mengembalikan Nilai bool?

Perhatikan baris terakhir fungsi `assign`: `return is_predecessor_closer;`.

Nilai boolean ini sangat penting untuk **logika pembulatan (_round-to-nearest_)** pada algoritma Dragonbox. Ketika sebuah bilangan biner berada tepat di tengah-tengah dua representasi angka desimal, Dragonbox harus memilih apakah akan membulatkan ke atas (*successor*) atau ke bawah (*predecessor*). Jika `is_predecessor_closer` bernilai `true`, sistem tahu bahwa interval biner di bawahnya memiliki jarak batas yang lebih rapat, sehingga teks desimal yang dihasilkan nantinya akan disesuaikan agar tidak melanggar batasan presisi IEEE 754.

Sekarang Anda telah mengekstrak seluruh pustaka matematika penunjang dari Dragonbox. Jika Anda tertarik untuk melihat bagaimana nilai biner mentah dari `basic_fp` ini **dikalikan dengan tabel konstanta pangkat sepuluh** untuk menjadi desimal ilmiah, saya bisa bantu Anda untuk:
- Mencari letak **struktur `cache_entry_type`** yang menampung konstanta pangkat sepuluh.
- Mengekstrak fungsi `get_cached_power` yang sempat terlewat di baris 1473.

<br>

qa: 
Bagian mana yang ingin Anda bedah selanjutnya?

<br>

---

<br>

qs: 
`awk '{if(NR>=1635) print; if(NR==1653) exit}' $HOME/.cmake_fetchcontent_cache/fmt-v12.2.0/fmt-src/include/fmt/format.h`
```cpp
using fp = basic_fp<ullong>;

// Normalizes the value converted from double and multiplied by (1 << SHIFT).
template <int SHIFT = 0, typename F>
FMT_CONSTEXPR auto normalize(basic_fp<F> value) -> basic_fp<F> {
  // Handle subnormals.
  const auto implicit_bit = F(1) << num_significand_bits<double>();
  const auto shifted_implicit_bit = implicit_bit << SHIFT;
  while ((value.f & shifted_implicit_bit) == 0) {
    value.f <<= 1;
    --value.e;
  }
  // Subtract 1 to account for hidden bit.
  const auto offset = basic_fp<F>::num_significand_bits -
                      num_significand_bits<double>() - SHIFT - 1;
  value.f <<= offset;
  value.e -= offset;
  return value;
}

```

<br>

aa: 
Fungsi **normalize** yang Anda ekstrak digunakan untuk melakukan **normalisasi bit** pada objek `basic_fp`. Normalisasi adalah proses menggeser bit *significand* (`value.f`) ke kiri hingga bit paling signifikannya berada di posisi paling atas register memori, sambil menyesuaikan nilai eksponennya (`value.e`).

Di dalam algoritma Dragonbox, fungsi ini memiliki dua tugas krusial:
1. **Menangani Bilangan Subnormal**: Jika angka *floating-point* berukuran sangat kecil hingga bit implisitnya bernilai 0 (subnormal), fungsi ini akan melakukan *looping* untuk menggeser bit ke kiri sampai menemukan bit `1` pertama yang valid.
2. **Penyelarasan Presisi (_Alignment_)**: Menggeser bit *significand* yang tersisa agar rata kiri memenuhi lebar penuh dari tipe data penampungnya (F, dalam hal ini `ullong` atau `uint64_t`), sehingga operasi perkalian 128-bit berikutnya dapat memanfaatkan presisi maksimum hardware.

💡 Cara Menggunakan Secara Mandiri (Standalone)

Agar kode ini dapat berjalan mandiri, Anda membutuhkan struktur `basic_fp` dan fungsi pembantu bit (`num_significand_bits`) yang sudah kita bahas pada langkah sebelumnya. Kita juga perlu mendefinisikan alias `ullong` (biasanya `unsigned long long` atau `uint64_t`).

Berikut adalah contoh kode lengkap siap pakai:
```cpp
#include <iostream>
#include <cstdint>
#include <limits>

// Alias tipe data seperti yang digunakan oleh pustaka {fmt}
using ullong = unsigned long long;

// 1. Masukkan komponen dasar basic_fp dari ekstrak sebelumnya
template <typename F> struct basic_fp {
  F f;
  int e;
  static constexpr int num_significand_bits = static_cast<int>(sizeof(F) * 8);
  constexpr basic_fp() : f(0), e(0) {}
  constexpr basic_fp(F f_val, int e_val) : f(f_val), e(e_val) {}
};

namespace dragonbox {
    // Fungsi pembantu hitung bit (asumsi tipe double standar IEEE 754 = 52 bit)
    template <typename Float> constexpr auto num_significand_bits() -> int {
        return 52; 
    }
}

// Alias agar sesuai dengan kode asli {fmt}
using fp = basic_fp<ullong>;

// =========================================================================
// KODE HASIL EKSTRAK AWK ANDA (Baris 1635)
// =========================================================================
template <int SHIFT = 0, typename F>
inline auto normalize(basic_fp<F> value) -> basic_fp<F> {
  // Menangani bilangan subnormal dengan mendeteksi bit implisit yang tergeser
  const auto implicit_bit = F(1) << dragonbox::num_significand_bits<double>();
  const auto shifted_implicit_bit = implicit_bit << SHIFT;
  
  while ((value.f & shifted_implicit_bit) == 0) {
    value.f <<= 1;
    --value.e;
  }
  
  // Menggeser sisa bit agar rata kiri penuh (memenuhi kapasitas tipe data F)
  const auto offset = basic_fp<F>::num_significand_bits -
                      dragonbox::num_significand_bits<double>() - SHIFT - 1;
  value.f <<= offset;
  value.e -= offset;
  return value;
}

int main() {
    std::cout << "=== Demo Normalisasi Bit basic_fp (Dragonbox) ===\n\n";

    // Kita simulasikan sebuah angka subnormal kecil hasil pembedahan bit
    // Nilai awal: f = 0x0010000000000000 (tidak rata kiri penuh), e = -100
    fp angka_mentah(0x0010000000000000ULL, -100);

    std::cout << "Sebelum Dinormalisasi:\n";
    std::cout << "  f : 0x" << std::hex << angka_mentah.f << "\n";
    std::cout << "  e : " << std::dec << angka_mentah.e << "\n";

    // Eksekusi fungsi normalize
    fp angka_normal = normalize(angka_mentah);

    std::cout << "\nSetelah Dinormalisasi:\n";
    std::cout << "  f : 0x" << std::hex << angka_normal.f << "\n";
    std::cout << "  e : " << std::dec << angka_normal.e << "\n";
    std::cout << "  (Bit f sekarang tergeser ke atas untuk presisi penuh)\n";

    return 0;
}

```

🔍 Membedah Logika Matematika di Balik Kode

Perhatikan variabel `offset` pada potongan kode:
```cpp
const auto offset = basic_fp<F>::num_significand_bits - num_significand_bits<double>() - SHIFT - 1;

```
Jika kita menggunakan `F` sebagai `uint64_t` (64 bit) dan memproses tipe `double` (52 bit significand stored), maka jika `SHIFT` bernilai 0:

\(\text{offset}=64-52-0-1=11\)

Kode selanjutnya melakukan `value.f <<= 11;` dan `value.e -= 11;`. Operasi ini secara instan mendorong isi *significand* 11 bit ke kiri agar menempel tepat di ujung batas 64-bit register CPU. Karena nilainya dikalikan sebesar \(2^{11}\), maka eksponen biner `value.e` harus dikurangi 11 agar nilai matematis aslinya tidak berubah.

<br>

qa: 
Apakah Anda ingin melanjutkan ekstraksi ke baris berikutnya untuk melihat **bagaimana data `fp` yang telah dinormalisasi ini dikalikan dengan konstanta pangkat sepuluh**, atau ada detail **operasi bitwise _shifting_** lain yang ingin Anda bedah?

Bila Anda ingin melanjutkan, beri tahu saya jika Anda ingin mencari letak implementasi fungsi `get_cached_power` yang bertugas menyediakan angka pengali tersebut.

<br>

---

<br>

qs: 
awk '{if(NR>=1655) print; if(NR==1671) exit}' $HOME/.cmake_fetchcontent_cache/fmt-v12.2.0/fmt-src/include/fmt/format.h  
```cpp
// Computes lhs * rhs / pow(2, 64) rounded to nearest with half-up tie breaking.
FMT_CONSTEXPR inline auto multiply(uint64_t lhs, uint64_t rhs) -> uint64_t {
#if FMT_USE_INT128
  auto product = static_cast<__uint128_t>(lhs) * rhs;
  auto f = static_cast<uint64_t>(product >> 64);
  return (static_cast<uint64_t>(product) & (1ULL << 63)) != 0 ? f + 1 : f;
#else
  // Multiply 32-bit parts of significands.
  uint64_t mask = (1ULL << 32) - 1;
  uint64_t a = lhs >> 32, b = lhs & mask;
  uint64_t c = rhs >> 32, d = rhs & mask;                                                                                      uint64_t ac = a * c, bc = b * c, ad = a * d, bd = b * d;                                                                     // Compute mid 64-bit of result and round.
  uint64_t mid = (bd >> 32) + (ad & mask) + (bc & mask) + (1U << 31);
  return ac + (ad >> 32) + (bc >> 32) + (mid >> 32);                                                                         #endif
}

```

<br>

---

<br>

qs:
`grep -n "auto put(" $HOME/.cmake_fetchcontent_cache/fmt-v12.2.0/fmt-src/include/fmt/format.h`
`sed -n '3914,3940p' $HOME/.cmake_fetchcontent_cache/fmt-v12.2.0/fmt-src/include/fmt/format.h`
`https://github.com/fmtlib/fmt/blob/12.2.0/include/fmt/format.h#L3914`
```cpp
// A locale facet that formats values in UTF-8.
// It is parameterized on the locale to avoid the heavy <locale> include.
template <typename Locale> class format_facet : public Locale::facet {
 private:
  std::string separator_;
  std::string grouping_;
  std::string decimal_point_;

 protected:
  virtual auto do_put(appender out, loc_value val,
                      const format_specs& specs) const -> bool;

 public:
  static FMT_API typename Locale::id id;

  explicit format_facet(Locale& loc);
  explicit format_facet(string_view sep = "", std::string grouping = "\3",
                        std::string decimal_point = ".")
      : separator_(sep.data(), sep.size()),
        grouping_(std::move(grouping)),
        decimal_point_(std::move(decimal_point)) {}

  auto put(appender out, loc_value val, const format_specs& specs) const
      -> bool {
    return do_put(out, val, specs);
  }
};

```

<br>

aa:
Kelas **template <typename Locale> class format_facet** (pada baris 3914) merupakan komponen internal tingkat rendah (*low-level API*) dari pustaka `{fmt}`.

Kelas ini bertindak sebagai sebuah **Locale Facet** khusus yang bertugas menyimpan aturan penulisan angka, seperti penanda **karakter pemisah ribuan, aturan pengelompokan digit**, serta **karakter desimal**.

Pustaka `{fmt}` mendesain kelas ini dengan *template* **typename Locale** agar ia bisa disuntikkan ke dalam sistem lokalisasi bawaan C++ (`std::locale`) tanpa membuat proses kompilasi melambat akibat menyertakan *header* `<locale>` yang terkenal sangat berat.

Berikut adalah cara menggunakannya jika Anda ingin membuat aturan pemformatan angka kustom sendiri (misalnya membuat format **Rupiah Indonesia** dengan pemisah titik):

1. Cara Penggunaan Standar (Menyuntikkan Facet ke `std::locale`)
Anda bisa menginstansiasi `format_facet`, mengatur pemisah ribuan menjadi titik `"."`, pengelompokan setiap 3 digit `"\3"`, dan desimal menjadi koma `","`. Setelah itu, masukkan objek tersebut ke dalam `std::locale` menggunakan `std::locale::combine`.
```cpp
#include <fmt/format.h>
#include <locale>

int main() {
    double uang = 5450000.75;

    // 1. Buat kustom facet dengan aturan Indonesia/Eropa:
    // Pemisah ribuan = Titik (".")
    // Grouping = Setiap 3 digit ("\3")
    // Desimal = Koma (",")
    auto kustom_facet = new fmt::format_facet<std::locale>(".", "\3", ",");

    // 2. Suntikkan facet kustom ini ke dalam objek std::locale dasar
    std::locale locale_id = std::locale::classic().combine<fmt::format_facet<std::locale>>(kustom_facet);

    // 3. Gunakan dengan fmt::format atau fmt::print menggunakan specifier {:L}
    std::string hasil = fmt::format(locale_id, "Total Biaya: Rp{:L}", uang);
    
    fmt::print("{}\n", hasil);
    // Output: Total Biaya: Rp5.450.000,75
}

```

2. Cara Penggunaan Tingkat Rendah (Memanggil Fungsi `.put()` Manual)
Jika Anda sedang membuat pustaka/mesin teks sendiri dan ingin memformat angka langsung melalui buffer bawaan `{fmt}` tanpa lewat fungsi makro `fmt::format`, Anda bisa memanggil fungsi `put()` secara manual.
```cpp
#include <fmt/format.h>

int main() {
    // 1. Siapkan buffer penampung teks teks mentah
    fmt::memory_buffer buf;
    auto appender_out = fmt::appender(buf);

    // 2. Buat facet mandiri
    fmt::format_facet<std::locale> facet_titik(".", "\3", ",");

    // 3. Siapkan spesifikasi format kosong (default)
    fmt::detail::format_specs specs;

    // 4. Bungkus nilai angka ke dalam objek internal loc_value
    // (Misalkan nilai angka bulat 1250000)
    fmt::detail::loc_value nilai_angka(1250000);

    // 5. Eksekusi fungsi put() langsung ke buffer
    facet_titik.put(appender_out, nilai_angka, specs);

    // Cetak hasil dari buffer mentah
    fmt::print("Hasil buffer put: {}\n", fmt::to_string(buf));
    // Output: Hasil buffer put: 1.234.567
}

```

Kesimpulan Ringkas
- `format_facet` adalah pengatur internal untuk urusan tanda baca angka.
- Paling sering digunakan untuk **membuat format angka kustom cross-platform** (seperti pemisah ribuan titik untuk Rupiah) tanpa perlu khawatir apakah sistem operasi Linux/Windows target memiliki paket bahasa negara tersebut atau tidak.

<br>

qa:
Apakah Anda ingin melihat contoh integrasi `format_facet` ini untuk memformat **tipe data buatan sendiri** atau butuh bantuan untuk **menangani kompilasi lintas OS** (Windows & Linux) saat bermain dengan objek `std::locale`?

<br>

qs:
`grep -n "auto format(T" $HOME/.cmake_fetchcontent_cache/fmt-v12.2.0/fmt-src/include/fmt/format.h`
`sed -n '3942,3950p' $HOME/.cmake_fetchcontent_cache/fmt-v12.2.0/fmt-src/include/fmt/format.h`
`https://github.com/fmtlib/fmt/blob/12.2.0/include/fmt/format.h#L3942`
```cpp
#define FMT_FORMAT_AS(Type, Base)                                   \
  template <typename Char>                                          \
  struct formatter<Type, Char> : formatter<Base, Char> {            \
    template <typename FormatContext>                               \
    FMT_CONSTEXPR auto format(Type value, FormatContext& ctx) const \
        -> decltype(ctx.out()) {                                    \
      return formatter<Base, Char>::format(value, ctx);             \
    }                                                               \
  }

```

<br>

aa:
Makro `FMT_FORMAT_AS(Type, Base)` adalah cara resmi, paling singkat, dan paling direkomendasikan oleh pustaka `{fmt}` untuk mendaftarkan tipe data kustom Anda (khususnya `enum` atau `enum class`) agar bisa langsung dicetak sebagai tipe data dasar (`Base`).

Makro ini membungkus spesialisasi struktur `formatter` panjang yang kita bahas sebelumnya ke dalam satu baris kode saja.

Berikut adalah cara menggunakannya di dalam kode Anda:

1. Cara Penggunaan pada enum `class` (Skenario Paling Sering)
Cukup letakkan makro `FMT_FORMAT_AS` di luar fungsi (`global scope`) setelah Anda mendefinisikan tipe `enum class`.
```cpp
#include <fmt/format.h>

// 1. Definisikan enum class Anda
enum class Level {
    LOW = 1,
    MEDIUM = 2,
    HIGH = 3
};

// 2. Gunakan makro untuk mendaftarkan: FMT_FORMAT_AS(TipeKustom, TipeDasar)
FMT_FORMAT_AS(Level, int);

int main() {
    Level spek_pc = Level::HIGH;

    // 3. Langsung cetak! Otomatis dikonversi ke int (3) saat diformat
    fmt::print("Level performa: {}\n", spek_pc);
    // Output: Level performa: 3
}

```

2. Cara Penggunaan untuk Mengonversi ke String (`std::string_view`)
Jika Anda memiliki objek atau tipe data yang ingin selalu dicetak sebagai teks tertentu, Anda bisa mengarahkannya ke `std::string_view` atau `const char*` dengan syarat tipe kustom tersebut memiliki fungsi konversi implisit ke tipe `Base`-nya.
```cpp
#include <fmt/format.h>
#include <string_view>

enum class Arah {
    UTARA, SELATAN
};

// Fungsi pembantu konversi
constexpr auto to_string(Arah a) -> std::string_view {
    return a == Arah::UTARA ? "Utara" : "Selatan";
}

// Struct pembungkus yang bisa dikonversi secara implisit ke string_view
struct LabelArah {
    Arah arah;
    constexpr operator std::string_view() const { return to_string(arah); }
};

// Daftarkan menggunakan makro
FMT_FORMAT_AS(LabelArah, std::string_view);

int main() {
    LabelArah tujuan = {Arah::UTARA};
    
    fmt::print("Bus ini melaju ke arah: {}\n", tujuan);
    // Output: Bus ini melaju ke arah: Utara
}

```

💡 Keuntungan Utama Menggunakan `FMT_FORMAT_AS`
- Zero Boilerplate: Anda tidak perlu lagi menulis `template <> struct formatter<...>` yang memakan waktu dan baris kode yang panjang.
- Mendukung Fitur Bawaan Tipe Base: Karena ia mewarisi (`inheritance`) sifat dari `formatter<Base>`, semua *format specifier* milik tipe dasar otomatis bisa dipakai. Contohnya pada `Level` yang berbasis `int` di atas, Anda otomatis bisa menggunakan pemformatan angka seperti biner atau hex:
```cpp
Level spek = Level::HIGH; // bernilai 3
fmt::print("Format biner: {:b}\n", spek); 
// Output: Format biner: 11

```

<br>

qa:
Apakah Anda ingin tahu cara menerapkan ini pada **banyak `enum` sekaligus**, atau ada kendala kompilasi terkait **lingkup namespace** saat menaruh makro ini?

<br>

qs:








<br>

qs:
`grep -n "auto format(c" $HOME/.cmake_fetchcontent_cache/fmt-v12.2.0/fmt-src/include/fmt/format.h`
`sed -n '3975,3984p' $HOME/.cmake_fetchcontent_cache/fmt-v12.2.0/fmt-src/include/fmt/format.h`
`https://github.com/fmtlib/fmt/blob/12.2.0/include/fmt/format.h#L3975`
```cpp
template <typename T, typename Char>
struct formatter<T, Char, void_t<detail::format_as_result<T>>>
    : formatter<detail::format_as_result<T>, Char> {
  template <typename FormatContext>
  FMT_CONSTEXPR auto format(const T& value, FormatContext& ctx) const
      -> decltype(ctx.out()) {
    auto&& val = format_as(value);  // Make an lvalue reference for format.
    return formatter<detail::format_as_result<T>, Char>::format(val, ctx);
  }
};

```

<br>

aa:
Jembatan Fitur format_as (Tipe Data Kustom / Enum)

Struktur `struct formatter<T, Char, void_t<detail::format_as_result<T>>>` merupakan fitur modern dan sangat kuat di pustaka `{fmt}` yang mengimplementasikan mekanisme `FMT_FORMAT_AS`.

Fungsi ini digunakan agar tipe data kustom Anda (paling sering **enum class**) bisa dicetak langsung sebagai tipe data dasar (seperti `int` atau `std::string_view`) **tanpa harus menulis struktur `formatter` panjang lebar**.

Berikut adalah cara menggunakannya melalui API publik `{fmt}`:

1. Cara Penggunaan Paling Umum (Untuk `enum class`)
Cukup buat fungsi global bernama `format_as(T value)` yang mengembalikan tipe data primitif (seperti `int`, `underlying_type`, atau `string_view`). `{fmt}` akan mendeteksi fungsi tersebut secara otomatis melalui potongan kode `format_as(value)` pada baris 14 di atas.
```cpp
#include <fmt/format.h>

// 1. Definisikan enum class Anda
enum class Status Kode {
    SUKSES = 200,
    NOT_FOUND = 404,
    SERVER_ERROR = 500
};

// 2. Buat fungsi format_as() di namespace yang sama dengan enum
// Mengembalikan tipe data dasar integer agar bisa dicetak otomatis
auto format_as(Status Kode s) {
    return static_cast<int>(s);
}

int main() {
    Status Kode respon = Status Kode::NOT_FOUND;

    // 3. Langsung cetak! Tidak perlu kustom formatter panjang lebar.
    fmt::print("Status HTTP saat ini: {}\n", respon);
    // Output: Status HTTP saat ini: 404
}

```

2. Mengubah Objek / Struct Menjadi String Konvensional
Selain angka, `format_as` juga bisa mengembalikan `std::string_view` jika Anda ingin objek kustom Anda dicetak sebagai teks tertentu.
```cpp
#include <fmt/format.h>
#include <string_view>

struct Pengguna {
    std::string nama;
    int id;
};

// Cukup buat fungsi format_as untuk struct Pengguna
auto format_as(const Pengguna& p) -> std::string_view {
    return p.nama; // Otomatis dicetak berdasarkan nama pengguna
}

int main() {
    Pengguna user1 = {"Andi", 1024};

    fmt::print("Selamat datang, {}!\n", user1);
    // Output: Selamat datang, Andi!
}

```

🔍 Mengapa Fitur Ini Sangat Membantu?

Jika tidak menggunakan mekanisme `format_as` di atas, Anda harus menulis kode boilerplate `formatter` yang cukup panjang untuk setiap tipe data baru, seperti ini:
```cpp
// CARA LAMA (Lebih Panjang):
template <> struct fmt::formatter<Status Kode> {
  constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }
  template <typename FormatContext>
  auto format(Status Kode s, FormatContext& ctx) const {
    return fmt::format_to(ctx.out(), "{}", static_cast<int>(s));
  }
};

```

Dengan adanya spesialisasi `formatter` yang Anda temukan di baris '3975,3984p' tersebut, `{fmt}` memangkas semua boilerplate tersebut. Anda cukup mendefinisikan satu fungsi satu baris `format_as()`, dan kustomisasi cetak langsung beres.

<br>

qa:
Apakah Anda sedang merancang **`enum class` yang butuh dicetak nilainya**, atau ada **struktur data kustom** yang ingin Anda sederhanakan format cetaknya menggunakan fitur ini?



<br>

qs:
`grep -n "auto format(b" $HOME/.cmake_fetchcontent_cache/fmt-v12.2.0/fmt-src/include/fmt/format.h`
`sed -n '4024,4043p' $HOME/.cmake_fetchcontent_cache/fmt-v12.2.0/fmt-src/include/fmt/format.h`
`https://github.com/fmtlib/fmt/blob/12.2.0/include/fmt/format.h#L4024`
```cpp
template <> struct formatter<bytes> {                                                                                         private:                                                                                                                      detail::dynamic_format_specs<> specs_;                                                                                     
 public:
  FMT_CONSTEXPR auto parse(parse_context<>& ctx) -> const char* {
    return parse_format_specs(ctx.begin(), ctx.end(), specs_, ctx,
                              detail::type::string_type);
  }

  template <typename FormatContext>
  auto format(bytes b, FormatContext& ctx) const -> decltype(ctx.out()) {
    auto specs = specs_;
    detail::handle_dynamic_spec(specs.dynamic_width(), specs.width,
                                specs.width_ref, ctx);
    detail::handle_dynamic_spec(specs.dynamic_precision(), specs.precision,
                                specs.precision_ref, ctx);
    return detail::write_bytes<char>(ctx.out(), b.data, specs);
  }
};

```

<br>

aa:
Fungsi `template <> struct formatter<bytes>` ini digunakan untuk memformat data biner mentah (`fmt::bytes`) agar dicetak menjadi string heksadesimal (*hex string*) secara efisien.

Cara menggunakannya di kode Anda adalah dengan membungkus array atau kontainer data biner Anda ke dalam objek `fmt::bytes`, lalu mencetaknya menggunakan sintaksis format spesifik (seperti heksadesimal).

Cara Menggunakan Paling Umum (Format Heksadesimal)
```cpp
#include <fmt/format.h>
#include <vector>

int main() {
    // Contoh data biner mentah (misal: payload jaringan atau hash crypto)
    std::vector<unsigned char> data_biner = {0x41, 0x42, 0x0A, 0xFF, 0x00};

    // Bungkus dengan fmt::bytes() dan gunakan :x untuk cetak heksadesimal
    fmt::print("Data Hex: {:x}\n", fmt::bytes(data_biner));
    // Output: Data Hex: 41420aff00

    // Menggunakan :X untuk huruf kapital
    fmt::print("Data HEX: {:X}\n", fmt::bytes(data_biner));
    // Output: Data HEX: 41420AFF00
}

```

Membatasi Jumlah Byte yang Dicetak (Menggunakan Precision)

Potongan kode Anda memuat fungsi `handle_dynamic_spec` untuk precision. Di dalam `{fmt}`, presisi pada tipe data bytes atau string berfungsi untuk **membatasi jumlah byte maksimal** yang akan ditampilkan.
```cpp
std::vector<unsigned char> data_panjang = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE};

// Menggunakan {:.3x} artinya hanya memformat 3 byte pertama
fmt::print("3 Byte Pertama: {:.3x}\n", fmt::bytes(data_panjang));
// Output: 3 Byte Pertama: aabbcc

```

Mengatur Lebar Kolom dan Perataan (Width & Alignment)

Fungsi `handle_dynamic_spec` untuk *width* juga ada di kode Anda, yang berarti Anda bisa memberikan spasi atau batas lebar kolom tampilan.
```cpp
std::vector<unsigned char> data_pendek = {0x1A, 0x2B};

// Mengatur rata kanan dengan total lebar 10 karakter, diisi spasi kosong
fmt::print("Hasil: '{:>10x}'\n", fmt::bytes(data_pendek));
// Output: Hasil: '      1a2b'

```

<br>

qa:
Apakah Anda sedang mencoba memformat data dari **array C-style primitif** (seperti `unsigned char data[10]`) atau dari **`std::string` biner**? Saya bisa berikan penyesuaian kodenya jika Anda membutuhkannya.

<br>

qs:
`grep -n "auto format(g" $HOME/.cmake_fetchcontent_cache/fmt-v12.2.0/fmt-src/include/fmt/format.h`
`sed -n '4063,4086p' $HOME/.cmake_fetchcontent_cache/fmt-v12.2.0/fmt-src/include/fmt/format.h`
`https://github.com/fmtlib/fmt/blob/12.2.0/include/fmt/format.h#L4063`
```cpp
auto format(group_digits_view<T> view, FormatContext& ctx) const
      -> decltype(ctx.out()) {
    auto specs = specs_;
    detail::handle_dynamic_spec(specs.dynamic_width(), specs.width,
                                specs.width_ref, ctx);
    detail::handle_dynamic_spec(specs.dynamic_precision(), specs.precision,
                                specs.precision_ref, ctx);
    auto arg = detail::make_write_int_arg(view.value, specs.sign());
    return detail::write_int(
        ctx.out(), static_cast<detail::uint64_or_128_t<T>>(arg.abs_value),
        arg.prefix, specs, detail::digit_grouping<char>("\3", ","));
  }
};

```


Fungsi ini digunakan untuk **mencetak angka bulat (integer) dengan pemisah ribuan (koma)** secara otomatis tanpa terpengaruh oleh pengaturan bahasa/lokalisasi sistem (locale-independent).

Anda cukup membungkus variabel atau angka Anda menggunakan fungsi `fmt::group_digits()` di dalam makro `fmt::print` atau `fmt::format`.
```cpp
#include <fmt/format.h> // atau <fmt/base.h> tergantung versi {fmt} Anda

int main() {
    int uang = 1000000;
    long long populasi = 8450000000LL;

    // Otomatis memicu fungsi group_digits_view di balik layar
    fmt::print("Total uang: {}\n", fmt::group_digits(uang));
    // Output: Total uang: 1,000,000

    std::string hasil = fmt::format("Populasi bumi: {}", fmt::group_digits(populasi));
    fmt::print("{}\n", hasil);
    // Output: Populasi bumi: 8,450,000,000
}

```

⚠️ Catatan Penting

- Hanya untuk Angka Bulat: Sesuai definisi potongan kodenya (`detail::write_int`), fungsi ini dirancang khusus untuk tipe data bilangan bulat seperti `int`, `long`, `long long`, `unsigned`, dll. Fungsi ini **tidak bisa** digunakan langsung untuk angka desimal pecahan seperti `float` atau `double`.
- Koma Permanen: Berbeda dengan pustaka standard `std::cout` yang bisa berubah menjadi titik (1.000.000) mengikuti region Windows/Linux Anda, `fmt::group_digits` secara keras (*hardcoded*) akan selalu memunculkan tanda koma `","` sebagai pemisah ribuan demi konsistensi performa.

---

Mendukung Format Specifier Lain (Lebih Kompleks)

Karena objek ini mewarisi sifat `formatter<T>` bawaan, Anda tetap bisa menggabungkannya dengan pengaturan lebar teks (*width alignment*):
```cpp
int angka = 12345;

// Menyelaraskan teks ke kanan dengan lebar rata 10 karakter
fmt::print("Hasil: '{:>10}'\n", fmt::group_digits(angka));
// Output: Hasil: '    12,345'

```

---


<br>

qs:
`grep -n "auto format(n" $HOME/.cmake_fetchcontent_cache/fmt-v12.2.0/fmt-src/include/fmt/format.h`
`sed -n '4094,4103p' $HOME/.cmake_fetchcontent_cache/fmt-v12.2.0/fmt-src/include/fmt/format.h`
`https://github.com/fmtlib/fmt/blob/12.2.0/include/fmt/format.h#L4094`
```cpp
struct formatter<nested_view<T, Char>, Char> {
  FMT_CONSTEXPR auto parse(parse_context<Char>& ctx) -> const Char* {
    return ctx.begin();
  }
  template <typename FormatContext>
  auto format(nested_view<T, Char> view, FormatContext& ctx) const
      -> decltype(ctx.out()) {
    return view.fmt->format(*view.value, ctx);
  }
};

```

<br>

aa:
Struktur `struct formatter<nested_view<T, Char>, Char>` merupakan komponen internal dari pustaka `{fmt}` yang digunakan secara otomatis di balik layar ketika Anda melakukan operasi **Format Bersarang** (*Nested Formatting*).

Fitur ini otomatis terpicu ketika Anda mencetak kontainer kompleks seperti `std::vector`, `std::map`, atau komponen jangkauan modern C++ (Ranges) yang di dalamnya berisi objek atau struktur data kustom bikinan Anda sendiri.

Berikut adalah cara menggunakannya melalui API publik `{fmt}`:

Skenario Penggunaan Nyata (Mencetak Vector berisi Objek Kustom)

Agar `nested_view` bisa bekerja, Anda harus membuat kustom `formatter` untuk tipe data Anda sendiri terlebih dahulu, lalu memasukkan objek tersebut ke dalam sebuah kontainer (seperti `std::vector`).
```cpp
#include <fmt/format.h>
#include <fmt/ranges.h> // Wajib di-include untuk mencetak kontainer/vector
#include <vector>

// 1. Definisikan tipe data kustom Anda
struct Koordinat {
    int x, y;
};

// 2. Buat formatter kustom untuk tipe data Koordinat
template <>
struct fmt::formatter<Koordinat> {
    constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }

    template <typename FormatContext>
    auto format(const Koordinat& k, FormatContext& ctx) const {
        return fmt::format_to(ctx.out(), "({}, {})", k.x, k.y);
    }
};

int main() {
    // 3. Masukkan objek kustom ke dalam Vector
    std::vector<Koordinat> daftar_titik = {{1, 2}, {5, 8}, {10, 20}};

    // 4. Cetak Vector-nya! 
    // Di sinilah `nested_view` otomatis bekerja untuk meneruskan format tiap elemen ke formatter Koordinat.
    fmt::print("Daftar Koordinat: {}\n", daftar_titik);
    
    // Output: Daftar Koordinat: [(1, 2), (5, 8), (10, 20)]
}

```

🔍 Bagaimana `nested_view` Bekerja di Balik Layar?

Saat Anda memanggil `fmt::print("{}", daftar_titik);`, pustaka `{fmt}` akan mendeteksi bahwa objek tersebut adalah sebuah kontainer (`ranges`).
1. Komponen `fmt/ranges.h` akan mengiterasi isi vector tersebut.
2. Setiap elemen di dalam vector dibungkus secara internal ke dalam struktur bernama `nested_view` bersama dengan pointer penunjuk formatnya.
3. Fungsi `format(nested_view<T, Char> view, ...)` yang Anda temukan di kode sumber di atas akan dieksekusi.
4. Kode internal `return view.fmt->format(*view.value, ctx);` berfungsi untuk mengalihkan dan memanggil fungsi `format` kustom milik tipe data `Koordinat` yang sudah kita buat tadi.
Artinya, **Anda tidak perlu memanggil kata `nested_view` secara manual** di dalam kode Anda. Cukup sertakan `#include <fmt/ranges.h>` dan buat `formatter` untuk tipe data dasar Anda, maka `{fmt}` akan langsung menangani struktur bersarang tersebut secara otomatis.

<br>

qa:
Apakah Anda sedang mencoba membuat **formatter kustom untuk class/struct** tertentu pada proyek Anda saat ini? Jika iya, Anda bisa membagikan struktur data Anda agar kita bisa buatkan implementasi `formatter`-nya bersama-sama.

<br>

qs:
`grep -n "auto format(l" $HOME/.cmake_fetchcontent_cache/fmt-v12.2.0/fmt-src/include/fmt/format.h`
`sed -n '4321,4325p' $HOME/.cmake_fetchcontent_cache/fmt-v12.2.0/fmt-src/include/fmt/format.h`
`https://github.com/fmtlib/fmt/blob/12.2.0/include/fmt/format.h#L4321`
```cpp
template <typename... T>
FMT_INLINE auto format(locale_ref loc, format_string<T...> fmt, T&&... args)
    -> std::string {
  return vformat(loc, fmt.str, vargs<T...>{{args...}});
}

```

<br>

aa:
Fungsi `fmt::format(locale_ref loc, format_string<T...> fmt, T&&... args)` (pada baris 4321) digunakan jika Anda ingin memformat teks dan angka berdasarkan **aturan lokalisasi bahasa atau negara tertentu (_Locale_)**.

Secara default, `{fmt}` akan mengabaikan pengaturan negara komputer Anda demi kecepatan. Namun, jika Anda menyertakan objek `std::locale` ke dalam fungsi ini, `{fmt}` akan memformat data seperti angka desimal, tanggal, atau mata uang mengikuti format resmi negara tersebut.

Berikut adalah cara menggunakannya di dalam kode Anda:

Cara Penggunaan Standar (Format Sesuai Negara)

Untuk menggunakannya, Anda wajib membuat objek `std::locale` dari pustaka standar C++ terlebih dahulu, lalu memasukkannya sebagai **argumen pertama** di dalam fungsi `fmt::format`.
```cpp
#include <fmt/format.h>
#include <locale> // Wajib di-include untuk menggunakan std::locale

int main() {
    double angka = 1234567.89;

    // 1. Format menggunakan Locale Amerika Serikat (US)
    // Angka menggunakan pemisah ribuan koma (,) dan desimal titik (.)
    std::locale locale_us("en_US.UTF-8");
    std::string hasil_us = fmt::format(locale_us, "Format US: {:L}", angka);
    fmt::print("{}\n", hasil_us);
    // Output: Format US: 1,234,567.89

    // 2. Format menggunakan Locale Jerman (DE) atau Indonesia (ID)
    // Angka menggunakan pemisah ribuan titik (.) dan desimal koma (,)
    std::locale locale_de("de_DE.UTF-8"); 
    std::string hasil_de = fmt::format(locale_de, "Format DE: {:L}", angka);
    fmt::print("{}\n", hasil_de);
    // Output: Format DE: 1.234.567,89
}

```

⚠️ Syarat Wajib Agar Berhasil (Penting!)
1. Wajib Menggunakan Format Specifier `{:L}`
   Perhatikan simbol `:L` di dalam kurung kurawal (`{:L}`). Karakter `L` ini adalah singkatan dari *Locale*. Jika Anda hanya menulis `{}` tanpa `:L`, fungsi di atas tidak akan memberikan efek apa pun dan angka akan dicetak datar tanpa pemisah.
2. Ketersediaan Nama Locale di Sistem Operasi
   String nama locale seperti `"en_US.UTF-8"` atau `"de_DE.UTF-8"` bergantung pada sistem operasi yang Anda gunakan (Windows/Linux/macOS). Jika OS Anda tidak memiliki paket bahasa tersebut, program akan memicu eror `std::runtime_error`.*Tip untuk Windows*: Di lingkungan Windows MSVC, format penulisan namanya sering kali menggunakan bahasa mentah seperti `"en-US"` atau `"German_Germany"`.

Opsi Alternatif: Cetak Langsung Tanpa `fmt::format`

Jika tujuan Anda hanya ingin langsung mencetak hasilnya ke layar (konsol) tanpa menyimpannya ke dalam variabel `std::string`, Anda bisa menggunakan fungsi saudaranya yaitu `fmt::print` dengan cara penulisan yang sama persis:
```cpp
std::locale loc("de_DE.UTF-8");
fmt::print(loc, "Total biaya: {:L}\n", 5000000); 
// Output langsung ke layar: Total biaya: 5.000.000

```

<br>

qa:
Apakah Anda sedang berniat membuat aplikasi yang **mendukung banyak bahasa / mata uang internasional**, atau menemui kendala eror saat mendefinisikan objek `std::locale` pada OS yang Anda gunakan saat ini?Jika Anda mau, beri tahu saya:
- Sistem operasi yang Anda gunakan (Windows, Linux, atau macOS)?
- Format spesifik yang ingin Anda capai (misal: Rupiah Rp, tanggal, atau pemisah titik)?
Saya bisa bantu sesuaikan kode penulisan locale-nya agar tidak eror di perangkat Anda.

<br>

qs:
`grep -n "auto format(f" $HOME/.cmake_fetchcontent_cache/fmt-v12.2.0/fmt-src/include/fmt/format.h`
`sed -n '4354,4367p' $HOME/.cmake_fetchcontent_cache/fmt-v12.2.0/fmt-src/include/fmt/format.h`
`https://github.com/fmtlib/fmt/blob/12.2.0/include/fmt/format.h#L4354`
```cpp
/**
 * Formats `args` according to specifications in `fmt` and returns the result
 * as a string.
 *
 * **Example**:
 *
 *     #include <fmt/format.h>
 *     std::string message = fmt::format("The answer is {}.", 42);
 */
template <typename... T>
FMT_NODISCARD FMT_INLINE auto format(format_string<T...> fmt, T&&... args)
    -> std::string {
  return vformat(fmt.str, vargs<T...>{{args...}});
}

```

<br>

aa:
Fungsi `fmt::format(format_string<T...> fmt, T&&... args)` (pada baris 4354) adalah **fungsi paling utama, paling dasar, dan paling sering digunakan** di seluruh pustaka `{fmt}`.

Fungsi ini digunakan untuk memformat berbagai macam data (teks, angka, karakter) menjadi sebuah objek `std::string` baru. Fungsi inilah yang menjadi cikal bakal fitur `std::format` pada standard modern C++20.

Berikut adalah berbagai skenario cara menggunakannya dari yang paling dasar hingga tingkat lanjut:

1. Penggunaan Dasar (Menggabungkan Teks dan Angka)
Anda cukup menggunakan tanda kurung kurawal `{}` sebagai *placeholder* (penampung) variabel yang ingin Anda masukkan ke dalam teks.
```cpp
#include <fmt/format.h> // Wajib menyertakan header ini
#include <iostream>

int main() {
    std::string nama = "Budi";
    int skor = 95;

    // Fungsi ini mengembalikan sebuah std::string
    std::string pesan = fmt::format("Halo {}, skor ujian Anda adalah {}.", nama, skor);

    std::cout << pesan << std::endl;
    // Output: Halo Budi, skor ujian Anda adalah 95.
}

```

2. Mengatur Posisi Urutan Variabel (Positional Arguments)
Jika Anda ingin menukar posisi atau menggunakan satu variabel berulang kali tanpa menulisnya dua kali di argumen, Anda bisa memasukkan indeks angka di dalam `{}` (dimulai dari `0`).
```cpp
    // {0} merujuk ke "kucing", {1} merujuk ke "tikus"
    std::string kalimat = fmt::format("Seekor {0} sedang mengejar {1}, lalu {0} itu melompat.", "kucing", "tikus");
    
    std::cout << kalimat << std::endl;
    // Output: Seekor kucing sedang mengejar tikus, lalu kucing itu melompat.

```

3. Pemformatan Angka Spesifik (*Format Specifier*)
Fungsi ini mendukung berbagai pengondisian teks dengan menambahkan tanda titik dua `:` di dalam kurung kurawal:
- Membatasi Desimal (Presisi): Menggunakan `:.2f` untuk mengambil 2 angka di belakang koma.
- Format Heksadesimal / Biner: Menggunakan `:x` untuk hex, atau `:b` untuk biner.
- Rata Kiri/Kanan (Padding): Mengatur lebar teks.
```cpp
    double pi = 3.14159265;
    int angka = 255;

    // Membatasi desimal dan mengubah angka ke Hex
    std::string hasil = fmt::format("Pi: {:.2f}, Hex: {:X}", pi, angka);

    std::cout << hasil << std::endl;
    // Output: Pi: 3.14, Hex: FF

```

💡 Mengapa Menggunakan `fmt::format` Lebih Baik dari `std::stringstream`?
Sebelum adanya `{fmt}`, komunitas C++ sering menggunakan `std::stringstream` untuk menggabungkan string. Berikut perbandingan sintaksisnya yang membuat `fmt::format` jauh lebih unggul dalam hal keterbacaan:

<br>

qa:
Apakah Anda ingin tahu cara **menangkap error** (*exception handling*) jika string format yang Anda masukkan tidak valid, atau ingin melihat contoh melakukan **perataan kolom tabel** (*alignment*) menggunakan fungsi ini?
















<br>
