

## Hambatan Lapisan Abstraksi Runtime (Bionic Libc)
Pada program C tingkat tinggi, fungsi `main()` sebenarnya **bukanlah titik awal (entry point) program yang sesungguhnya** di memori.
- Saat program dimuat, fungsi pertama yang dieksekusi oleh OS Android adalah `_start` (`__dl__start` pada `linker64`).
- Loader ini bertugas memanggil konstruktor internal pustaka C (`libc.so`) seperti `__register_atfork` untuk menyiapkan ekosistem runtime.
- Setelah semua persiapan sistem selesai, barulah fungsi pembungkus tersebut memanggil alamat memori dari `main()` Anda.
- Di program `.S`, Anda biasanya langsung mendeklarasikan `.global _start` tanpa lapisan penengah ini.

## Masalah Mangling Nama Fungsi C++
`__dl__ZN6soinfo26call_pre_init_constructorsEvInid` adalah nama fungsi yang mengalami pembusukan nama (*name mangling*). Karena internal linker Android menggunakan C++, kompilator mengubah nama fungsi asli menjadi simbol unik agar mendukung fitur seperti *function overloading*. Di bahasa assembly atau C murni, nama fungsi akan tersimpan persis sesuai teks aslinya.



<br>
