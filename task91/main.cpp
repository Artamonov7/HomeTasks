#include <iostream>
#include <vector>
#include <cstring>
#include "string.h"
#include "list.h"
#include "string_list.h"

void test_contains(iterable_object* obj, const char* value) {
    std::cout << "Contains \"" << value << "\": " << (obj->contains(value) ? "yes" : "no") << std::endl;
}

int main() {
    std::vector<iterable_object*> objects;
    int choice;

    do {
        std::cout << "\n1. Create string\n2. Create list<int>\n3. Create list<double>\n4. Create string_list\n";
        std::cout << "5. Show all contains tests\n6. Check length\n7. Delete object\n0. Exit\nChoice: ";
        std::cin >> choice;

        if (choice == 1) {
            char str[100];
            std::cout << "Enter string: ";
            std::cin >> str;
            objects.push_back(new string(str));
        }
        else if (choice == 2) {
            list<int>* lst = new list<int>();
            int n, val;
            std::cout << "How many ints? ";
            std::cin >> n;
            for (int i = 0; i < n; ++i) {
                std::cout << "Enter int: ";
                std::cin >> val;
                lst->add(val);
            }
            objects.push_back(lst);
        }
        else if (choice == 3) {
            list<double>* lst = new list<double>();
            int n;
            double val;
            std::cout << "How many doubles? ";
            std::cin >> n;
            for (int i = 0; i < n; ++i) {
                std::cout << "Enter double: ";
                std::cin >> val;
                lst->add(val);
            }
            objects.push_back(lst);
        }
        else if (choice == 4) {
            string_list* sl = new string_list();
            int n;
            char buf[100];
            std::cout << "How many strings in list? ";
            std::cin >> n;
            for (int i = 0; i < n; ++i) {
                std::cout << "Enter string: ";
                std::cin >> buf;
                sl->add_string(buf);
            }
            objects.push_back(sl);
        }
        else if (choice == 5) {
            char query[100];
            std::cout << "Enter search value: ";
            std::cin >> query;
            for (size_t i = 0; i < objects.size(); ++i) {
                std::cout << "Object " << i << " (len=" << objects[i]->length() << "): ";
                test_contains(objects[i], query);
            }
        }
        else if (choice == 6) {
            int idx;
            std::cout << "Index to get max length: ";
            std::cin >> idx;
            if (idx >= 0 && idx < (int)objects.size()) {
                std::cout << objects[idx]->max_length() << std::endl;
            }
        }
        else if (choice == 7) {
            int idx;
            std::cout << "Index to delete: ";
            std::cin >> idx;
            if (idx >= 0 && idx < (int)objects.size()) {
                delete objects[idx];
                objects.erase(objects.begin() + idx);
            }
        }
    } while (choice != 0);

    for (auto obj : objects) delete obj;
    return 0;
}
