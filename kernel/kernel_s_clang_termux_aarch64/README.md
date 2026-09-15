


```assembly
.global _start
.text

_start:
    // ====================================================
    // LANGKAH 1: Ambil alamat Program Break (Awal Heap) saat ini
    // ====================================================
    mov x0, #0          // Argumen 0: Meminta alamat saat ini
    mov x8, #214        // ID Syscall 214 = brk (ARM64)
    svc #0              // Panggil Kernel!
    
    // Hasil: Register x0 sekarang berisi alamat awal Heap (misal: 0x555556a000)
    // Mari simpan alamat awal ini ke register x19 untuk cadangan
    mov x19, x0         

    // ====================================================
    // LANGKAH 2: Hitung alamat baru (Alamat Awal + 1000 byte)
    // ====================================================
    mov x1, #1000       // Kita ingin memesan memori sebesar 1000 byte
    add x0, x19, x1     // x0 = Alamat Awal + 1000 byte (Batas baru yang diminta)

    // ====================================================
    // LANGKAH 3: Minta Kernel menggeser batas Heap ke alamat baru
    // ====================================================
    mov x8, #214        // ID Syscall 214 = brk
    svc #0              // Panggil Kernel!

    // Hasil: Jika sukses, x0 akan berisi alamat batas baru tersebut.
    // Sekarang Anda memiliki 1000 byte memori yang valid di alamat [x19] sampai [x19 + 1000]!

    // ====================================================
    // LANGKAH 4: Contoh Penggunaan Memori Heap Baru
    // ====================================================
    mov w2, #65         // Angka 65 = Karakter 'A' ASCII
    strb w2, [x19]      // Simpan karakter 'A' langsung ke dalam memori heap baru kita!

    // ====================================================
    // LANGKAH 5: Keluar dari program secara bersih (sys_exit)
    // ====================================================
    mov x0, #0          // Status keluar: 0 (Sukses)
    mov x8, #93         // ID Syscall 93 = exit (ARM64)
    svc #0              // Panggil Kernel untuk menutup program

```

<br>
