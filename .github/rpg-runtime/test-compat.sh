#!/usr/bin/env bash
set -euo pipefail
root=$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)
work=$(mktemp -d)
trap 'rm -rf "$work"' EXIT
includes=(-I"$root/src" -I"$root/deps/libretro-common/include" -I"$root/deps")
flags=(-ffunction-sections -fdata-sections -Dencoding_crc32=neocd_encoding_crc32 -Dencoding_crc32_ogg=neocd_encoding_crc32_ogg)
cc "${includes[@]}" "${flags[@]}" -c "$root/deps/libretro-common/encodings/encoding_crc32.c" -o "$work/crc.o"
cc "${includes[@]}" "${flags[@]}" -c "$root/deps/libretro-common/encodings/encoding_deflate.c" -o "$work/deflate.o"
c++ -std=c++14 -D__EMSCRIPTEN__ "${includes[@]}" "${flags[@]}" -Wl,--gc-sections \
  "$root/src/archivezip.cpp" "$root/.github/rpg-runtime/test-compat.cpp" "$work/crc.o" "$work/deflate.o" -o "$work/test"
"$work/test"
