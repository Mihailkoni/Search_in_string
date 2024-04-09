#include <iostream>
#include <vector> // для массива
#include <chrono> // для времени
#include <fstream> // для чтения и записи файла
#include <string> // для работы со строками
#include <iomanip> // для перевода времени из(fixed);
#include <algorithm> // для max min
using namespace std;
using namespace chrono;

struct key {
    int s; // для номера строки
    struct group { //Номер группы
        char l; // буква номера группы
        int num; // число номера группы
    }group;

    struct fio { //ФИО
        string surname; // фамилия
        string name; // имя
        string patronymic; // отчество
    }fio;

    struct Direction {
        string title; // название направления
    }dir;

    const string to_String_Fio() { // для возвращения целой строки FIO
        return fio.surname + " " + fio.name + " " + fio.patronymic;
    }
};


/// <summary> 
/// как в паскале комментарии к функциям
/// </summary>

///////////////////////////////////////////
const int prime_number = 2;

int pow_for_hash(int num,int exp) {// функция для возведения в степень
    int res = 1;
    for (int i = 0; i < exp; i++) {
        res *= num;
    }
    return res;
}

int first_hash(string const& str, int len) { // функция вычисления хэша
    int hash = 0;
    for (int i = 0; i < len; i++) {
        hash += (int)str[i] * pow_for_hash(prime_number, i);
    }
    return hash;
}

int refresh_hash(string const& str, int delete_index, int add_index, int old_hash, int pattern_len) { // рефреш(обновление) хэш
    int newHash = (old_hash - (int)str[delete_index]) / prime_number;
    newHash += (int)str[add_index] * pow_for_hash(prime_number, pattern_len - 1);
    return newHash;
}

bool check_equals_strings(string const& str1, string const str2, int start1, int end1, int start2, int end2) { // проверка на равность строк
    if ((end1 - start1) != (end2 - start2)) {
        return false;
    }
    while ((start1 <= end1) && (start2 <= end2)) {
        if (str1[start1] != str2[start2]) {
            return false;
        }
        start1++;
        start2++;
    }
    return true;
}

bool RK(string const str,int& str_len,int& str_hash, string const& pattern, int const& pattern_len, int const& pattern_hash,int const& pattern_count) { // рабин-карп
    int count = 0;
    for (int j = 0; j <= (str_len - pattern_len); j++) {
            if ((str_hash == pattern_hash) && check_equals_strings(str, pattern, j, j + pattern_len - 1, 0, pattern_len - 1)) {
                //answer.push_back(arr[i]);
                count++;
                if (count == pattern_count) {
                    return true;
                }
            }
            if (j < (str_len - pattern_len)) {
                str_hash = refresh_hash(str, j, j + pattern_len, str_hash, pattern_len);
            }
    }
    return false;
};

///////////////////////////////////////////

const int alphabet_size = 256;
const int pattern_max_len = 10;

void bad_symbol(const string& pattern,int& pattern_len,int bad_symbols[]) {
    for (int i = 0; i < alphabet_size; ++i) {
        bad_symbols[i] = pattern_len;
    }
    for (int j = 0; j < pattern_len - 1; ++j) {
        bad_symbols[pattern[j]] = pattern_len - j - 1;
    }
}

void good_suffix(const string& pattern, int& pattern_len, int good_suffixs[]) {
    int f[pattern_max_len];
    for (int i = 0; i < pattern_len; ++i) {
        good_suffixs[i] = 0;
    }
    f[pattern_len] = pattern_len + 1;
    for (int i = pattern_len; i > 0; --i) {
        int j = pattern_len + 1;
        while ((j <= pattern_len) && (pattern[i - 1] != pattern[j - 1])) {
            if (good_suffixs[j] == 0) {
                good_suffixs[j] = j - 1;
            }
            j = f[j];
        }
        f[i - 1] = --j;
    }
    int p = f[0];
    for (int j = 0; j <= pattern_len; ++j) {
        if (good_suffixs[j] == 0) {
            good_suffixs[j] = p;
        }
        if (j == p) {
            p = f[p];
        }
    }
}   

int max_for_TBM(int a, int b, int c) {
    if (a >= b && a >= c) return a;
    if (b >= a && b >= c) return b;
    return c;
}

bool TBM(string const str,int str_len,string const pattern,int pattern_len,int pattern_count,int good_suffixs[pattern_max_len],int bad_symbols[alphabet_size]) {
    int count = 0; // счётчик вхождения паттерна
    int i = 0, u = 0, shift = 0;
    while (i <= str_len - pattern_len) {
        int j = pattern_len - 1;
        while ((j >= 0) && (pattern[j] == str[i + j])) {
            --j;
            if ((u != 0) && (j == pattern_len - 1 - shift)) {
                j -= u;
            }
        }
        if (j < 0) {
            count++;
            if (count == pattern_count) {
                return true;
            }
            shift = good_suffixs[0];
            u = pattern_len - shift;
        }
        else {
            int v = pattern_len - 1 - j;
            int turbo_Shift = u - v;
            int bad_symbol_shift = bad_symbols[str[i + j] - pattern_len + j + 1];
            shift = max_for_TBM(turbo_Shift,bad_symbol_shift,good_suffixs[j + 1]);
            if (shift == good_suffixs[j + 1]) {
                if ((pattern_len - shift) < v) {
                    u = pattern_len - shift;
                }
                else {
                    u = v;
                }
                //u = min(pattern_len - shift, v);
            }
            else {
                if (turbo_Shift < bad_symbol_shift) {
                    if (shift < (u + 1)) {
                        shift = u + 1;
                    }
                    //shift = max(shift,u+1);
                }
                u = 0;
            }
        }
        i += shift;
    }
    return false;
};

///////////////////////////////////////////

void read_file(vector<key>& arr, const string& filename) {
    string line;
    int count = 0;
    int p1, p2, p3, p4 = 0;

    ifstream in(filename);
    if (in.is_open()) {
        while (getline(in, line)) {
            count += 1;
            key k;
            k.group.l = line[0];
            k.group.num = stoi(line.substr(1, 4));
            p1 = line.find_first_of(' ');
            p2 = line.find_first_of(' ', p1 + 1);
            k.fio.surname = line.substr(p1 + 1, p2 - p1 - 1);
            p3 = line.find_first_of(' ', p2 + 1);
            k.fio.name = line.substr(p2 + 1, p3 - p2 - 1);
            p4 = line.find_last_of(' ');
            k.fio.patronymic = line.substr(p3 + 1, p4 - p3 - 1);
            k.dir.title = line.substr(p4 + 1);
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
            file << arr[i].group.l << arr[i].group.num << " " << arr[i].fio.surname << " " << arr[i].fio.name << " " << arr[i].fio.patronymic << " " << arr[i].dir.title << " " << arr[i].s << "\n";
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
    int pattern_title_count = 2; // количество вхождений паттерна в строке направления

    auto startRK = high_resolution_clock::now(); // время начала 
    for (int i = 0; i < n; i++) {

        string str_fio = arrRK[i].to_String_Fio();// строка поля фио
        int str_fio_len = str_fio.length(); // длина строки фио
        int str_fio_hash = first_hash(str_fio, pattern_fio_len); // хэш фио

        string str_title = arrRK[i].dir.title; // строка поля направления
        int str_title_len = str_title.length(); // длина строки направления
        int str_title_hash = first_hash(str_title, pattern_title_len); // хэш направления

        if (RK(str_fio, str_fio_len, str_fio_hash, pattern_fio, pattern_fio_len, pattern_fio_hash, pattern_fio_count) && RK(str_title,str_title_len,str_title_hash,pattern_title,pattern_title_len,pattern_title_hash,pattern_title_count)) {
            answer.push_back(arrRK[i]);
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
    int pattern_title_count = 2; // количество вхождений паттерна в строке направления
    int good_title_suffix[pattern_max_len], bad_title_symbols[alphabet_size];
    good_suffix(pattern_title, pattern_title_len, good_title_suffix);
    bad_symbol(pattern_title, pattern_title_len, bad_title_symbols);

    auto startTBM = high_resolution_clock::now(); // время начала
    for (int i = 0; i < n; i++) {

        string str_fio = arrTBM[i].to_String_Fio();// строка поля фио
        int str_fio_len = str_fio.length(); // длина строки фио

        string str_title = arrTBM[i].dir.title; // строка поля направления
        int str_title_len = str_title.length(); // длина строки направления

        if ((TBM(str_fio, str_fio_len, pattern_fio, pattern_fio_len, pattern_fio_count,good_fio_suffixs,bad_fio_symbols)) && (TBM(str_title,str_title_len,pattern_title,pattern_title_len,pattern_title_count,good_title_suffix,bad_title_symbols))) {
            answer.push_back(arrTBM[i]);
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