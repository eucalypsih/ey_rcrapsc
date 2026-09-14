#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Fungsi pembungkus (wrapper) untuk malloc
void* __wrap_malloc(size_t size) {
    extern void* __real_malloc(size_t);

    // BUKTI PENTING: Gunakan fflush agar teks langsung keluar ke layar tanpa tertahan buffer
    fprintf(stderr, "[BUKTI] Heap digunakan! Meminta alokasi: %zu byte\n", size);
    fflush(stderr);

    return __real_malloc(size);
}

int main() {
    printf("1. Menguji teks biasa (Stack)...\n");

    printf("2. Memicu alokasi heap manual...\n");
    
    // Trik: Alokasikan dan gunakan datanya agar tidak dihapus oleh optimasi Clang
    char *ptr = (char *)malloc(128); 
    if (ptr != NULL) {
        strcpy(ptr, "Mengisi data ke heap agar tidak di-optimize out oleh Clang 21");
        printf(" -> Data isi heap: %s\n", ptr);
        free(ptr); 
    }

    return 0;
}
