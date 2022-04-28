FROM debian:11.3

RUN apt-get update \
  && apt-get --no-install-recommends -y install \
    bzip2 \
    ca-certificates \
    cmake \
    file \
    gcc \
    g++ \
    git \
    make \
    libgtk-3-dev \
    librsvg2-dev \
    wget \
  && rm -rf /var/lib/apt/lists/*

ARG WXWIDGETS_VERSION=3.1.6
RUN mkdir -p /code/deps/wxwidgets \
  && wget -q -O - https://github.com/wxWidgets/wxWidgets/releases/download/v${WXWIDGETS_VERSION}/wxWidgets-${WXWIDGETS_VERSION}.tar.bz2 \
    | tar --strip-components=1 -C /code/deps/wxwidgets -jxf - \
  && cd /code/deps/wxwidgets \
  && ./configure \
    --with-gtk \
    --with-libtiff=no \
    --disable-shared \
  && make -j$(nproc) \
  && make install \
  && rm -rf /code/deps/wxwidgets

# Deployment tool
ENV APPIMAGE_EXTRACT_AND_RUN=1
ENV DEPLOY_GTK_VERSION=3
RUN \
  wget -q -P /usr/local/bin/ \
    "https://raw.githubusercontent.com/linuxdeploy/linuxdeploy-plugin-gtk/master/linuxdeploy-plugin-gtk.sh" \
  && wget -q -O /usr/local/bin/linuxdeploy \
    "https://github.com/linuxdeploy/linuxdeploy/releases/download/continuous/linuxdeploy-$(uname -m).AppImage" \
  && chmod -R a+x /usr/local/bin/
