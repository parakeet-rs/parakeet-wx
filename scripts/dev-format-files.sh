#!/bin/sh

cd "$(git rev-parse --show-toplevel)"

find parakeet-crypto -regex '.*\.\(cpp\|hpp\|cc\|cxx\|hh\|h\)' -exec clang-format -i {} \+
find src -regex '.*\.\(cpp\|hpp\|cc\|cxx\|hh\|h\)' -exec clang-format -i {} \+
