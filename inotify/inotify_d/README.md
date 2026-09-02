# ey_rcrc_d

# `sys/inotify.h`

Untuk memantau aktivitas perubahan direktori secara langsung (*real-time*), Anda bisa menggunakan pustaka bawaan Linux bernama `sys/inotify.h`. Pustaka ini jauh lebih efisien daripada melakukan pengecekan berulang (`polling loop`) menggunakan `getcwd`.

## Fungsi Inisialisasi
    `inotify_init()`
    Membuat instansiasi `inotify` baru dan mengembalikan file descriptor (`fd`).
    Membuat instansiasi baru untuk menangat sistem notifikasi Linux.

    `inotify_init1(int flags)`
     Versi lebih modern dari `inotify_init`. Fungsi ini menerima parameter `flags` seperti `IN_NONBLOCK` (agar proses membaca tidak menghentikan/memblokir program) atau `IN_CLOEXEC` (menutup otomatis saat proses eksekusi program lain).

## Fungsi Manajemen Pantauan (Watch)
    `inotify_add_watch()`
    `inotify_add_watch(int fd, const char *pathname, uint32_t mask)` Menambahkan direktori atau file baru ke dalam daftar pantauan `inotify`.
    Mendaftarkan `cwd` ke sistem pemantau dengan filter spesifik (`IN_CREATE`, `IN_DELETE`, `IN_MODIFY`).

    `inotify_rm_watch(int fd, int wd)`
    Menghapus item dari daftar pantauan berdasarkan *watch descriptor* (wd) yang didapat saat memanggil `inotify_add_watch`.

## Fungsi Operasi Standar (I/O)
    `read(fd, ...)`
    `read(int fd, void *buf, size_t count)`
    Mengambil data atau log aktivitas baru yang masuk ke antrean `inotify`.
    Fungsi ini akan memblokir eksekusi (blocking) dan hanya akan berjalan jika ada aktivitas nyata pada folder tersebut, sehingga sangat hemat CPU.

    `close(int fd)`
    Menutup *file descriptor* `inotify` dan menghapus seluruh daftar pantauan yang terikat di dalamnya dari memori.

## Aktivitas Akses & Modifikasi
    `IN_ACCESS`
    Berkas diakses atau dibaca (misalnya dibaca lewat perintah `cat` atau dibuka program lain).

    `IN_MODIFY`
    modifikasi
    Berkas diubah atau ditulis ulang.

    `IN_ATTRIB`
    perubahan hak akses
    Metadata berkas berubah (seperti hak akses `chmod`, kepemilikan `chown`, atau waktu modifikasi).

## Aktivitas Buka & Tutup Berkas
    `IN_OPEN`
    Berkas atau direktori dibuka.

    `IN_CLOSE_WRITE`
    Berkas yang dibuka untuk ditulis, kini telah ditutup (sangat berguna untuk mendeteksi kapan proses *download* atau *save* selesai sempurna).

    `IN_CLOSE_NOWRITE`
    Berkas yang dibuka hanya untuk dibaca, kini telah ditutup.

    `IN_CLOSE`
    Kombinasi dari `IN_CLOSE_WRITE` dan `IN_CLOSE_NOWRITE` (berkas ditutup, apa pun statusnya).

## Aktivitas Perpindahan (Move / Rename)
    `IN_MOVED_FROM`
    Berkas dipindahkan *keluar* dari direktori yang dipantau (atau namanya diubah).

    `IN_MOVED_TO`
    Berkas dipindahkan *masuk* ke direktori yang dipantau.

    `IN_MOVE`
    Kombinasi dari `IN_MOVED_FROM` dan `IN_MOVED_TO`.

## Aktivitas Struktur Direktori
    `IN_CREATE`
    pembuatan
    Berkas atau sub-direktori baru dibuat di dalam folder pantauan.

    `IN_DELETE`
    penghapusan
    Berkas atau sub-direktori dihapus dari folder pantauan.

    `IN_DELETE_SELF`
    Direktori utama yang sedang dipantau itu sendiri dihapus.

    `IN_MOVE_SELF`
    Direktori utama yang sedang dipantau itu sendiri dipindahkan.

## Fitur Tambahan di dalam Struktur Data (`struct inotify_event`)
Selain fungsi, saat Anda membaca data menggunakan `read()`, struktur data yang dikembalikan memiliki variabel *flag* internal untuk memberikan informasi status tambahan:
    `IN_ISDIR`
    Menandakan bahwa aktivitas yang terjadi dilakukan pada sebuah **direktori/folder**, bukan berkas biasa.

    `IN_Q_OVERFLOW`
    Menandakan bahwa antrean aktivitas penuh karena terlalu banyak kejadian dalam waktu singkat, sehingga ada beberapa notifikasi yang terlewat.

    `IN_UNMOUNT`
    Menandakan bahwa media penyimpanan (seperti Flashdisk atau HDD) tempat direktori tersebut berada telah dilepas (*unmounted*).

## Poin Penting pada Fitur Non-Blocking
    `errno == EAGAIN`
    Karena fungsi tidak lagi menunggu aktivitas, jika folder sedang sepi, `read()` akan langsung mengembalikan nilai `-1` dan mengisi variabel global `errno` dengan `EAGAIN`. Ini adalah tanda aman bagi program untuk melewati proses pembacaan log dan lanjut eksekusi ke bawah.

    `IN_ISDIR`
    Di dalam kode di atas, saya menambahkan pengecekan bendera `event->mask & IN_ISDIR` sehingga notifikasi Anda sekarang bisa membedakan secara spesifik apakah yang dibuat/dihapus itu sebuah **Berkas** atau **Folder (Direktori)**.

## Strategi Pemisahan & Penanganan Masalah
    `IN_Q_OVERFLOW`
    Jika aplikasi Anda menulis ribuan file dalam satu detik, kernel Linux akan mengirimkan sinyal overflow ini. Kode di atas langsung mendeteksi kondisi tersebut dan menggunakan `realloc()` untuk menduplikasi ukuran buffer memori secara dinamis di latar belakang.

    `LogThrottling`
    Kode ini mencatat berkas terakhir dan waktu modifikasinya. Jika proses seperti instalasi paket atau kompilasi program mengubah berkas yang sama ratusan kali dalam satu detik, sistem hanya akan memunculkan satu notifikasi setiap 2 detik.


































<br>
