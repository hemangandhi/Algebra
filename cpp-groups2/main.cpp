#include <iostream>
#include <optional>
#include <ostream>
#include <set>
#include <concepts>
#include <algorithm>
#include <array>

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

// Ints mod Mod.
// GroupElement.
template <int Mod>
class ModInt {
 public:
  // We'll allow implicit conversion for elegance.
  ModInt(int n) : value_(n % Mod) {}
  ModInt<Mod> operator+(ModInt<Mod> m) const {
    return ModInt(value_ + m.value_);
  }

  // This is for the container in Group.
  bool operator<(const ModInt<Mod>& other) const {
    return value_ < other.value_;
  }
  bool operator==(const ModInt<Mod>& other) const {
    return value_ == other.value_;
  }

  // Sticking to positive bc % operator might round the wrong way
  // (I'm too lazy to check and this definition is also correct.)
  ModInt<Mod> operator-() const { return Mod - value_; }

  int value() const { return value_; }

 private:
  // Invariant: this will always be modulo Mod.
  int value_;
};

// A way to have abuse of notation for abelian things.
// This is since Abelian groups mostly use `+` (which is also how people tend
// to think about integers as groups). Anything you can wrap in this class
// should be a GroupElement except with operator+ instead of operator*.
// If Ab has an operator*, that information is lost.
template <typename Ab>
requires requires(Ab a1, Ab a2) {
  { a1 + a2 } -> std::convertible_to<Ab>;
  { -a1 } -> std::convertible_to<Ab>;
  std::totally_ordered<Ab>;
}
class AbusePlusNotation {
 public:
  AbusePlusNotation(Ab v) : victim_(v) {}
  AbusePlusNotation<Ab> operator*(const AbusePlusNotation<Ab>& a) const {
    return victim_ + a.victim_;
  }
  AbusePlusNotation operator-() const { return -victim_; }

  // Ordering is needed for GroupElement.
  bool operator<(const AbusePlusNotation<Ab>& other) const { return victim_ < other.victim_; }
  bool operator==(const AbusePlusNotation<Ab>& other) const { return victim_ == other.victim_; }
  
  friend std::ostream& operator<<(std::ostream& o, const AbusePlusNotation<Ab>& m) {
    o << m.victim_;
    return o;  
  }

 private:
  Ab victim_;
};

template <int Mod>
std::ostream& operator<<(std::ostream& o, const ModInt<Mod>& m) {
  o << m.value();
  return o;
}

typedef Group<AbusePlusNotation<ModInt<2>>> Z2;

// Permutation groups
template<size_t N>
class Permutation{
 public:
  template<size_t M>
  requires (M <= N)
    explicit Permutation(const std::array<int, M>& dests) {
    for(size_t i = 0; i < M; i++){
      dests_[i] = dests[i];
    }
    for(size_t i = M; i < N; i++) {
      dests_[i] = i;
    }
  }

  template<size_t M>
  requires (M <= N)
  explicit Permutation(const Permutation<M>& other) : Permutation(other.get_mapping()) {}

  Permutation<N> operator-() const {
    std::array<int, N> new_dests;
    for(size_t i = 0; i < N; i++) {
      new_dests[dests_[i]] = i;
    }
    return Permutation<N>(new_dests);
  }

  // Note: this follows the convention of function composition so that f * g is
  // "apply g and then f".
  Permutation<N> operator*(const Permutation<N>& other) const {
    std::array<int, N> new_dests;
    for(size_t i = 0; i < N; i++) {
      new_dests[i] = dests_[other.dests_[i]];
    }
    return Permutation<N>(new_dests);
  }

  // A lexicographic order for sorting permutations.
  bool operator==(const Permutation<N>& other) const{
    size_t i;
    for(i = 0; i < N && dests_[i] == other.dests_[i]; i++);
    return i == N;
  }
  bool operator<(const Permutation<N>& other) const {
    size_t i;
    for(i = 0; i < N && dests_[i] == other.dests_[i]; i++);
    return i < N && dests_[i] < other.dests_[i];
  }

  // Get the generators for the permutation group on N elements.
  static std::set<Permutation<N>> GetGroupGenerators() {
    std::array<int, 2> two_cycle{1, 0};
    std::array<int, N> big_cycle;
    big_cycle[0] = N - 1;
    for(size_t i = 1; i < N; i++) {
      big_cycle[i] = i - 1;
    }
    return std::set{Permutation<N>(Permutation<2>(two_cycle)), Permutation<N>(big_cycle)};
  }

  friend std::ostream& operator<<(std::ostream& o, const Permutation<N>& s) {
    o << "[ ";
    for(size_t i = 0; i < N; i++) {
      if (i > 0) o << ", ";
      o << s.dests_[i];
    }
    o << "]";
    return o;
  }

  const std::array<int, N> get_mapping() const { return dests_; }
  
 private:
  std::array<int, N> dests_;
};

int main(int argc, char** argv) {
  Z2 z = *Z2::Create({AbusePlusNotation(ModInt<2>(1))});
  std::cout << z << std::endl;;

  auto g = Permutation<5>::GetGroupGenerators();
  auto s5 = Group<Permutation<5>>::Create(g);
  if (s5.has_value()) {
    std::cout << *s5 << std::endl;
    std::cout << s5->elements().size();
  }
  else std::cout << "s5 isn't real.";
  return 0;
}
