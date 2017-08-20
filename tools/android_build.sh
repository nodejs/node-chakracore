#!/bin/bash

set -e

ROOT=${PWD}

if [ $# -eq 0 ]
then
  echo "Requires a path to the Android NDK"
  echo "Usage: android_build.sh <ndk_path> [target_arch]"
  exit
fi

SCRIPT_DIR="$(dirname "$BASH_SOURCE")"
cd "$SCRIPT_DIR"
SCRIPT_DIR=${PWD}

cd "$ROOT"
cd "$1"
ANDROID_NDK_PATH=${PWD}
cd "$SCRIPT_DIR"
cd ../

BUILD_ARCH() {
  make clean
  # Clean previous toolchain.
  rm -rf android-toolchain/
  source ./android-configure "$ANDROID_NDK_PATH" $TARGET_ARCH
  make V= -j $(getconf _NPROCESSORS_ONLN)
  mkdir -p "out_android/$TARGET_ARCH/"
  cp "out/Release/lib.target/libnode.so" "out_android/$TARGET_ARCH/libnode.so"
}

if [ $# -eq 2 ]
then
  TARGET_ARCH=$2
else
  TARGET_ARCH="arm"
fi
BUILD_ARCH

cd "$ROOT"

