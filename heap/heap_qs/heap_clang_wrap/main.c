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
