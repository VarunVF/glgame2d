#include <glgame2d/Mixer.hpp>

#include <cstdlib>

#include "glgame2d/logging.hpp"

#include <miniaudio.h>


namespace glgame2d {


Mixer::Mixer()
{
    ma_result result;
    result = ma_engine_init(NULL, &m_Engine);
    if (result != MA_SUCCESS)
    {
        logging::error("[Mixer] Failed to initalise engine");
        std::exit(result);
    }
}

Mixer::~Mixer()
{
    ma_engine_uninit(&m_Engine);
}

Mixer::Sound Mixer::load(const char* soundFile)
{
    Sound sound;
    ma_result result;

    result = ma_sound_init_from_file(&m_Engine, soundFile, MA_SOUND_FLAG_DECODE, NULL, NULL, &sound.sound);
    sound.isLoaded = (result == MA_SUCCESS);
    return sound;
}

void Mixer::play(Sound &sound)
{
    if (!sound.isLoaded) {
        logging::warn("[Mixer] Sound for playing was not loaded correctly");
        return;
    }

    ma_sound_start(&sound.sound);
}

void Mixer::play(const char *soundFile)
{
    // inline sound: play once, then put up for recycling
    ma_engine_play_sound(&m_Engine, soundFile, NULL);
}


} // namespace glgame2d
