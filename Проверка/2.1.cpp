#include <iostream>
#include <vector> // для массива
#include <chrono> // для времени
#include <fstream> // для чтения и записи файла
#include <string> // для работы со строками
#include <iomanip> // для перевода времени из(fixed);
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

    const string to_String() { // для возвращения целой строки
        return fio.surname + " " + fio.name + " " + fio.patronymic + " " + dir.title;
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
//переписать чтобы искал количество паттернов и чтобы на каждое поле свой паттерн(вынести for в callRK и посокращать RK)
void RK(vector<key>& arr, long const& arr_len, string const& pattern, int const& pattern_len, int& pattern_hash, vector<key>& answer) { // рабин-карп
    for (int i = 0; i < arr_len; i++) {
        const string str = arr[i].to_String();
        int str_len = str.length();
        int str_hash = first_hash(str,pattern_len);
        for (int j = 0; j <= (str_len - pattern_len); j++) {
            if ((str_hash == pattern_hash) && check_equals_strings(str, pattern, j, j + pattern_len - 1, 0, pattern_len - 1)) {
                answer.push_back(arr[i]);
            }
            if (j < (str_len - pattern_len)) {
                str_hash = refresh_hash(str, j, j + pattern_len, str_hash, pattern_len);
            }
        }
    }
};

///////////////////////////////////////////

void TBM() {

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

    string const pattern = "SZYF"; // паттерн
    int pattern_len = pattern.length(); // длина паттерна
    int pattern_hash = first_hash(pattern, pattern_len); // хэш паттерна

    auto startRK = high_resolution_clock::now(); // время начала 
    RK(arrRK, n, pattern, pattern_len, pattern_hash, answer);
    auto endRK = high_resolution_clock::now(); // время конца
    duration<double> diffRK = endRK - startRK; // время работы алгоритма
    write_file(answer, diffRK, "Output_Rabin_Karp.txt"); // запись в файл
};

/*void Call_Turbo_Boyer_Moore() {
    vector<key>arrTBM;
    read_file(arrTBM,"Input.txt");
    long n = arrTBM.size();
    //время

};*/

int main() {
    Call_Rabin_Karp();
    //Call_Turbo_Boyer_Moore();
    return 0;
}