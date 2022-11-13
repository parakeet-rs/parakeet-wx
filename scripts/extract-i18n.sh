#!/bin/sh
cd "$(git rev-parse --show-toplevel)"

xgettext -d src -s --keyword=_ -p ./locale -o um-desktop.pot $(find ./src \( -name '*.cpp' -o -name '*.h' \) -print)
