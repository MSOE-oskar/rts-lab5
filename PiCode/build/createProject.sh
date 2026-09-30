#!/bin/sh
# Always configure from a clean cache. The Eclipse indexer probe results are
# stored as INTERNAL cache entries (CMAKE_EXTRA_GENERATOR_CXX_SYSTEM_*) and are
# guarded by "if (NOT <already set>)", so they are computed exactly once and
# never refreshed by a plain re-run of cmake.
rm -rf CMakeCache.txt CMakeFiles

cmake ../src -G"Eclipse CDT4 - Unix Makefiles" \
  -DCMAKE_ECLIPSE_VERSION=4.5 \
  -DCMAKE_ECLIPSE_GENERATE_SOURCE_PROJECT=FALSE \
  -DCMAKE_ECLIPSE_MAKE_ARGUMENTS=-j4 \
  -DCMAKE_TOOLCHAIN_FILE=../Toolchain-rpi.cmake \
  -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_CXX_FLAGS="-std=c++11" \
  -DCMAKE_C_FLAGS="-std=gnu11" \
  -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
  -DCMAKE_ECLIPSE_GENERATE_LINKED_RESOURCES=OFF \
  -Wno-dev -Wno-deprecated

# Sanity check: the indexer should now agree with the build.
# 201103L means C++11, 201703L means the probe ran at the compiler default.
echo
echo "Indexer __cplusplus:"
grep -o '__cplusplus;[0-9]*L' CMakeCache.txt
echo "Indexer include dirs:"
grep '^CMAKE_EXTRA_GENERATOR_CXX_SYSTEM_INCLUDE_DIRS' CMakeCache.txt | tr ';' '\n' | tail -n +2

# Create a folder for the indexer
mkdir -p build/make.debug.linux.x86_64.Local build/make.run.linux.x86_64.Local
