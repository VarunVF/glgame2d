#pragma once

// Logging for glgame2d.
// Note:
// - Main thread only.
// - Never throws. Exceptions, formatting failures are swallowed.
// - An empty sink disables output. The default sink is consoleSink.
// - Sinks receive raw text without any prefix or color code.
// - Sinks must not call log functions.
// Usage:
//   logging::setSink({});                    // turn output off (e.g. release builds)
//   logging::setSink(logging::consoleSink);  // back to the console

#include <format>
#include <functional>
#include <string>
#include <string_view>


namespace glgame2d::logging {


enum class Level { Info, Warn, Error };

using Sink = std::function<void(Level, std::string_view)>;

// Installs a sink and returns the one it replaced.
Sink setSink(Sink sink);

// The default sink. Info goes to stdout, Warn/Error goes to stderr.
// Only terminals get colored output.
// Writes nothing if the stream doesn't exist (Windows GUI app with no console).
void consoleSink(Level level, std::string_view text) noexcept;


namespace detail {


// Sends text to the current sink.
void write(Level level, std::string_view text) noexcept;

// Pure formatting used by consoleSink. Each log level adds a prefix, even if colored.
std::string formatForConsole(Level level, std::string_view text, bool color);

template<typename... Args>
void log(Level level, std::format_string<Args...> fmt, Args&&... args) noexcept
{
    try {
        detail::write(level, std::format(fmt, std::forward<Args>(args)...));
    } catch (...) {
        // do nothing
    }
}


} // namespace detail


template<typename... Args>
void info(std::format_string<Args...> fmt, Args&&... args) noexcept
{
    detail::log(Level::Info, fmt, std::forward<Args>(args)...);
}

template<typename... Args>
void warn(std::format_string<Args...> fmt, Args&&... args) noexcept
{
    detail::log(Level::Warn, fmt, std::forward<Args>(args)...);
}

template<typename... Args>
void error(std::format_string<Args...> fmt, Args&&... args) noexcept
{
    detail::log(Level::Error, fmt, std::forward<Args>(args)...);
}


} // namespace glgame2d::logging
