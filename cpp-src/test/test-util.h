#ifndef ALGEBRA_TEST_TEST_UTIL_H_
#define ALGEBRA_TEST_TEST_UTIL_H_

#include <string>
#include <vector>

struct TestCase {

    TestCase();

    ~TestCase();

    virtual bool operator() = 0;
    virtual std::string ToString = 0;

    static void ExecuteTests();

    private:
        int idx;
        static std::vector<TestCase*> tests;
};

#endif
