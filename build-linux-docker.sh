#!/bin/sh

set -ex

# Build
mkdir -p build/app
cd build/app
cmake ../..
make -j$(nproc)

# Prepare for packaging
rm -rf packaging/um-desktop
mkdir -p packaging/um-desktop
cp -R AppDir/. packaging/um-desktop
cp build/app/um-desktop packaging/um-desktop/AppRun

# Do package
if [ -z "${DEPLOY_GTK_VERSION}" ]; then
  DEPLOY_GTK_VERSION=3
fi
export DEPLOY_GTK_VERSION
linuxdeploy --appdir packaging/um-desktop --plugin gtk --output appimage
