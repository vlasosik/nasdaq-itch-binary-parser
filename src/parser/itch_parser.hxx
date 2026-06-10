#pragma once
#include <vector>
#include <bit>

#include <entities/types.hxx>
#include <exceptions/parse_error.hxx>

class itch_parser {
public:
    itch_message parse(std::vector<std::byte> &stream);

private:
    template<typename T>
    T decode() {
        if (end - start < sizeof(T)) {
            throw parse_error();
        }

        T field{};
        const auto size = sizeof(field);
        std::memcpy(&field, start, size);
        if constexpr (std::is_integral_v<T>) {
            field = std::byteswap(field);
        }
        start += size;

        return field;
    }

    uint64_t decode_timestamp();

    system_event_message parse_system_message(std::vector<std::byte> &stream);
    stock_directory parse_stock_directory(std::vector<std::byte> &byte);
    stock_trading_action parse_stock_trading_action(std::vector<std::byte> &byte);

    add_order_no_mpid parse_add_order_no_mpid(std::vector<std::byte> &byte);
    add_order_mpid parse_add_order_mpid(std::vector<std::byte> &byte);

    order_executed parse_order_executed(std::vector<std::byte> &byte);
    order_executed_with_price parse_order_executed_with_price(std::vector<std::byte> &byte);

    order_cancel parse_order_cancel(std::vector<std::byte> &byte);
    order_delete parse_order_delete(std::vector<std::byte> &byte);
    order_replace parse_order_replace(std::vector<std::byte> &byte);

    std::byte *start{};
    std::byte *end{};
};
