#ifndef ALGEBRA_UTIL_COMPILE_TIME_PRIMES_H_
#define ALGEBRA_UTIL_COMPILE_TIME_PRIMES_H_

// A compile-time sieve implementation of the list of primes.
// Prime<N>::value is the Nth prime with Prime<0>::value as 2.
// Constructs N * Primes<N>::value

// Forward-declaration of primes since the sieve needs it.
template <int N>
struct Primes;

namespace {

    // Check if K is not divisible by all primes up to and including the
    // Nth one.
    template<int N, int K>
    struct Check{
        static constexpr bool value = (K % Primes<N>::value != 0) && Check<N - 1, K>::value;
    };

    // Base case: odd K are not divisible by the 0th prime (2).
    template<int K>
    struct Check<0, K>{
        static constexpr bool value = (K % 2 != 0);
    };

    // An upwards loop to the next prime. N is the number of primes known
    // so that K can be correctly checked for primality. Stop informs the
    // compiler when a check has passed and K is the next prime.
    template<int N, int K, bool Stop>
    struct Extend{
        // constexpr need to be initialized, but this template is
        // specialized exhaustively (for Stop being true or false),
        // so you'll never get this "-1"
        static constexpr int value = -1;
    };

    // If the check has passed, that is K is not divisible by any of the
    // first N primes, the extension of our list of primes is value K (the
    // N + 1st prime). (This is the base case.)
    template<int N, int K>
    struct Extend<N, K, true>{
        static constexpr int value = K;
    };

    // If the check failed, K is divisible by one of the first N primes, so
    // we check K + 1 (producing another Extend type to test that).
    template<int N, int K>
    struct Extend<N, K, false>{
        static constexpr int value = Extend<N, K + 1, Check<N, K + 1>::value>::value;
    };

}

// Initialize the search for the Nth prime by extending from the list of
// N-1 primes.
template<int N>
struct Primes{
    static constexpr int value = Extend<N-1, Primes<N - 1>::value + 1, Check<N-1, Primes<N - 1>::value + 1>::value>::value;
};

// The 0th prime is 2. This is the base case.
template<>
struct Primes<0>{
    static constexpr int value = 2;
};

#endif
