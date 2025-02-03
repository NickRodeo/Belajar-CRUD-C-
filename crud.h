#include<fstream>

namespace crud{

    struct Buku{
        int pk;
        char nama[200];
        char tahun[6];
        char penulis[200];
    };

    //Prototype Function
    void checkDatabase(std::fstream& data);
    int getOption();
    int getDataSize(std::fstream& data);
    Buku readData(std::fstream& data, int size);
    void writeDataBuku(std::fstream& data, int pos, Buku& dataBuku);
    void addDataBuku(std::fstream& data);
    void displayDataBuku(std::fstream& data);
    void updateDataBuku(std::fstream& data);
    void deleteDataBuku(std::fstream& data);
}