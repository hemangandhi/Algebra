#include "test-util.hpp"

#include <iostream>

//static
void TestCase::ExecuteTests(bool break_on_first){
    for(const auto creator: tests){
        std::unique_ptr<TestCase> test = std::move(creator());
        if(!(*test)()){
            std::cout << "Test " << test->ToString() << " failed!" << std::endl;
            if(break_on_first) break;
        }
    }
}

//protected static
bool TestCase::Register(TestCase::Creator c){
    tests.push_back(c);
    return true;
}
