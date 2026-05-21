
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
            cout << "           |\n";
        grandTotal += curr->subtotal;
    }
    cout << "+------------------------------------------------------+\n";
    if (withSubtotal)
        cout << "| TOTAL: Rp" << right << setw(14) << grandTotal
             << "                              |\n"
             << "+------------------------------------------------------+\n";
}

// fungsi sorting menyorting

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

// fungsi buat searching

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

// binary/berdasar kode menu
int binarySearchByKode(Menu arr[], int n, string kode) {
    string kodeUpper = kode;
    for (char &c : kodeUpper) c = toupper(c);
    
    Menu temp[100];
    int  orig[100];
    for (int i = 0; i < n; i++) { temp[i] = arr[i]; orig[i] = i; }
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (temp[j].kode_menu > temp[j+1].kode_menu) {
                swap(temp[j], temp[j+1]);
                swap(orig[j], orig[j+1]);
            }
    int left = 0, right = n - 1;
    while (left <= right) {
        int mid = (left + right) / 2;
        string midKode = temp[mid].kode_menu;
        for (char &c : midKode) c = toupper(c);

        if      (midKode == kodeUpper) return orig[mid];
        else if (midKode <  kodeUpper) left  = mid + 1;
        else                           right = mid - 1;
    }
    return -1;
}

// submenu utama
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
