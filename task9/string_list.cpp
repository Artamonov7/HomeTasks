#include "string_list.h"
#include <cstring>

string_list::string_list() : string(), list<string>() {}

int string_list::length() const {
    return list<string>::length();
}

bool string_list::contains(const char *value) const {
    for (int i = 0; i < list<string>::length(); ++i){
        const char *elem = get(i).get_data();
        if (strcmp(elem, value) == 0 || strstr(elem, value) != nullptr)
            return true;
    }
    return false;
}

void string_list::add_string(const char* str) {
    list<string>::add(string(str));
}

string_list::~string_list() { }
