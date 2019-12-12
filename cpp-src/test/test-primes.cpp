#include "test-util.hpp"
#include "../util/compile-time-primes.hpp"

MAKE_TEST(PrimeTest){
    return Primes<0>::value == 2 && Primes<5>::value == 13;
}
