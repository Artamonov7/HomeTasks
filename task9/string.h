#ifndef STRING_H
#define STRING_H

#include "iterable_object.h"

class string : public virtual iterable_object {
    char *data;
    int len;
protected:
    void set_data(const char* str);
public:
    string();
    string(const char *str);
    string(const string& other);
    virtual ~string();

    string& operator=(const string& other);

    int length() const override;
    bool contains(const char *substr) const override;
    const char *get_data() const;
};

#endif
