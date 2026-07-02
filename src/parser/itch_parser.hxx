#pragma once
#include <span>
#include <bit>

#include <entities/types.hxx>
#include <exceptions/parse_error.hxx>

/**
 * @brief Decoder for NASDAQ TotalView-ITCH 5.0 messages.
 *
 * Takes a view of a single message body (length prefix already stripped by the reader),
 * dispatches on the first byte (message type) and decodes
 * the fields into the corresponding struct, returned as an itch_message variant
 * Unknown message types yield std::monostate.
 *
 * Wire format handling:
 *   - integers are big-endian and byte-swapped to host order on decode
 *   - timestamps are 6 bytes on the wire, widened to uint64_t
 *   - alpha fields are copied verbatim as fixed-size char arrays
 *
 * The view must stay valid for the duration of the parse() call only;
 * decoded structs own their data. Throws parse_error if a message is
 * shorter than its declared fields. Not thread-safe: decoding state
 * (cursor pointers) is kept in the object between internal calls.
 */

class itch_parser {
public:
    using bytes_view = std::span<const std::byte>;

    itch_message parse(bytes_view message);

private:
    template<typename T>
    T decode() {
        if (end - start < sizeof(T)) {
            throw parse_error();
        }

        T field{};
        const auto size = sizeof(field);
        memcpy(&field, start, size);
        if constexpr (std::is_integral_v<T>) {
            field = std::byteswap(field);
        }
        start += size;

        return field;
    }

    uint64_t decode_timestamp();

    system_event_message parse_system_message(bytes_view payload);

    stock_directory parse_stock_directory(bytes_view payload);

    stock_trading_action parse_stock_trading_action(bytes_view payload);

    add_order_no_mpid parse_add_order_no_mpid(bytes_view payload);

    add_order_mpid parse_add_order_mpid(bytes_view payload);

    order_executed parse_order_executed(bytes_view payload);

    order_executed_with_price parse_order_executed_with_price(bytes_view payload);

    order_cancel parse_order_cancel(bytes_view payload);

    order_delete parse_order_delete(bytes_view payload);

    order_replace parse_order_replace(bytes_view payload);

    const std::byte *start{};
    const std::byte *end{};
};
