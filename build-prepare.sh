#/bin/bash

bash extract-i18n.sh
bash build-po.sh

rm -rf dist && mkdir -p dist

./prepare-ui-hidpi.py

cp -R locale/. dist/
find dist -name '*.po' -delete
find dist -name '*.pot' -delete
cp readme.enduser.txt dist/readme.txt
cp LICENSE dist/
