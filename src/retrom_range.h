#ifndef RETROM_RANGE_H
#define RETROM_RANGE_H
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
// The adapter owns the URL, cache and credentials. Native code only names ranges.
EM_JS(int, retrom_range_open, (const char* path), {
    var bridge = globalThis.RETROM_NEOCD_RANGE;
    return bridge && UTF8ToString(path).split('/').pop() === bridge.filename ? 1 : 0;
});
EM_JS(int, retrom_range_read, (double offset, unsigned length, void* destination), {
    return Asyncify.handleSleep(function(wakeUp) {
        var bridge = globalThis.RETROM_NEOCD_RANGE;
        function copy(bytes) {
            if (!bytes || bytes.length !== length) return -1;
            HEAPU8.set(bytes, destination);
            return bytes.length;
        }
        try {
            var value = bridge.read(offset, length);
            if (!value.then) { wakeUp(copy(value)); return; }
            bridge.begin();
            value.then(function(bytes) {
                try { wakeUp(copy(bytes)); } finally { bridge.end(); }
            }, function(error) {
                try { wakeUp(-1); } finally { bridge.end(); bridge.fail(error); }
            });
        } catch(error) {
            wakeUp(-1);
            if (bridge) bridge.fail(error);
        }
    });
});
#else
inline int retrom_range_open(const char*) { return 0; }
inline int retrom_range_read(double, unsigned, void*) { return -1; }
#endif
#endif
