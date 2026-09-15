#!/file/bin/bash

# 1. Definisikan basis folder proyek Anda
r="ey_rcrapsc"
rp="/data/data/com.termux/files/home/${r}"
sd="/kernel/kernel_s_clang_termux_aarch64"
TARGET_DIR="${rp}${sd}"

# 2. Dapatkan nama file argumen (default ke alokasi_heap.S jika kosong)
FILE_NAME="${1:-main.S}"
BASE_NAME=$(basename "$FILE_NAME" .S)

echo "=== Memulai Otomasi Kompilasi AArch64 ==="
echo "Direktori Kerja Saat Ini (PWD): $PWD"

# 3. Masuk ke direktori target eksekusi internal
cd "$TARGET_DIR" || { echo "Gagal masuk ke direktori proyek!"; exit 1; }

# 4. Proses Kompilasi Assembly Murni (Freestanding)
echo "Mengompilasi ${FILE_NAME}..."
clang -nostdlib -static "$FILE_NAME" -o "$BASE_NAME"

if [ $? -eq 0 ]; then
    echo "✅ Kompilasi Sukses! Output biner: ${TARGET_DIR}/${BASE_NAME}"
    echo "----------------------------------------"
    echo "Menjalankan program..."
    
    # 5. Eksekusi program murni hasil compile
    ./"$BASE_NAME"
    echo "Program selesai dieksekusi dengan kode keluar: $?"
else
    echo "🛑 Kompilasi Gagal!"
    exit 1
fi
