#!/bin/sh

set -ex

# Build
mkdir -p build/app
cd build/app
cmake ../..
make -j$(nproc)

# Prepare for packaging
rm -rf packaging/src
mkdir -p packaging/src
cp -R AppDir/. packaging/src
cp build/app/src packaging/src/AppRun

# Do package
if [ -z "${DEPLOY_GTK_VERSION}" ]; then
  DEPLOY_GTK_VERSION=3
fi
export DEPLOY_GTK_VERSION
linuxdeploy --appdir packaging/src --plugin gtk --output appimage
