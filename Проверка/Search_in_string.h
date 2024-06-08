#pragma once

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
        string fio;
    }fio;

    struct Direction {
        string title; // название направления
    }dir;
};


const int prime_number = 2;

int pow_for_hash(int num, int exp) {// функция для возведения в степень
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

bool RK(string const str, int& str_len, int& str_hash, string const& pattern, int const& pattern_len, int const& pattern_hash, int const& pattern_count) { // рабин-карп
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

void bad_symbol(const string& pattern, int& pattern_len, int bad_symbols[]) {
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

bool TBM(string const str, int str_len, string const pattern, int pattern_len, int pattern_count, int good_suffixs[pattern_max_len], int bad_symbols[alphabet_size]) {
    int count = 0; // счётчик вхождения паттерна
    int str_index = 0; // текущий индекс в тексте
    int good_suffix_rem = 0; // Остаток от сдвига по "Хорошему суффиксу"
    int pattern_shift = pattern_len; // сдвиг шаблона
    while (str_index <= str_len - pattern_len) { // идём по строке
        int pattern_index = pattern_len - 1; // текущий индекс в шаблоне

        while ((pattern_index >= 0) && (pattern[pattern_index] == str[str_index + pattern_index])) { // Проверка совпадения символов шаблона и текста
            --pattern_index; // сдвигаемся по шаблону
            if ((good_suffix_rem != 0) && (pattern_index == pattern_len - 1 - pattern_shift)) {// проверка условия хорошего суффикса
                pattern_index -= good_suffix_rem;
            }
        }
        if (pattern_index < 0) {// нашлось вхождение
            count++;// добавляем
            if (count == pattern_count) {
                return true; // если кол-во равно, то выходим
            }
            pattern_shift = good_suffixs[0];
            good_suffix_rem = pattern_len - pattern_shift;
        }
        else { // если не совпал последний символ
            int v = pattern_len - 1 - pattern_index;
            int turbo_Shift = good_suffix_rem - v;
            int bad_symbol_shift = bad_symbols[str[str_index + pattern_index] - pattern_len + pattern_index + 1];
            pattern_shift = max_for_TBM(turbo_Shift, bad_symbol_shift, good_suffixs[pattern_index + 1]);
            if (pattern_shift == good_suffixs[pattern_index + 1]) {
                if ((pattern_len - pattern_shift) < v) {
                    good_suffix_rem = pattern_len - pattern_shift;
                }
                else {
                    good_suffix_rem = v;
                }
                //u = min(pattern_len - shift, v);
            }
            else {
                if (turbo_Shift < bad_symbol_shift) {
                    if (pattern_shift < (good_suffix_rem + 1)) {
                        pattern_shift = good_suffix_rem + 1;
                    }
                    //shift = max(shift,u+1);
                }
                good_suffix_rem = 0;
            }
        }
        str_index += pattern_shift; // обновляем текущий индекс в тексте
    }
    return false;
};