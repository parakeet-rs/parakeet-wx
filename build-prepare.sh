#/bin/bash

bash build-po.sh

rm -rf dist && mkdir -p dist

cp -R locale/. dist/
find dist -name '*.po' -delete
find dist -name '*.pot' -delete
cp readme.enduser.txt dist/readme.txt
cp LICENSE dist/
