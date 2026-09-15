#include <stdio.h>
#include <stdlib.h>

void fungsi_uji_calloc() {
    printf("2. Memicu alokasi heap manual via calloc...\n");
    
    // Menggunakan calloc: 32 elemen, masing-masing berukuran 4 byte (sizeof(int))
    // Total memori: 128 byte, otomatis di-set ke angka 0
    int *ptr = (int *)calloc(32, sizeof(int)); 
    
    if (ptr != NULL) {
        printf(" -> Memori calloc telah dipesan (Alamat awal heap: %p).\n", (void*)ptr);
        printf(" -> Nilai elemen pertama (pasti 0): %d\n", ptr[0]);
        
        // Bebaskan memori heap
        free(ptr);
        
        // === PAKSA CLANG ASAN UNTUK MEMBUKTIKAN HEAP CALLOC ===
        printf("\n--- Memicu Pengecekan Heap ASan via Use-After-Free (calloc) ---\n");
        
        // Sengaja membaca array int yang sudah di-free agar ASan mendeteksi pelanggaran
        int bukti = ptr[0]; 
        printf("Hasil baca (tidak akan tercetak): %d\n", bukti);
    }
}

int main() {
    printf("1. Menguji teks biasa (Stack)...\n");
    fungsi_uji_calloc();
    return 0;
}
