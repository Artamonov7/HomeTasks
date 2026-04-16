#include "string_list.h"
#include <cstring>

string_list::string_list() : string(), list<string>() {}

int string_list::length() const {
    return list<string>::length();
}

bool string_list::contains(const char *value) const {
    for (int i = 0; i < list<string>::length(); ++i){
        const char *elem = get(i).get_data();
        if (strstr(elem, value))
            return true;
    }
    return false;
}

int string_list::max_length() const {
    int max = 0;
    for (int i = 0; i < list<string>::length(); ++i)
        if (get(i).length() > max)
            max = get(i).length();
    return max;
}
void string_list::add_string(const char* str) {
    list<string>::add(string(str));
}

string_list::~string_list() { }
