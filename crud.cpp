#include<iostream>
#include<string>
#include<fstream>
#include<iomanip>
#include "crud.h"

//Mengecek Apakah Database Ada
void crud::checkDatabase(std::fstream& data)
{
    data.open("data.bin", std::ios::in | std::ios :: out | std::ios::binary);
    if(!data.is_open()){
        data.close();
        data.open("data.bin", std::ios:: trunc | std::ios::in | std::ios :: out | std::ios::binary);
    }
}

//Mendapat Option Pemilihan
int crud::getOption()
{
    int input; 
    system("clear");
    std::cout << "\nSelamat Datang Di Program CRUD\n";
    std::cout << "==============================\n";
    std::cout << "1. Tambah Data\n";
    std::cout << "2. Tampilkan Data\n";
    std::cout << "3. Ubah Data\n";
    std::cout << "4. Hapus Data\n";
    std::cout << "5. Selesai\n";
    std::cout << "==============================\n";
    std::cout << "Pilih [1 - 5] : "; std::cin >> input;
    std::cin.ignore();
    return input;
}

//Mendapatkan Jumlah Data dari Database
int crud::getDataSize(std::fstream& data)
{
    int start, end;
    data.seekg(0, std::ios::beg);
    start = data.tellg();
    data.seekg(0, std::ios::end);
    end = data.tellg();
    return (end - start) / sizeof(Buku);
}

//Mengambil Data dari Database
crud::Buku crud::readData(std::fstream& data, int size)
{
    Buku readBuku;
    data.seekg((size - 1) * sizeof(crud::Buku), std::ios::beg);
    data.read(reinterpret_cast<char*>(&readBuku), sizeof(crud::Buku));
    return readBuku;
}

//Tambahkan Data ke Database
void crud::writeDataBuku(std::fstream& data, int pos, crud::Buku& dataBuku)
{
    data.seekp((pos - 1) * (sizeof(dataBuku)), std::ios::beg);
    data.write(reinterpret_cast<char*>(&dataBuku), sizeof(crud::Buku));
}

//Membuat Data
void crud::addDataBuku(std::fstream& data)
{
    crud::Buku dataBuku, lastBuku;
    int size = crud::getDataSize(data);

    if(size == 0){
        dataBuku.pk = 1;
    }else{
        lastBuku = crud::readData(data, size);
        dataBuku.pk = lastBuku.pk + 1;
    }
    std::cout << "Nama Buku : ";
    std::cin.getline(dataBuku.nama, sizeof(dataBuku.nama));
    std::cout << "Tahun Rilis Buku : ";
    std::cin.getline(dataBuku.tahun, sizeof(dataBuku.tahun));
    std::cout << "Nama Penulis Buku : ";
    std::cin.getline(dataBuku.penulis, sizeof(dataBuku.penulis));

    crud::writeDataBuku(data, size + 1, dataBuku);
}
//Tampilkan Data
void crud::displayDataBuku(std::fstream& data)
{
    int size = crud::getDataSize(data);
    crud::Buku showBuku;
    std::cout << std::left;
    std::cout << std::setw(10)  << "No."
        << std::setw(10)  << "Pk."
        << std::setw(30) << "Nama Buku"
        << std::setw(20) << "Tahun Rilis"
        << std::setw(30) << "Nama Penulis" << "\n";

    for(int pos = 1; pos <= size; pos++){
        showBuku = crud::readData(data, pos);
        std::cout << std::setw(10)  << pos
            << std::setw(10)  << showBuku.pk
            << std::setw(30) << showBuku.nama
            << std::setw(20) << showBuku.tahun
            << std::setw(30) << showBuku.penulis << "\n";
    }
}

//Ubah Data
void crud::updateDataBuku(std::fstream& data)
{
    label_ulang:
    int nomor; 
    std::cout << "Masukkan Nomor Data : "; 
    std::cin >> nomor; 
    std::cin.ignore();
    crud::Buku dataBuku = crud::readData(data, nomor);
    if(nomor <= 0 || nomor > crud::getDataSize(data)){
        std::cout << "Data Tidak Di Temukan!\n";
        data.clear();
        goto label_ulang;
    }

    std::cout << "\nPilihan Data\n";
    std::cout << "Nama Buku : " << dataBuku.nama << "\n"; 
    std::cout << "Tahun Rilis Buku : " << dataBuku.tahun << "\n";
    std::cout << "Nama Penulis Buku : " << dataBuku.penulis << "\n";

    std::cout << "\nMerubah Data\n";
    std::cout << "Nama Buku : "; std::cin.getline(dataBuku.nama, sizeof(dataBuku.nama)); 
    std::cout << "Tahun Rilis Buku : "; std::cin.getline(dataBuku.tahun, sizeof(dataBuku.tahun));
    std::cout << "Nama Penulis Buku : "; std::cin.getline(dataBuku.penulis, sizeof(dataBuku.penulis));

    crud::writeDataBuku(data, nomor, dataBuku);
}

//Hapus Data
void crud::deleteDataBuku(std::fstream& data)
{
    std::fstream tempData;
    int nomor, size, count = 0;
    std::cout << "Pilih Nomor : ";
    std::cin >> nomor; 
    std::cin.ignore();

    crud::Buku tempBuku;

    tempData.open("temp.bin", std::ios::trunc | std::ios::in | std::ios::out | std::ios::binary);

    size = crud::getDataSize(data);
    for(int pos = 1; pos <= size; pos++){
        tempBuku = crud::readData(data, pos);
        if(pos != nomor){
            count++;
            crud::writeDataBuku(tempData, count, tempBuku);
        }else{
            std::cout << "\nData Terhapus\n\n";
        }
    }

    size = crud::getDataSize(tempData);
    data.close();
    data.open("data.bin", std::ios::trunc | std::ios::in | std::ios::out | std::ios::binary);
    data.close();

    data.open("data.bin", std::ios::in | std::ios::out | std::ios::binary);
    for(int pos = 1; pos <= size; pos++){
        tempBuku = crud::readData(tempData, pos);
        crud::writeDataBuku(data, pos,tempBuku);
    }

    tempData.close();
}
