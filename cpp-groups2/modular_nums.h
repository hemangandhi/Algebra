#ifndef ALGEBRA_MODULAR_INTS_H_
#define ALGEBRA_MODULAR_INTS_H_

#include <ostream>
#include <set>
#include <concepts>

#include "group.h"

namespace groups {

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

} // namespace groups

#endif
