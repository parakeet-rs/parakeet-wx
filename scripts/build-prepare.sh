#/bin/bash

cd "$(git rev-parse --show-toplevel)"

bash scripts/extract-i18n.sh
bash scripts/build-po.sh

rm -rf dist && mkdir -p dist

./scripts/prepare-ui-hidpi.py
./scripts/codegen.sh

cp -R locale/. dist/
find dist -name '*.po' -delete
find dist -name '*.pot' -delete
cp readme.enduser.txt dist/readme.txt
cp LICENSE dist/
