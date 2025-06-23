#pragma once

#include <cassert>

class RefCountClass {
public:
    RefCountClass() : num_refs(1) {}
    RefCountClass(const RefCountClass&) : num_refs(1) {}
    virtual ~RefCountClass() { assert(num_refs == 0); }

    virtual void Add_Ref() { ++num_refs; }
    virtual void Release_Ref() {
        assert(num_refs > 0);
        if (--num_refs == 0) Delete_This();
    }
    int Num_Refs() const { return num_refs; }

protected:
    virtual void Delete_This() { delete this; }

private:
    int num_refs;
};

#define REF_PTR_SET(dst, src) do { if (src) (src)->Add_Ref(); if (dst) (dst)->Release_Ref(); (dst) = (src); } while (0)
#define REF_PTR_RELEASE(x) do { if (x) (x)->Release_Ref(); (x) = nullptr; } while (0)
#define NEW_REF(C, P) (new C P)
#define SET_REF_OWNER(P) (P)
