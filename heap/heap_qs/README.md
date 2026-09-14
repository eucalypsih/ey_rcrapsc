# 

Untuk membuktikan apakah suatu program C melakukan alokasi memori di heap atau tidak, kita bisa menggunakan beberapa metode. Di lingkungan Linux atau Termux, ada tiga cara paling efektif: **menggunakan perangkat pelacak (Valgrind), melakukan intersepsi fungsi (`__wrap_malloc`), atau dengan memantau statistik sistem (`malloc_info`)**.

Berikut adalah cara mempraktikkan ketiga metode tersebut untuk membuktikan penggunaan heap:

---

## Metode 1: Menggunakan Valgrind (Paling Sederhana & Akurat)
**Valgrind** adalah perkakas analisis memori yang dapat menghitung secara presisi berapa kali program Anda memanggil alokasi *heap* (`malloc`, `calloc`, `realloc`) dan berapa total bita yang dialokasikan.

### 1. Buat Kode Pengujian (`uji_heap.c`)
Kita akan membuat program yang sengaja mengalokasikan memori di *heap*, dan satu lagi yang hanya menggunakan *stack*.
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void fungsi_stack() {
    char buffer[500]; // Dialokasikan di STACK (Zero Heap)
    strcpy(buffer, "Halo dari Stack!");
    printf("%s\n", buffer);
}

void fungsi_heap() {
    // Dialokasikan di HEAP
    char *buffer = (char *)malloc(500 * sizeof(char)); 
    if (buffer != NULL) {
        strcpy(buffer, "Halo dari Heap!");
        printf("%s\n", buffer);
        free(buffer); // Jangan lupa didealokasikan
    }
}

int main() {
    printf("--- Memulai Pengujian ---\n");
    fungsi_stack();
    fungsi_heap(); 
    return 0;
}

```

### 2. Kompilasi dan Jalankan dengan Valgrind
Instal dan jalankan biner Anda di bawah pengawasan Valgrind:
```bash
# Instal valgrind (Jika di Ubuntu/Linux PC)
sudo apt install valgrind -y

# Kompilasi dengan simbol debugging (-g)
gcc -g uji_heap.c -o uji_heap

# Jalankan analisis memori
valgrind --tool=memcheck --leak-check=summary ./uji_heap

```

### 3. Cara Membaca Buktinya
Perhatikan bagian **HEAP SUMMARY** pada laporan keluaran Valgrind:
```text
==12345== HEAP SUMMARY:
==12345==     in use at exit: 0 bytes in 0 blocks
==12345==   total heap usage: 2 allocs, 2 frees, 1,524 bytes allocated

```
- Bukti: Tulisan `total heap usage: 2 allocs` membuktikan bahwa terjadi 2 kali alokasi di *heap* (1 dari fungsi *malloc* kita, dan 1 lagi biasanya dilakukan secara internal oleh `printf`/`stdout` untuk menyiapkan buffer teks).

---

## Metode 2: Teknik Intersepsi Linker (`__wrap_malloc`)
Jika Anda berada di **Termux** (di mana Valgrind terkadang tidak stabil pada arsitektur ARM64 tertentu), Anda bisa membuktikannya secara langsung di dalam kode menggunakan fitur perintah *linker* GCC/Clang bernama `--wrap`. Teknik ini akan memotong setiap panggilan `malloc` dan mengarahkannya ke fungsi buatan kita sendiri.

### 1. Buat Kode Intersepsi (`cek_malloc.c`)
```c
#include <stdio.h>
#include <stdlib.h>

// Fungsi pembungkus (wrapper) untuk malloc
void* __wrap_malloc(size_t size) {
    // Memanggil malloc asli menggunakan fungsi real bawaan
    extern void* __real_malloc(size_t);
    
    // Tulis bukti ke stderr bahwa heap disentuh!
    fprintf(stderr, "[BUKTI] Heap digunakan! Meminta alokasi: %zu byte\n", size);
    
    return __real_malloc(size);
}

int main() {
    printf("1. Menguji teks biasa (Stack)...\n");
    
    printf("2. Memicu alokasi heap manual...\n");
    void *ptr = malloc(128); // Ini akan memicu __wrap_malloc
    free(ptr);
    
    return 0;
}

```

### 2. Kompilasi dengan Bendera Wrap Linker
Anda harus memberi tahu kompilator untuk membungkus fungsi `malloc:`
```bash
gcc cek_malloc.c -o cek_malloc -Wl,--wrap=malloc
./cek_malloc

```

### 3. Hasil Tampilan Terminal:
```text
1. Menguji teks biasa (Stack)...
2. Memicu alokasi heap manual...
[BUKTI] Heap digunakan! Meminta alokasi: 128 byte

```
- Bukti: Jika kalimat **[BUKTI] Heap digunakan!** muncul, artinya baris kode tersebut terbukti menyentuh memori *heap*.

---

## Metode 3: Membaca Profil Glibc (`malloc_info`)
Bagi pengguna Linux standar (glibc), Anda bisa mencetak struktur XML dari internal allocator sistem ke layar untuk melihat kondisi *heap* saat program berjalan menggunakan fungsi `<malloc.h>`.
```c
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

int main() {
    // Cetak kondisi heap sebelum alokasi
    printf("=== Kondisi Heap Awal ===\n");
    malloc_info(0, stdout);

    void* p = malloc(2000); // Alokasi di heap

    // Cetak kondisi heap sesudah alokasi
    printf("\n=== Kondisi Heap Setelah Malloc ===\n");
    malloc_info(0, stdout);

    free(p);
    return 0;
}

```
Saat dijalankan, Anda akan melihat log XML yang menunjukkan ukuran `block` memori yang aktif berpindah dari status kosong menjadi terisi di dalam arena *heap*.

<br>

 ---

 <br>

 aa: 
 Karena Anda menggunakan `make` sebagai *build system* dan **Clang 21** sebagai kompilatornya, **cara terbaik dan paling bersih untuk membuktikan alokasi memori di *heap* adalah dengan memanfaatkan Sanitizers bawaan Clang (khususnya AddressSanitizer) atau Teknik Linker Wrapper (`-Wl,--wrap`) yang disematkan langsung ke dalam** `Makefile`.

Berikut adalah dua skenario penerapan yang bisa Anda pasang langsung di dalam proyek Anda:

---

## Pendekatan 1: Menggunakan Linker Wrapper di `Makefile` (Tanpa Pustaka Eksternal)
Teknik ini memotong panggilan fungsi `malloc` di tingkat linker Clang. Pendekatan ini sangat ringan dan bekerja 100% stabil di Termux maupun Linux PC.

### 1. File Kode Sumber (`main.c`)
Tambahkan fungsi pembungkus (*wrapper*) ini di dalam salah satu file kode Anda (atau buat file `pelacak.c` terpisah):
```c
#include <stdio.h>
#include <stdlib.h>

// Fungsi pembungkus malloc milik Clang/GCC Linker
void* __wrap_malloc(size_t size) {
    extern void* __real_malloc(size_t);
    
    // Cetak bukti ke stderr agar tidak mengganggu stdout program utama
    fprintf(stderr, " -> [BUKTI HEAP] malloc memicu alokasi sebesar: %zu bytes\n", size);
    
    return __real_malloc(size);
}

```

### 2. Konfigurasi `Makefile`
Tambahkan bendera `-Wl,--wrap=malloc` pada bagian `LDFLAGS` di `Makefile` Anda untuk memberi tahu Clang agar mengalihkan semua fungsi `malloc` ke fungsi pembungkus kita.
```makefile
CC = clang
CFLAGS = -Wall -Wextra -O2
# Beritahu linker untuk membungkus fungsi malloc
LDFLAGS = -Wl,--wrap=malloc

TARGET = program_uji
SRCS = main.c

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET)

```

---

## Pendekatan 2: Menggunakan Clang AddressSanitizer (ASan) & LeakSanitizer
Clang 21 memiliki fitur luar biasa bernama **AddressSanitizer** (`-fsanitize=address`). Fitur ini tidak hanya bisa menangkap *bug* memori (seperti *buffer overflow*), tetapi secara otomatis melacak seluruh alokasi memori di *heap* yang bocor (*leak*) atau belum dibebaskan saat program selesai.

Anda cukup memodifikasi `Makefile` Anda seperti ini tanpa perlu mengubah kode C sama sekali:
```makefile
CC = clang
# Tambahkan bendera sanitize di CFLAGS dan LDFLAGS
CFLAGS = -Wall -Wextra -g -fsanitize=address
LDFLAGS = -fsanitize=address

TARGET = program_asan
SRCS = main.c

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET)

```

### Cara Membaca Buktinya:
Jika ada bagian kode Anda yang mengalokasikan memori ke *heap* menggunakan `malloc` dan sengaja **tidak Anda `free()` di akhir fungsi**, saat program dijalankan dan keluar, Clang ASan akan langsung memuntahkan laporan detail seperti ini ke terminal Anda:
```text
=================================================================
==12345==ERROR: LeakSanitizer: detected memory leaks

Direct leak of 500 byte(s) in 1 object(s) allocated from:
    #0 0x4c274d in malloc
    #1 0x4f9321 in fungsi_format_anda /home/user/project/main.c:12
    #2 0x4f9502 in main /home/user/project/main.c:25

```
Laporan di atas adalah bukti konkret dari Clang yang menunjukkan baris ke berapa di kode Anda (`main.c:12`) yang menyentuh memori heap.








<br>

