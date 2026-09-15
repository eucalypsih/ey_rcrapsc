#include <stdio.h>
#include <stdlib.h>

void fungsi_uji_heap() {
    printf("2. Memicu alokasi heap manual...\n");
    
    // Alokasi memori ke heap
    char *ptr = (char *)malloc(128); 
    if (ptr != NULL) {
        ptr[0] = 'A'; // Mengisi data ke heap
        printf(" -> Memori 128 byte telah dipesan di heap (Alamat: %p).\n", (void*)ptr);
        
        // Bebaskan memori
        free(ptr);
        
        // === TRICK UNTUK MEMAKSA CLANG ASAN MENCETAK DATA HEAP ===
        printf("\n--- Memicu Pengecekan Heap ASan via Use-After-Free ---\n");
        
        // Sengaja membaca memori yang SUDAH di-free agar ASan langsung mengamuk!
        char bukti = ptr[0]; 
        printf("Hasil baca (tidak akan tercetak karena crash): %c\n", bukti);
    }
}

int main() {
    printf("1. Menguji teks biasa (Stack)...\n");
    fungsi_uji_heap();
    return 0;
}
