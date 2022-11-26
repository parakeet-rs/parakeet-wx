#!/bin/sh
set -ex

cd "$(git rev-parse --show-toplevel)"

./vcpkg/bootstrap-vcpkg.sh --disableMetrics || {
  echo "ERROR: could not bootstrap vcpkg."
  exit 1
}

VCPKG_PORTS=vcpkg/ports
VCPKG_OVERLAYS=vcpkg_overlays
rm -rf "${VCPKG_OVERLAYS}" && mkdir -p "${VCPKG_OVERLAYS}"

# Apply linux patches
for patch_file in patches/*.patch; do
  PACKAGE="$(basename -s ".patch" "${patch_file}")"
  cp -R "${VCPKG_PORTS}/${PACKAGE}" "${VCPKG_OVERLAYS}/${PACKAGE}"

  (cd "${VCPKG_OVERLAYS}" && patch -p2) <"${patch_file}"
done
