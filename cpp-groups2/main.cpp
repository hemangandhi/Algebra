#include <iostream>
#include <optional>
#include <set>

#include "group.h"
#include "permutations.h"
#include "modular_nums.h"

int main(int argc, char** argv) {
  auto z = *groups::Z2::Create({groups::AbusePlusNotation(groups::ModInt<2>(1))});
  std::cout << z << std::endl;;

  auto g = groups::Permutation<5>::GetGroupGenerators();
  auto s5 = groups::Group<groups::Permutation<5>>::Create(g);
  if (s5.has_value()) {
    std::cout << *s5 << std::endl;
    std::cout << s5->elements().size();
  }
  else std::cout << "s5 isn't real.";
  return 0;
}
