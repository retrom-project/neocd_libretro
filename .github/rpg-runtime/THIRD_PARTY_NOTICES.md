# NeoCD browser integration notices

NeoCD's top-level LICENSE.md is LGPL-3.0. It is not a complete license summary
for the combined browser binary. The linked EmulatorJS RetroArch frontend is
GPL-3.0; its exact commit is recorded in retrom-fork.json. The bundled Z80
emulator in src/3rdparty/z80/z80.cpp explicitly limits use to non-commercial
purposes. Do not describe or distribute the combined binary as unrestricted
LGPL-only software. Component notices remain authoritative.

The build appends the original NeoCD license, the pinned frontend's COPYING,
and third-party source notices to the candidate LICENSE.md and embedded
license.txt. The complete NeoCD source, including unmodified component notices,
is supplied in source.tar.gz. The pinned frontend source is available at:
https://github.com/EmulatorJS/RetroArch/tree/6dd4353937ef48b6ec0bfbdbb15d1c5992d86927

No BIOS or game is included. Product validation uses operator-supplied files.
