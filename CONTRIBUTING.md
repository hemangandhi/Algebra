This is just a note on dev-related docs/ideas I have.

Note: this is going to just work in linux.

# The Testing "Framework"

I wanted to mess around with this sort of system myself. So it's weird.
To add a test:

1. Make a `.cpp` file for the test.
1. `#include "test-util.hpp"`. Of course, including other stuff is OK.
1. `MAKE_TEST(NameOfYourTest){ /* body, can be multi-line */}`
1. Add a rule to the Makefile to build a library as you need to.

To run tests, see `run_cpp_tests.sh`.

# Style

Google C++ style is the only one I know, so I try to somewhat adhere to it.
I don't like that field names should end with an underscore, so for now, I
disobey that. Furthermore, I will be making more things const than usual.

`compile-time-primes.hpp` is a mess, but hopefully a well-enough documented one.
It cannot be stylistically better since it is an implementation of a fairly bad
idea.
