#include "itch_parser.hxx"

itch_message itch_parser::parse(std::vector<std::byte> &stream) {
    switch (static_cast<char>(stream[0])) {
        case 'S':
            return {parse_system_message(stream)};
        case 'R':
            return {parse_stock_directory(stream)};
        case 'H':
            return {parse_stock_trading_action(stream)};
        case 'A':
            return {parse_add_order_no_mpid(stream)};
        case 'F':
            return {parse_add_order_mpid(stream)};
        case 'E':
            return {parse_order_executed(stream)};
        case 'C':
            return {parse_order_executed_with_price(stream)};
        case 'X':
            return {parse_order_cancel(stream)};
        case 'D':
            return {parse_order_delete(stream)};
        case 'U':
            return {parse_order_replace(stream)};
        default:
            return {};
    }
}

uint64_t itch_parser::decode_timestamp() {
    constexpr uint8_t timestamp_size = 6;
    if (end - start < timestamp_size) {
        throw parse_error();
    }

    uint64_t value{};
    for (int i = 0; i < timestamp_size; ++i) {
        value = value << 8 | static_cast<uint8_t>(start[i]);
    }
    start += timestamp_size;

    return value;
}

system_event_message itch_parser::parse_system_message(std::vector<std::byte> &stream) {
    start = stream.data();
    end = stream.data() + stream.size();

    system_event_message msg{};
    msg.message_type = decode<uint8_t>();
    msg.stock_locate = decode<uint16_t>();
    msg.tracking_number = decode<uint16_t>();
    msg.timestamp = decode_timestamp();
    msg.event_code = decode<uint8_t>();

    return msg;
}

stock_directory itch_parser::parse_stock_directory(std::vector<std::byte> &byte) {
    start = byte.data();
    end = byte.data() + byte.size();

    stock_directory msg{};
    msg.message_type = decode<uint8_t>();
    msg.stock_locate = decode<uint16_t>();
    msg.tracking_number = decode<uint16_t>();
    msg.timestamp = decode_timestamp();
    msg.stock = decode<std::array<char, 8> >();
    msg.market_category = decode<uint8_t>();
    msg.financial_status_indicator = decode<uint8_t>();
    msg.round_lot_size = decode<uint32_t>();
    msg.round_lots_only = decode<uint8_t>();
    msg.issue_classification = decode<uint8_t>();
    msg.issue_subtype = decode<std::array<char, 2> >();
    msg.authenticity = decode<uint8_t>();
    msg.short_sale_threshold_indicator = decode<uint8_t>();
    msg.ipo_flag = decode<uint8_t>();
    msg.luld_reference_price_tier = decode<uint8_t>();
    msg.etp_flag = decode<uint8_t>();
    msg.etp_leverage_factor = decode<uint32_t>();
    msg.inverse_indicator = decode<uint8_t>();

    return msg;
}

stock_trading_action itch_parser::parse_stock_trading_action(std::vector<std::byte> &byte) {
    start = byte.data();
    end = byte.data() + byte.size();

    stock_trading_action msg{};
    msg.message_type = decode<uint8_t>();
    msg.stock_locate = decode<uint16_t>();
    msg.tracking_number = decode<uint16_t>();
    msg.timestamp = decode_timestamp();
    msg.stock = decode<std::array<char, 8> >();
    msg.trading_state = decode<uint8_t>();
    msg.reserved = decode<uint8_t>();
    msg.reason = decode<std::array<char, 4> >();

    return msg;
}

add_order_no_mpid itch_parser::parse_add_order_no_mpid(std::vector<std::byte> &byte) {
    start = byte.data();
    end = byte.data() + byte.size();

    add_order_no_mpid msg{};
    msg.message_type = decode<uint8_t>();
    msg.stock_locate = decode<uint16_t>();
    msg.tracking_number = decode<uint16_t>();
    msg.timestamp = decode_timestamp();
    msg.order_reference_number = decode<uint64_t>();
    msg.buy_sell_indicator = decode<uint8_t>();
    msg.shares = decode<uint32_t>();
    msg.stock = decode<std::array<char, 8> >();
    msg.price = decode<uint32_t>();

    return msg;
}

add_order_mpid itch_parser::parse_add_order_mpid(std::vector<std::byte> &byte) {
    start = byte.data();
    end = byte.data() + byte.size();

    add_order_mpid msg{};
    msg.message_type = decode<uint8_t>();
    msg.stock_locate = decode<uint16_t>();
    msg.tracking_number = decode<uint16_t>();
    msg.timestamp = decode_timestamp();
    msg.order_reference_number = decode<uint64_t>();
    msg.buy_sell_indicator = decode<uint8_t>();
    msg.shares = decode<uint32_t>();
    msg.stock = decode<std::array<char, 8> >();
    msg.price = decode<uint32_t>();
    msg.attribution = decode<std::array<char, 4> >();

    return msg;
}

order_executed itch_parser::parse_order_executed(std::vector<std::byte> &byte) {
    start = byte.data();
    end = byte.data() + byte.size();

    order_executed msg{};
    msg.message_type = decode<uint8_t>();
    msg.stock_locate = decode<uint16_t>();
    msg.tracking_number = decode<uint16_t>();
    msg.timestamp = decode_timestamp();
    msg.order_reference_number = decode<uint64_t>();
    msg.executed_shares = decode<uint32_t>();
    msg.match_number = decode<uint64_t>();

    return msg;
}

order_executed_with_price itch_parser::parse_order_executed_with_price(std::vector<std::byte> &byte) {
    start = byte.data();
    end = byte.data() + byte.size();

    order_executed_with_price msg{};
    msg.message_type = decode<uint8_t>();
    msg.stock_locate = decode<uint16_t>();
    msg.tracking_number = decode<uint16_t>();
    msg.timestamp = decode_timestamp();
    msg.order_reference_number = decode<uint64_t>();
    msg.executed_shares = decode<uint32_t>();
    msg.match_number = decode<uint64_t>();
    msg.printable = decode<uint8_t>();
    msg.execution_price = decode<uint32_t>();

    return msg;
}

order_cancel itch_parser::parse_order_cancel(std::vector<std::byte> &byte) {
    start = byte.data();
    end = byte.data() + byte.size();

    order_cancel msg{};
    msg.message_type = decode<uint8_t>();
    msg.stock_locate = decode<uint16_t>();
    msg.tracking_number = decode<uint16_t>();
    msg.timestamp = decode_timestamp();
    msg.order_reference_number = decode<uint64_t>();
    msg.cancelled_shares = decode<uint32_t>();

    return msg;
}

order_delete itch_parser::parse_order_delete(std::vector<std::byte> &byte) {
    start = byte.data();
    end = byte.data() + byte.size();

    order_delete msg{};
    msg.message_type = decode<uint8_t>();
    msg.stock_locate = decode<uint16_t>();
    msg.tracking_number = decode<uint16_t>();
    msg.timestamp = decode_timestamp();
    msg.order_reference_number = decode<uint64_t>();

    return msg;
}

order_replace itch_parser::parse_order_replace(std::vector<std::byte> &byte) {
    start = byte.data();
    end = byte.data() + byte.size();

    order_replace msg{};
    msg.message_type = decode<uint8_t>();
    msg.stock_locate = decode<uint16_t>();
    msg.tracking_number = decode<uint16_t>();
    msg.timestamp = decode_timestamp();
    msg.original_order_reference_number = decode<uint64_t>();
    msg.new_order_reference_number = decode<uint64_t>();
    msg.shares = decode<uint32_t>();
    msg.price = decode<uint32_t>();

    return msg;
}
