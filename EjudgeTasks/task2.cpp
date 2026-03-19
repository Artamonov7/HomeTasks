#include <iostream>

int increment_counter() 
{
  static int counter = 0;
  return counter++;
}

struct Data {
    int value;
    Data (int v = 42) : value(v) {}
    ~Data () {
        increment_counter();
    }
    operator int() const { 
        return value; 
    }
    Data& operator=(int x) {
        value = x; return *this;
    }
};

class DataBuffer {
    Data *buf;
    size_t sz;

public:
    DataBuffer (size_t size) : buf(new Data[size]), sz(size) {}
    DataBuffer (const DataBuffer&) = delete;
    DataBuffer& operator=(const DataBuffer&) = delete;
    DataBuffer(DataBuffer&& other) noexcept : buf(other.buf), sz(other.sz) {
        other.buf = nullptr;
        other.sz = 0;
    }

    ~DataBuffer () {
        delete[] buf;
    }
    
    DataBuffer& operator=(DataBuffer&& other) noexcept {
        if (this == &other) 
            return *this;
        delete[] buf;
        buf = other.buf;
        sz = other.sz;
        other.buf = nullptr;
        other.sz = 0;
        return *this;
    }

    size_t size() const {
        return sz;
    }
    Data& operator[](size_t i) {
        return buf[i];
    }
    const Data& operator[](size_t i) const {
        return buf[i];
    }
};

void print_buffer(const DataBuffer &buffer) {
  for (size_t i = 0; i < buffer.size(); i++) 
    std::cout << "buffer[" << i << "] == " << buffer[i] << std::endl;
}

int main()
{
  {
    DataBuffer buffer_1{5}, buffer_2{3};
    std::cout << "buffer_1.size() == " << buffer_1.size() << std::endl;
    std::cout << "buffer_2.size() == " << buffer_2.size() << std::endl;

    for (size_t i = 0; i < buffer_2.size(); i++) 
      buffer_2[i] = i + 1;

    std::cout << "buffer_1[0] == " << buffer_1[0].value << std::endl;
    std::cout << "buffer_2[0] == " << buffer_2[0].value << std::endl;
    std::swap(buffer_1, buffer_2);
    // DataBuffer buffer_copy = buffer_1; // forbidden!
    std::cout << "buffer_1.size() == " << buffer_1.size() << std::endl;
    std::cout << "buffer_2.size() == " << buffer_2.size() << std::endl;
    print_buffer(buffer_1);
  }
  std::cout << "~Data() called " << increment_counter() << " times" << std::endl;
  return 0;
}



