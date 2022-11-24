#ifndef _PROGJOGOS_AUDIO_H_
#define _PROGJOGOS_AUDIO_H_

#include "Sound.h"
#include "Types.h"
#include <XAudio2.h>
#include <string>
#include <unordered_map>
using std::string;
using std::unordered_map;

class Audio
{
  private:
    IXAudio2 *audioEngine;
    IXAudio2MasteringVoice *masterVoice;
    unordered_map<uint, Sound *> soundTable;

  public:
    Audio();
    ~Audio();

    void Add(uint id, string filename, uint nVoices = 1);
    void Play(uint id, bool repeat = false);
    void Stop(uint id);
    void Stop(uint id, uint voice);
    void Volume(uint id, float level);
    void Frequency(uint id, float level);
};

#endif
