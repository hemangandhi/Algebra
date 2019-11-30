#ifndef ALGEBRA_GROUP_H_
#define ALGEBRA_GROUP_H_

#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "predicate.hpp"

namespace algebra {

template <typename Element>
virtual class Set{
    virtual bool Contains(const Element& e) const = 0;
    // returns nullopt_t to say that the set is "infinite"
    virtual std::optional<std::vector<Element>> Enumerate() const = 0;
}

class Subgroup;
class GroupPredicate;

template <typename Element>
virtual class Group : public Set<Element>{
    virtual Element Identity() const = 0;
    virtual Element Multiply(const Element& e1, const Element& e2) const = 0;
    virtual Element Invert(const Element& e) = 0;

    // Tries to generate all the subgroups that satisfy a predicate. Returns true
    // iff all subgroups were generated. The expectation is that the generated subgroups
    // are moved out.
    virtual bool GenerateSubgroups(GroupPredicate cond, std::vector<std::unique_ptr<Subgroup>>* output);
};

template <typename Element>
virtual class Subgroup: public Group<Element> {
    virtual const Group * GetParent() const = 0;
    virtual bool Normal() const = 0;
    virtual const Group * Quotient() const = 0;
}

template <typename Element>
class SetPredicate: public predicate::Predicate<Set<Element>>{
    public:
        bool Validate(const Set<Element>& s) override {
            return s.Contains(e);
        }

        static SetPredicate<Element> Contains(const Element& e){
            return SetPredicate(e);
        }
    private:
        Element expected;
};

template <typename Element>
class ElementPredicate: public predicate::Predicate<Element>{
        bool Validate(const Set<Element>& s) override;

        static ElementPredicate<Element> OrderWhich(predicate::NumericPredicate is);
        static ElementPredicate<Element> Generates(predicate::NumericPredicate is);
    private:
        std::optional<Group> generates;
        std::optional<predicate::NumericPredicate> order_condition;
};

template <typename Element>
class GroupPredicate: public SetPredicate {
        bool Validate(const Set<Element>& s) override;
    private:
        ElementPredicate<Element> element;
        std::unique_pointer<GroupPredicate> has_subgroup;
};

} // namespace algebra

#endif
