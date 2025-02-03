#include<iostream>
#include<string>
#include<fstream>
#include<iomanip>
#include "crud.h"

int main()
{
    std::fstream data;
    crud::checkDatabase(data);

    enum option{CREATE = 1, READ, UPDATE, DELETE, FINISH};

    int pilihan = crud::getOption();

    while(pilihan != 5){
        switch(pilihan){
            case CREATE :   
                std::cout << "Menambahkan Data Buku\n";
                crud::addDataBuku(data);
                break;
            case READ :   
                std::cout << "Menampilkan Data Buku\n";
                crud::displayDataBuku(data);
                break;
            case UPDATE :   
                std::cout << "Mengubah Data Buku\n";
                crud::displayDataBuku(data);
                crud::updateDataBuku(data);
                crud::displayDataBuku(data);
                break;
            case DELETE :   
                std::cout << "Menghapus Data Buku\n";
                crud::displayDataBuku(data);
                crud::deleteDataBuku(data);
                crud::displayDataBuku(data);
                break;
            default :   
                std::cout << "Pilihan Tidak Di Temukan\n";
                break;
        }

        label_continue:

        std::string yn_option; 
        std::cout << "Lanjutkan?(y/n) : ";
        
        std::getline(std::cin, yn_option);

        if(yn_option == "y" || yn_option == "Y"){
            pilihan = crud::getOption();
        }else if(yn_option == "n" || yn_option == "N"){
            break;
        }else{
            goto label_continue;
        }
    }
    data.close();
    std::cout << "Anda Keluar Dari Program CRUD\n";
    return 0;
}

