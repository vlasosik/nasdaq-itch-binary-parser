#include <reader/binary_file_reader.hxx>
#include <parser/itch_parser.hxx>
#include <quill/Backend.h>
#include <quill/Frontend.h>
#include <quill/LogMacros.h>
#include <quill/sinks/ConsoleSink.h>

auto main() -> int {
    constexpr auto path = "data/08302019.NASDAQ_ITCH50.gz";

    quill::BackendOptions backend_options;
    quill::Backend::start(backend_options);

    auto console_sink = quill::Frontend::create_or_get_sink<quill::ConsoleSink>("console");
    auto logger = quill::Frontend::create_or_get_logger("root", std::move(console_sink));
    logger->set_log_level(quill::LogLevel::Debug);

    binary_file_reader file_reader(path);
    itch_parser parser;

    auto start = std::chrono::system_clock::now();
    while (file_reader.next()) {
        auto stream = file_reader.read();
        // LOG_DEBUG(logger, "msg type: {}", static_cast<char>(stream[0]));
        auto msg = parser.parse(stream);
    }
    auto end = std::chrono::system_clock::now();
    auto result = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();

    LOG_INFO(logger, "Itch parser successful completed in: {}s", result);
    return EXIT_SUCCESS;
}
