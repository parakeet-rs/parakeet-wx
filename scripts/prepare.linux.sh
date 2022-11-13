#!/bin/sh
set -ex

cd "$(git rev-parse --show-toplevel)"

./vcpkg/bootstrap-vcpkg.sh --disableMetrics || {
  echo "ERROR: could not bootstrap vcpkg."
  exit 1
}

VCPKG_PORTS=vcpkg/ports
VCPKG_OVERLAYS=vcpkg_overlays

# Apply linux patches
for patch_file in patches/linux-*.patch; do
  PKG_NAME="$(basename "$patch_file" | sed -E 's/^linux-(.+)\.patch$/\1/')"
  PKG_PORT_PATH="${VCPKG_PORTS}/${PKG_NAME}"
  PKG_OVERLAY="${VCPKG_OVERLAYS}/${PKG_NAME}"
  if [ ! -d "${PKG_PORT_PATH}" ]; then
    echo "ERROR: could not find port for ${PKG_NAME}."
    exit 1
  fi
  rm -rf "${PKG_OVERLAY}"
  mkdir -p "${PKG_OVERLAY}"
  cp -R "${PKG_PORT_PATH}/." "${PKG_OVERLAY}/."
  (cd "${PKG_OVERLAY}" && patch -p3) <"${patch_file}"
done
