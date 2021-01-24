# Some algebra

This repo has a Java library for algebra, and to prove a point, 2 C++ implementations.

Since this fork is mainly for the C++, that's what the README will focus on.

## cpp-src

This is the older source with:

1. some untested predicate system for querying about groups.
1. some broken unit testing framework (grumble grumble why can't I assign a lambda to a function).

## cpp-groups2

The author of the Java learned C++ and this is my take on that. His version is [a separate repo](https://github.com/AdamJamil/Algebra-cpp)
but since I have C++ code for similar stuff here and an explosion of meaningless repositories in states I don't remember, I thought it
simpler to stick my version in this fork.

This tries to follow the google C++ style guide, but without help from clang-format.

Also, this requires the use of gcc >= 10 for concepts. If using nix, see the .nix file here for the correct nix-shell invocation.