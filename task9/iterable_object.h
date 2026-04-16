#ifndef ITERABLE_OBJECT_H
#define ITERABLE_OBJECT_H

class iterable_object {
public:
    virtual ~iterable_object() {}
    virtual int length() const = 0;
    virtual int max_length() const { return 0;}
    virtual bool contains(const char* value) const = 0;
};

#endif
