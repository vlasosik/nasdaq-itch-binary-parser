#include <quill/Backend.h>
#include <quill/Frontend.h>
#include <quill/LogMacros.h>
#include <quill/sinks/ConsoleSink.h>

#include "parser/itch_parser.hxx"
#include "reader/binary_file_reader.hxx"

/**
 * @brief Benchmark entry point: parses of NASDAQ ITCH 5.0 data.
 *
 * Memory-maps the raw (uncompressed) ITCH file, iterates over all
 * length-prefixed messages via binary_file_reader, decodes each one
 * with itch_parser, and reports the total elapsed wall-clock time.
 *
 * Expected input: ~8.8 GB ITCH feed (~310M messages).
 */

auto main() -> int {
    std::string path("data/08302019.NASDAQ_ITCH50");

    quill::BackendOptions backend_options;
    quill::Backend::start(backend_options);

    auto console_sink = quill::Frontend::create_or_get_sink<quill::ConsoleSink>("console");
    auto logger = quill::Frontend::create_or_get_logger("root", std::move(console_sink));
    logger->set_log_level(quill::LogLevel::Debug);

    binary_file_reader file_reader(path);
    itch_parser parser;

    const auto start = std::chrono::steady_clock::now();
    size_t count_msg = 0;
    while (file_reader.next()) {
        auto stream = file_reader.read();
        parser.parse(stream);
        ++count_msg;
    }
    const auto end = std::chrono::steady_clock::now();
    const std::chrono::duration<double> elapsed = end - start;

    LOG_INFO(logger, "time processing messages: {:.2f}s", elapsed.count());
    LOG_INFO(logger, "count messages: {}", count_msg);

    return EXIT_SUCCESS;
}
