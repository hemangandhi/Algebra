#ifndef ALGEBRA_GROUP_H_
#define ALGEBRA_GROUP_H_

#include <optional>
#include <string>
#include <vector>

#include "predicate.hpp"

namespace algebra {

class Subgroup;

template <typename Element>
virtual class Group {
    virtual bool Contains(const Element& e) const = 0;
    virtual std::optional<std::vector<Element>> UnderlyingSet() const = 0;
    virtual Element Identity() const = 0;
    virtual Element Multiply(const Element& e1, const Element& e2) const = 0;
    virtual Element Invert(const Element& e) = 0;

    virtual void GenerateSubgroups(predicate::GroupPredicate cond, std::vector<Subgroup *>* output);
};

template <typename Element>
virtual class Subgroup::Group<Element> {
    virtual const Group * GetParent() const = 0;
    virtual bool Normal() const = 0;
    virtual const Group * Quotient() const = 0;
}

} // namespace algebra

#endif
