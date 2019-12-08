#include <string>
#include <vector>

struct TestCase {
    virtual bool operator() = 0;
    virtual std::string ToString = 0;

    static void ExecuteTests();

    private:
        static std::vector<TestCase*> tests;
};
