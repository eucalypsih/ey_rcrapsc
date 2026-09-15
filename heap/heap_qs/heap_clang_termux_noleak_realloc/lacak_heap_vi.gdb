# 1. Matikan batasan teks layar agar log keluar penuh
set width 0
set height 0

# 2. UBAH DI SINI: Gunakan nomor syscall 214 (indeks 'brk' untuk ARM64 Android)
catch syscall 214
commands
    silent
    printf "\n====================================================\n"
    printf "[PERINGATAN OS] Kernel memicu syscall brk (214)!\n"
    printf "Ini adalah awal mula ruang HEAP diciptakan/diperluas oleh OS.\n"
    printf "====================================================\n"
    continue
end

# 3. Mencegat alokasi fungsi malloc kustom/sistem
break malloc
commands 1
    silent
    # Di ARM64 (Termux), ukuran bita yang diminta malloc disimpan di register $x0
    printf "\n[MALLOC TERCEGAT] Program meminta alokasi heap sebesar: %d bita\n", $x0
    printf "[LOKASI KODE] Pemicu alokasi ini berasal dari:\n"
    where 2
    printf "----------------------------------------------------\n"
    continue
end

# 4. Jalankan program secara otomatis
run
