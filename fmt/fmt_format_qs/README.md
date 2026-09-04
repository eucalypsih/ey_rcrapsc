# fmt_format_qs


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
