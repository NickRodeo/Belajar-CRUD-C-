#include<iostream>
#include<string>
#include<fstream>
#include<iomanip>

using namespace std;

struct Buku{
    int pk;
    char nama[200];
    char tahun[6];
    char penulis[200];
};

//Prototype
void checkDatabase(fstream& data);
int getOption();
int getDataSize(fstream& data);
Buku readData(fstream& data, int size);
void writeDataBuku(fstream& data, int pos, Buku& dataBuku);
void addDataBuku(fstream& data);
void displayDataBuku(fstream& data);
void updateDataBuku(fstream& data);
void deleteDataBuku(fstream& data);

int main()
{
    fstream data;
    checkDatabase(data);

    enum option{CREATE = 1, READ, UPDATE, DELETE, FINISH};

    int pilihan = getOption();

    while(pilihan != 5){
        switch(pilihan){
            case CREATE :   
                cout << "Menambahkan Data Buku\n";
                addDataBuku(data);
                break;
            case READ :   
                cout << "Menampilkan Data Buku\n";
                displayDataBuku(data);
                break;
            case UPDATE :   
                cout << "Mengubah Data Buku\n";
                displayDataBuku(data);
                updateDataBuku(data);
                displayDataBuku(data);
                break;
            case DELETE :   
                cout << "Menghapus Data Buku\n";
                displayDataBuku(data);
                deleteDataBuku(data);
                displayDataBuku(data);
                break;
            default :   
                cout << "Pilihan Tidak Di Temukan\n";
                break;
        }

        label_continue:

        string yn_option; 
        cout << "Lanjutkan?(y/n) : ";
        
        getline(cin, yn_option);

        if(yn_option == "y" || yn_option == "Y"){
            pilihan = getOption();
        }else if(yn_option == "n" || yn_option == "N"){
            break;
        }else{
            goto label_continue;
        }
    }
    data.close();
    cout << "Anda Keluar Dari Program CRUD\n";
    return 0;
}

//Mengecek Apakah Database Ada
void checkDatabase(fstream& data)
{
    data.open("data.bin", ios::in | ios :: out | ios::binary);
    if(!data.is_open()){
        data.close();
        data.open("data.bin", ios:: trunc | ios::in | ios :: out | ios::binary);
    }
}

//Mendapat Option Pemilihan
int getOption()
{
    int input; 
    system("clear");
    system("cls");
    cout << "\nSelamat Datang Di Program CRUD\n";
    cout << "==============================\n";
    cout << "1. Tambah Data\n";
    cout << "2. Tampilkan Data\n";
    cout << "3. Ubah Data\n";
    cout << "4. Hapus Data\n";
    cout << "5. Selesai\n";
    cout << "==============================\n";
    cout << "Pilih [1 - 5] : "; cin >> input;
    cin.ignore();
    return input;
}

//Mendapatkan Jumlah Data dari Database
int getDataSize(fstream& data)
{
    int start, end;
    data.seekg(0, ios::beg);
    start = data.tellg();
    data.seekg(0, ios::end);
    end = data.tellg();
    return (end - start) / sizeof(Buku);
}

//Mengambil Data dari Database
Buku readData(fstream& data, int size)
{
    Buku readBuku;
    data.seekg((size - 1) * sizeof(Buku), ios::beg);
    data.read(reinterpret_cast<char*>(&readBuku), sizeof(Buku));
    return readBuku;
}

//Tambahkan Data ke Database
void writeDataBuku(fstream& data, int pos, Buku& dataBuku)
{
    data.seekp((pos - 1) * (sizeof(dataBuku)), ios::beg);
    data.write(reinterpret_cast<char*>(&dataBuku), sizeof(Buku));
}

//Membuat Data
void addDataBuku(fstream& data)
{
    Buku dataBuku, lastBuku;
    int size = getDataSize(data);

    if(size == 0){
        dataBuku.pk = 1;
    }else{
        lastBuku = readData(data, size);
        dataBuku.pk = lastBuku.pk + 1;
    }
    cout << "Nama Buku : ";
    cin.getline(dataBuku.nama, sizeof(dataBuku.nama));
    cout << "Tahun Rilis Buku : ";
    cin.getline(dataBuku.tahun, sizeof(dataBuku.tahun));
    cout << "Nama Penulis Buku : ";
    cin.getline(dataBuku.penulis, sizeof(dataBuku.penulis));

    writeDataBuku(data, size + 1, dataBuku);
}

//Tampilkan Data
void displayDataBuku(fstream& data)
{
    int size = getDataSize(data);
    Buku showBuku;
    cout << left;
    cout << setw(10)  << "No."
         << setw(10)  << "Pk."
         << setw(30) << "Nama Buku"
         << setw(20) << "Tahun Rilis"
         << setw(30) << "Nama Penulis" << "\n";

    for(int pos = 1; pos <= size; pos++){
        showBuku = readData(data, pos);
        cout << setw(10)  << pos
             << setw(10)  << showBuku.pk
             << setw(30) << showBuku.nama
             << setw(20) << showBuku.tahun
             << setw(30) << showBuku.penulis << "\n";
    }
}

//Ubah Data
void updateDataBuku(fstream& data)
{
    label_ulang:
    int nomor; 
    cout << "Masukkan Nomor Data : "; 
    cin >> nomor; 
    cin.ignore();
    Buku dataBuku = readData(data, nomor);
    if(nomor <= 0 || nomor > getDataSize(data)){
        cout << "Data Tidak Di Temukan!\n";
        data.clear();
        goto label_ulang;
    }

    cout << "\nPilihan Data\n";
    cout << "Nama Buku : " << dataBuku.nama << "\n"; 
    cout << "Tahun Rilis Buku : " << dataBuku.tahun << "\n";
    cout << "Nama Penulis Buku : " << dataBuku.penulis << "\n";

    cout << "\nMerubah Data\n";
    cout << "Nama Buku : "; cin.getline(dataBuku.nama, sizeof(dataBuku.nama)); 
    cout << "Tahun Rilis Buku : "; cin.getline(dataBuku.tahun, sizeof(dataBuku.tahun));
    cout << "Nama Penulis Buku : "; cin.getline(dataBuku.penulis, sizeof(dataBuku.penulis));

    writeDataBuku(data, nomor, dataBuku);
}

//Hapus Data
void deleteDataBuku(fstream& data)
{
    fstream tempData;
    int nomor, size, count = 0;
    cout << "Pilih Nomor : ";
    cin >> nomor; 
    cin.ignore();

    Buku tempBuku;

    tempData.open("temp.bin", ios::trunc | ios::in | ios::out | ios::binary);

    size = getDataSize(data);
    for(int pos = 1; pos <= size; pos++){
        tempBuku = readData(data, pos);
        if(pos != nomor){
            count++;
            writeDataBuku(tempData, count, tempBuku);
        }else{
            cout << "\nData Terhapus\n\n";
        }
    }

    size = getDataSize(tempData);
    data.close();
    data.open("data.bin", ios::trunc | ios::in | ios::out | ios::binary);
    data.close();

    data.open("data.bin", ios::in | ios::out | ios::binary);
    for(int pos = 1; pos <= size; pos++){
        tempBuku = readData(tempData, pos);
        writeDataBuku(data, pos,tempBuku);
    }

    tempData.close();
}