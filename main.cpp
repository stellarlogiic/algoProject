
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

