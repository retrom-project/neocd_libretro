#include "archivezip.h"
#include "libretro_log.h"
#include <encodings/crc32.h>
#include <encodings/deflate.h>
#include <file/archive_file.h>
#include <cassert>
#include <cstdlib>
#include <cstring>

namespace Libretro { namespace Log { void message(retro_log_level, const char*, ...) {} } }
std::string make_path_separator(const char* path, const char* separator, const char* name)
{ return std::string(path) + separator + name; }
// Represents the API exposed by the pinned, older EJS frontend. Deliberately
// does not provide file_archive_get_file_crc32_and_size: old code cannot link.
extern "C" int file_archive_compressed_read(const char* path, void** data, const char*, int64_t* length)
{
    assert(std::string(path).rfind("bios.zip#", 0) == 0);
    if (std::string(path) == "bios.zip#missing.bin") return 0;
    *length = std::string(path) == "bios.zip#empty.bin" ? 0 : 4;
    *data = malloc(4);
    memcpy(*data, "BIOS", 4);
    return 1;
}
extern "C" uint64_t cpu_features_get() { return 0; }
int main()
{
    assert(ArchiveZip::getFileSize("bios.zip", "neocd.bin") == 4);
    assert(ArchiveZip::getFileSize("bios.zip", "missing.bin") == -1);
    assert(ArchiveZip::getFileSize("bios.zip", "empty.bin") == -1);
    char buffer[4] = {};
    size_t read = 0;
    assert(ArchiveZip::readFile("bios.zip", "neocd.bin", buffer, 3, &read));
    assert(read == 3 && memcmp(buffer, "BIO", 3) == 0);
    assert(!ArchiveZip::readFile("bios.zip", "missing.bin", buffer, 4, &read));
    const uint8_t text[] = "123456789";
    assert(encoding_crc16_ccitt(0xffff, text, 9) == 0x29b1);
    assert(encoding_crc32(0, text, 9) == 0xcbf43926);
    // RFC 1951 stored final block, independent of our compressor.
    const uint8_t compressed[] = {1, 4, 0, 251, 255, 'B', 'I', 'O', 'S'};
    auto stream = rinflate_new(-15);
    assert(stream);
    for (int i = 0; i < 2; ++i) {
        rinflate_reset(stream, -15);
        rinflate_set_in(stream, compressed, sizeof(compressed));
        rinflate_set_out(stream, reinterpret_cast<uint8_t*>(buffer), sizeof(buffer));
        size_t consumed = 0, written = 0;
        assert(rinflate_process(stream, &consumed, &written) == RDEFLATE_PROCESS_END);
        assert(consumed == sizeof(compressed) && written == 4 && memcmp(buffer, "BIOS", 4) == 0);
    }
    rinflate_free(stream);
}
