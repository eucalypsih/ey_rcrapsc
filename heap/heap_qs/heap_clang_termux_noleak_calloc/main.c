#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void fungsi_stack() {
    char buffer[500]; // Dialokasikan di STACK (Zero Heap)
    strcpy(buffer, "Halo dari Stack!");
    printf("%s\n", buffer);
}

void fungsi_heap_calloc() {
    // === DIUBAH MENJADI VERSI CALLOC ===
    // Argumen 1: Jumlah elemen (500)
    // Argumen 2: Ukuran per elemen (sizeof(char))
    // Seluruh isi buffer otomatis di-set ke 0 (bersih dari data sampah)
    char *buffer = (char *)calloc(500, sizeof(char)); 
    
    if (buffer != NULL) {
        strcpy(buffer, "Halo dari Heap (via Calloc)!");
        printf("%s\n", buffer);
        
        free(buffer); // Tetap wajib didealokasikan dari heap
    }
}

int main() {
    printf("--- Memulai Pengujian (Versi Calloc) ---\n");
    fungsi_stack();
    fungsi_heap_calloc(); 
    return 0;
}
