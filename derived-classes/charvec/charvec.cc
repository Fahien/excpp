#include <cstdio>

/// Consider this class, define new_charvec to allocate contiguous memory for a CharVec object so
/// that the elements can be indexed through element as shown. Under what circumstances does this
/// trick cause serious problems?
/// Considering this exercise is in the derived classes section, the answer would be probably
/// related to implementing a CharVec subclass, which by storing its own attributes in the area
/// of memory next to element, would overlap the same area of memory dedicated to elements.
class CharVec {
  public:
    static CharVec* new_charvec(int s);

    char& operator[](int i) { return element[i]; }

    private:
    int size;
    char element[1];
};

CharVec* CharVec::new_charvec(int s) {
    void* mem = ::operator new(sizeof(CharVec) + (s - 1) * sizeof(element[0]));

    auto ret = reinterpret_cast<CharVec*>(mem);
    ret->size = s;

    return ret;
}

int main()
{
    auto charvec = CharVec::new_charvec(10);

    for (size_t i = 0; i < 10; ++i) {
        (*charvec)[i] = 'a' + i;
        std::printf("%c ", (*charvec)[i]);
    }

    return 0;
}
