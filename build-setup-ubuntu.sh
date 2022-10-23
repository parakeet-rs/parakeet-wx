#/bin/sh

set -ex

sudo apt-get update -y

sudo apt-get install -y \
  curl \
  gettext \
  libgcrypt20-dev \
  libgtk-3-dev \
  libjpeg-dev \
  libpng-dev \
  libsecret-1-dev \
  libsystemd-dev \
  libtiff-dev \
  libwebp-dev \
  libzstd-dev \
  pkg-config
