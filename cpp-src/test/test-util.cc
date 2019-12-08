#include "test-util.hpp"

#include <iostream>

TestCase::TestCase() {
    tests.push_back(this);
    idx = tests.size() - 1;
}

TestCase::~TestCase() {
    for(auto val = test.begin() + idx; val < test.end(); val++){
        val->idx--;
    }
    tests.erase(test.begin() + idx);
}

//static
void TestCase::ExecuteTests(bool break_on_first = false){
    for(const auto* test: tests){
        if(!*test()){
            std::cout << "Test " << test->ToString() << " failed!" << std::endl;
            if(break_on_first) break;
        }
    }
}
