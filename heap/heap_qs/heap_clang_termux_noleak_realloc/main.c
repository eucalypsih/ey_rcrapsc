#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void fungsi_stack() {
    char buffer[500]; // Dialokasikan di STACK (Zero Heap)
    strcpy(buffer, "Halo dari Stack!");
    printf("%s\n", buffer);
}

void fungsi_heap_realloc() {
    // 1. Alokasi memori awal di HEAP berukuran sangat kecil (hanya 5 byte)
    char *buffer = (char *)malloc(5 * sizeof(char));
    if (buffer == NULL) return;

    // Isi dengan teks pendek (4 karakter + 1 null terminator = 5 byte)
    strcpy(buffer, "Awal");
    printf("%s (Ukuran Awal: 5 byte)\n", buffer);

    // 2. === DIUBAH MENJADI VERSI REALLOC ===
    // Memperbesar blok memori heap yang sama dari 5 byte menjadi 500 byte
    // realloc akan menjaga data "Awal" tetap aman di memori baru
    char *new_buffer = (char *)realloc(buffer, 500 * sizeof(char));
    
    if (new_buffer != NULL) {
        // Amankan penunjuk pointer baru
        buffer = new_buffer; 

        // Tambahkan teks baru ke dalam memori yang sudah diperbesar
        strcat(buffer, " digabung dengan teks setelah Realloc hingga 500 byte!");
        printf("%s\n", buffer);
        
        // 3. Bebaskan memori biner hasil realloc
        free(buffer); 
    } else {
        // Jika realloc gagal, memori lama (buffer) harus dibebaskan manual
        free(buffer);
    }
}

int main() {
    printf("--- Memulai Pengujian (Versi Realloc) ---\n");
    fungsi_stack();
    fungsi_heap_realloc(); 
    return 0;
}
