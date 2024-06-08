#include <iostream>
#include <vector> // для массива
#include <chrono> // для времени
#include <fstream> // для чтения и записи файла
#include <string> // для работы со строками
#include <iomanip> // для перевода времени из(fixed);
#include "Search_in_string.h"
using namespace std;
using namespace chrono;


void read_file(vector<key>& arr, const string& filename) {
    string line;
    int count = 0;
    int p1, p2 = 0;

    ifstream in(filename);
    if (in.is_open()) {
        while (getline(in, line)) {
            count += 1;
            key k;
            k.group.l = line[0];
            k.group.num = stoi(line.substr(1, 4));
            p1 = line.find(' ');
            p2 = line.rfind(' ');
            k.fio.fio = line.substr(p1, p2-p1);
            k.dir.title = line.substr(p2 + 1);
            k.s = count;
            arr.push_back(k);
        }
    }
    else {
        cerr << "error";
    }
};

void write_file(vector<key>& arr, duration<double> diff, const string& filename) {
    ofstream file(filename);
    long n = arr.size();
    if (file.is_open()) {
        for (int i = 0; i < n; i++) {
            file << arr[i].group.l << arr[i].group.num << " " << arr[i].fio.fio << " " << arr[i].dir.title << " " << arr[i].s << "\n";
        }
        file << fixed << diff.count() << " s" << "\n";
    }
    else {
        cerr << "error";
    }
};

void Call_Rabin_Karp() {
    vector<key>arrRK; // массив для ключей
    vector<key>answer; // массив для ответов
    read_file(arrRK, "Input.txt"); // чтение файла
    long n = arrRK.size(); // длинна массива

    string const pattern_fio = "SZ"; // паттерн фио
    int pattern_fio_len = pattern_fio.length(); // длина паттерна фио
    int pattern_fio_hash = first_hash(pattern_fio, pattern_fio_len); // хэш паттерна фио
    int pattern_fio_count = 1; // количество вхождений паттерна в строке фио

    string const pattern_title = "H"; // паттерн направления
    int pattern_title_len = pattern_title.length(); // длина паттерна направления
    int pattern_title_hash = first_hash(pattern_title, pattern_title_len);
    int pattern_title_count = 1; // количество вхождений паттерна в строке направления

    auto startRK = high_resolution_clock::now(); // время начала 
    for (int i = 0; i < n; i++) {

        string str_fio = arrRK[i].fio.fio;// строка поля фио
        int str_fio_len = str_fio.length(); // длина строки фио
        int str_fio_hash = first_hash(str_fio, pattern_fio_len); // хэш фио

        string str_title = arrRK[i].dir.title; // строка поля направления
        int str_title_len = str_title.length(); // длина строки направления
        int str_title_hash = first_hash(str_title, pattern_title_len); // хэш направления

        if (RK(str_fio, str_fio_len, str_fio_hash, pattern_fio, pattern_fio_len, pattern_fio_hash, pattern_fio_count) && RK(str_title,str_title_len,str_title_hash,pattern_title,pattern_title_len,pattern_title_hash,pattern_title_count)) {
            answer.push_back(arrRK[i]);//закидываем ответ
        }
       
    }
    auto endRK = high_resolution_clock::now(); // время конца
    duration<double> diffRK = endRK - startRK; // время работы алгоритма
    write_file(answer, diffRK, "Output_Rabin_Karp.txt"); // запись в выходной файл
};

void Call_Turbo_Boyer_Moore() {
    vector<key>arrTBM;// массив для ключей
    vector<key>answer;// массив для ответов
    read_file(arrTBM, "Input.txt"); // чтение файла
    long n = arrTBM.size(); // длинна массива

    string const pattern_fio = "SZ"; // паттерн фио
    int pattern_fio_len = pattern_fio.length(); // длина паттерна фио
    int pattern_fio_count = 1; // количество вхождений паттерна в строке фио
    int good_fio_suffixs[pattern_max_len], bad_fio_symbols[alphabet_size];
    good_suffix(pattern_fio, pattern_fio_len, good_fio_suffixs);
    bad_symbol(pattern_fio, pattern_fio_len, bad_fio_symbols);



    string const pattern_title = "H"; // паттерн направления
    int pattern_title_len = pattern_title.length(); // длина паттерна направления
    int pattern_title_count = 1; // количество вхождений паттерна в строке направления
    int good_title_suffix[pattern_max_len], bad_title_symbols[alphabet_size]; // объявление массивов "хорошего суффикса" и "плохого символа"
    good_suffix(pattern_title, pattern_title_len, good_title_suffix); // заполнение
    bad_symbol(pattern_title, pattern_title_len, bad_title_symbols); // заполнение

    auto startTBM = high_resolution_clock::now(); // время начала
    for (int i = 0; i < n; i++) {

        string str_fio = arrTBM[i].fio.fio;// строка поля фио
        int str_fio_len = str_fio.length(); // длина строки фио

        string str_title = arrTBM[i].dir.title; // строка поля направления
        int str_title_len = str_title.length(); // длина строки направления

        if ((TBM(str_fio, str_fio_len, pattern_fio, pattern_fio_len, pattern_fio_count,good_fio_suffixs,bad_fio_symbols)) && (TBM(str_title,str_title_len,pattern_title,pattern_title_len,pattern_title_count,good_title_suffix,bad_title_symbols))) {
            answer.push_back(arrTBM[i]); // закидываем ответ
        }
    }
    auto endTBM = high_resolution_clock::now(); // время конца
    duration<double>diffTBM = endTBM - startTBM; // время работы алгоритма
    write_file(answer, diffTBM, "Output_Turbo_Boyer_Moore.txt"); // запись в выходной файл
}

int main() {
    Call_Rabin_Karp();
    Call_Turbo_Boyer_Moore();
    return 0;
}