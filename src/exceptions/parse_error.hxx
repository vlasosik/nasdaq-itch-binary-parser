#pragma once
#include <source_location>
#include <fmt/core.h>

class parse_error : public std::runtime_error {
public:
    explicit parse_error(const std::source_location loc = std::source_location::current())
        : std::runtime_error(fmt::format("itch parse error (func: {}, line: {})", loc.function_name(), loc.line())) {
    }
};
