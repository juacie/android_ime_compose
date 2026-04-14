#ifndef _CAPI_ARRAY_REF_H_
#define _CAPI_ARRAY_REF_H_

#include <stddef.h>

template <typename _T>
class ArrayRef {
public:
    typedef _T value_type;
    typedef const _T* const_iterator_type;

    ArrayRef(const_iterator_type p, size_t _len)
            : ptr(p), len(_len) {}

    const_iterator_type begin() const {
        return ptr;
    }

    const_iterator_type end() const {
        return ptr + len;
    }

    size_t size() const {
        return len;
    }

    bool operator==(const ArrayRef &rhs) const = delete;

private:
    const _T* ptr;
    size_t len;
};

#endif
