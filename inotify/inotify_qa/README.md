# ey_rcrc_inotify_qa

# sys/inotify.h
Apakah sistem pemantau ini ingin Anda integrasikan dengan **sistem log file** atau langsung dikirim ke **Node.js** via _stream_?

Apakah Anda membutuhkan contoh **logika percabangan `if-else`** di dalam C untuk menangani aktivitas perpindahan (`IN_MOVE`) atau penutupan berkas (`IN_CLOSE_WRITE`) ini?

Apakah Anda ingin melihat contoh penggunaan `inotify_init1` **dengan fitur non-blocking** agar program C Anda tetap bisa melakukan tugas lain sambil memantau file?

Apakah Anda ingin menambahkan **mekanisme pembatasan (throttling)** atau **manajemen alokasi memori** jika aktivitas file di direktori tersebut mendadak sangat padat?

Jika Anda ingin membuat sistem pemantau ini bekerja pada **banyak folder sekaligus (rekursif)**, kita perlu membuat struktur data *array* atau *linked-list* untuk menampung banyak nilai wd (Watch Descriptor). Apakah Anda memerlukan **implementasi pelacakan multi-folder (rekursif)** tersebut?

Apakah Anda ingin mengintegrasikan log notifikasi ini agar hasilnya langsung **ditulis ke dalam sebuah file log (`.txt` atau `.log`)** secara otomatis?

Apakah Anda ingin menambahkan sistem **rotasi log (log rotation)** agar ukuran berkas `.log` tersebut tidak membengkak terlalu besar saat disimpan berhari-hari?

Apakah Anda berencana menjalankan program pemantau ini sebagai **layanan latar belakang (Daemon Service)** agar otomatis berjalan secara permanen di server/Linux Anda?

Jika layanan ini sudah berjalan di latar belakang, apakah Anda memerlukan instruksi untuk **mengirimkan notifikasi berbasis Webhook (seperti ke Discord, Telegram, atau Slack)** setiap kali terjadi aktivitas mencurigakan pada folder tersebut?

Apakah Anda memerlukan contoh **logika penyaringan (filter nama berkas)** agar Webhook hanya mendeteksi berkas-berkas konfigurasi sensitif saja?

Apakah Anda ingin menguji fungsionalitas ini dengan membuat simulasi **skrip pengujian otomatis (test script)** untuk melihat respon pemisahan log lokal dan pengiriman webhook-nya?















<br>
