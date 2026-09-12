"""Retain verbatim source notices together with both top-level licenses."""
from pathlib import Path
import re
import sys

root, frontend, output = map(Path, sys.argv[1:])
parts = [("Integration", (root / ".github/rpg-runtime/THIRD_PARTY_NOTICES.md").read_text()),
         ("NeoCD", (root / "LICENSE.md").read_text()),
         ("EmulatorJS RetroArch", frontend.read_text())]
for directory in (root / "src/3rdparty", root / "deps"):
    for path in sorted(directory.rglob("*")):
        if not path.is_file() or path.suffix not in (".c", ".h", ".cpp"):
            continue
        source = path.read_text(errors="replace")
        notice = re.match(r"\s*(/\*.*?\*/)", source, re.S)
        if notice:
            parts.append((str(path.relative_to(root)), notice.group(1)))
output.write_text("\n\n".join(f"## {name}\n\n{text}" for name, text in parts))
