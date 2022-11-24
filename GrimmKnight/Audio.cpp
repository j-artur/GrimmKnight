#include "Audio.h"
#include "Sound.h"

Audio::Audio()
{
    audioEngine = nullptr;
    masterVoice = nullptr;

    if (SUCCEEDED(XAudio2Create(&audioEngine, 0, XAUDIO2_DEFAULT_PROCESSOR)))
    {
        audioEngine->CreateMasteringVoice(&masterVoice);
    }
}

Audio::~Audio()
{
    for (const auto &[id, sound] : soundTable)
    {
        for (uint k = 0; k < sound->tracks; ++k)
            sound->voices[k]->DestroyVoice();

        delete sound;
    }

    if (masterVoice)
        masterVoice->DestroyVoice();

    if (audioEngine)
        audioEngine->Release();
}

void Audio::Add(uint id, string filename, uint nVoices)
{
    Sound *sound = new Sound(filename, nVoices);

    for (uint i = 0; i < nVoices; ++i)
        audioEngine->CreateSourceVoice(&sound->voices[i], (WAVEFORMATEX *)&sound->format);

    soundTable[id] = sound;
}

void Audio::Play(uint id, bool repeat)
{
    Sound *selected = soundTable[id];

    if (repeat)
        selected->buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
    else
        selected->buffer.LoopCount = 0;

    selected->voices[selected->index]->Stop();
    selected->voices[selected->index]->FlushSourceBuffers();
    selected->voices[selected->index]->SetVolume(selected->volume);
    selected->voices[selected->index]->SubmitSourceBuffer(&selected->buffer);
    selected->voices[selected->index]->Start();

    selected->index = (selected->index + 1) % selected->tracks;
}

void Audio::Stop(uint id)
{
    Sound *selected = soundTable[id];

    for (uint i = 0; i < selected->tracks; ++i)
        selected->voices[i]->Stop();
}

void Audio::Stop(uint id, uint voice)
{
    Sound *selected = soundTable[id];

    selected->voices[voice]->Stop();
}

void Audio::Volume(uint id, float level)
{
    if (level < 0)
        level = 0;

    if (level > XAUDIO2_MAX_VOLUME_LEVEL)
        level = XAUDIO2_MAX_VOLUME_LEVEL;

    Sound *selected = soundTable[id];

    selected->volume = level;
    selected->voices[selected->index]->SetVolume(level);
}

void Audio::Frequency(uint id, float level)
{
    if (level < XAUDIO2_MIN_FREQ_RATIO)
        level = XAUDIO2_MIN_FREQ_RATIO;

    Sound *selected = soundTable[id];

    selected->frequency = level;
    selected->voices[selected->index]->SetFrequencyRatio(level);
}
