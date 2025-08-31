#pragma once

#include <miniaudio.h>


namespace glgame2d {


class Mixer
{
public:
    struct Sound
    {
        ma_sound sound;
        bool isLoaded;
    };

public:
    Mixer();
    ~Mixer();

    Sound load(const char* soundFile);
    void play(Sound& sound);
    void play(const char* soundFile);

private:
    ma_engine m_Engine;
};


} // namespace glgame2d
