#ifndef LIST_H
#define LIST_H

#include "iterable_object.h"
#include "string.h"

template <typename T>
class list : public virtual iterable_object {
    T *arr;
    int size;
    int cap;
protected:
    void resize();
public:
    list();
    list(const list& other);
    list& operator=(const list& other);
    virtual ~list();

    void add(const T& item);
    int length() const override;
    bool contains(const char *value) const override;

    const T& get(int index) const;
    T& get(int index);
};

#endif
