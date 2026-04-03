#ifndef STRING_LIST_H
#define STRING_LIST_H

#include "string.h"
#include "list.h"

class string_list : public virtual string, public virtual list<string> {
public:
    string_list();
    virtual ~string_list();
    bool contains(const char *value) const override;
    int length() const override;
    void add_string(const char *str);
};

#endif
