#/bin/bash

bash extract-i18n.sh
bash build-po.sh

rm -rf dist && mkdir -p dist

APP_VERSION="$(jq <vcpkg.json -r '.["version-string"]')"

./prepare-ui-hidpi.py

cp -R locale/. dist/
find dist -name '*.po' -delete
find dist -name '*.pot' -delete
cp readme.enduser.txt dist/readme.txt
cp LICENSE dist/
