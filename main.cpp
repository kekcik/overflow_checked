//
//  main.cpp
//  overflow_checked
//
//  Created by Иван Трофимов on 24.10.16.
//  Copyright © 2016 Иван Трофимов. All rights reserved.
//

#include <iostream>
#include <limits>
#include <assert.h>

#define oc overflow_checked

struct signNum {};
struct unsignNum {};

template <bool T>
struct check {
    typedef signNum type;
};
template <>
struct check<false> {
    typedef unsignNum type;
};

template <typename T>
void checkAdd(T f, T s, signNum) {
    assert(!(((s > 0) && (f > std::numeric_limits<T>::max() - s)) ||((s < 0) && (f < std::numeric_limits<T>::min() - s))));
}
template <typename T>
void checkAdd(T f, T s, unsignNum) {
    assert(f <= std::numeric_limits<T>::max() - s);
}

template <typename T>
void checkSub(T f, T s, signNum) {

    assert(!(((s > 0) && (f < std::numeric_limits<T>::min() + s)) || ((s < 0) && (f > std::numeric_limits<T>::max() + s))));
}
template <typename T>
void checkSub(T f, T s, unsignNum) {
    assert(f >= s);
}

template <typename T>
void checkMult(T f, T s) {
    if (f == 0 || s == 0) return;
    assert(((f * s / s) == f) &&
           ((f * s / f) == s));
}

template <typename T>
void checkDiv(T f, T s, signNum) {
    assert(!((f == std::numeric_limits<T>::min()) && (s == -1)) && s != 0);
}
template <typename T>
void checkDiv(T f, T s, unsignNum) {
    assert(s != 0);
}

template <typename T>
void checkUno(T number, signNum)
{
    assert(number != std::numeric_limits<T>::min());
}
template<typename T>
void checkUno(T number, unsignNum) {
    assert(number == 0);
}

template <typename T>
struct overflow_checked {
    
    oc<T>() {
        number = 0;
    }
    
    oc<T>(T s) {
        number = s;
    }
    
    oc<T> operator=(oc<T> const s) {
        number = s.number;
        return *this;
    }
    
    oc<T> operator+=(oc<T> const s) {
        checkAdd(number, s.number, typename check<std::numeric_limits<T>::is_signed>::type());
        number += s.number;
        return *this;
    }
    
    oc<T> operator-=(oc<T> const s) {
        checkSub(number, s.number, typename check<std::numeric_limits<T>::is_signed>::type());
        number -= s.number;
        return *this;
    }
    
    oc<T> operator*=(oc<T> const s) {
        checkMult(number, s.number);
        number *= s.number;
        return *this;
    }
    
    oc<T> operator/=(oc<T> const s) {
        checkDiv(number, s.number, typename check<std::numeric_limits<T>::is_signed>::type());
        number /= s.number;
        return *this;
    }
    
    oc<T> operator-() {
        checkUno(number, typename check<std::numeric_limits<T>::is_signed>::type());
        number = -number;
        return *this;
    }
    
private:
    T number;
};

template <typename T>
oc<T> operator+(oc<T> f, oc<T> s) {
    return f += s;
}

template <typename T>
oc<T> operator-(oc<T> f, oc<T> s) {
    return f -= s;
}

template <typename T>
oc<T> operator*(oc<T> f, oc<T> s) {
    return f *= s;
}

template <typename T>
oc<T> operator/(oc<T> f, oc<T> s) {
    return f /= s;
}

int main() {return 0;}
