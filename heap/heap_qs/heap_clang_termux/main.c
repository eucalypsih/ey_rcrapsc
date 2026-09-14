#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// === TAMBAHKAN FUNGSI PEMBUNGKUS (WRAPPER) INI ===
void* __wrap_malloc(size_t size) {
    // Memanggil fungsi malloc yang asli bawaan sistem
    extern void* __real_malloc(size_t);
    
    // Tulis bukti ke stderr agar tidak mengganggu stdout utama
    fprintf(stderr, "\n[BUKTI HEAP] malloc dipanggil! Alokasi memori sebesar: %zu byte\n", size);
    
    return __real_malloc(size);
}
// ================================================

void fungsi_stack() {
    char buffer[500]; // Dialokasikan di STACK (Zero Heap)
    strcpy(buffer, "Halo dari Stack!");
    printf("%s\n", buffer);
}

void fungsi_heap() {
    // Dialokasikan di HEAP
    char *buffer = (char *)malloc(500 * sizeof(char)); // Ini akan memicu __wrap_malloc
    if (buffer != NULL) {
        strcpy(buffer, "Halo dari Heap!");
        printf("%s\n", buffer);
        free(buffer); 
    }
}

int main() {
    printf("--- Memulai Pengujian ---\n");
    fungsi_stack();
    fungsi_heap();
    return 0;
}
