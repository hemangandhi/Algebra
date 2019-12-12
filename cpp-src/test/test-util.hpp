#ifndef ALGEBRA_TEST_TEST_UTIL_H_
#define ALGEBRA_TEST_TEST_UTIL_H_

#include <string>
#include <vector>
#include <functional>
#include <memory>

struct TestCase {
    typedef std::function<std::unique_ptr<TestCase>()> Creator;
    virtual bool operator()() = 0;
    virtual std::string ToString() = 0;

    static void ExecuteTests(bool break_on_first);

    protected:
        static bool Register(Creator c);

    private:
        static std::vector<Creator> tests;
};

#define MAKE_TEST(name)\
    struct name: public TestCase{\
        bool operator()() override;\
        std::string ToString(){ return #name;}\
        static const TestCase::Creator maker = [](){\
            return std::make_unique<name>();\
        };\
        static const bool registered = TestCase::Register(maker);\
    };\
    bool name::operator()()

#endif
