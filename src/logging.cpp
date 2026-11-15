#include "glgame2d/logging.hpp"

#include <cstdio>
#include <utility>
#include <string_view>

#ifdef _WIN32
#include <io.h>
#else
#include <unistd.h>
#endif

namespace glgame2d::logging {


namespace {


constexpr std::string_view DEFAULT = "\033[39m";
constexpr std::string_view RED     = "\033[31m";
constexpr std::string_view YELLOW  = "\033[33m";
constexpr std::string_view RESET   = "\033[0m";

Sink& currentSink()
{
    static Sink sink = consoleSink;
    return sink;
}

struct StreamInfo
{
    bool exists = false;  // false: don't write at all
    bool color  = false;  // true: stream is a terminal
};

StreamInfo inspect(std::FILE* stream) noexcept
{
    StreamInfo info;

#ifdef _WIN32
    int fd = _fileno(stream);
    if (fd < 0) {
        info.exists = false;
        info.color  = false;
    } else {
        info.exists = true;
        info.color  = _isatty(fd);
    }
#else
    int fd = fileno(stream);
    info.exists = fd != -1;
    info.color  = isatty(fd) != 0;
#endif  // _WIN32

    return info;
}

} // namespace


Sink setSink(Sink sink)
{
    return std::exchange(currentSink(), std::move(sink));
}


void consoleSink(Level level, std::string_view text) noexcept
{
    static const StreamInfo stdoutInfo = inspect(stdout);
    static const StreamInfo stderrInfo = inspect(stderr);

    const StreamInfo* info = &stderrInfo;
    FILE* stream = stderr;
    if (level == Level::Info) {
        info = &stdoutInfo;
        stream = stdout;
    }

    if (!info->exists)
        return;

    try {
        std::string line = detail::formatForConsole(level, text, info->color);
        std::fputs(line.c_str(), stream);
    } catch (...) {
        // do nothing
    }
}


namespace detail {


std::string formatForConsole(Level level, std::string_view text, bool color)
{
    std::string_view color_code = "", prefix = "";
    switch (level) {
        case Level::Info:
            prefix = "info";
            color_code = DEFAULT;
            break;
        case Level::Warn:
            color_code = YELLOW;
            prefix = "warn";
            break;
        case Level::Error:
            color_code = RED;
            prefix = "error";
            break;
    }

    if (color)
        return std::format("{}[{}] {}{}\n", color_code, prefix, text, RESET);
    else
        return std::format("[{}] {}\n", prefix, text);
}

void write(Level level, std::string_view text) noexcept
{
    const Sink& sink = currentSink();
    if (sink) {
        try {
            sink(level, text);
        } catch (...) {
            // do nothing
        }
    }
}


} // namespace detail


} // namespace glgame2d::logging
