#ifndef __TEST_H__
#define __TEST_H__

#include "utility.h"
#include "memory.h"
#include "vector.h"
#include "list.h"
#include <iostream>
#include <random>
#include <ctime>

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

void testVector() {
    srand((unsigned int)time(NULL));
    tinySTL::vector<int> v;
    for(int i = 0; i < 10; ++i) v.push_back(rand()%10);
    UNIT_TEST(true, v[v.size()-1] == v.back());
    UNIT_TEST(16, v.capacity());
    v.pop_back();
    v.erase(v.begin()+3);
    //print_elements(v);
    for(int i = 0; i < 10; ++i) {
        int randn = rand() % 8;
        auto ret_itr = v.insert(v.begin() + randn, -randn);
    }
    tinySTL::vector<int> w(v);
    UNIT_TEST(true, w[0] == v[0]);
    tinySTL::vector<int> u(tinySTL::move(v));
    UNIT_TEST(true, v.empty());
    tinySTL::vector<int> x;
    x = u;
    tinySTL::swap(x, v);
    UNIT_TEST(true, x.empty());
    tinySTL::vector<int> y;
    y = tinySTL::move(w);
    tinySTL::swap(v[0], v[3]);
    UNIT_TEST(true, y.front() == v[3]);
    y.resize(16);
    UNIT_TEST(16, y.size());
    y.clear();
    UNIT_TEST(true, y.empty()); 
}

void testList() {
    srand((unsigned int)time(NULL));
    tinySTL::list<int> l;
    for(int i = 0; i < 5; ++i) l.push_back(rand()%10);
    UNIT_TEST(5, l.size());
    for(int i = 0; i < 5; ++i) l.push_front(-rand()%10);
    //print_elements(l);
    for(int i = 0; i < 2; ++i) {
        l.pop_back();
        l.pop_front();
    }
    //print_elements(l);
    for(int i = 0; i < 4; ++i) {
        int randn = rand() % 5;
        auto ret_itr = l.insert(l.begin() + randn, -randn);
    }
    for(int i = 0; i < 4; ++i) {
        int randn = rand() % 5;
        auto ret_itr = l.erase(l.begin() + randn);
    }
    UNIT_TEST(6, l.size());
    tinySTL::list<int>w(l);
    tinySTL::list<int>u(tinySTL::move(w));
    UNIT_TEST(true, w.empty());
    w = u;
    tinySTL::list<int> x;
    x = tinySTL::move(u);
    UNIT_TEST(6, x.size());
    UNIT_TEST(0, u.size());
    x.clear();
    UNIT_TEST(0, x.size());
}


#endif