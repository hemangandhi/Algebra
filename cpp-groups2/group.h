#ifndef ALGEBRA_GROUP_H_
#define ALGEBRA_GROUP_H_


#include <array>
#include <algorithm>
#include <concepts>
#include <optional>
#include <ostream>
#include <set>


namespace groups {

// Group concept: requires ordering and multiplication.
template <typename T>
concept GroupElement = requires(T e1, T e2) {
  // Product.
  { e1 * e2 } -> std::convertible_to<T>;
  // Inverses (notational note: this is inconsistent with the use of * above but
  // C++ doesn't have a canonical exponentiation operator or multiplicative
  // inverse unary thing, so we'll just use unary minus).
  { -e1 } -> std::convertible_to<T>;
  // Ordering is for the container so doesn't need to behave well with the math.
  std::totally_ordered<T>;
};

// A group of elements.
template <GroupElement E>
class Group {
 public:
  // Creates the group from the elements if possible. However, the following
  // properties are ensured:
  // - the presence of a unique identity
  // - the inverses behaving like inverses
  // - the identity being an identity.
  // std::nullopt is returned when the properties cannot be assured.
  // Since it requires a different, slower, traversal, assurances of
  // associativity are provided through a separate instance method.
  // This algorithm does, however, require associativity for correctness.
  static std::optional<Group> Create(const std::set<E>& generators) {
    std::set<E> elements;
    std::optional<E> identity;
    // Step 1: copy in the generators and generate cycles.
    bool set_identity = false;
    for(const E& g: generators) {
      std::optional<E> prev;
      for(E gn = g * g; gn != g; gn = g * gn) {
	prev = gn;
	elements.insert(gn);
      }
      if (prev.has_value() && !identity.has_value()) {
        identity = prev;
      } else if (identity != prev) {
	// If the identity is non-unique, the algorithm below may not work, so
	// we quit while we're ahead.
	return std::nullopt;
      } else if (!prev.has_value()) {
	// prev was unset because we saw the trivial group
	if (generators.size() == 1) {
	  return Group<E>(g, generators, true);
	}
	return std::nullopt;
      }
      elements.insert(g);
    }

    // Step 1b: run away if there's one generator
    if(generators.size() == 1) {
      return Group<E>(*identity, elements, true);
    }

    // Step 2: Bellman-Ford time.
    // TODO: use temps to avoid redundant use of *.
    bool abelian = true;
    for(bool augmented = true; augmented;) {
      augmented = false;
      for (const E& x: elements) {
	if (x * (-x) != *identity
	    || x * *identity != x
	    || *identity * x != x) return std::nullopt;
	if (x == *identity) continue;
	for (const E& y: elements) {
	  if (y == *identity || x == y) continue;
	  if (!elements.contains(x * y)) {
	    // N.B. if x < x * y, we need not run the outer loop again.
	    augmented = !(x < (x * y));
	    // If we're using the optimization, we may need to check y * x
	    // to be sure. But that element could be small enough.
	    if (!augmented && !elements.contains(y * x)) {
	      elements.insert(y * x);
	      augmented = !(x < (y * x));
	    }
	    elements.insert(x * y);
	    abelian = x * y == y * x;
	  } // If statement for x * y.
	}
      }
    } // Bellman-Ford outer loop
    return Group<E>(*identity, elements, abelian);
  }

  const std::set<E>& elements() const { return elements_; }
  const E& identity() const { return identity_; }
  bool is_abelian() const { return abelian_; }

  bool TestAssociativity() const {
    for (const E& a: elements_) {
      for (const E& b: elements_) {
	for (const E& c: elements_) {
	  return (a * b) * c == a * (b * c);
	}
      }
    }
  }

 private:
  Group(const E& identity, const std::set<E>& elements, bool abelian)
    : elements_(elements), identity_(identity), abelian_(abelian) {}
  std::set<E> elements_;
  E identity_;
  bool abelian_;
};

// Print all groups of printable things.
template <GroupElement E>
requires requires(E e, std::ostream o) { o << e; }
std::ostream& operator<<(std::ostream& o, const Group<E>& g) {
  if (g.is_abelian()) o << "abelian ";
  bool first = true;
  o << "{ ";
  for (const E& e : g.elements()) {
    if (!first) o << ", ";
    o << e;
    first = false;
  }
  o << " }";
  return o;
}

} // namespace groups

#endif
