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

The web build exposes `RETROM_NEOCD_RANGE` bridge v1 for a placeholder CHD
named by content digest. rchd requests bounded byte ranges; the adapter supplies
bytes synchronously from its LRU or asynchronously through Asyncify. Native
filesystem CHDs remain supported when no matching bridge is installed. The
bridge never receives URLs or credentials from CHD metadata. Its wasm regression
checks synchronous reads, suspended reads, filename isolation and I/O failure.
State format remains `emulatorjs-state-v1`; the bridge is not serialized.

Stable releases use annotated `retrom-core-g3118c6901787-rN` tags from the maintenance
branch. The PR workflow builds the core, exercises native compatibility and the WASM
Range bridge, and verifies the archive member set, WASM header and component notices.
Tag builds publish the three declared assets and `rpg-runtime-release.json`. Source
archives normalize timestamps and ownership for repeatable output.
