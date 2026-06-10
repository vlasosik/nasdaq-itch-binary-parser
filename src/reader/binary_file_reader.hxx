#pragma once
#include <fstream>
#include <vector>
#include <stdexcept>

#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include <zlib.h>

constexpr uint8_t HEADER_LENGTH = 2;
constexpr uint32_t CHUNK = 1024 * 1024; // 1 MB decompress buffer

class binary_file_reader {
public:
    explicit binary_file_reader(std::string_view path);

    binary_file_reader(const binary_file_reader &) = delete;

    binary_file_reader &operator=(const binary_file_reader &) = delete;

    bool next();

    std::vector<std::byte> read();

    ~binary_file_reader();

private:
    bool decompress_chunk();

    void compact();

    int fd;
    size_t size;
    std::byte *data{};

    z_stream strm{};
    bool strm_initialized;
    bool strm_done{};

    std::vector<uint8_t> buffer;
    size_t cursor = 0;
};
