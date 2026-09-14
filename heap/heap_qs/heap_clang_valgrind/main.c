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
