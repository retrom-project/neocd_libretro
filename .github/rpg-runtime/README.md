# Retrom NeoCD candidate

The upstream baseline and EmulatorJS linker are pinned in `retrom-fork.json`.
`master` is the upstream mirror; integration uses `feat/neogeocd` from
`retrom/g3118c6901787`. No game or BIOS is included in build inputs.

Build through the Retrom PFB `pfb-core-build CORE=neocd` command. The wrapper
accepts an absolute empty output directory and emits the core, combined component notices,
source archive and a digest-checked candidate descriptor. Emscripten and the
linker commit are fixed by the recipe. A candidate is not a published release.

Browser integration initially accepts CHD and uses an installed CDZ BIOS.
Validate Review Preview, Product Launch, standard gamepad directions/confirm,
CD audio and instant state restoration in a new Launch before release.

The native `test-compat.sh` checks the old frontend archive API, CRC16 and
CRC32 known vectors and raw DEFLATE hunk reuse. Every candidate runs it first.
See THIRD_PARTY_NOTICES.md for the Z80 non-commercial restriction.
