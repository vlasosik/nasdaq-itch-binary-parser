#include "binary_file_reader.hxx"

#include <string>

binary_file_reader::binary_file_reader(const std::string &path) {
    fd = open(path.c_str(), O_RDONLY);
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
}

bool binary_file_reader::next() {
    if (pos + HEADER_LENGTH > size) {
        return false;
    }

    uint16_t temp{};
    memcpy(&temp, data + pos, HEADER_LENGTH);
    msg_len = std::byteswap(temp);
    pos += msg_len + HEADER_LENGTH;

    return pos < size;
}

std::span<std::byte> binary_file_reader::read() const {
    const auto start = pos - msg_len;
    return std::span{data + start, msg_len};
}

binary_file_reader::~binary_file_reader() {
    if (data) {
        munmap(data, size);
    }
    if (fd != -1) {
        close(fd);
    }
}
