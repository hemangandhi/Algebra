#ifndef ALGEBRA_PERUMTATIONS_H_
#define ALGEBRA_PERUMTATIONS_H_

#include <array>
#include <optional>
#include <ostream>
#include <set>

#include "group.h"

namespace groups {

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

} // namespace groups

#endif
