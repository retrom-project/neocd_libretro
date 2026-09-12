#!/usr/bin/env bash
set -euo pipefail

core_name=${1:?core name is required}
shift
test -f /source.tar && test -d /work && test -d /output
test ! -e /work/core && test ! -e /work/retroarch

restore_host_ownership() {
  chown -R "${RETROM_HOST_UID:?}:${RETROM_HOST_GID:?}" /work /output
}
trap restore_host_ownership EXIT

mkdir -p /work/core /work/retroarch /work/EmulatorJS/data/cores
tar -C /work/core -xf /source.tar

git -C /work/retroarch init -q
git -C /work/retroarch remote add origin https://github.com/EmulatorJS/RetroArch.git
for attempt in 1 2 3; do
  if git -C /work/retroarch fetch -q --depth 1 origin 6dd4353937ef48b6ec0bfbdbb15d1c5992d86927; then break; fi
  if [ "$attempt" = 3 ]; then exit 1; fi
  sleep 2
done
git -C /work/retroarch checkout -q --detach FETCH_HEAD

export EMSCRIPTEN="$(dirname "$(command -v emcc)")"
cd /work/core
em++ -O2 -Isrc .github/rpg-runtime/test-range.cpp -s ASYNCIFY=1 \
  -s ASYNCIFY_IMPORTS=retrom_range_read -s ASYNCIFY_STACK_SIZE=65536 -o /work/test-range.js
node /work/test-range.js
emmake make -f Makefile clean "$@"
emmake make -j"4" -f Makefile platform=emscripten \
  INITIAL_HEAP=268435456 AUTO_MEMORY_GROWTH=1 "$@"

archive=$(find . -maxdepth 1 -type f -name "${core_name}_libretro_emscripten.bc" -print)
test -n "$archive" && test -f "$archive"
install -m 0644 "$archive" "/work/retroarch/emulatorjs/${core_name}_libretro_emscripten.bc"
install -m 0644 "$archive" /work/retroarch/libretro_emscripten.a

cat >> /work/retroarch/Makefile.emulatorjs <<'EOF'
LDFLAGS += -s ASYNCIFY_IMPORTS=retrom_range_read -s ASYNCIFY_STACK_SIZE=65536
EOF

emmake make -C /work/retroarch -f Makefile.emulatorjs \
  HAVE_CHD=1 HAVE_THREADS=0 PTHREAD_POOL_SIZE=0 ASYNC=1 HAVE_OPENGLES3=1 \
  STACK_SIZE=4194304 INITIAL_HEAP=134217728 ERROR_ON_UNDEFINED_SYMBOLS=1 \
  TARGET="${core_name}_libretro.js" -j"4"

install -m 0644 "/work/retroarch/${core_name}_libretro.js" /output/
install -m 0644 "/work/retroarch/${core_name}_libretro.wasm" /output/

install -m 0644 /work/retroarch/COPYING /output/frontend-COPYING
