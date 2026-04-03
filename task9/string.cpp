#include "string.h"
#include <cstring>

void string::set_data(const char *str){
    delete[] data;
    len = strlen(str);
    data = new char[len + 1];
    strcpy(data, str);
}

string::string() : data(nullptr), len(0) {
    data = new char[1];
    data[0] = '\0';
}

string::string(const char *str) : data(nullptr), len(0) {
    set_data(str);
}

string::string(const string& other) : data(nullptr), len(0) {
    len = other.len;
    data = new char[len + 1];
    strcpy(data, other.data);
}

string& string::operator=(const string& other) {
    if (this != &other) {  // проверка на самоприсваивание
        delete[] data;      // удаляем старые данные
        len = other.len;
        data = new char[len + 1];
        strcpy(data, other.data);
    }
    return *this;
}

string::~string(){
    delete[] data;
}

int string::length() const {
    return len;
}

bool string::contains(const char *substr) const {
    return strstr(data, substr) != nullptr;
}

const char *string::get_data() const {
    return data;
}
