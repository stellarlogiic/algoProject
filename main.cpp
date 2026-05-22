#include <iostream>
#include <string>
#include <iomanip>

using namespace std;

// struktur data
struct Menu {
    string kode_menu;
    string nama_menu;
    int harga;
};

struct NodePesanan {
    string kode_menu;
    string nama_menu;
    int harga;
    int jumlah;
    int subtotal;
    NodePesanan *prev;
    NodePesanan *next;
};

// global variabel
Menu daftarMenu[100];
int jumlahMenu = 0;

NodePesanan *headPesanan = nullptr;
NodePesanan *tailPesanan = nullptr;

int nextIdTransaksi = 1;

// utility
void clearScreen() {
    system("clear || cls");
}

void pressEnter() {
    cout << "\nTekan Enter untuk melanjutkan";
    cin.ignore();
    cin.get();
}

// kembaliin salinan string dalam huruf kecil
string toLower(string str) {
    for (int i = 0; i < (int)str.size(); i++)
        str[i] = tolower(str[i]);
    return str;
}

// inisialisasi data
void initMenuData() {
    Menu menuAwal[] = {
        {"M001", "Nasi Goreng",  25000},
        {"M002", "Ayam Geprek",  18000},
        {"M003", "Mie Ayam",     15000},
        {"M004", "Es Teh",        5000},
        {"M005", "Es Jeruk",      8000},
        {"M006", "Bakso",        20000},
        {"M007", "Soto Ayam",    22000},
        {"M008", "Cappuccino",   15000}
    };
    jumlahMenu = sizeof(menuAwal) / sizeof(menuAwal[0]);
    for (int i = 0; i < jumlahMenu; i++)
        daftarMenu[i] = menuAwal[i];
}

// double linked list
NodePesanan* buatNodePesanan(Menu menu, int jumlah) {
    NodePesanan *node = new NodePesanan();
    node->kode_menu = menu.kode_menu;
    node->nama_menu = menu.nama_menu;
    node->harga     = menu.harga;
    node->jumlah    = jumlah;
    node->subtotal  = menu.harga * jumlah;
    node->prev      = nullptr;
    node->next      = nullptr;
    return node;
}

void tambahPesananKeList(NodePesanan *pesanan) {
    if (!headPesanan) {
        headPesanan = tailPesanan = pesanan;
    } else {
        tailPesanan->next = pesanan;
        pesanan->prev     = tailPesanan;
        tailPesanan       = pesanan;
    }
}

void hapusPesananDariList(NodePesanan *pesanan) {
    if (pesanan->prev) pesanan->prev->next = pesanan->next;
    else               headPesanan         = pesanan->next;
    if (pesanan->next) pesanan->next->prev = pesanan->prev;
    else               tailPesanan         = pesanan->prev;
    delete pesanan;
}

void hapusSemuaPesanan() {
    while (headPesanan) {
        NodePesanan *temp = headPesanan;
        headPesanan = headPesanan->next;
        delete temp;
    }
    tailPesanan = nullptr;
}

// kalkulasi
int hitungTotalBayar() {
    int total = 0;
    for (NodePesanan *curr = headPesanan; curr; curr = curr->next)
        total += curr->subtotal;
    return total;
}

// tampilan
void tampilkanMenu(Menu arr[]) {
    cout << "\n+=====================================================+\n";
    cout << "|              DAFTAR MENU RESTORAN                   |\n";
    cout << "+=====================================================+\n";
    cout << "| Kode  | Nama Menu                   | Harga         |\n";
    cout << "+-------+-----------------------------+---------------+\n";
    for (int i = 0; i < jumlahMenu; i++) {
        cout << "| " << left  << setw(5) << arr[i].kode_menu << " | "
             << left  << setw(27) << arr[i].nama_menu << " | "
             << "Rp"  << right << setw(9) << arr[i].harga << "   |\n";
    }
    cout << "+=====================================================+\n";
}

void tampilkanTabelPesanan(bool withSubtotal) {
    if (!headPesanan) {
        cout << "\n[!] Belum ada pesanan.\n";
        return;
    }
    cout << "\n+------------------------------------------------------+\n";
    if (withSubtotal)
        cout << "| No | Kode | Nama Menu              | Jml | Subtotal  |\n";
    else
        cout << "| No | Kode | Nama Menu              | Jml |           |\n";
    cout << "+----+------+------------------------+-----+-----------+\n";

    int no = 1, grandTotal = 0;
    for (NodePesanan *curr = headPesanan; curr; curr = curr->next, no++) {
        cout << "| "  << left  << setw(2)  << no              << " | "
             << left  << setw(4)  << curr->kode_menu           << " | "
             << left  << setw(22) << curr->nama_menu           << " | "
             << right << setw(3)  << curr->jumlah              << " | ";
        if (withSubtotal)
            cout << "Rp" << right << setw(7) << curr->subtotal << " |\n";
        else
            cout << "          |\n";
        grandTotal += curr->subtotal;
    }
    cout << "+------------------------------------------------------+\n";
    if (withSubtotal)
        cout << "| TOTAL: Rp" << right << setw(14) << grandTotal
             << "                              |\n"
             << "+------------------------------------------------------+\n";
}

// fungsi sorting
void bubbleSort(Menu arr[], int n, bool ascending, bool (*cmp)(Menu&, Menu&, bool)) {
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (cmp(arr[j], arr[j+1], ascending))
                swap(arr[j], arr[j+1]);
}

bool cmpHarga(Menu &a, Menu &b, bool asc) {
    return asc ? (a.harga > b.harga) : (a.harga < b.harga);
}

bool cmpKode(Menu &a, Menu &b, bool asc) {
    return asc ? (a.kode_menu > b.kode_menu) : (a.kode_menu < b.kode_menu);
}

int partitionByName(Menu arr[], int low, int high, bool ascending) {
    string pivot = arr[high].nama_menu;
    int i = low - 1;
    for (int j = low; j < high; j++) {
        bool kondisi = ascending ? (arr[j].nama_menu <= pivot)
                                 : (arr[j].nama_menu >= pivot);
        if (kondisi) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i+1], arr[high]);
    return i + 1;
}

void quickSortByName(Menu arr[], int low, int high, bool ascending) {
    if (low < high) {
        int pi = partitionByName(arr, low, high, ascending);
        quickSortByName(arr, low,    pi - 1, ascending);
        quickSortByName(arr, pi + 1, high,   ascending);
    }
}

// fungsi searching

// sequential/berdasar nama
int sequentialSearch(Menu arr[], string keyword) {
    string keyLower = toLower(keyword);
    for (int i = 0; i < jumlahMenu; i++) {
        string nama = toLower(arr[i].nama_menu);
        if (nama.find(keyLower) != string::npos)
            return i;
    }
    return -1;
}

int binarySearchByKode(Menu arr[], int n, string kode) {
    string kodeUpper = kode;
    for (char &c : kodeUpper) c = toupper(c);

    int sortedIdx[100];
    for (int i = 0; i < n; i++) sortedIdx[i] = i;

    // bubble sort buat di kode_menu
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (arr[sortedIdx[j]].kode_menu > arr[sortedIdx[j+1]].kode_menu)
                swap(sortedIdx[j], sortedIdx[j+1]);

    // binary search 
    int left = 0, right = n - 1;
    while (left <= right) {
        int mid = (left + right) / 2;
        string midKode = arr[sortedIdx[mid]].kode_menu;
        for (char &c : midKode) c = toupper(c);

        if      (midKode == kodeUpper) return sortedIdx[mid]; // kembalikan index asli
        else if (midKode <  kodeUpper) left  = mid + 1;
        else                           right = mid - 1;
    }
    return -1;
}

// submenu lihat menu
void menuLihatMenu() {
    clearScreen();
    tampilkanMenu(daftarMenu);

    cout << "\n+=====================================================+\n";
    cout << "| 1. Urutkan Menu                                     |\n";
    cout << "| 2. Cari Menu                                        |\n";
    cout << "| 3. Kembali                                          |\n";
    cout << "+=====================================================+\n";
    cout << "Pilih: ";
    int pilih; cin >> pilih;

    if (pilih == 1) {
        clearScreen();
        cout << "\n+=====================================================+\n";
        cout << "| 1. Berdasarkan Harga                                |\n";
        cout << "| 2. Berdasarkan Nama                                 |\n";
        cout << "| 3. Berdasarkan Kode                                 |\n";
        cout << "+=====================================================+\n";
        cout << "Pilih kategori: ";
        int kat; cin >> kat;

        cout << "\n| 1. Ascending   \n| 2. Descending\n";
        cout << "Pilih urutan: ";
        int ur; cin >> ur;
        bool asc = (ur == 1);

        Menu temp[100];
        for (int i = 0; i < jumlahMenu; i++) temp[i] = daftarMenu[i];

        switch (kat) {
            case 1:
                bubbleSort(temp, jumlahMenu, asc, cmpHarga);
                cout << "\n[Bubble Sort - Harga " << (asc ? "Termurah->Termahal" : "Termahal->Termurah") << "]\n";
                break;
            case 2:
                quickSortByName(temp, 0, jumlahMenu - 1, asc);
                cout << "\n[Quick Sort - Nama " << (asc ? "A->Z" : "Z->A") << "]\n";
                break;
            case 3:
                bubbleSort(temp, jumlahMenu, asc, cmpKode);
                cout << "\n[Bubble Sort - Kode " << (asc ? "Awal->Akhir" : "Akhir->Awal") << "]\n";
                break;
            default:
                cout << "\n[!] Pilihan tidak valid.\n";
                pressEnter(); return;
        }
        tampilkanMenu(temp);
        cout << "\n[INFO] Data asli tidak berubah.\n";
        pressEnter();

    } else if (pilih == 2) {
        clearScreen();
        cout << "\n+=====================================================+\n";
        cout << "| 1. Berdasarkan nama                                 |\n";
        cout << "| 2. Berdasarkan kode                                 |\n";
        cout << "+=====================================================+\n";
        cout << "Pilih metode: ";
        int met; cin >> met; cin.ignore();

        string keyword;
        cout << (met == 1 ? "Masukkan nama menu : " : "Masukkan kode menu : ");
        getline(cin, keyword);

        if (met == 1) {
            int idx = sequentialSearch(daftarMenu, keyword);
            if (idx != -1) {
                cout << "\n[DITEMUKAN]\n";
                cout << "Kode  : " << daftarMenu[idx].kode_menu << "\n";
                cout << "Nama  : " << daftarMenu[idx].nama_menu << "\n";
                cout << "Harga : Rp" << daftarMenu[idx].harga << "\n";
            } else {
                cout << "\n[!] Menu '" << keyword << "' tidak ditemukan.\n";
            }
        } else if (met == 2) {
            int idx = binarySearchByKode(daftarMenu, jumlahMenu, keyword);
            if (idx != -1) {
                cout << "\n[DITEMUKAN]\n";
                cout << "Kode  : " << daftarMenu[idx].kode_menu << "\n";
                cout << "Nama  : " << daftarMenu[idx].nama_menu << "\n";
                cout << "Harga : Rp" << daftarMenu[idx].harga << "\n";
            } else {
                cout << "\n[!] Kode '" << keyword << "' tidak ditemukan.\n";
            }
        }
        pressEnter();
    }
}

// forward declaration
void prosesTransaksi();

void tambahPesanan() {
    cin.ignore(); 
    char lagi = 'y';
    while (lagi == 'y' || lagi == 'Y') {
        clearScreen();
        tampilkanMenu(daftarMenu);

        string input;
        cout << "\nMasukkan kode atau nama menu: ";
        getline(cin, input); 

        if (input.empty()) {
            cout << "\n[!] Input tidak boleh kosong! Silakan masukkan kode (cth: M001) atau nama menu.\n";
            pressEnter();
            lagi = 'y'; continue;
        }

        int idx = binarySearchByKode(daftarMenu, jumlahMenu, input);
        if (idx == -1)
            idx = sequentialSearch(daftarMenu, input);

        if (idx == -1) {
            cout << "\n[!] Menu '" << input << "' tidak ditemukan! Coba kode (M001-M008) atau nama menu.\n";
            pressEnter();
            lagi = 'y'; continue;
        }

        cout << "Menu : " << daftarMenu[idx].nama_menu
             << " (Rp" << daftarMenu[idx].harga << ")\n";
        cout << "Jumlah: ";
        int jumlah; cin >> jumlah;
        if (jumlah <= 0) {
            cout << "\n[!] Jumlah tidak valid!\n";
            pressEnter();
            lagi = 'y'; continue;
        }

        NodePesanan *baru = buatNodePesanan(daftarMenu[idx], jumlah);
        tambahPesananKeList(baru);
        cout << "\n[v] " << daftarMenu[idx].nama_menu << " x" << jumlah << " ditambahkan!\n";

        cout << "\nTambah pesanan lagi? (y/n): ";
        cin >> lagi;
        cin.ignore(); 
    }

    clearScreen();
    cout << "\n+=====================================================+\n";
    cout << "|                RINGKASAN PESANAN                    |\n";
    cout << "+=====================================================+\n";
    tampilkanTabelPesanan(true);

    cout << "\n| 1. Lanjut ke Pembayaran  \n| 2. Kembali ke Menu \n";
    cout << "Pilih: ";
    int pilih; cin >> pilih;
    if (pilih == 1) prosesTransaksi();
}

// hapus pesanan
void menuHapusPesanan() {
    clearScreen();
    if (!headPesanan) {
        cout << "\n[!] Belum ada pesanan.\n";
        pressEnter(); return;
    }

    tampilkanTabelPesanan(true);

    cout << "\nMasukkan nomor pesanan yang akan dihapus (0 = batal): ";
    int nomor; cin >> nomor;
    if (nomor == 0) return;

    NodePesanan *curr = headPesanan;
    for (int i = 1; curr && i < nomor; i++) curr = curr->next;

    if (curr) {
        cout << "\n[v] Pesanan '" << curr->nama_menu << "' dihapus.\n";
        hapusPesananDariList(curr);
    } else {
        cout << "\n[!] Nomor tidak valid.\n";
    }
    pressEnter();
}

void prosesTransaksi() {
    clearScreen();
    if (!headPesanan) {
        cout << "\n[!] Tidak ada pesanan aktif.\n";
        pressEnter(); return;
    }

    cout << "\n+=====================================================+\n";
    cout << "|               RINGKASAN PESANAN                     |\n";
    cout << "+=====================================================+\n";
    tampilkanTabelPesanan(true);

    string nama;
    cout << "\nNama pelanggan: ";
    cin.ignore();
    getline(cin, nama);

    int total = hitungTotalBayar();

    cout << "\n+=====================================================+\n";
    cout << "| Total: Rp" << right << setw(17) << total << "                          |\n";
    cout << "+-----------------------------------------------------+\n";
    cout << "| 1. Tunai                                            |\n";
    cout << "| 2. QRIS                                             |\n";
    cout << "| 3. Debit/Credit Card                                |\n";
    cout << "+=====================================================+\n";
    cout << "Pilih metode: ";
    int met; cin >> met;

    int    bayar = 0, kembalian = 0;
    string metode;

    switch (met) {
        case 1:
            metode = "Tunai";
            cout << "\nMasukkan jumlah uang: Rp";
            cin >> bayar;
            while (bayar < total) {
                int kurang = total - bayar;
                cout << "[!] Uang kurang Rp." << kurang;
                cout << "\n[!] Silakan masukkan ulang nominal: Rp.";
                cin >> bayar;
            }
            kembalian = bayar - total;
            cout << "\n[CASH] Pembayaran berhasil!\n";
            break;
        case 2:
            metode = "QRIS";
            cout << "\n[QRIS] Pembayaran berhasil!\n";
            bayar = total; kembalian = 0;
            break;
        case 3:
            metode = "Debit/Credit Card";
            cout << "\n[DEBIT/CREDIT CARD] Pembayaran berhasil!\n";
            bayar = total; kembalian = 0;
            break;
        default:
            metode = "Tunai";
            cout << "\n[CASH] Pembayaran berhasil!\n";
            bayar = total; kembalian = 0;
    }

    cout << "\n+=====================================================+\n";
    cout << "|                    STRUK BAYAR                      |\n";
    cout << "+=====================================================+\n";
    cout << "| Pelanggan    : " << left << setw(37) << nama             << "|\n";
    cout << "| ID Transaksi : #" << left << setw(36) << nextIdTransaksi << "|\n";
    cout << "+-----------------------------------------------------+\n";
    for (NodePesanan *curr = headPesanan; curr; curr = curr->next)
        cout << "| " << left  << setw(22) << curr->nama_menu
             << " x" << setw(2) << curr->jumlah
             << "  Rp" << right << setw(14) << curr->subtotal << "        |\n";
    cout << "+-----------------------------------------------------+\n";
    cout << "| Total      : Rp" << right << setw(17) << total     << "                    |\n";
    cout << "| Bayar      : Rp" << right << setw(17) << bayar     << "                    |\n";
    cout << "| Kembalian  : Rp" << right << setw(17) << kembalian << "                    |\n";
    cout << "| Metode     : "   << left  << setw(39) << metode    << "|\n";
    cout << "+=====================================================+\n";

    nextIdTransaksi++;
    hapusSemuaPesanan();
    cout << "\n[v] Transaksi selesai. Pesanan telah dihapus dari daftar.\n";
    pressEnter();
}

// main program
int main() {
    initMenuData();

    int pilihan;
    do {
        clearScreen();
        cout << "\n+=====================================================+\n";
        cout << "|                  MENU UTAMA                         |\n";
        cout << "+=====================================================+\n";
        cout << "| 1. Lihat Menu                                       |\n";
        cout << "| 2. Tambah Pesanan                                   |\n";
        cout << "| 3. Lihat Pesanan Aktif                              |\n";
        cout << "| 4. Hapus Pesanan                                    |\n";
        cout << "| 0. Keluar                                           |\n";
        cout << "+=====================================================+\n";
        cout << "Pilihan: ";
        cin >> pilihan;

        switch (pilihan) {
            case 1: menuLihatMenu(); break;
            case 2: tambahPesanan(); break;
            case 3:
                clearScreen();
                cout << "\n+=====================================================+\n";
                cout << "|               PESANAN AKTIF                         |\n";
                cout << "+=====================================================+\n";
                tampilkanTabelPesanan(true);
                cout << "\n| 1. Lanjut Bayar\n| 2. Kembali\n";
                cout << "Pilih: ";
                {
                    int p; cin >> p;
                    if (p == 1) prosesTransaksi();
                }
                break;
            case 4: menuHapusPesanan(); break;
            case 0:
                clearScreen();
                cout << "\n+=====================================================+\n";
                cout << "|         TERIMA KASIH TELAH BERBELANJA!              |\n";
                cout << "+=====================================================+\n";
                break;
            default:
                cout << "\n[!] Pilihan tidak valid.\n";
                pressEnter();
        }
    } while (pilihan != 0);

    hapusSemuaPesanan();
    return 0;
}
