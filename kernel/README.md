


##  Aturan Baku Kernel (Application Binary Interface / ABI)
Di dalam arsitektur ARM64 (AArch64) untuk Linux dan Android, kernel memiliki aturan baku yang tidak bisa ditawar mengenai cara berkomunikasi dengan program.

Aturan ini disebut **Calling Convention**:
- Register `x8` selalu digunakan untuk menyimpan **Nomor ID Syscall**.
- Register `x0` selalu digunakan untuk menyimpan **Argumen Pertama** dari *syscall* tersebut.

## 💡 Mengapa Metode `add x0, x19, x1` Tetap Menjadi Standar Terbaik?
Meskipun bisa diganti dengan Alternatif 1, pendekatan penjumlahan dinamis add tetap menjadi cara paling aman dan standar industri yang digunakan oleh pustaka resmi seperti glibc/bionic libc.

Dengan tetap mempertahankan formula `add x0, x19, x1`, program Anda dijamin **100% kebal dari pengacakan alamat ASLR Android** tersebut karena ia selalu menghitung secara adaptif dari mana pun alamat awal yang diberikan oleh kernel.


qa: 
ingin kita coba memicu **Syscall write (ID 64)** untuk mencetak huruf 'A' yang ada di *heap* tadi ke layar terminal Termux Anda secara fisik?

<br>

---

<br>

System Call `sbrk` (`214`) pada arsitektur ARM64 Linux dan Android hanya menerima **tepat 1 argumen**, yang ditaruh di register `x0`. Register `x1` tidak dibaca oleh kernel untuk syscall ini.

Sesuai dengan dokumentasi resmi kernel Linux, fungsi dari *syscall* `sys_brk` didefinisikan secara internal seperti ini:


ss_2026-09-16-05-29-18-319.png


| Nama Syscall | Nomor ID ( x8 ) | Register Argumen yang Dibaca Kernel | Fungsi |
| :--- | :--- | :--- | :--- |
| **brk** | 214 | **x0** (hanya 1 argumen) [tent, 1] | Mengatur batas ukuran memori *heap* [tent, 1]. |
| **exit** | 93 | **x0** (hanya 1 argumen) [tent, 1] | Menutup program dengan kode keluar (misal: 0) [tent, 1]. |
| **write** | 64 | **x0** (File Descriptor / stdout)<br>**x1** (Alamat memori teks)<br>**x2** (Jumlah karakter / byte) | Mencetak teks secara fisik ke layar terminal. |




<br>
