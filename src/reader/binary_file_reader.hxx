#pragma once
#include <span>
#include <bit>

#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/fcntl.h>
#include <unistd.h>

/**
 * @brief Zero-copy reader for raw NASDAQ ITCH 5.0 files.
 *
 * Memory-maps an uncompressed ITCH file (read-only, MAP_PRIVATE) and
 * iterates over its length-prefixed messages: each message is a 2-byte
 * big-endian length header followed by the message body.
 *
 * Usage: call next() to advance to the following message; while it
 * returns true, read() yields a non-owning view of the current message
 * body (header excluded). The view points directly into the mapping,
 * so no bytes are copied; it remains valid only as long as the reader
 * is alive.
 *
 * Non-copyable: owns the file descriptor and the mapping, both released
 * in the destructor.
 */

constexpr uint8_t HEADER_LENGTH = 2;

class binary_file_reader {
public:
    explicit binary_file_reader(const std::string &path);

    binary_file_reader(const binary_file_reader &) = delete;

    binary_file_reader &operator=(const binary_file_reader &) = delete;

    bool next();

    [[nodiscard]] std::span<std::byte> read() const;

    ~binary_file_reader();

private:
    int fd{};
    size_t size{};
    size_t pos{};
    size_t msg_len{};
    std::byte *data{};
};
