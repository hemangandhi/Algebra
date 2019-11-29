#ifndef ALGEBRE_PREDICATE_H_
#define ALGEBRE_PREDICATE_H_

#include <vector>
#include <optional>
#include <functional>

namespace predicate{

template <typename Structure>
virtual class Predicate{
    virtual bool Validate(const Structure&) = 0;
};

template <typename Structure>
class WrappedFunction: public Predicate<Structure> {
    public:
        explicit WrappedFunction(std::function<const Structure&, bool> pred) : predicate(pred) {}

        bool Validate(const Structure& e) override {
            return predicate(e);
        }
    private:
        std::function<const Structure&, bool> predicate;
};

/*
 * NOTE: the lambdas below seem to capture a copy of
 * the references' values (ie. a copy of the values
 * referred to), so lifetimes should not be a concern.
 *
 * This is an assumption I am unsure I've tested quite
 * enough.
 */

template <typename Structure>
operator& WrappedFunction<Structure>(const Predicate<Structure>& p1, const Predicate<Structure>& p2){
    return WrappedFunction([=](const Structure& s){
        return p1.Validate(s) && p2.Validate(s);
    });
}

template <typename Structure>
operator| WrappedFunction<Structure>(const Predicate<Structure>& p1, const Predicate<Structure>& p2){
    return WrappedFunction([=](const Structure& s){
        return p1.Validate(s) || p2.Validate(s);
    });
}

template <typename Structure>
operator^ WrappedFunction<Structure>(const Predicate<Structure>& p1, const Predicate<Structure>& p2){
    return WrappedFunction([=](const Structure& s){
        return p1.Validate(s) ^ p2.Validate(s);
    });
}

template <typename Structure>
operator~ WrappedFunction<Structure>(const Predicate<Structure>& p1){
    return WrappedFunction([=](const Structure& s){
        return !p1.Validate(s);
    });
}

// A predicate on numbers, looking at comparisons, divisibility,
// and primality. The static creators are easier to use than the
// constructor, but the constructor is public for masochists.
class NumericPred: public Predicate<int> {
    public:
        bool Validate(const int& in){
            if(div_int != 0 && in % div_int != 0) return false;
            if(upper_bound.has_value() && in > upper_bound) return false;
            if(lower_bound.has_value() && in < lower_bound) return false;
            return !prime || CheckPrime(in);
        }

    // TODO: custom (specialized?) overloads of the above &, |, ^, ~ to
    // not bloat memory with wacky lambda thingies and just make the good validator.

    // Factories! Make your numeric predicates in a lyrical English-sounding interface!
    static NumericPred IsNumber(int n){
        // Abusing the inclusivity of the bounding stuff
        return NumericPred(0, {n}, {n}, false);
    }
    static NumericPred IsAtLeast(int n){
        return NumericPred(0, {std::nullopt_t}, {n}, false);
    }
    static NumericPred IsAtMost(int n){
        return NumericPred(0, {n}, {std::nullopt_t}, false);
    }
    static NumericPred Divides(int n){
        return NumericPred(n, {std::nullopt_t}, {std::nullopt_t}, false);
    }
    static NumericPred IsPrime(){
        return NumericPred(0, {std::nullopt_t}, {std::nullopt_t}, true);
    }

    private:
        // TODO: better ways to express stuff like "it's OK to Miller-Rabin"
        // or something so that this isn't the trashiest. (Ooh, template param?)
        bool CheckPrime(const int& in){
            for(int i = 2; i < in; i++){ // Recall: loop won't run if in == 2
                if(in % i == 0) return false;
            }
            return true;
        }

        int div_int;
        std::optional<int> upper_bound, lower_bound;
        bool prime
};

} // namespace predicate

#endif
