#include <assert.h>
#include <stdlib.h>

void * mem_pick(int n) {
    assert(n >= 0);
    void * p = calloc(1, n);
    assert(p != NULL);
    return p;
}

void mem_drop(void * p) {
    if (p != NULL) {
        free(p);
    }
}

void mem_reset(void * p, int n) {
    assert(p != NULL);
    assert(n > 0);
    byte * q = (byte *)p;
    while (n--) {
        *q++ = 0;
    }
}

boolean mem_resize(void ** p, int n) {
    assert(n > 0);
    void * q = realloc(*p, n);
    if (q != NULL) {
        *p = q;
        return true;
    }
    return false;
}

int mem_compare(const byte * p1, int p1size, const byte * p2, int p2size) {
    assert(p1size >= 0);
    assert(p2size >= 0);

    if (p1 == p2) {
        return 0;
    } else if (p1 == NULL) {
        return -1;
    } else if (p2 == NULL) {
        return 1;
    }

    int n = p1size < p2size ? p1size : p2size;
    if (n >= 24) {
        const word * w1 = (const word *) p1;
        const word * w2 = (const word *) p2;
        while (n >= sizeof(word)) {
            if (*w1 != *w2) {
                return *w1 < *w2 ? -1 : 1;
            }
            ++w1;
            ++w2;
            n -= sizeof(word);
        }

        p1 = (const byte *) w1;
        p2 = (const byte *) w2;
    }
    while (n > 0 && *p1 == *p2) {
        ++p1;
        ++p2;
        --n;
    }

    return n == 0 ? (p1size - p2size) : (*p1 - *p2);
}

void mem_copy(byte * dst, const byte * src, int n) {
    assert(dst != NULL);
    assert(src != NULL);
    assert(n >= 0);
    // XXX fail in case of overlapping

    const word * src1 = (const word *) src;
    word * dst1 = (word *) dst;
    while (n >= sizeof(word)) {
        *dst1++ = *src1++;
        n -= sizeof(word);
    }

    src = (const byte *) src1;
    dst = (byte *) dst1;
    while (n-- > 0) {
        *dst++ = *src++;
    }
}

int32 hash_bkdr(const byte * p, const int n) {
    // BKDR hash seed: 31, 131, 1313, 13131, 131313, ...
    // hashCode = hashCode * SEED + i;

    int32 hashCode = 0;
    const byte * p1 = p + n;
    while (p < p1) {
        hashCode = (hashCode << 5) - hashCode + (0xFF & *p++);
    }
    return hashCode;
}

int32 hash_djb(const byte * p, const int n) {
    // DJB hash seed: 5381
    // hashCode = hashCode * 33 + i static const int SEED = 33;

    int32 hashCode = 5381;
    const byte * p1 = p + n;
    while (p < p1) {
        hashCode = (hashCode << 5) + hashCode + (0xFF & *p++);
    }
    return hashCode;
}

static void setBits(byte bits, byte targetBits, boolean value) {
    if (value) {
        bits |= targetBits;
    } else {
        bits &= ~targetBits;
    }
}

static int testBits(byte bits, byte targetBits) {
    return bits & targetBits;
}

void setSingleBit(const void * stream, int offset, boolean value) {
    assert(stream != NULL);
    assert(offset >= 0);
    const byte * a = (const byte *) stream;
    setBits(a[offset >> 3], 1 << (7 - (offset & 7)), value);
}

public
boolean testSingleBit(const void * stream, int offset) {
    assert(stream != NULL);
    assert(offset >= 0);
    const byte * a = (const byte *) stream;
    return testBits(a[offset >> 3], 1 << (7 - (offset & 7))) != 0 ? true : false;
}
