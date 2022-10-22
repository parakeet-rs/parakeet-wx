#/bin/bash

bash build-po.sh

rm -rf dist && mkdir -p dist

APP_VERSION="$(jq <vcpkg.json -r '.["version-string"]')"

cat >um-crypto/include/um-crypto/constants.build_time.h <<EOF
#define UM_CRYPTO_VERSION "${APP_VERSION}"
EOF

cat >um-desktop/constants.build_time.h <<EOF
#define UM_DESKTOP_VERSION_STR "${APP_VERSION}"
EOF

cp -R locale/. dist/
find dist -name '*.po' -delete
find dist -name '*.pot' -delete
cp readme.enduser.txt dist/readme.txt
cp LICENSE dist/
