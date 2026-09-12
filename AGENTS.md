# NeoCD fork maintenance

`retrom-fork.json` defines the immutable upstream baseline, adapter ABI and release assets.
`master` is an upstream fast-forward mirror. Retrom patches belong on `retrom/g3118c6901787`.
Use `feat/*`, `fix/*`, `build/*` or `sync/upstream-*` working branches and PR into the maintenance branch.

Run `.github/rpg-runtime/test-compat.sh` and the pinned WASM build before merging.
The WASM build runs `.github/rpg-runtime/test-range.cpp` with real Asyncify suspension.
Keep native local CHD behavior, bounded bridge reads and instant-state ABI compatible.
Do not commit games, BIOS, build outputs or credentials. Retain all component license notices.

Release only annotated, immutable `retrom-core-g3118c6901787-rN` tags from commits already
merged into the maintenance branch. `.github/rpg-runtime/build-release.py` builds and validates
all assets; `.github/workflows/core.yml` must pass for the exact PR head before merge.
A release follows actual Retrom PFB preview, gamepad, save and new-Launch restore verification.
