#!/bin/sh
xgettext -d um_desktop -s --keyword=_ -p ./locale -o um_desktop.pot $(find ./src \( -name '*.cpp' -o -name '*.h' \) -print)
