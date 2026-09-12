#include <cassert>
#include <cstdint>
#include "retrom_range.h"
EM_JS(void, setup_range, (), {
    globalThis.RETROM_NEOCD_RANGE = {
        filename: 'test.chd', begin: function() {}, end: function() {}, fail: function() {},
        read: function(offset, length) {
            if (offset === 0) return new Uint8Array(length).fill(7);
            if (offset === 1) return new Promise(function(resolve) {setTimeout(function() {resolve(new Uint8Array(length).fill(9));}, 5);});
            return Promise.reject(new Error('cancelled'));
        }
    };
});
int main() {
    setup_range();
    assert(retrom_range_open("/content/test.chd"));
    assert(!retrom_range_open("/content/other.chd"));
    uint8_t bytes[8] = {};
    assert(retrom_range_read(0, sizeof(bytes), bytes) == 8);
    for (auto byte : bytes) assert(byte == 7);
    assert(retrom_range_read(1, sizeof(bytes), bytes) == 8);
    for (auto byte : bytes) assert(byte == 9);
    assert(retrom_range_read(2, sizeof(bytes), bytes) == -1);
    return 0;
}
