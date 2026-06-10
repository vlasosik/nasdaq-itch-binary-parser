#pragma once
#include <array>
#include <cstdint>
#include <variant>

/**
 * @brief NASDAQ TotalView-ITCH 5.0 message structs.
 *
 * @note Wire encoding:
 *   - All integers are big-endian (network byte order)
 *   - Alpha fields are ASCII, left-justified, right-padded with spaces
 *   - Timestamps are 6 bytes on the wire but stored as uint64_t; read only 6 bytes when parsing
 *   - Prices are integers with implied decimal places, e.g. Price(4) means divide by 10'000
 */

struct system_event_message {
    uint8_t message_type; // 'S'
    uint16_t stock_locate; // always 0
    uint16_t tracking_number;
    uint64_t timestamp; // nanoseconds since midnight (6 bytes on wire)
    uint8_t event_code; // 'O','S','Q','M','E','C'
};

struct stock_directory {
    uint8_t message_type; // 'R'
    uint16_t stock_locate;
    uint16_t tracking_number;
    uint64_t timestamp;
    std::array<char, 8> stock; // left-justified, space-padded
    uint8_t market_category;
    uint8_t financial_status_indicator;
    uint32_t round_lot_size;
    uint8_t round_lots_only;
    uint8_t issue_classification;
    std::array<char, 2> issue_subtype;
    uint8_t authenticity;
    uint8_t short_sale_threshold_indicator;
    uint8_t ipo_flag;
    uint8_t luld_reference_price_tier;
    uint8_t etp_flag;
    uint32_t etp_leverage_factor;
    uint8_t inverse_indicator;
};

struct stock_trading_action {
    uint8_t message_type; // 'H'
    uint16_t stock_locate;
    uint16_t tracking_number;
    uint64_t timestamp;
    std::array<char, 8> stock;
    uint8_t trading_state; // 'H','P','Q','T'
    uint8_t reserved;
    std::array<char, 4> reason;
};

struct reg_sho_restriction {
    uint8_t message_type; // 'Y'
    uint16_t stock_locate;
    uint16_t tracking_number;
    uint64_t timestamp;
    std::array<char, 8> stock;
    uint8_t reg_sho_action; // '0','1','2'
};

struct market_participant_position {
    uint8_t message_type; // 'L'
    uint16_t stock_locate;
    uint16_t tracking_number;
    uint64_t timestamp;
    std::array<char, 4> mpid;
    std::array<char, 8> stock;
    uint8_t primary_market_maker; // 'Y','N'
    uint8_t market_maker_mode; // 'N','P','S','R','L'
    uint8_t market_participant_state; // 'A','E','W','S','D'
};

struct mwcb_decline_level {
    uint8_t message_type; // 'V'
    uint16_t stock_locate; // always 0
    uint16_t tracking_number;
    uint64_t timestamp;
    uint64_t level_1; // Price(8)
    uint64_t level_2; // Price(8)
    uint64_t level_3; // Price(8)
};

struct mwcb_status {
    uint8_t message_type; // 'W'
    uint16_t stock_locate; // always 0
    uint16_t tracking_number;
    uint64_t timestamp;
    uint8_t breached_level; // '1','2','3'
};

struct quoting_period_update {
    uint8_t message_type; // 'K'
    uint16_t stock_locate; // always 0
    uint16_t tracking_number;
    uint64_t timestamp;
    std::array<char, 8> stock;
    uint32_t ipo_quotation_release_time; // seconds since midnight
    uint8_t ipo_quotation_release_qualifier; // 'A','C'
    uint32_t ipo_price; // Price(4)
};

struct luld_auction_collar {
    uint8_t message_type; // 'J'
    uint16_t stock_locate;
    uint16_t tracking_number;
    uint64_t timestamp;
    std::array<char, 8> stock;
    uint32_t auction_collar_reference_price; // Price(4)
    uint32_t upper_auction_collar_price; // Price(4)
    uint32_t lower_auction_collar_price; // Price(4)
    uint32_t auction_collar_extension;
};

struct operational_halt {
    uint8_t message_type; // 'h'
    uint16_t stock_locate;
    uint16_t tracking_number;
    uint64_t timestamp;
    std::array<char, 8> stock;
    uint8_t market_code; // 'Q','B','X'
    uint8_t operational_halt_action; // 'H','T'
};

struct add_order_no_mpid {
    uint8_t message_type; // 'A'
    uint16_t stock_locate;
    uint16_t tracking_number;
    uint64_t timestamp;
    uint64_t order_reference_number;
    uint8_t buy_sell_indicator; // 'B','S'
    uint32_t shares;
    std::array<char, 8> stock;
    uint32_t price; // Price(4)
};

struct add_order_mpid {
    uint8_t message_type; // 'F'
    uint16_t stock_locate;
    uint16_t tracking_number;
    uint64_t timestamp;
    uint64_t order_reference_number;
    uint8_t buy_sell_indicator; // 'B','S'
    uint32_t shares;
    std::array<char, 8> stock;
    uint32_t price; // Price(4)
    std::array<char, 4> attribution;
};

struct order_executed {
    uint8_t message_type; // 'E'
    uint16_t stock_locate;
    uint16_t tracking_number;
    uint64_t timestamp;
    uint64_t order_reference_number;
    uint32_t executed_shares;
    uint64_t match_number;
};

struct order_executed_with_price {
    uint8_t message_type; // 'C'
    uint16_t stock_locate;
    uint16_t tracking_number;
    uint64_t timestamp;
    uint64_t order_reference_number;
    uint32_t executed_shares;
    uint64_t match_number;
    uint8_t printable; // 'Y','N'
    uint32_t execution_price; // Price(4)
};

struct order_cancel {
    uint8_t message_type; // 'X'
    uint16_t stock_locate;
    uint16_t tracking_number;
    uint64_t timestamp;
    uint64_t order_reference_number;
    uint32_t cancelled_shares;
};

struct order_delete {
    uint8_t message_type; // 'D'
    uint16_t stock_locate;
    uint16_t tracking_number;
    uint64_t timestamp;
    uint64_t order_reference_number;
};

struct order_replace {
    uint8_t message_type; // 'U'
    uint16_t stock_locate;
    uint16_t tracking_number;
    uint64_t timestamp;
    uint64_t original_order_reference_number;
    uint64_t new_order_reference_number;
    uint32_t shares;
    uint32_t price; // Price(4)
};

struct trade_non_cross {
    uint8_t message_type; // 'P'
    uint16_t stock_locate;
    uint16_t tracking_number;
    uint64_t timestamp;
    uint64_t order_reference_number;
    uint8_t buy_sell_indicator; // 'B','S'
    uint32_t shares;
    std::array<char, 8> stock;
    uint32_t price; // Price(4)
    uint64_t match_number;
};

struct cross_trade {
    uint8_t message_type; // 'Q'
    uint16_t stock_locate;
    uint16_t tracking_number;
    uint64_t timestamp;
    uint64_t shares;
    std::array<char, 8> stock;
    uint32_t cross_price; // Price(4)
    uint64_t match_number;
    uint8_t cross_type; // 'O','C','H','I'
};

struct broken_trade {
    uint8_t message_type; // 'B'
    uint16_t stock_locate;
    uint16_t tracking_number;
    uint64_t timestamp;
    uint64_t match_number;
};

struct noii {
    uint8_t message_type; // 'I'
    uint16_t stock_locate;
    uint16_t tracking_number;
    uint64_t timestamp;
    uint64_t paired_shares;
    uint64_t imbalance_shares;
    uint8_t imbalance_direction; // 'B','S','N','O'
    std::array<char, 8> stock;
    uint32_t far_price; // Price(4)
    uint32_t near_price; // Price(4)
    uint32_t current_reference_price; // Price(4)
    uint8_t cross_type; // 'O','C','H','I'
    uint8_t price_variation_indicator;
};

struct rpii {
    uint8_t message_type; // 'N'
    uint16_t stock_locate;
    uint16_t tracking_number;
    uint64_t timestamp;
    std::array<char, 8> stock;
    uint8_t interest_flag; // 'B','S','A','N'
};

struct direct_listing {
    uint8_t message_type; // 'O'
    uint16_t stock_locate;
    uint16_t tracking_number;
    uint64_t timestamp;
    std::array<char, 8> stock;
    uint8_t open_eligibility_status;
    uint32_t minimum_allowable_price; // Price(4)
    uint32_t maximum_allowable_price; // Price(4)
    uint32_t near_execution_price; // Price(4)
    uint64_t near_execution_time;
    uint32_t lower_price_range_collar; // Price(4)
    uint32_t upper_price_range_collar; // Price(4)
};

using itch_message = std::variant<
    system_event_message,
    stock_directory,
    stock_trading_action,
    reg_sho_restriction,
    market_participant_position,
    mwcb_decline_level,
    mwcb_status,
    quoting_period_update,
    luld_auction_collar,
    operational_halt,
    add_order_no_mpid,
    add_order_mpid,
    order_executed,
    order_executed_with_price,
    order_cancel,
    order_delete,
    order_replace,
    trade_non_cross,
    cross_trade,
    broken_trade,
    noii,
    rpii,
    direct_listing,
    std::monostate
>;
