#!/bin/sh

find um-crypto -regex '.*\.\(cpp\|hpp\|cc\|cxx\|hh\|h\)' -exec clang-format -i {} \+
find um-desktop -regex '.*\.\(cpp\|hpp\|cc\|cxx\|hh\|h\)' -exec clang-format -i {} \+
