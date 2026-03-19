#include <iostream>

class Reverse {
    unsigned long long num;
    bool rd;
public:
    Reverse() : rd(false) {
        if (std:: cin >> num){
            rd = true;
            Reverse next;
        }
    }

    ~Reverse() {
        if (rd){
            static bool flag = false;
            if (!flag){
                flag = true;
                std:: cout << num;
                return;
            }
            std:: cout << ' ' << num;
        }
    }
};

int main()
{
    Reverse r;
    return 0;
}
