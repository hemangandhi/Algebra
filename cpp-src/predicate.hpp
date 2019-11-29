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
class WrappedFunction: public Predicate {
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
operator& WrappedFunction<Structure>(const Predicate& p1, const Predicate& p2){
    return WrappedFunction([=](const Structure& s){
        return p1.Validate(s) && p2.Validate(s);
    });
}

template <typename Structure>
operator| WrappedFunction<Structure>(const Predicate& p1, const Predicate& p2){
    return WrappedFunction([=](const Structure& s){
        return p1.Validate(s) || p2.Validate(s);
    });
}

template <typename Structure>
operator^ WrappedFunction<Structure>(const Predicate& p1, const Predicate& p2){
    return WrappedFunction([=](const Structure& s){
        return p1.Validate(s) ^ p2.Validate(s);
    });
}

template <typename Structure>
operator~ WrappedFunction<Structure>(const Predicate& p1){
    return WrappedFunction([=](const Structure& s){
        return !p1.Validate(s);
    });
}

} // namespace predicate

#endif
