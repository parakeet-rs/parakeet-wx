#!/bin/sh
xgettext -d um-desktop -s --keyword=_ -p ./locale -o um-desktop.pot $(find ./um-desktop \( -name '*.cpp' -o -name '*.h' \) -print)
