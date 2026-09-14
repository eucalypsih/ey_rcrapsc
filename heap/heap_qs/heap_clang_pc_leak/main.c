#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void fungsi_heap_bocor() {
    // Alokasi memori ke heap, sengaja tidak di-free di akhir fungsi
    char *buffer = (char *)malloc(500 * sizeof(char));
    if (buffer != NULL) {
        strcpy(buffer, "Menguji alokasi heap dengan Clang ASan");
        printf("%s\n", buffer);
    }
}

int main() {
    printf("--- Memulai Pengujian Heap via Clang 21 ---\n");
    fungsi_heap_bocor();
    return 0;
}
