#ifndef _PROGJOGOS_SOUND_H_
#define _PROGJOGOS_SOUND_H_

#include "Types.h"
#include <string>
#include <xaudio2.h>
using std::string;

class Sound
{
  private:
    WAVEFORMATEXTENSIBLE format;
    XAUDIO2_BUFFER buffer;
    float volume;
    float frequency;

    IXAudio2SourceVoice **voices;
    uint tracks;
    uint index;

    HRESULT FindChunk(HANDLE hFile, DWORD fourcc, DWORD &dwChunkSize, DWORD &dwChunkDataPosition);

    HRESULT ReadChunkData(HANDLE hFile, void *buffer, DWORD buffersize, DWORD bufferoffset);

    friend class Audio;

  public:
    Sound(string fileName, uint nTracks);
    ~Sound();
};

#endif
