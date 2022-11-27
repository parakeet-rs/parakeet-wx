#/bin/sh

set -ex

sudo apt-get update -y

sudo apt-get install -y \
  bison \
  build-essential \
  cmake \
  curl \
  gettext \
  libgcrypt20-dev \
  libgtk-3-dev \
  libjpeg-dev \
  libnotify-dev \
  libpng-dev \
  libsecret-1-dev \
  libsystemd-dev \
  libtiff-dev \
  libwebp-dev \
  libzstd-dev \
  ninja-build \
  pkg-config \
  tar \
  unzip \
  zip

if [ "$1" = "--native-deps" ]; then
  # FIXME: is nlohmann/json available?
  sudo apt-get install -y \
    libboost-dev \
    libboost-filesystem-dev \
    libboost-system-dev \
    libboost-thread-dev

  if [ "$2" = "--jammy-wx3.2" ]; then
    curl https://repos.codelite.org/CodeLite.asc \
      | gpg --dearmor \
      | sudo tee /usr/share/keyrings/CodeLite.gpg >/dev/null
    echo "deb [signed-by=/usr/share/keyrings/CodeLite.gpg] https://repos.codelite.org/wx3.2/ubuntu/ jammy universe" \
      | sudo tee /etc/apt/sources.list.d/CodeLite-wxWidgets.list
    sudo apt-get update
    sudo apt-get install -y \
      libwxbase3.2-0-unofficial \
      libwxbase3.2unofficial-dev \
      libwxgtk3.2-0-unofficial \
      libwxgtk3.2unofficial-dev \
      wx3.2-headers \
      wx-common \
      wx3.2-i18n
  else
    sudo apt-get install -y libwxgtk3.0-gtk3-dev
  fi
fi
