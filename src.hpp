#ifndef SRC_HPP
#define SRC_HPP
#include <iostream>

struct Permutation { // 置换
    int* mapping; // 我觉得这里存啥你应该知道
    size_t size; // 上面那个数组的长度

    Permutation(size_t size) {
        this->size = size;
        this->mapping = new int[size];
        for (size_t i = 0; i < size; ++i) {
            this->mapping[i] = i;
        }
    }

    Permutation(const int* mapping, size_t size) {
        this->size = size;
        this->mapping = new int[size];
        for (size_t i = 0; i < size; ++i) {
            this->mapping[i] = mapping[i];
        }
    }

    Permutation(const Permutation& other) {
        this->size = other.size;
        this->mapping = new int[this->size];
        for (size_t i = 0; i < this->size; ++i) {
            this->mapping[i] = other.mapping[i];
        }
    }

    Permutation& operator=(const Permutation& other) {
        if (this != &other) {
            delete[] this->mapping;
            this->size = other.size;
            this->mapping = new int[this->size];
            for (size_t i = 0; i < this->size; ++i) {
                this->mapping[i] = other.mapping[i];
            }
        }
        return *this;
    }

    Permutation(Permutation&& other) noexcept : mapping(other.mapping), size(other.size) {
        other.mapping = nullptr;
        other.size = 0;
    }

    Permutation& operator=(Permutation&& other) noexcept {
        if (this != &other) {
            delete[] mapping;
            mapping = other.mapping;
            size = other.size;
            other.mapping = nullptr;
            other.size = 0;
        }
        return *this;
    }

    ~Permutation() {
        delete[] mapping;
    }

    void apply(int* permutation) const {
        int* temp = new int[size];
        for (size_t i = 0; i < size; ++i) {
            temp[i] = permutation[mapping[i]];
        }
        for (size_t i = 0; i < size; ++i) {
            permutation[i] = temp[i];
        }
        delete[] temp;
    }

    Permutation operator*(const Permutation& other) const {
        Permutation result(size);
        for (size_t i = 0; i < size; ++i) {
            result.mapping[i] = mapping[other.mapping[i]];
        }
        return result;
    }

    Permutation inverse() const {
        Permutation result(size);
        for (size_t i = 0; i < size; ++i) {
            result.mapping[mapping[i]] = i;
        }
        return result;
    }

    friend std::ostream& operator<<(std::ostream& os, const Permutation& p) {
        os << "[";
        for (size_t i = 0; i < p.size; ++i) {
            os << p.mapping[i];
            if (i < p.size - 1) {
                os << " ";
            }
        }
        os << "]";
        return os;
    }
};

struct Transposition { // 对换
    int a, b;

    Transposition() {
        a = 0;
        b = 0;
    }

    Transposition(int a, int b)  {
        this->a = a;
        this->b = b;
    }

    void apply(int* permutation, size_t size) const {
        int temp = permutation[a];
        permutation[a] = permutation[b];
        permutation[b] = temp;
    }

    friend std::ostream& operator<<(std::ostream& os, const Transposition& t) {
        os << "{" << t.a << " " << t.b << "}";
        return os;
    }

    Permutation toPermutation(size_t size) const {
        Permutation p(size);
        p.mapping[a] = b;
        p.mapping[b] = a;
        return p;
    }
};

struct Cycle { // 轮换
    int* elements;
    size_t size;

    Cycle(const int* elements, size_t size) {
        this->size = size;
        this->elements = new int[size];
        for (size_t i = 0; i < size; ++i) {
            this->elements[i] = elements[i];
        }
    }

    Cycle(const Cycle& other) {
        this->size = other.size;
        this->elements = new int[this->size];
        for (size_t i = 0; i < this->size; ++i) {
            this->elements[i] = other.elements[i];
        }
    }

    Cycle& operator=(const Cycle& other) {
        if (this != &other) {
            delete[] this->elements;
            this->size = other.size;
            this->elements = new int[this->size];
            for (size_t i = 0; i < this->size; ++i) {
                this->elements[i] = other.elements[i];
            }
        }
        return *this;
    }

    Cycle(Cycle&& other) noexcept : elements(other.elements), size(other.size) {
        other.elements = nullptr;
        other.size = 0;
    }

    Cycle& operator=(Cycle&& other) noexcept {
        if (this != &other) {
            delete[] elements;
            elements = other.elements;
            size = other.size;
            other.elements = nullptr;
            other.size = 0;
        }
        return *this;
    }

    ~Cycle() {
        delete[] elements;
    }

    void apply(int* permutation, size_t size) const {
        Permutation p = toPermutation(size);
        p.apply(permutation);
    }

    friend std::ostream& operator<<(std::ostream& os, const Cycle& c) {
        os << "{";
        for (size_t i = 0; i < c.size; ++i) {
            os << c.elements[i];
            if (i < c.size - 1) {
                os << " ";
            }
        }
        os << "}";
        return os;
    }

    Permutation toPermutation(size_t size) const {
        Permutation p(size);
        if (this->size > 0) {
            for (size_t i = 0; i < this->size; ++i) {
                p.mapping[elements[i]] = elements[(i + 1) % this->size];
            }
        }
        return p;
    }
};

#endif