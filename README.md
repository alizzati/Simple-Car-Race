# 🏎️ Medan's Fast Furious 2D - Chapter 1

Proyek game balap mobil 2D sederhana yang dibuat menggunakan C++ dan OpenGL/GLUT. Game ini menampilkan simulasi balapan malam hari dengan fisika dasar (akselerasi, friksi, batas kecepatan) dan animasi latar belakang (lampu gedung berkedip).

Proyek ini dibuat sebagai latihan dalam pemahaman dasar grafik komputer, *event handling* (input keyboard), dan *game loop* (fisika real-time) menggunakan GLUT.

## 🌟 Fitur Utama

* **Balapan Dua Pemain:** Kontrol terpisah untuk dua mobil.
* **Fisika Dasar:** Termasuk percepatan, gesekan, dan batas kecepatan maksimal.
* **Kondisi Menang Akurat:** Mobil dinyatakan menang ketika lampu depannya melewati garis *finish*.
* **Visual Malam Hari:** Latar belakang dengan *skyline* kota, lampu jalan, dan animasi lampu jendela gedung yang berkedip.

## ⚙️ Persyaratan Sistem

Proyek ini menggunakan **GLUT** (OpenGL Utility Toolkit) dan kompiler C++.

* **Sistem Operasi:** Windows, macOS, atau Linux.
* **Kompiler:** GCC/G++ (dengan dukungan OpenGL/GLUT) atau Visual Studio.
* **Library:** FreeGLUT (disarankan) atau GLUT bawaan sistem.

## 🛠️ Cara Mengkompilasi dan Menjalankan

### A. Windows (Menggunakan Visual Studio atau MinGW/GCC)

1.  **Pastikan GLUT terinstal:** Jika menggunakan Visual Studio, pastikan Anda telah mengkonfigurasi file header GLUT/FreeGLUT dan file library (.lib).
2.  **Kompilasi:** Kompilasi file `.cpp` dengan menghubungkan library OpenGL, GLU, dan GLUT/FreeGLUT.
    ```bash
    g++ nama_file.cpp -o race_game -lglut -lGL -lGLU
    ```
3.  **Jalankan:**
    ```bash
    ./race_game
    ```

### B. Linux/macOS

1.  **Instal FreeGLUT:**
    ```bash
    # Pada Debian/Ubuntu:
    sudo apt-get install freeglut3 freeglut3-dev
    # Pada macOS (menggunakan Homebrew):
    brew install freeglut
    ```
2.  **Kompilasi:**
    ```bash
    g++ nama_file.cpp -o race_game -lglut -lGL -lGLU
    ```
3.  **Jalankan:**
    ```bash
    ./race_game
    ```

## 🎮 Cara Bermain

Tujuannya adalah menjadi mobil pertama yang mencapai garis *finish* (garis putih vertikal).

| Mobil | Gerakan Vertikal (Y) | Gerakan Horizontal (X) |
| :--- | :--- | :--- |
| **Mobil 1 (Merah)** | Panah Atas ($\uparrow$) / Panah Bawah ($\downarrow$) | Panah Kanan ($\rightarrow$) / Panah Kiri ($\leftarrow$) |
| **Mobil 2 (Biru)** | W / S | D / A |

### Perintah Tambahan

| Tombol | Fungsi |
| :--- | :--- |
| **R** | *Restart* permainan setelah ada pemenang. |
| **ESC** | Keluar dari program. |

---

## 🏗️ Struktur Kode

* **`struct Car`**: Mendefinisikan properti mobil (posisi, kecepatan, fisika, warna).
* **`resetGame()`**: Mengatur ulang semua variabel ke kondisi awal balapan.
* **`drawCar()` / `drawBuildings()` / dll.**: Fungsi-fungsi untuk rendering visual.
* **`applyPhysics()`**: Menerapkan gesekan dan batas kecepatan.
* **`checkWinner()`**: Memeriksa apakah lampu depan mobil telah melewati garis *finish*.
* **`idleFunc()`**: Jantung *game loop* yang memproses input dan fisika setiap *frame*.

***

### 📝 Perbaikan dan Pengembangan

Jika Anda ingin menyesuaikan permainan:

1.  **Kecepatan:** Ubah nilai `maxSpeed` di `resetGame()` untuk mengatur kecepatan maksimal mobil.
2.  **Kontrol:** Sesuaikan nilai `accel` atau `friction` di `resetGame()` untuk mengubah cara mobil berakselerasi atau mengerem.
