#!/bin/bash

set -e

ROOT=${PWD}

SCRIPT_DIR="$(dirname "$BASH_SOURCE")"
cd "$SCRIPT_DIR"
SCRIPT_DIR=${PWD}

#should be the node's source root
cd ../

LIBRARY_PATH='out/Release'
TARGET_LIBRARY_PATH='tools/ios-framework/bin'

./configure --dest-os=ios --dest-cpu=arm64 --without-chakra-jit --enable-static --with-intl=none --openssl-no-asm
make

mkdir -p $TARGET_LIBRARY_PATH

cp $LIBRARY_PATH/libcares.a $TARGET_LIBRARY_PATH/
cp $LIBRARY_PATH/libChakraCoreStatic.a $TARGET_LIBRARY_PATH/
cp $LIBRARY_PATH/libchakrashim.a $TARGET_LIBRARY_PATH/
cp $LIBRARY_PATH/libhttp_parser.a $TARGET_LIBRARY_PATH/
cp $LIBRARY_PATH/libnghttp2.a $TARGET_LIBRARY_PATH/
cp $LIBRARY_PATH/libnode.a $TARGET_LIBRARY_PATH/
cp $LIBRARY_PATH/libopenssl.a $TARGET_LIBRARY_PATH/
cp $LIBRARY_PATH/libuv.a $TARGET_LIBRARY_PATH/
cp $LIBRARY_PATH/libzlib.a $TARGET_LIBRARY_PATH/

cd "$ROOT"
