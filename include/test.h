#ifndef __TEST_H__
#define __TEST_H__

#include "string.h"
#include "complex.h"
#include <iostream>

namespace tinySTL {
    class test {
    public:
        static void test_complex();
        static void test_string();
    };

    void test::test_complex() {
        complex c1(2.0, 1.0);
        complex c2(4.0);
        std::cout << c1 << ' ' << c2 << std::endl;
        std::cout << c1 + c2 << ' ' << (c1 += c2) << std::endl;
        std::cout << (c1 == c2) << ' ' << (c2 == c2) << std::endl;  
    }

    void test::test_string() {
        String s1("hello");
        String s2("world");
        String s3(s2);
        s3 = s1;
        std::cout << s1 << ' ' << s2 << ' ' << s3 << std::endl;     
    }
}

#endif