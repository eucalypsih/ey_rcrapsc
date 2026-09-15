# 1. Jangan berhenti saat memproses sinyal internal
set width 0
set height 0

# 2. Pasang breakpoint pada sistem alokasi kernel terbawah (awal penciptaan heap)
# Di Linux/Android, heap dimulai ketika OS memanggil brk() untuk memindahkan batas memori.
catch syscall brk
commands
    silent
    printf "\n====================================================\n"
    printf "[PERINGATAN OS] Kernel memicu syscall 'brk'!\n"
    printf "Ini adalah awal mula ruang HEAP diciptakan/diperluas oleh OS.\n"
    printf "====================================================\n"
    continue
end

# 3. Pasang breakpoint pada fungsi malloc pertama
break malloc
commands 1
    silent
    # Cetak argumen pertama malloc (ukuran bita yang diminta)
    # Pada arsitektur ARM64 (Termux), argumen pertama ada di register $x0.
    # Jika di PC/Linux x86_64, ganti $x0 menjadi $rdi.
    printf "\n[MALLOC TERCEGAT] Program meminta alokasi heap sebesar: %d bita\n", $x0
    
    # Cetak backtrace untuk melihat fungsi apa di kode kita yang memicu heap pertama kali
    printf "[LOKASI KODE] Pemicu alokasi ini berasal dari:\n"
    where 2
    
    printf "----------------------------------------------------\n"
    continue
end

# 4. Jalankan program secara otomatis begitu GDB dimulai
run
