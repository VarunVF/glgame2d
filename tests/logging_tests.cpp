#include <catch2/catch_test_macros.hpp>

#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

#include <glgame2d/logging.hpp>


using namespace glgame2d::logging;


namespace {


struct LogEntry {
    Level level;
    std::string text;
};

// Installs a capturing sink and restores the previous one on destruction
class ScopedLogSink {
public:
    ScopedLogSink()
    {
        m_Previous = setSink([this](Level level, std::string_view text) {
            entries.push_back({level, std::string(text)});
        });
    }

    ~ScopedLogSink() { setSink(std::move(m_Previous)); }

    ScopedLogSink(const ScopedLogSink&) = delete;
    ScopedLogSink& operator=(const ScopedLogSink&) = delete;

    std::vector<LogEntry> entries;

private:
    Sink m_Previous;
};


} // namespace


TEST_CASE("The default sink is consoleSink", "[logging]")
{
    Sink previous = setSink({});
    auto* fn = previous.target<void (*)(Level, std::string_view) noexcept>();
    REQUIRE(fn != nullptr);
    CHECK(*fn == &consoleSink);
    setSink(std::move(previous));
}

TEST_CASE("Sink receives the level and the formatted text", "[logging]")
{
    ScopedLogSink log;

    warn("x = {}", 3);

    REQUIRE(log.entries.size() == 1);
    CHECK(log.entries[0].level == Level::Warn);
    CHECK(log.entries[0].text == "x = 3");  // no color codes or prefix
}

TEST_CASE("Each function reports its own level", "[logging]")
{
    ScopedLogSink log;

    info("i");
    warn("w");
    error("e");

    REQUIRE(log.entries.size() == 3);
    CHECK(log.entries[0].level == Level::Info);
    CHECK(log.entries[1].level == Level::Warn);
    CHECK(log.entries[2].level == Level::Error);
}

TEST_CASE("Replacing a sink restores the previous one", "[logging]")
{
    ScopedLogSink outer;
    {
        ScopedLogSink inner;
        info("to inner");
        CHECK(inner.entries.size() == 1);
        CHECK(outer.entries.empty());
    }
    info("to outer");
    CHECK(outer.entries.size() == 1);
}


TEST_CASE("A throwing sink does not propagate out of logging", "[logging]")
{
    // Logging should never throw.
    Sink previous = setSink([](Level, std::string_view) {
        throw std::runtime_error("sink failed");
    });

    CHECK_NOTHROW(error("should not throw"));

    setSink(std::move(previous));
}

TEST_CASE("An empty sink disables output", "[logging]")
{
    Sink previous = setSink({});

    CHECK_NOTHROW(info("discarded"));

    setSink(std::move(previous));
}

// Default console formatting

TEST_CASE("Console format without color has a level prefix and no escape codes", "[logging]")
{
    const std::string line = detail::formatForConsole(Level::Warn, "low on ammo", false);

    CHECK(line.find("warn") != std::string::npos);
    CHECK(line.find("low on ammo") != std::string::npos);
    CHECK(line.ends_with('\n'));
    CHECK(line.find('\033') == std::string::npos);
}

TEST_CASE("Console format with color wraps warn and error in escape codes", "[logging]")
{
    const std::string warn  = detail::formatForConsole(Level::Warn, "w", true);
    const std::string error = detail::formatForConsole(Level::Error, "e", true);

    CHECK(warn.find("\033[33m") != std::string::npos);
    CHECK(error.find("\033[31m") != std::string::npos);
    CHECK(warn.ends_with("\033[0m\n"));
    CHECK(error.ends_with("\033[0m\n"));
}
