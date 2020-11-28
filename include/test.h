#ifndef __TEST_H__
#define __TEST_H__

#include "utility.h"
#include "memory.h"
#include <iostream>

static int main_ret = 0;
static int test_count = 0;
static int test_pass = 0;

#define UNIT_TEST_BASE(equality, expect, actual) \
    do {\
        test_count++;\
        if (equality)\
            test_pass++;\
        else \
        { \
            std::cerr << __FILE__ << ":" << __LINE__ << ":  except: " \
                << expect << " actual: " << actual << std::endl;\
            main_ret = 1;\
        }\
    } while (0)

#define UNIT_TEST(expect, actual) UNIT_TEST_BASE((expect) == (actual), (expect), (actual))

template<typename T>
void print_elements(const T& container) {
    for(const auto& ele : container)
        std::cout << ele << " ";
    std::cout << std::endl;
}

template<typename T>
void print_element_with_pair(const T& container) {
    for(const auto& pair : container)
        std::cout << "[" << pair.first 
            << ", " << pair.second << "]" << std::endl;
    std::cout << std::endl;
}

void testUtility() {
    int i = 31, j = 42;
    tinySTL::swap(i, j);
    UNIT_TEST(31, j);
    UNIT_TEST(42, i);

    /*
    int arr0[5] = {1,2,3,4,5};
    int arr1[5] = {5,4,3,2,1};
    tinySTL::swap(arr0, arr1);
    UNIT_TEST(5, arr0[0]);
    */

    tinySTL::pair<std::int32_t, bool> p0; // (0, false)
    tinySTL::pair<std::int32_t, bool> p1(42, true);
    UNIT_TEST(true, p0 < p1); 
    tinySTL::pair<std::int32_t, bool> p2(tinySTL::move(p0));
    p0 = p1;
    UNIT_TEST(42, p0.first);
    p2 = tinySTL::move(p1);
    UNIT_TEST(42, p2.first);

    auto p = tinySTL::make_pair(1, std::string{ "pair" });
    UNIT_TEST(1, p.first);
    UNIT_TEST(std::string{ "pair" }, p.second);
    const auto cp = p;
    UNIT_TEST(1, cp.first);
    p.first = 2;
    UNIT_TEST(true, cp != p);
}

void testMemory() {
    tinySTL::shared_ptr<int> sp0;
    UNIT_TEST(false, static_cast<bool>(sp0));
    tinySTL::shared_ptr<int> sp1(new int(42));
    UNIT_TEST(true, sp1.unique());
    UNIT_TEST(42, *sp1);
    sp0 = sp1;
    UNIT_TEST(2, sp1.use_count());

    tinySTL::unique_ptr<float> up0(new float(1.0));
    UNIT_TEST(1.0, *up0); 
    tinySTL::unique_ptr<float> up1 = tinySTL::move(up0);
    UNIT_TEST(true, up0.get() == nullptr);
    up0.swap(up1);
    UNIT_TEST(1.0, *up0);
    UNIT_TEST(true, up1.get() == nullptr);

    tinySTL::weak_ptr<int> wp0(new int(3));
    UNIT_TEST(true, wp0.expired());
    tinySTL::weak_ptr<int> wp1{sp1};
    UNIT_TEST(2, wp1.use_count());
    tinySTL::weak_ptr<int> wp2 = tinySTL::move(wp1);
    auto sp2 = wp2.lock();
    UNIT_TEST(42, *sp2);
    UNIT_TEST(3, sp2.use_count());
}


#endif