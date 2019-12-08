
template <int N>
struct Primes;

namespace {

    template<int N, int K>
    struct Check{
        static constexpr bool value = (K % Primes<N - 1>::value != 0) && Check<N - 1, K>::value;
    };

    template<int K>
    struct Check<0, K>{
        static constexpr bool value = (K % 2 != 0);
    };

    template<int N, int K, bool Stop>
    struct Extend{
        static constexpr int value = -1;
    };

    template<int N, int K>
    struct Extend<N, K, true>{
        static constexpr int value = K;
    };

    template<int N, int K>
    struct Extend<N, K, false>{
        static constexpr int value = Extend<N, K + 1, Check<N, K + 1>::value>::value;
    };

}

template<int N>
struct Primes{
    static constexpr int value = Extend<N-1, Primes<N - 1>::value + 1, Check<N-1, Primes<N - 1>::value + 1>::value>::value;
};

template<>
struct Primes<0>{
    static constexpr int value = 2;
};
