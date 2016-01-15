#!/bin/bash -eux

git submodule update --init --recursive
# we are in ~/i-score
./tools/travis/deps.sh


case "$TRAVIS_OS_NAME" in
  linux)
    sudo apt install libvtk6-dev
  ;;
  osx)
    set +e
    brew install homebrew/scicence/vtk 
    set -e
  ;;
esac
