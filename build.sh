#!/bin/sh

export PKG_CONFIG_PATH="$(pwd)/prefix/usr/libdata/pkgconfig/"
export CPPFLAGS="-I$(pwd)/prefix/usr/include/"

rm -r ./_build
meson setup ./_build
muon -C ./_build samu

rm -r ./_build
muon setup ./_build
muon -C ./_build samu
