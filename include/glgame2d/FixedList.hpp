#pragma once

#include <array>
#include <cassert>
#include <cstddef>


namespace glgame2d {


template<typename T, std::size_t N>
class FixedList
{
public:
    std::size_t size() const { return count; }
    static constexpr std::size_t capacity() { return N; }

    void push_back(const T& value)
    {
        assert(count < N);
        items[count++] = value;
    }

    auto begin() const { return items.begin(); }
    auto end() const { return items.begin() + count; }
    auto begin() { return items.begin(); }
    auto end() { return items.begin() + count; }

private:
    std::array<T, N> items = {};
    std::size_t count = 0;
};


} // namespace glgame2d
