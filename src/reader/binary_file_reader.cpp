#include "binary_file_reader.hxx"

binary_file_reader::binary_file_reader(const std::string_view path) {
    fd = open(path.data(), O_RDONLY);
    if (fd == -1) {
        throw std::runtime_error("Failed to open the file");
    }

    struct stat st{};
    fstat(fd, &st);

    const auto ptr = mmap(nullptr, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (ptr == MAP_FAILED) {
        close(fd);
        throw std::runtime_error("Error mapping file to memory");
    }

    data = static_cast<std::byte *>(ptr);
    size = static_cast<size_t>(st.st_size);

    strm.next_in = reinterpret_cast<uint8_t *>(data);
    strm.avail_in = static_cast<uInt>(size);

    if (inflateInit2(&strm, 16 + MAX_WBITS) != Z_OK) {
        munmap(data, size);
        close(fd);
        throw std::runtime_error("inflateInit2 failed");
    }
    strm_initialized = true;
    buffer.reserve(CHUNK * 2);
}

bool binary_file_reader::decompress_chunk() {
    if (strm_done) {
        return false;
    }

    const size_t old_size = buffer.size();
    buffer.resize(old_size + CHUNK);

    strm.next_out = buffer.data() + old_size;
    strm.avail_out = CHUNK;

    const int ret = inflate(&strm, Z_NO_FLUSH);

    const size_t produced = CHUNK - strm.avail_out;
    buffer.resize(old_size + produced);

    if (ret == Z_STREAM_END) {
        strm_done = true;
        return produced > 0;
    }

    if (ret != Z_OK && ret != Z_BUF_ERROR) {
        throw std::runtime_error("zlib inflate error");
    }

    return true;
}

void binary_file_reader::compact() {
    if (cursor == 0) return;
    const size_t remaining = buffer.size() - cursor;
    if (remaining > 0) {
        std::memmove(buffer.data(), buffer.data() + cursor, remaining);
    }
    buffer.resize(remaining);
    cursor = 0;
}


bool binary_file_reader::next() {
    while (true) {
        if (const size_t available = buffer.size() - cursor; available >= HEADER_LENGTH) {
            const uint16_t msg_len =
                    static_cast<uint16_t>(buffer[cursor]) << 8 | static_cast<uint16_t>(buffer[cursor + 1]);

            if (available >= static_cast<size_t>(HEADER_LENGTH + msg_len)) {
                return true;
            }
        }

        if (strm_done) {
            return false;
        }

        compact();
        if (!decompress_chunk()) {
            return false;
        }
    }
}

std::vector<std::byte> binary_file_reader::read() {
    const uint16_t msg_len =
            static_cast<uint16_t>(buffer[cursor]) << 8 | static_cast<uint16_t>(buffer[cursor + 1]);

    cursor += HEADER_LENGTH;

    std::vector<std::byte> msg(msg_len);
    std::memcpy(msg.data(), buffer.data() + cursor, msg_len);
    cursor += msg_len;

    return msg;
}

binary_file_reader::~binary_file_reader() {
    if (strm_initialized) {
        inflateEnd(&strm);
    }
    if (data) {
        munmap(data, size);
    }
    if (fd != -1) {
        close(fd);
    }
}
